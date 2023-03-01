
# build

VMware Workstation + Ubuntu16.04 + Linux5.4.19

> 不要编译低版本内核, 不要编译...

## 基本环境搭建

编译内核的配一个Linux环境应该不是问题了,略过

**注意**, 不要选择高版本的Ubuntu,因为

如果是虚拟机的话内存(swap area)划大一些(8GB)

## 下载Linux内核源代码

本文以5.4.19版本Linux内核源代码为例, 可以在 [Linux Kernel](https://mirrors.edge.kernel.org/pub/linux/kernel/) 找到所有版本

```bash
wget https://mirrors.edge.kernel.org/pub/linux/kernel/v5.x/linux-5.4.19.tar.gz
tar xf linux-5.4.19.tar.gz
cd linux-5.4.19
```

## 安装编译依赖

```
sudo apt update
sudo apt-get install git fakeroot build-essential ncurses-dev xz-utils libssl-dev bc flex libelf-dev bison vim
```

编译选项和编译

```bash
make menuconfig
```

打开后不做修改,save,exit,得到.config文件

```bash
make -j`nproc`
```

可以得到 `vmlinux` 和 `arch/x86/boot/bzImage`

![20230301013759](https://raw.githubusercontent.com/learner-lu/picbed/master/20230301013759.png)

vmlinux和bzImage都是Linux内核编译生成的可执行文件，它们的主要区别在于它们的文件格式和用途。

vmlinux是Linux内核编译生成的未压缩的内核镜像文件，它包含了整个内核的代码和数据，可以用来调试内核。它通常比较大，因为它没有被压缩。

bzImage是Linux内核编译生成的压缩内核镜像文件，它是用来引导启动Linux操作系统的。它是通过将vmlinux压缩成一个单独的文件，并添加一些引导代码和头部信息来生成的。

当系统引导时，bzImage首先会被加载到内存中，然后被解压缩成vmlinux形式的内核映像。因为vmlinux文件比较大，而且内核启动过程中需要加载和解压缩文件，所以bzImage文件通常比vmlinux文件小很多。

因此，vmlinux主要用于内核开发和调试，而bzImage则是用于实际的Linux操作系统启动。

https://blog.csdn.net/weixin_60043341/article/details/126386952

## 可能遇到的问题

- https://blog.csdn.net/nongchaoer201012/article/details/114609518
- https://blog.csdn.net/Luckiers/article/details/124544179
- https://www.zhihu.com/search?q=%E7%BC%96%E8%AF%91linux%E5%86%85%E6%A0%B8&type=content&vertical=article