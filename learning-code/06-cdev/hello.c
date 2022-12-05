/*  
 *一口Linux
 *2021.6.21
 *version: 1.0.0
*/

#include <linux/module.h>
#include <linux/init.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/poll.h>
#include <linux/proc_fs.h>
#include <linux/skbuff.h>
#include <linux/seq_file.h>
#include <asm/uaccess.h>



unsigned int major = 237;


int hello_open (struct inode *inode, struct file *file)
{
	printk("hello_open()\n");
	return 0;
}

int hello_release (struct inode *inode, struct file *file)
{
	printk("hello_release()\n");
	return 0;
}

struct file_operations fops ={
	.open = hello_open,
	.release = hello_release,
};

static int hello_init(void)
{
	int ret;
	
	printk("hello_init()\n");


	ret = register_chrdev(major, "peng", &fops);
	if(ret<0)
	{
		printk("register_chrdev fail\n");
		return ret;
	}
	return 0;	
}

static void hello_exit(void)
{
	printk("hello_exit()\n");
	unregister_chrdev(major,"peng");
	return ;
}

MODULE_LICENSE("GPL");
module_init(hello_init);
module_exit(hello_exit);
