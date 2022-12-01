---
sort: 3
---

## 符号导出

Linux 内核的全局符号表在 `/usr/src/linux-headers-4.15.0-142-generic/Module.symvers` 中

模块中变量的导出可以使用 `EXPORT_SYMBOL` 宏

```c
#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("PD");

extern int num;
extern  void show(void);
static int hello_init(void)
{
	printk("hello_init %d\n",num);
	show();
	return 0;
}
static void hello_exit(void)
{
	printk("hello_exit \n");
	return;
}

module_init(hello_init);
module_exit(hello_exit);
```

进入 a 目录后编译可得导出符号表 `Module.symvers`,其中包含了这两个导出的符号及其所在目录位置

```txt
0x00000000      show    /home/lzx/Driver/export/a/helloa        EXPORT_SYMBOL
0x00000000      num     /home/lzx/Driver/export/a/helloa        EXPORT_SYMBOL
```

接着导入模块之后可以看到输出结果

```bash
dmesg -c
insmod helloa.ko
dmesg
```

```bash
[ 1156.649716] helloa: loading out-of-tree module taints kernel.
[ 1156.649759] helloa: module verification failed: signature and/or required key missing - tainting kernel
[ 1156.650809] hello_init 
```

b中的文件如下

```c
#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("PD");

extern int num;
extern void show(void);
static int hello_init(void)
{
	printk("hello_init %d\n",num);
	show();
	return 0;
}
static void hello_exit(void)
{
	printk("hello_exit \n");
	return;
}

module_init(hello_init);
module_exit(hello_exit);
```

接着将a中的导出符号表复制到b中

```bash
cp a/Module.symvers b/
```

接下来加载 hellob.ko 之后的输出结果如下

```bash
[ 1156.649716] helloa: loading out-of-tree module taints kernel.
[ 1156.649759] helloa: module verification failed: signature and/or required key missing - tainting kernel
[ 1156.650809] hello_init 
[ 1292.628304] hello_init 100
[ 1292.628305] aaaa:  num =100
```

可以看到成功加载,其中`extern int num;` `extern void show(void);` 导入了外部变量和外部函数

加载时必须先加载a后加载b

卸载时必须先卸载b模块再卸载a模块,如果先卸载a会出现如下错误

```bash
root@ubuntu:/home/lzx/Driver/export/b# rmmod helloa.ko
rmmod: ERROR: Module helloa is in use by: hellob
```

Linux 模块中有很强的依赖性,必须按顺序加载卸载
