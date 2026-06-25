# Makefile for sfm17_test application

# 使用Yocto SDK的ARM交叉编译器（GCC 5.3.0，与内核编译保持一致）
CROSS_COMPILE ?= /opt/fsl-imx-x11/4.1.15-2.1.0/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-
CC := $(CROSS_COMPILE)gcc

# Yocto SDK sysroot路径（ARM目标系统的根文件系统）
SYSROOT ?= /opt/fsl-imx-x11/4.1.15-2.1.0/sysroots/cortexa7hf-neon-poky-linux-gnueabi

# 架构指定为ARM
ARCH ?= arm

# 编译选项（添加sysroot支持和硬浮点配置）
CFLAGS := -Wall -O2 --sysroot=$(SYSROOT) -mfloat-abi=hard -mfpu=neon
LDFLAGS := --sysroot=$(SYSROOT) -mfloat-abi=hard

# 目标文件
TARGET := sfm17_test
SRCS := sfm17_test.c
OBJS := $(SRCS:.c=.o)

# 默认目标
all: $(TARGET)

# 链接生成可执行文件
$(TARGET): $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^
	@echo "Build complete: $(TARGET)"

# 编译源文件
%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

# 清理
clean:
	rm -f $(TARGET) $(OBJS)
	@echo "Clean complete"

# 安装到目标设备（可选）
install: $(TARGET)
	@echo "Please manually copy $(TARGET) to your target device"

# 显示帮助信息
help:
	@echo "Usage:"
	@echo "  make              - Build the application"
	@echo "  make clean        - Remove built files"
	@echo "  make install      - Show install instructions"
	@echo ""
	@echo "Cross-compile toolchain: $(CROSS_COMPILE)"
	@echo "Architecture: $(ARCH)"

.PHONY: all clean install help
