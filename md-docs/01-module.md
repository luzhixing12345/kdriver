
# 01-module

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

printk和printf是用于在Linux系统中输出调试信息的两个函数，它们之间有以下区别

- printk是在内核空间中使用的函数，而printf是在用户空间中使用的函数。
- printk可以输出到系统日志（system log）或控制台（console），以帮助开发人员进行内核级别的调试，而printf通常输出到控制台，以帮助开发人员进行用户空间级别的调试。
- printk函数具有八个不同的日志级别（log level），可以根据不同的调试需求选择不同的日志级别，从而控制信息的显示和过滤。而printf函数没有这种灵活性，无法按照日志级别控制输出。
- 由于printk是在内核空间中使用的函数，因此需要小心使用，避免在过多输出调试信息时导致系统资源的浪费和性能问题。相比之下，printf可以更自由地使用，因为用户空间相对于内核空间更加容错。

printk和printf有着不同的使用场景和特点，可以根据需要灵活使用。如果需要在内核空间中进行调试，则应该使用printk；如果需要在用户空间中进行调试，则应该使用printf. 我们的驱动代码运行在内核态,均使用printk进行日志记录

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

在运行的时候主要需要使用到如下的命令

- su : 切换到root
- dmesg -c : 清除内核环缓存（kernel ring buffer）中的消息，这些消息通常由内核和系统的组件生成。使用该命令可以清除已经记录的消息，以便在控制台或日志中减少垃圾消息的显示。该命令通常在调试和故障排除期间使用，以便清除旧的消息，并在下一次记录消息时获得干净的环境
- insmod: 向内核加载一个新的模块。它将指定的模块文件复制到内存中，并将其与内核中已经存在的代码进行连接。如果模块文件中的任何依赖项尚未加载，则insmod会自动加载它们。通过加载内核模块，可以向Linux内核添加新功能或驱动程序，而无需重新编译整个内核
- rmmod: 从内核中卸载已经加载的模块。当不再需要一个模块时，可以使用该命令将其从内存中删除，以释放资源并减少内核的大小。如果正在使用该模块，则必须先停止使用该模块，然后才能将其卸载
- lsmod: 列出当前已经加载的内核模块。可以通过查看lsmod输出中的模块列表来确定哪些模块可以使用rmmod卸载

> 以上命令都需要root权限

```bash
sudo su
make
dmesg -c # 删除其余信息
insmod hello.ko # 导入模块
dmesg
lsmod # 显示所有模块
# lsmod hello
rmmod hello.ko # 删除模块
```

```bash
root@kamilu:/home/kamilu/Desktop/linux-driver/src/01-module# insmod hello.ko
insmod: ERROR: could not insert module hello.ko: Key was rejected by service
root@kamilu:/home/kamilu/Desktop/linux-driver/src/01-module# dmesg
[ 1920.607501] Loading of unsigned module is rejected
```