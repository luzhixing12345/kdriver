---
sort: 4
---

# 模块传参

如果模块所有的变量内容都是固定的那显然是很死板的,linux内核也提供了动态的调整指定参数的方式

`includel/linux/Moduleparam.h`

函数原型 : `module_param(name,type,perm)`

- name: 用来接收参数的变量名
- type: 参数的数据类型

  linux内核支持的模块参数类型: bool invbool charp int long short uint ulong ushort

- perm: 参数的访问权限

其他函数原型: `module_param_string(name,string,len,perm);` `module_param_arry(name,type,num_point,perm);` `module_param_named(name, value, type, perm)`

```c
#include <linux/init.h>
#include <linux/module.h>

static char *whom = "hello \n";
static int var = 1;

static int hello_init(void)
{
	printk("hello_init %s \n",whom);
	return 0;
}
static void hello_exit(void)
{
	printk("hello_exit  %d\n",var);
	return;
}
MODULE_LICENSE("GPL");
module_param(whom,charp,0644);
module_param_named(var_out,var,int,0644);

module_init(hello_init);
module_exit(hello_exit);
```

这里定义了一个字符串 char* whom 和一个变量 int var,然后对于whom使用 `module_param` 定义为参数 ,对于var使用`module_param_named`重命名为`var_out`然后定义为参数

使用make编译之后insmod rmmod会得到如下输出结果,其中可以看到 `whom = hello` `var = 1`,如定义所示

```bash
insmod hello.ko
rmmod hello
```

```bash
[ 5032.974316] hello: loading out-of-tree module taints kernel.
[ 5032.974362] hello: module verification failed: signature and/or required key missing - tainting kernel
[ 5032.976327] hello_init hello 
                
[ 5067.147675] hello_exit  1
```

如果在导入模块的时候修改参数

```bash
insmod hello.ko var_out=99 hello="kamilu"
```

```bash
[ 5255.350463] hello_init kamilu 
[ 5263.988903] hello_exit  99
```

## perm

perm为执行的权限,前文代码中可以看到使用了 `0644`

linux内核中有一些很重要的资源信息借助虚拟文件系统的形式呈现给我们,实际上当我们加载模块之后,我们就可以在`/sys/module/`路径下找到我们刚刚加载的模块`hello`,linux会为每一个模块创建一个目录,在parameters目录下就可以看到我们的两个参数

```bash
root@ubuntu:/sys/module/hello/parameters# ls -l
total 0
-rw-r--r-- 1 root root 4096 Dec  4 06:06 var_out
-rw-r--r-- 1 root root 4096 Dec  4 06:06 whom
```

sysfs: 内核给一些重要的资源创建的目录或文件,每一个模块会在 /sys/module 下创建一个文件 