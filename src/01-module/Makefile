ifneq ($(KERNELRELEASE),)
obj-m := hello.o
hello-objs := hellox.o func.o
else
# KDIR := /home/kamilu/Downloads/linux-4.19.83
KDIR := /lib/modules/$(shell uname -r)/build
PWD  :=$(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules 
clean:
	rm -f *.ko *.o *.mod.o *.symvers *.cmd  *.mod.c *.order
endif
