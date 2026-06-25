#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/gpio.h>
#include <linux/of_gpio.h>
#include <linux/delay.h>
#include <linux/device.h>
#include <linux/slab.h>
#include <linux/mutex.h>
#include <linux/fcntl.h>
#include <linux/tty.h>
#include <linux/file.h>
#include <asm/delay.h>

// 设备名称与节点配置
#define DEV_NAME "sfm17"
#define DEV_CLASS "sfm17_class"

// 模组通讯参数（默认值，支持通过指令修改）
#define DEFAULT_BAUDRATE 115200
#define DATA_BITS 8
#define STOP_BITS 1
#define PARITY_NONE 0
#define CMD_LEN 8          // 指令包长度（8字节）
#define ACK_TIMEOUT_MS 2000 // 指令超时时间（2000ms = 2秒）
#define MAX_DATA_LEN 8206  // 最大数据包长度（特征值/图像数据）

// 模组指令类型定义（核心指令）
#define CMD_REG_3C3R_1 0x01  // 3C3R注册第1次按压
#define CMD_REG_3C3R_2 0x02  // 3C3R注册第2次按压
#define CMD_REG_3C3R_3 0x03  // 3C3R注册第3次按压
#define CMD_DEL_USER 0x04     // 删除指定用户
#define CMD_DEL_ALL 0x05     // 删除所有用户
#define CMD_GET_USER_CNT 0x09 // 获取用户总数
#define CMD_VERIFY_1V1 0x0B   // 1:1比对
#define CMD_VERIFY_1VN 0x0C   // 1:N比对
#define CMD_GET_FREE_ID 0x0D  // 获取未使用ID
#define CMD_SET_BAUDRATE 0x21 // 设置波特率
#define CMD_GET_IMAGE 0x24    // 获取图像
#define CMD_GET_VERSION 0x26  // 获取版本信息
#define CMD_SET_RING_COLOR 0xC3 // 设置LED灯环颜色
#define CMD_BREAK 0xFE        // 打断指令

// LED灯环颜色定义
#define RING_OFF 0x07      // 关闭
#define RING_RED 0x03      // 红色
#define RING_GREEN 0x05    // 绿色
#define RING_BLUE 0x06     // 蓝色
#define RING_YELLOW 0x01   // 黄色
#define RING_PURPLE 0x02   // 紫色
#define RING_CYAN 0x04     // 青色

// 模组返回错误码定义
#define ACK_SUCCESS 0x00
#define ACK_FAIL 0x01
#define ACK_FULL 0x04
#define ACK_NOUSER 0x05
#define ACK_USER_EXIST 0x07
#define ACK_TIMEOUT_ERR 0x08  // 重命名，避免与ACK_TIMEOUT宏冲突
#define ACK_HW_ERR 0x0A
#define ACK_IMAGE_ERR 0x10
#define ACK_BREAK 0x18

// 驱动私有数据结构
struct sfm17_dev {
    struct file *uart_filp;       // 串口文件指针（兼容4.1.15内核）
    int major;                    // 主设备号
    struct class *cls;            // 设备类
    struct device *dev;           // 设备结构体
    int touch_gpio;               // TOUCH_OUT唤醒引脚（可选）
    unsigned char recv_buf[MAX_DATA_LEN + 4]; // 接收缓冲区
    int recv_len;                 // 接收数据长度
    struct mutex lock;            // 并发保护锁（允许睡眠）
    struct ktermios old_termios;  // 保存原有串口配置，用于恢复
};

static struct sfm17_dev *sfm17_dev;

// 异或校验和计算（第2~6字节）
static unsigned char sfm17_calc_chk(unsigned char *cmd)
{
    unsigned char chk = 0;
    int i;
    for (i = 1; i < 6; i++) { // 字节2~6异或
        chk ^= cmd[i];
    }
    return chk;
}

