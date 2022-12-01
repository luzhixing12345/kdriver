---
sort: 2
---

# 01-模块

## 代码部分

```c

#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("PD");

static int hello_init(void)
{
	printk("hello_init \n");
	return 0;
}
static void hello_exit(void)
{
	printk("hello_exit \n");
	return;
}

module_init(hello_init); //insmod
module_exit(hello_exit);//rmmod

```

模块的加载如上述代码所示,其中 `MODULE_LICENSE` 和 `MODULE_AUTHOR` 用于说明开源协议和作者,为可选项,建议加上

应使用 `printk` 作为内核模块的输出显示

最后将对应函数绑定到模块进入和推出

```Makefile
ifneq ($(KERNELRELEASE),)
obj-m:=hello.o
else
KDIR :=/lib/modules/$(shell uname -r)/build
PWD  :=$(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm -f *.ko *.o *.mod.o *.symvers *.cmd  *.mod.c *.order
endif
```

Makefile文件一共执行三次,第一次 `KERNELRELEASE` 宏为false,执行else

`uname -r` 的返回值为linux内核路径的文件夹名字,接着进入该路径执行linux内核的modules目标,M带入当前路径

第二次进入使用gcc生成hello.o

第三次进入生成其余文件,其中 `hello.ko`(kernel object) 为模块文件

## 运行

```bash
sudo su
make
demesg -c # 删除其余信息
insmod hello.ko # 导入模块
dmesg
lsmod # 显示所有模块
# lsmod hello
rmmod hello.ko # 删除模块
```
