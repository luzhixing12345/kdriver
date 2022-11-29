---
sort: 1
---

# 开始

参考 [一口Linux](https://space.bilibili.com/661326452)

## 下载Linux内核源码

[下载地址](https://links.jianshu.com/go?to=http%3A%2F%2Fftp.sjtu.edu.cn%2Fsites%2Fftp.kernel.org%2Fpub%2Flinux%2Fkernel%2F)

笔者使用的是 Linux3.14.0-fs4412, 你可以在 [这里下载](https://github.com/luzhixing12345/LinuxDriver/releases/download/v0.0.1/linux-3.14.tar.xz)

## 配置虚拟机, Ubuntu16.04

VMWare Work statation + Ubuntu16.04 iso

默认完成安装,略

## 配置开发环境

```bash
sudo apt-get install lsb-core
```

[下载gcc4.6.4](https://github.com/luzhixing12345/LinuxDriver/releases/download/v0.0.1/gcc-4.6.4.tar.xz)

将解压后将bin目录加入环境变量

```bash
sudo vim /etc/bash.bashrc
export  PATH=$PATH:/home/peng/toolchain/gcc-4.6.4/bin
source /etc/bash.bashrc
```

测试

```bash
arm-none-linux-gnueabi-gcc  –v 
```

至此完成基础环境搭建