// 串口发送指令包（8字节，使用vfs_write接口，兼容4.1.15）
static int sfm17_send_cmd(unsigned char *cmd)
{
    int ret;
    mm_segment_t old_fs;
    loff_t pos = 0;

    if (!sfm17_dev->uart_filp || !cmd)
        return -EINVAL;

    // 填充校验和与结尾字节
    cmd[6] = sfm17_calc_chk(cmd);
    cmd[7] = 0xF5;

    // 调试：打印发送的命令
    dev_info(sfm17_dev->dev, "TX: %02X %02X %02X %02X %02X %02X %02X %02X\n",
             cmd[0], cmd[1], cmd[2], cmd[3], cmd[4], cmd[5], cmd[6], cmd[7]);

    // 切换到内核空间地址
    old_fs = get_fs();
    set_fs(KERNEL_DS);

    // 串口发送（阻塞模式，vfs_write接口）
    ret = vfs_write(sfm17_dev->uart_filp, cmd, CMD_LEN, &pos);
    
    // 恢复地址空间
    set_fs(old_fs);

    if (ret != CMD_LEN) {
        dev_err(sfm17_dev->dev, "send cmd failed: cmd_type=0x%02X, expected=%d bytes, actual=%d bytes\n", 
                cmd[1], CMD_LEN, ret);
        return -EIO;
    }

    // 短暂延时，保证指令发送完整
    msleep(10);
    return 0;
}

// 串口接收数据（带超时，使用vfs_read接口，兼容4.1.15）
static int sfm17_recv_data(unsigned char *buf, int max_len, int timeout)
{
    int ret, len = 0;
    unsigned long start = jiffies;
    unsigned char tmp_buf[1];
    mm_segment_t old_fs;
    loff_t pos = 0;
    int i;

    if (!sfm17_dev->uart_filp || !buf)
        return -EINVAL;

    // 清空接收缓冲区
    memset(buf, 0, max_len);

    dev_info(sfm17_dev->dev, "waiting for response (timeout=%dms)...\n", timeout);

    // 切换到内核空间地址
    old_fs = get_fs();
    set_fs(KERNEL_DS);

    // 超时等待接收数据（按字节接收，保证帧完整性）
    while (time_before(jiffies, start + msecs_to_jiffies(timeout))) {
        pos = 0;
        ret = vfs_read(sfm17_dev->uart_filp, tmp_buf, 1, &pos);
        if (ret > 0) {
            buf[len++] = tmp_buf[0];
            // 检查是否接收完成（8字节指令包以F5结尾）
            if (len >= CMD_LEN && buf[len-1] == 0xF5)
                break;
            // 数据包（>8字节）需根据长度字段判断（简化处理，适用于核心指令）
            if (len >= MAX_DATA_LEN)
                break;
        } else if (ret < 0) {
            set_fs(old_fs);
            dev_err(sfm17_dev->dev, "recv data error, ret=%d\n", ret);
            return ret;
        }
        msleep(1);
    }

    // 恢复地址空间
    set_fs(old_fs);

    if (len == 0) {
        dev_err(sfm17_dev->dev, "recv timeout: no data received after %dms\n", timeout);
        return -ETIMEDOUT;
    }

    // 调试：打印接收的数据
    dev_info(sfm17_dev->dev, "RX (%d bytes): ", len);
    for (i = 0; i < len && i < 16; i++) {
        printk(KERN_CONT "%02X ", buf[i]);
    }
    printk(KERN_CONT "\n");

    // 校验指令包（F5开头+结尾+校验和）
    if (buf[0] != 0xF5 || buf[len-1] != 0xF5) {
        dev_err(sfm17_dev->dev, "invalid frame: expected start=0xF5 end=0xF5, got start=0x%02X end=0x%02X\n", 
                buf[0], buf[len-1]);
        dev_err(sfm17_dev->dev, "frame dump: ");
        for (i = 0; i < len && i < 16; i++) {
            printk(KERN_CONT "%02X ", buf[i]);
        }
        printk(KERN_CONT "\n");
        return -EINVAL;
    }

    // 校验和验证（仅8字节指令包）
    if (len == CMD_LEN) {
        unsigned char chk = sfm17_calc_chk(buf);
        if (buf[6] != chk) {
            dev_err(sfm17_dev->dev, "checksum failed: calculated=0x%02X, received=0x%02X\n", chk, buf[6]);
            dev_err(sfm17_dev->dev, "packet dump: ");
            for (i = 0; i < CMD_LEN; i++) {
                printk(KERN_CONT "%02X ", buf[i]);
            }
            printk(KERN_CONT "\n");
            return -EINVAL;
        }
    }

    return len;
}

