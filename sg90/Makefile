# 内核源码路径（替换为你的实际路径）
KERNELDIR := /home/hhs/duojikaifa/linux-imx-rel_imx_4.1.15_2.1.0_ga_alientek
CURRENT_PATH := $(shell pwd)
obj-m := sg90_pwm_driver.o

# 编译规则
all:
	make -C $(KERNELDIR) M=$(CURRENT_PATH) modules ARCH=arm CROSS_COMPILE=arm-linux-gnueabihf-
clean:
	make -C $(KERNELDIR) M=$(CURRENT_PATH) clean
