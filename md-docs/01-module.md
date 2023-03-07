
# 01-module

## 代码部分

```c
#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("kamilu");

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

## 编译模块与装载

在编译与装载的时候主要需要使用到如下的命令

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
root@ubuntu:/home/lzx/Driver/module# dmesg -c
[  617.165226] hello: loading out-of-tree module taints kernel.
[  617.165343] hello: module verification failed: signature and/or required key missing - tainting kernel
[  617.166355] hello_init 
[  804.914998] hello_exit
```

## 跨内核版本

### 问题描述

上述我们在编写makefile的时候使用的是Ubuntu系统提供的Linux header: `/lib/modules/$(shell uname -r)/build`

但是这样编译出的驱动模块是不跨版本的,也就是说如果我们将这个驱动模块使用insmod装载到另一个kernel中就会发现被拒绝了

比如我们修改一下Makefile

```Makefile
ifneq ($(KERNELRELEASE),)
obj-m:=hello.o
else
KDIR := /home/kamilu/Downloads/linux-4.19.83
# KDIR := /lib/modules/$(shell uname -r)/build
PWD  :=$(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm -f *.ko *.o *.mod.o *.symvers *.cmd  *.mod.c *.order
endif
```

我们将 `KDIR` 替换为了一个已经编译好的内核,版本是4.19.83, 此时编译可以通过,但是装载的时候就会出现问题

```bash
root@ubuntu:/home/kamilu/linux-driver/src/01-module# insmod hello.ko
insmod: ERROR: could not insert module hello.ko: Invalid module format
```

这是因为编译驱动的内核版本和当前运行的内核版本不匹配, 我们可以查看 /var/log/kern.log 文件的日志信息

```bash
root@ubuntu:/home/kamilu/linux-driver/src/01-module# tail -n 5 /var/log/kern.log
Mar  7 00:11:20 ubuntu NetworkManager[853]: <info>  [1678176680.2353]   wins '192.168.232.2'
Mar  7 00:11:20 ubuntu NetworkManager[853]: <info>  [1678176680.2353] dhcp4 (ens33): state changed bound -> bound
Mar  7 00:11:58 ubuntu kernel: [17716.315596] hello_init
Mar  7 00:12:21 ubuntu kernel: [17739.215909] hello_exit
Mar  7 00:12:53 ubuntu kernel: [17771.091891] hello: version magic '4.19.83 SMP mod_unload ' should be '4.15.0-112-generic SMP mod_unload '
```

这里提示 `4.19.83 SMP mod_unload ` 版本和 `4.15.0-112-generic SMP mod_unload ` 版本不匹配

### 解决方法

解决这个问题有三种方法, 第一种方法是最可靠的, 就是使用与内核版本相同的Linux header库进行编译

第二种方式是使用宏 `MODULE_INFO`, MODULE_INFO是一个用于向内核模块添加元信息的宏，它不是一个C语言中的变量，而是一个预处理器指令，它会在编译时被替换为一些特定的代码，用于向.modinfo段中插入一些键值对，例如MODULE_INFO(vermagic, "4.15.0-112-generic SMP mod_unload ")就会插入vermagic=4.15.0-112-generic SMP mod_unload这样的信息。这些信息可以通过modinfo命令来查看，也可以用于内核模块的版本检查。

```c
#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("kamilu");
MODULE_INFO(vermagic, "4.15.0-112-generic SMP mod_unload ");
```

这样我们即在代码中指定了内核版本为当前运行的内核版本,通过了驱动版本检查

**但是请注意**,这种方式可能会带来一些问题, 如果驱动使用到了一些被遗弃或修改的API接口可能会出现严重的问题, 但这种方式在前期开发的时候还是带来了很大的方便

第三种方式就是直接将驱动模块编译进内核, 内核开发人员可以根据不同需求针对某个CPU家族的某种特定处理器进行优化, 从而充分利用目标平台的特性

最好的办法是使用GPL许可发布驱动,将其贡献给内核主分支, 这是最容易被其他人接受的方式

## 多文件编译

对于单个文件 hello.c 直接使用 `obj-m := hello.o` 即可

```Makefile
ifneq ($(KERNELRELEASE),)
obj-m:=hello.o
else
# KDIR := /home/kamilu/Downloads/linux-4.19.83
KDIR := /lib/modules/$(shell uname -r)/build
PWD  :=$(shell pwd)
all:
	make -C $(KDIR) M=$(PWD) modules
clean:
	rm -f *.ko *.o *.mod.o *.symvers *.cmd  *.mod.c *.order
endif
```

如果是使用多文件编程则需要稍作修改

```Makefile
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
```

这里的 hello.o 代表模块名, 对应的模块为 `hello-objs`, 由 hellox.c func.c 两个文件编译得到

注意这里的模块名是 hello, 编译所需的两个文件都不能叫hello否则会重名出现错误

## 参考

- [Linux驱动实践：带你一步一步编译内核驱动程序](https://blog.csdn.net/weixin_60043341/article/details/126386952)