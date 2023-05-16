# SPDX-License-Identifier: GPL-2.0


ifneq ($(KERNELRELEASE),)

obj-m += core/
obj-m += cxl_pci.o
obj-m += cxl_mem.o
obj-m += cxl_acpi.o
obj-m += cxl_pmem.o
obj-m += cxl_port.o

cxl_mem-y := mem.o
cxl_pci-y := pci.o
cxl_acpi-y := acpi.o
cxl_pmem-y := pmem.o security.o
cxl_port-y := port.o
else
KDIR :=/lib/modules/$(shell uname -r)/build
PWD  :=$(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	@rm -f *.ko *.o *.mod.o *.symvers *.mod.c *.order .*.cmd .cache.mk
	@rm -r .tmp_versions
endif