// 3C3R注册（用户ID自动分配，权限1）
static int sfm17_reg_3c3r(void)
{
    unsigned char cmd[CMD_LEN] = {0xF5, 0x00, 0x00, 0x00, 0x01, 0x00, 0x00, 0xF5};
    unsigned char ack[CMD_LEN];
    int ret;
    u16 user_id; // 提前声明，符合ISO C90标准，避免混合声明和代码

    // 第1次按压（自动分配ID，权限1）
    cmd[1] = CMD_REG_3C3R_1;
    ret = sfm17_send_cmd(cmd);
    if (ret) return ret;
    ret = sfm17_recv_data(ack, CMD_LEN, ACK_TIMEOUT_MS);
    if (ret != CMD_LEN || ack[5] != ACK_SUCCESS) {
        dev_err(sfm17_dev->dev, "reg step1 failed, err=0x%x\n", ack[5]);
        return -EIO;
    }
    dev_info(sfm17_dev->dev, "reg step1: first press success\n");
    msleep(500); // 等待用户抬手

    // 第2次按压
    cmd[1] = CMD_REG_3C3R_2;
    cmd[2] = cmd[3] = cmd[4] = 0x00;
    ret = sfm17_send_cmd(cmd);
    if (ret) return ret;
    ret = sfm17_recv_data(ack, CMD_LEN, ACK_TIMEOUT_MS);
    if (ret != CMD_LEN || ack[5] != ACK_SUCCESS) {
        dev_err(sfm17_dev->dev, "reg step2 failed, err=0x%x\n", ack[5]);
        return -EIO;
    }
    dev_info(sfm17_dev->dev, "reg step2: second press success\n");
    msleep(500);

    // 第3次按压（注册完成）
    cmd[1] = CMD_REG_3C3R_3;
    ret = sfm17_send_cmd(cmd);
    if (ret) return ret;
    ret = sfm17_recv_data(ack, CMD_LEN, ACK_TIMEOUT_MS);
    if (ret != CMD_LEN || ack[5] != ACK_SUCCESS) {
        dev_err(sfm17_dev->dev, "reg step3 failed, err=0x%x\n", ack[5]);
        return -EIO;
    }

    // 解析用户ID（高8位+低8位）
    user_id = (ack[2] << 8) | ack[3];
    dev_info(sfm17_dev->dev, "3C3R reg success, user_id=%d\n", user_id);
    return user_id;
}

// 1:N比对（返回匹配的用户ID，失败返回-1）
static int sfm17_verify_1vn(void)
{
    unsigned char cmd[CMD_LEN] = {0xF5, CMD_VERIFY_1VN, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF5};
    unsigned char ack[CMD_LEN];
    int ret;
    u16 user_id;
    u8 role;

    dev_info(sfm17_dev->dev, "wait for fingerprint press...\n");
    ret = sfm17_send_cmd(cmd);
    if (ret) return ret;
    ret = sfm17_recv_data(ack, CMD_LEN, 8000); // 延长超时（8秒，改为毫秒级，更合理）
    if (ret != CMD_LEN) {
        dev_err(sfm17_dev->dev, "verify timeout or frame error\n");
        return -ETIMEDOUT;
    }

    if (ack[5] == ACK_SUCCESS) {
        user_id = (ack[2] << 8) | ack[3];
        role = ack[4];
        dev_info(sfm17_dev->dev, "1:N verify success, user_id=%d, role=%d\n", user_id, role);
        return user_id;
    } else {
        dev_err(sfm17_dev->dev, "1:N verify failed, err=0x%x\n", ack[5]);
        return -EIO;
    }
}

// 删除所有用户
static int sfm17_del_all(void)
{
    unsigned char cmd[CMD_LEN] = {0xF5, CMD_DEL_ALL, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF5};
    unsigned char ack[CMD_LEN];
    int ret;

    ret = sfm17_send_cmd(cmd);
    if (ret) return ret;
    ret = sfm17_recv_data(ack, CMD_LEN, ACK_TIMEOUT_MS);
    if (ret == CMD_LEN && ack[5] == ACK_SUCCESS) {
        dev_info(sfm17_dev->dev, "delete all users success\n");
        return 0;
    } else {
        dev_err(sfm17_dev->dev, "delete all failed, err=0x%x\n", ack[5]);
        return -EIO;
    }
}

