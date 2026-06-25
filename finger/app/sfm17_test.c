#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <termios.h>

#define DEV_PATH "/dev/sfm17"

// 命令字定义（与驱动一致）
#define CMD_REG 0x01
#define CMD_VERIFY 0x02
#define CMD_DEL_ALL 0x03
#define CMD_GET_CNT 0x04
#define CMD_BREAK 0x05
#define CMD_SET_LED_BLUE 0x06  // 新增：设置LED为蓝色

// 设置终端为非阻塞模式
static struct termios orig_termios;

void disable_raw_mode() {
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

void enable_raw_mode() {
    struct termios raw;
    
    tcgetattr(STDIN_FILENO, &orig_termios);
    atexit(disable_raw_mode);
    
    raw = orig_termios;
    raw.c_lflag &= ~(ICANON | ECHO);
    raw.c_cc[VMIN] = 1;  // 至少读取1个字符
    raw.c_cc[VTIME] = 0; // 无超时，阻塞等待
    
    tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

void print_led_verification_guide() {
    printf("\n========================================\n");
    printf("      LED Verification Guide\n");
    printf("========================================\n");
    printf("Expected LED behavior:\n");
    printf("\n1. DRIVER LOAD (insmod):\n");
    printf("   - LED turns BLUE (solid)\n");
    printf("   - Indicates module connection OK\n");
    printf("   - Check: dmesg | grep 'LED status'\n");
    printf("\n2. DEVICE OPEN (/dev/sfm17):\n");
    printf("   - LED turns GREEN (solid)\n");
    printf("   - Indicates ready for operations\n");
    printf("   - Check: dmesg | grep 'LED command'\n");
    printf("\n3. DEVICE CLOSE:\n");
    printf("   - LED turns OFF\n");
    printf("   - Indicates device closed\n");
    printf("\n4. DRIVER UNLOAD (rmmod):\n");
    printf("   - LED remains OFF\n");
    printf("   - Module powered down\n");
    printf("\nTroubleshooting:\n");
    printf("- No LED change: Check UART connection\n");
    printf("- Wrong color: Check kernel log for errors\n");
    printf("- LED stuck: Power cycle the module\n");
    printf("========================================\n");
}

void print_menu() {
    printf("\n");
    printf("========================================\n");
    printf("    SFM17 Fingerprint Module Test\n");
    printf("========================================\n");
    printf("  1 - Register fingerprint (3C3R)\n");
    printf("  2 - Verify fingerprint (1:N)\n");
    printf("  3 - Delete all users\n");
    printf("  4 - Get user count\n");
    printf("  5 - Send break command\n");
    printf("  6 - Set LED to BLUE\n");
    printf("  h - Show LED verification guide\n");
    printf("  q - Quit program\n");
    printf("========================================\n");
    printf("LED Status Guide:\n");
    printf("  BLUE  - Driver loaded successfully\n");
    printf("  GREEN - Device ready for operations\n");
    printf("  OFF   - Device closed/driver unloaded\n");
    printf("========================================\n");
    printf("Select option: ");
    fflush(stdout);
}

int send_command(int fd, unsigned char cmd, const char *desc) {
    int ret;
    
    printf("\n[DEBUG] Sending command: 0x%02X (%s)\n", cmd, desc);
    
    ret = write(fd, &cmd, 1);
    if (ret < 0) {
        perror("[ERROR] write command failed");
        return -1;
    }
    
    printf("[DEBUG] Command sent successfully, ret=%d\n", ret);
    return 0;
}

int main(int argc, char *argv[])
{
    int fd, ret;
    unsigned char cmd;
    char input;
    int running = 1;

    printf("========================================\n");
    printf("  SFM17 Fingerprint Module Test Tool\n");
    printf("========================================\n");
    printf("\n[SETUP] LED Connection Verification:\n");
    printf("1. Load driver: insmod drive/sfm17_fingerprint.ko\n");
    printf("2. Check LED: Should turn BLUE when driver loads\n");
    printf("3. Check logs: dmesg | tail -10\n");
    printf("4. If LED is BLUE, module connection is verified\n");
    printf("\n[DEBUG] Opening device %s...\n", DEV_PATH);
    
    // 打开设备
    fd = open(DEV_PATH, O_RDWR);
    if (fd < 0) {
        perror("[ERROR] open /dev/sfm17 failed");
        printf("\n[TROUBLESHOOTING]\n");
        printf("1. Check if driver is loaded: lsmod | grep sfm17\n");
        printf("2. Load driver: insmod drive/sfm17_fingerprint.ko\n");
        printf("3. Check device node: ls -l /dev/sfm17\n");
        printf("4. Check kernel log: dmesg | tail -20\n");
        return -1;
    }

    printf("[DEBUG] Device opened successfully (fd=%d)\n", fd);
    printf("\n[LED STATUS] Device opened - LED should now be GREEN\n");
    printf("[INFO] If LED is not GREEN, check kernel log for errors\n");
    printf("[LOG CHECK] Run: dmesg | grep sfm17 | tail -5\n");

    // 设置终端为原始模式（单字符输入）
    enable_raw_mode();

    // 显示菜单
    print_menu();

    // 主循环
    while (running) {
        input = getchar();
        
        if (input == EOF || input == 0) {
            continue;
        }

        switch (input) {
            case '1':
                printf("\n[INFO] Starting 3C3R registration...\n");
                printf("[INFO] Please press your finger 3 times when prompted\n");
                printf("[LED] Watch for LED color changes during registration\n");
                ret = send_command(fd, CMD_REG, "Register");
                if (ret == 0) {
                    printf("[INFO] Registration command sent\n");
                    printf("[LOG CHECK] Monitor kernel log: dmesg -w | grep sfm17\n");
                    printf("[INFO] Check for registration status and LED feedback\n");
                }
                print_menu();
                break;

            case '2':
                printf("\n[INFO] Starting 1:N verification...\n");
                printf("[INFO] Please press your finger\n");
                printf("[LED] Watch for LED color changes during verification\n");
                ret = send_command(fd, CMD_VERIFY, "Verify");
                if (ret == 0) {
                    printf("[INFO] Verification command sent\n");
                    printf("[LOG CHECK] Monitor kernel log: dmesg -w | grep sfm17\n");
                    printf("[INFO] Check for match result and LED feedback\n");
                }
                print_menu();
                break;

            case '3':
                printf("\n[WARNING] Deleting all registered users...\n");
                ret = send_command(fd, CMD_DEL_ALL, "Delete All");
                if (ret == 0) {
                    printf("[INFO] Delete command sent\n");
                    printf("[LOG CHECK] Check kernel log: dmesg | grep sfm17 | tail -3\n");
                }
                print_menu();
                break;

            case '4':
                printf("\n[INFO] Getting user count...\n");
                ret = send_command(fd, CMD_GET_CNT, "Get Count");
                if (ret == 0) {
                    printf("[INFO] Get count command sent\n");
                    printf("[LOG CHECK] Check kernel log: dmesg | grep sfm17 | tail -3\n");
                    printf("[INFO] Look for user count in kernel messages\n");
                }
                print_menu();
                break;

            case '5':
                printf("\n[INFO] Sending break command...\n");
                ret = send_command(fd, CMD_BREAK, "Break");
                if (ret == 0) {
                    printf("[INFO] Break command sent\n");
                    printf("[LOG CHECK] Check kernel log: dmesg | grep sfm17 | tail -3\n");
                }
                print_menu();
                break;

            case '6':
                printf("\n[INFO] Setting LED to BLUE...\n");
                ret = send_command(fd, CMD_SET_LED_BLUE, "Set LED Blue");
                if (ret == 0) {
                    printf("[INFO] LED set to BLUE command sent\n");
                    printf("[LOG CHECK] Check kernel log: dmesg | grep sfm17 | tail -3\n");
                    printf("[INFO] LED should now be BLUE\n");
                }
                print_menu();
                break;

            case 'q':
            case 'Q':
                printf("\n[INFO] Exiting program...\n");
                running = 0;
                break;

            case 'h':
            case 'H':
                print_led_verification_guide();
                print_menu();
                break;

            case '\n':
            case '\r':
                // 忽略换行符
                break;

            default:
                printf("\n[WARNING] Invalid option: '%c'\n", input);
                print_menu();
                break;
        }
    }

    // 关闭设备前，先将LED设置为蓝色
    printf("\n[DEBUG] Setting LED to BLUE before closing device...\n");
    ret = send_command(fd, CMD_SET_LED_BLUE, "Set LED Blue");
    if (ret == 0) {
        printf("[INFO] LED set to BLUE command sent\n");
        printf("[LOG CHECK] Check kernel log: dmesg | grep sfm17 | tail -3\n");
        // 等待命令执行完成
        usleep(500000); // 500ms
    }
    
    // 关闭设备
    printf("\n[DEBUG] Closing device...\n");
    close(fd);
    printf("\n[LED STATUS] Device closed - LED should now be BLUE\n");
    printf("[INFO] LED remains BLUE to indicate driver is still loaded\n");
    printf("[LOG CHECK] Final check: dmesg | grep sfm17 | tail -5\n");
    printf("\n[CLEANUP] To unload driver: rmmod sfm17_fingerprint\n");
    printf("[INFO] LED should turn OFF when driver is unloaded\n");
    printf("[INFO] Program terminated\n");

    return 0;
}