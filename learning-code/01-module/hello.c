/*  
 *一口Linux
 *2021.6.21
 *version: 1.0.0
*/

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