// 获取用户总数
static int sfm17_get_user_cnt(void)
{
    unsigned char cmd[CMD_LEN] = {0xF5, CMD_GET_USER_CNT, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF5};
    unsigned char ack[CMD_LEN];
    int ret;
    u16 cnt;

    ret = sfm17_send_cmd(cmd);
    if (ret) return ret;
    ret = sfm17_recv_data(ack, CMD_LEN, ACK_TIMEOUT_MS);
    if (ret == CMD_LEN && ack[5] == ACK_SUCCESS) {
        cnt = (ack[2] << 8) | ack[3];
        dev_info(sfm17_dev->dev, "current user count: %d\n", cnt);
        return cnt;
    } else {
        dev_err(sfm17_dev->dev, "get user cnt failed, err=0x%x\n", ack[5]);
        return -EIO;
    }
}

// 打断当前指令（如注册/比对过程中退出）
static int sfm17_send_break(void)
{
    unsigned char cmd[CMD_LEN] = {0xF5, CMD_BREAK, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF5};
    unsigned char ack[CMD_LEN];
    int ret;

    ret = sfm17_send_cmd(cmd);
    if (ret) return ret;
    ret = sfm17_recv_data(ack, CMD_LEN, ACK_TIMEOUT_MS);
    if (ret == CMD_LEN && ack[5] == ACK_BREAK) {
        dev_info(sfm17_dev->dev, "break cmd success\n");
        return 0;
    } else {
        dev_err(sfm17_dev->dev, "break cmd failed, err=0x%x\n", ack[5]);
        return -EIO;
    }
}

// 设置LED灯环颜色（参考厂商STM32示例代码）
static int sfm17_set_ring_color(unsigned char start_color, unsigned char end_color, unsigned int period)
{
    unsigned char cmd[CMD_LEN] = {0xF5, CMD_SET_RING_COLOR, 0x00, 0x00, 0x00, 0x00, 0x00, 0xF5};
    unsigned char ack[CMD_LEN];
    int ret;
    unsigned char period_byte;

    // 周期转换：period单位为ms，需要除以10（范围300-2000ms -> 30-200）
    period = (period < 300) ? 300 : period;
    period = (period > 2000) ? 2000 : period;
    period_byte = period / 10;

    // 构造命令：cmd[2]=起始颜色, cmd[3]=结束颜色, cmd[4]=周期
    cmd[2] = start_color;
    cmd[3] = end_color;
    cmd[4] = period_byte;

    ret = sfm17_send_cmd(cmd);
    if (ret) return ret;
    ret = sfm17_recv_data(ack, CMD_LEN, ACK_TIMEOUT_MS);
    if (ret == CMD_LEN && ack[5] == ACK_SUCCESS) {
        dev_info(sfm17_dev->dev, "set ring color success (start=0x%x, end=0x%x, period=%dms)\n", 
                 start_color, end_color, period);
        return 0;
    } else {
        dev_err(sfm17_dev->dev, "set ring color failed, err=0x%x\n", ack[5]);
        return -EIO;
    }
}

