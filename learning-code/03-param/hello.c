/*  
 *一口Linux
 *2021.6.21
 *version: 1.0.0
*/

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
