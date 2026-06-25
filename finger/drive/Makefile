# 驱动模块名（sfm17_fingerprint.c）
obj-m += sfm17_fingerprint.o

# 你的内核源码路径（必须正确，否则编译失败）
KERNELDIR ?= /home/hhs/chuchang/linux_kernal
# 使用Yocto SDK的ARM交叉编译器（GCC 5.3.0，与内核编译保持一致）
CROSS_COMPILE ?= /opt/fsl-imx-x11/4.1.15-2.1.0/sysroots/x86_64-pokysdk-linux/usr/bin/arm-poky-linux-gnueabi/arm-poky-linux-gnueabi-
# 架构指定为ARM
ARCH ?= arm
# 当前目录（无需修改）
PWD := $(shell pwd)

# 编译目标
all:
	# -C：进入内核源码目录；M：指定驱动源码目录；ARCH/CC：交叉编译参数
	$(MAKE) -C $(KERNELDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

# 清理目标
clean:
	$(MAKE) -C $(KERNELDIR) M=$(PWD) clean
	# 额外清理编译生成的临时文件
	rm -rf .*.cmd *.o *.mod.c *.ko .tmp_versions modules.order Module.symvers