// 测试模块连接（在驱动加载时调用，设置蓝色LED验证连接）
static int sfm17_test_module_connection(void)
{
    struct tty_struct *tty;
    struct ktermios new_termios;
    int ret;

    dev_info(sfm17_dev->dev, "testing module connection...\n");

    // 临时打开串口设备
    sfm17_dev->uart_filp = filp_open("/dev/ttymxc1", O_RDWR | O_NOCTTY, 0);
    if (IS_ERR(sfm17_dev->uart_filp)) {
        ret = PTR_ERR(sfm17_dev->uart_filp);
        dev_warn(sfm17_dev->dev, "cannot open uart for test, ret=%d (will retry on device open)\n", ret);
        sfm17_dev->uart_filp = NULL;
        return ret;
    }

    // 获取tty结构体并配置串口
    tty = ((struct tty_file_private *)sfm17_dev->uart_filp->private_data)->tty;
    if (!tty) {
        dev_warn(sfm17_dev->dev, "get tty failed for test\n");
        filp_close(sfm17_dev->uart_filp, NULL);
        sfm17_dev->uart_filp = NULL;
        return -EIO;
    }

    // 保存原有配置
    sfm17_dev->old_termios = tty->termios;

    // 配置串口参数（115200 8N1）
    new_termios = tty->termios;
    new_termios.c_cflag = B115200 | CS8 | CLOCAL | CREAD;
    new_termios.c_iflag = IGNPAR;
    new_termios.c_oflag = 0;
    new_termios.c_lflag = 0;
    new_termios.c_cc[VTIME] = 0;
    new_termios.c_cc[VMIN] = 1;
    tty_set_termios(tty, &new_termios);

    dev_info(sfm17_dev->dev, "uart configured for test: 115200 8N1\n");

    // 等待模块上电稳定（500ms）
    msleep(500);
    dev_info(sfm17_dev->dev, "waiting for module power-up stabilization...\n");

    // 发送蓝色LED命令验证通信
    ret = sfm17_set_ring_color(RING_BLUE, RING_BLUE, 500);
    if (ret == 0) {
        dev_info(sfm17_dev->dev, "module connection test SUCCESS - blue LED should be ON\n");
    } else {
        dev_warn(sfm17_dev->dev, "module connection test FAILED - check wiring and power\n");
    }

    // 恢复串口配置并关闭
    tty_set_termios(tty, &sfm17_dev->old_termios);
    filp_close(sfm17_dev->uart_filp, NULL);
    sfm17_dev->uart_filp = NULL;

    return ret;
}

// 用户空间读写接口：通过命令字触发对应功能
#define CMD_USER_REG 0x01    // 用户空间注册命令
#define CMD_USER_VERIFY 0x02 // 用户空间比对命令
#define CMD_USER_DEL_ALL 0x03// 用户空间删除所有命令
#define CMD_USER_GET_CNT 0x04// 用户空间获取用户数命令
#define CMD_USER_BREAK 0x05  // 用户空间打断命令
#define CMD_USER_SET_LED_BLUE 0x06 // 用户空间设置LED蓝色命令

static ssize_t sfm17_write(struct file *filp, const char __user *buf, size_t size, loff_t *off)
{
    unsigned char cmd;
    int ret;

    if (size != 1) {
        dev_err(sfm17_dev->dev, "only support 1-byte cmd\n");
        return -EINVAL;
    }

    // 从用户空间获取命令字
    if (copy_from_user(&cmd, buf, 1)) {
        dev_err(sfm17_dev->dev, "copy from user failed\n");
        return -EFAULT;
    }

    // 执行对应功能
    mutex_lock(&sfm17_dev->lock);
    switch (cmd) {
        case CMD_USER_REG:
            ret = sfm17_reg_3c3r();
            break;
        case CMD_USER_VERIFY:
            ret = sfm17_verify_1vn();
            break;
        case CMD_USER_DEL_ALL:
            ret = sfm17_del_all();
            break;
        case CMD_USER_GET_CNT:
            ret = sfm17_get_user_cnt();
            break;
        case CMD_USER_BREAK:
            ret = sfm17_send_break();
            break;
        case CMD_USER_SET_LED_BLUE:
            ret = sfm17_set_ring_color(RING_BLUE, RING_BLUE, 500);
            if (ret == 0) {
                dev_info(sfm17_dev->dev, "LED set to BLUE by user command\n");
            }
            break;
        default:
            dev_err(sfm17_dev->dev, "invalid cmd: 0x%x\n", cmd);
            ret = -EINVAL;
            break;
    }
    mutex_unlock(&sfm17_dev->lock);

    return ret < 0 ? ret : 1; // 成功返回1字节（命令执行状态）
}

