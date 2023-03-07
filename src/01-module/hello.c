
#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("kamilu");
MODULE_INFO(vermagic, "4.15.0-112-generic SMP mod_unload ");

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
