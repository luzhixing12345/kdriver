/*  
 *一口Linux
 *2021.6.21
 *version: 1.0.0
*/
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>

static int major = 233;
static int minor = 0;

static dev_t devno;

static int hello_init(void)
{
	int result;
	
	printk("hello_init \n");
	devno = MKDEV(major,minor);
	
	result = register_chrdev_region(devno, 1, "test");
	if(result<0)
	{
		printk("register_chrdev_region fail \n");
		return result;
	}
	
	return 0;
}
static void hello_exit(void)
{
	printk("hello_exit \n");
	unregister_chrdev_region(devno,1);
	return;
}

module_init(hello_init);
module_exit(hello_exit);
//proc/devices