// 配置串口参数（115200 8N1，兼容4.1.15内核）
static int sfm17_config_uart(void)
{
    struct tty_struct *tty;
    struct ktermios new_termios;

    if (!sfm17_dev->uart_filp)
        return -EINVAL;

    // 获取tty结构体
    tty = ((struct tty_file_private *)sfm17_dev->uart_filp->private_data)->tty;
    if (!tty) {
        dev_err(sfm17_dev->dev, "get tty failed\n");
        return -EIO;
    }

    dev_info(sfm17_dev->dev, "tty structure validated (ptr=%p)\n", tty);

    // 保存原有配置（4.1.15内核中termios直接访问，无需mutex）
    sfm17_dev->old_termios = tty->termios;

    // 配置新的串口参数
    new_termios = tty->termios;
    new_termios.c_cflag = B115200 | CS8 | CLOCAL | CREAD; // 115200 8N1
    new_termios.c_iflag = IGNPAR; // 忽略奇偶校验错误
    new_termios.c_oflag = 0;      // 原始输出模式
    new_termios.c_lflag = 0;      // 原始输入模式
    new_termios.c_cc[VTIME] = 0;  // 无超时
    new_termios.c_cc[VMIN] = 1;   // 至少接收1字节

    // 应用新配置
    tty_set_termios(tty, &new_termios);

    dev_info(sfm17_dev->dev, "UART config applied: baudrate=115200, format=8N1, mode=blocking\n");
    dev_info(sfm17_dev->dev, "termios: c_cflag=0x%x, c_iflag=0x%x, VMIN=%d, VTIME=%d\n",
             new_termios.c_cflag, new_termios.c_iflag, 
             new_termios.c_cc[VMIN], new_termios.c_cc[VTIME]);

    return 0;
}

// 打开设备：初始化串口与GPIO（可选，使用filp_open接口）
static int sfm17_open(struct inode *inode, struct file *filp)
{
    int ret;

    // 打开串口设备（对应UART2，/dev/ttymxc1，使用阻塞模式）
    sfm17_dev->uart_filp = filp_open("/dev/ttymxc1", O_RDWR | O_NOCTTY, 0);
    if (IS_ERR(sfm17_dev->uart_filp)) {
        ret = PTR_ERR(sfm17_dev->uart_filp);
        dev_err(sfm17_dev->dev, "open /dev/ttymxc1 failed, ret=%d\n", ret);
        sfm17_dev->uart_filp = NULL;
        return ret;
    }

    dev_info(sfm17_dev->dev, "uart /dev/ttymxc1 opened successfully\n");

    // 配置串口参数（115200 8N1）
    ret = sfm17_config_uart();
    if (ret != 0) {
        dev_err(sfm17_dev->dev, "config uart failed, ret=%d\n", ret);
        filp_close(sfm17_dev->uart_filp, NULL);
        sfm17_dev->uart_filp = NULL;
        return ret;
    }

    dev_info(sfm17_dev->dev, "uart configured: 115200 8N1\n");

    // 等待模块初始化完成（200ms）
    dev_info(sfm17_dev->dev, "waiting for module ready...\n");
    msleep(200);

    // 设备打开成功，点亮绿色LED灯环（表示驱动已就绪）
    ret = sfm17_set_ring_color(RING_GREEN, RING_GREEN, 500);
    if (ret != 0) {
        dev_warn(sfm17_dev->dev, "set ring color failed (ret=%d), but device still opened\n", ret);
    } else {
        dev_info(sfm17_dev->dev, "LED ring set to GREEN\n");
    }

    dev_info(sfm17_dev->dev, "device opened successfully\n");
    return 0;
}

// 关闭设备：释放串口与资源（使用filp_close接口）
static int sfm17_release(struct inode *inode, struct file *filp)
{
    struct tty_struct *tty;
    int ret;

    dev_info(sfm17_dev->dev, "device close sequence started\n");

    if (sfm17_dev->uart_filp) {
        // 步骤1：关闭LED灯环（在UART关闭之前）
        dev_info(sfm17_dev->dev, "step 1: sending LED off command before UART close\n");
        ret = sfm17_set_ring_color(RING_OFF, RING_OFF, 500);
        if (ret != 0) {
            dev_warn(sfm17_dev->dev, "LED off command failed (ret=%d) - continuing with close sequence\n", ret);
        } else {
            dev_info(sfm17_dev->dev, "LED off command successful - LED ring turned off\n");
        }
        
        // 步骤2：恢复串口配置
        dev_info(sfm17_dev->dev, "step 2: restoring UART configuration\n");
        tty = ((struct tty_file_private *)sfm17_dev->uart_filp->private_data)->tty;
        if (tty) {
            tty_set_termios(tty, &sfm17_dev->old_termios);
            dev_info(sfm17_dev->dev, "UART configuration restored to original settings\n");
        }
        
        // 步骤3：关闭串口
        dev_info(sfm17_dev->dev, "step 3: closing UART interface\n");
        filp_close(sfm17_dev->uart_filp, NULL);
        sfm17_dev->uart_filp = NULL;
        dev_info(sfm17_dev->dev, "UART interface closed successfully\n");
    } else {
        dev_info(sfm17_dev->dev, "UART was not open - no cleanup needed\n");
    }
    
    dev_info(sfm17_dev->dev, "device close sequence completed successfully\n");
    return 0;
}

