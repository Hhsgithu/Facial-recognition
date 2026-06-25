#include <linux/module.h>
#include <linux/platform_device.h>
#include <linux/pwm.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/slab.h>
#include <linux/of.h>

// SG90参数：20ms周期（50Hz），0°=0.5ms(2.5%)，90°=1.5ms(7.5%)，180°=2.5ms(12.5%)
#define SG90_PERIOD_NS    20000000  // PWM周期：20ms
#define SG90_MIN_DUTY_NS  500000    // 0°占空比：0.5ms
#define SG90_MAX_DUTY_NS  2500000   // 180°占空比：2.5ms
#define SG90_DEV_NAME     "sg90_servo"
#define SG90_DEV_CNT      1

// 设备结构体（封装所有资源）
struct sg90_dev {
    dev_t devid;            // 设备号
    struct cdev cdev;       // 字符设备
    struct class *class;    // 类
    struct device *device;  // 设备
    struct pwm_device *pwm; // PWM设备
};
static struct sg90_dev *sg90_devp;

// 应用层写函数：接收角度，设置PWM占空比
static ssize_t sg90_write(struct file *filp, const char __user *buf, size_t count, loff_t *ppos) {
    char angle_buf[4];
    int angle, duty_ns;
    struct sg90_dev *dev = filp->private_data;

    // 拷贝应用层数据到内核
    if (copy_from_user(angle_buf, buf, count)) {
        return -EFAULT;
    }
    angle = simple_strtoul(angle_buf, NULL, 10);

    // 角度范围校验（0~180°）
    if (angle < 0) angle = 0;
    if (angle > 180) angle = 180;

    // 角度转占空比（线性映射）
    duty_ns = SG90_MIN_DUTY_NS + (angle * (SG90_MAX_DUTY_NS - SG90_MIN_DUTY_NS)) / 180;

    // 配置并启用PWM
    pwm_config(dev->pwm, duty_ns, SG90_PERIOD_NS);
    pwm_enable(dev->pwm);

    return count;
}

// 应用层打开函数：绑定私有数据
static int sg90_open(struct inode *inode, struct file *filp) {
    filp->private_data = sg90_devp;
    return 0;
}

// 应用层释放函数：关闭PWM
static int sg90_release(struct inode *inode, struct file *filp) {
    struct sg90_dev *dev = filp->private_data;
    pwm_disable(dev->pwm);
    return 0;
}

// 文件操作集合
static const struct file_operations sg90_fops = {
    .owner = THIS_MODULE,
    .open = sg90_open,
    .write = sg90_write,
    .release = sg90_release,
};

// 平台驱动probe函数：匹配设备树后初始化
static int sg90_probe(struct platform_device *pdev) {
    int ret;
    struct device_node *node = pdev->dev.of_node;

    // 1. 分配设备结构体内存
    sg90_devp = devm_kzalloc(&pdev->dev, sizeof(struct sg90_dev), GFP_KERNEL);
    if (!sg90_devp) return -ENOMEM;

    // 2. 动态申请设备号
    ret = alloc_chrdev_region(&sg90_devp->devid, 0, SG90_DEV_CNT, SG90_DEV_NAME);
    if (ret < 0) {
        dev_err(&pdev->dev, "alloc chrdev region failed!\n");
        return ret;
    }

    // 3. 初始化cdev并添加到内核
    cdev_init(&sg90_devp->cdev, &sg90_fops);
    sg90_devp->cdev.owner = THIS_MODULE;
    ret = cdev_add(&sg90_devp->cdev, sg90_devp->devid, SG90_DEV_CNT);
    if (ret < 0) {
        unregister_chrdev_region(sg90_devp->devid, SG90_DEV_CNT);
        dev_err(&pdev->dev, "cdev add failed!\n");
        return ret;
    }

    // 4. 创建类和设备（自动生成/dev/sg90_servo）
    sg90_devp->class = class_create(THIS_MODULE, "sg90_class");
    if (IS_ERR(sg90_devp->class)) {
        cdev_del(&sg90_devp->cdev);
        unregister_chrdev_region(sg90_devp->devid, SG90_DEV_CNT);
        return PTR_ERR(sg90_devp->class);
    }
    sg90_devp->device = device_create(sg90_devp->class, NULL, sg90_devp->devid, NULL, SG90_DEV_NAME);
    if (IS_ERR(sg90_devp->device)) {
        class_destroy(sg90_devp->class);
        cdev_del(&sg90_devp->cdev);
        unregister_chrdev_region(sg90_devp->devid, SG90_DEV_CNT);
        return PTR_ERR(sg90_devp->device);
    }

    // 5. 从设备树获取PWM设备
    sg90_devp->pwm = of_pwm_get(node, NULL);
    if (IS_ERR(sg90_devp->pwm)) {
        dev_err(&pdev->dev, "get pwm failed!\n");
        device_destroy(sg90_devp->class, sg90_devp->devid);
        class_destroy(sg90_devp->class);
        cdev_del(&sg90_devp->cdev);
        unregister_chrdev_region(sg90_devp->devid, SG90_DEV_CNT);
        return PTR_ERR(sg90_devp->pwm);
    }

    dev_info(&pdev->dev, "sg90 pwm driver probe success!\n");
    return 0;
}

// 平台驱动remove函数：释放资源
static int sg90_remove(struct platform_device *pdev) {
    // 禁用PWM
    pwm_disable(sg90_devp->pwm);
    // 释放PWM设备
    pwm_put(sg90_devp->pwm);
    // 注销字符设备
    cdev_del(&sg90_devp->cdev);
    unregister_chrdev_region(sg90_devp->devid, SG90_DEV_CNT);
    // 删除类和设备
    device_destroy(sg90_devp->class, sg90_devp->devid);
    class_destroy(sg90_devp->class);

    dev_info(&pdev->dev, "sg90 pwm driver remove success!\n");
    return 0;
}

// 设备树匹配表（与设备树中sg90_servo节点的compatible匹配）
static const struct of_device_id sg90_of_match[] = {
    { .compatible = "atk,sg90-pwm" },
    {},
};
MODULE_DEVICE_TABLE(of, sg90_of_match);

// 平台驱动结构体
static struct platform_driver sg90_driver = {
    .probe = sg90_probe,
    .remove = sg90_remove,
    .driver = {
        .name = "sg90_pwm_driver",
        .of_match_table = sg90_of_match,
        .owner = THIS_MODULE,
    },
};

// 模块加载/卸载
module_platform_driver(sg90_driver);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Your Name");
MODULE_DESCRIPTION("SG90 Servo PWM Driver for i.MX6ULL");
MODULE_ALIAS("platform:sg90_pwm_driver");