/*  
 *一口Linux
 *2021.6.21
 *version: 1.0.0
*/
#include <linux/init.h>
#include <linux/module.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("PD");

static int num = 100;
static void show(void)
{
	printk("aaaa:  num =%d \n",num);
}
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
EXPORT_SYMBOL(num);
EXPORT_SYMBOL(show);
module_init(hello_init);
module_exit(hello_exit);