// 字符设备操作集
static const struct file_operations sfm17_fops = {
    .owner = THIS_MODULE,
    .open = sfm17_open,
    .write = sfm17_write,
    .release = sfm17_release,
};

// 设备树匹配表（可选：通过设备树配置GPIO/串口）
static const struct of_device_id sfm17_of_match[] = {
    { .compatible = "sfm,sfm17" },
    { }
};
MODULE_DEVICE_TABLE(of, sfm17_of_match);

// 模块初始化
static int __init sfm17_init(void)
{
    int ret;

    // 分配设备私有数据
    sfm17_dev = kzalloc(sizeof(struct sfm17_dev), GFP_KERNEL);
    if (!sfm17_dev)
        return -ENOMEM;

    // 初始化互斥锁（允许睡眠）
    mutex_init(&sfm17_dev->lock);

    // 初始化串口文件指针
    sfm17_dev->uart_filp = NULL;

    // 注册字符设备（动态分配主设备号）
    sfm17_dev->major = register_chrdev(0, DEV_NAME, &sfm17_fops);
    if (sfm17_dev->major < 0) {
        dev_err(NULL, "register chrdev failed\n");
        ret = sfm17_dev->major;
        goto err_free_dev;
    }

    // 创建设备类与设备节点
    sfm17_dev->cls = class_create(THIS_MODULE, DEV_CLASS);
    if (IS_ERR(sfm17_dev->cls)) {
        dev_err(NULL, "class create failed\n");
        ret = PTR_ERR(sfm17_dev->cls);
        goto err_unregister_chrdev;
    }

    sfm17_dev->dev = device_create(sfm17_dev->cls, NULL, MKDEV(sfm17_dev->major, 0), NULL, DEV_NAME);
    if (IS_ERR(sfm17_dev->dev)) {
        dev_err(NULL, "device create failed\n");
        ret = PTR_ERR(sfm17_dev->dev);
        goto err_destroy_class;
    }

    dev_info(NULL, "sfm17 fingerprint driver init success, /dev/%s created\n", DEV_NAME);

    // 测试模块连接（设置蓝色LED验证硬件连接）
    ret = sfm17_test_module_connection();
    if (ret == 0) {
        dev_info(NULL, "module connection verified - blue LED is ON\n");
    } else {
        dev_warn(NULL, "module connection test failed (ret=%d) - driver loaded but check hardware\n", ret);
        // 继续加载驱动，即使测试失败（可能是模块未上电或接线问题）
    }

    return 0;

err_destroy_class:
    class_destroy(sfm17_dev->cls);
err_unregister_chrdev:
    unregister_chrdev(sfm17_dev->major, DEV_NAME);
err_free_dev:
    kfree(sfm17_dev);
    return ret;
}

// 模块卸载
static void __exit sfm17_exit(void)
{
    // 释放设备节点与类
    device_destroy(sfm17_dev->cls, MKDEV(sfm17_dev->major, 0));
    class_destroy(sfm17_dev->cls);

    // 注销字符设备
    unregister_chrdev(sfm17_dev->major, DEV_NAME);

    // 清理互斥锁
    mutex_destroy(&sfm17_dev->lock);

    // 释放私有数据
    kfree(sfm17_dev);

    dev_info(NULL, "sfm17 fingerprint driver exit success\n");
}

module_init(sfm17_init);
module_exit(sfm17_exit);

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("SFM-V1.7 Fingerprint Module Driver (compatible with 4.1.15 kernel)");
MODULE_AUTHOR("hhs");
MODULE_ALIAS("sfm17");