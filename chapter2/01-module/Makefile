
MODULE_NAME = hello
MODULE_OBJS = hellox.o func.o

ifneq ($(KERNELRELEASE),)
obj-m := $(MODULE_NAME).o
$(MODULE_NAME)-objs := $(MODULE_OBJS)
else
# KDIR := /home/kamilu/Downloads/linux-4.19.83
KDIR := /lib/modules/$(shell uname -r)/build
PWD  := $(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules 
clean:
	@rm -f *.ko *.o *.mod.o *.symvers *.mod.c *.order .*.cmd .cache.mk
endif
