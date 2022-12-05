/*  
 *一口Linux
 *2021.6.21
 *version: 1.0.0
*/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>

static int major = 233;
static int minor = 0;
static dev_t devno;
static struct cdev cdev;
static int hello_open (struct inode *inode, struct file *filep)
{
	printk("hello_open()\n");
	return 0;
}
static struct file_operations hello_ops = 
{
	.open = hello_open,
};
static int hello_init(void)
{
	int result;
	int error;	
	printk("hello_init \n");
	devno = MKDEV(major,minor);	
	result = register_chrdev_region(devno, 1, "test");
	if(result<0)
	{
		printk("register_chrdev_region fail \n");
		return result;
	}
	cdev_init(&cdev,&hello_ops);
	error = cdev_add(&cdev,devno,1);
	if(error < 0)
	{
		printk("cdev_add fail \n");
		unregister_chrdev_region(devno,1);
		return error;
	}
	return 0;
}
static void hello_exit(void)
{
	printk("hello_exit \n");
	cdev_del(&cdev);
	unregister_chrdev_region(devno,1);
	return;
}
module_init(hello_init);
module_exit(hello_exit);
//proc/devices
