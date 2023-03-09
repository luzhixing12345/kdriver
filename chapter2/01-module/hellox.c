
#include <linux/init.h>
#include <linux/module.h>
#include "func.h"

MODULE_LICENSE("GPL");
MODULE_AUTHOR("kamilu");
MODULE_INFO(vermagic, "4.15.0-112-generic SMP mod_unload ");

static int __init hello_init(void)  
{  
    printk(KERN_INFO "Hello world!\n");  
    printk(KERN_INFO "The value of my_variable is: %d\n", add(my_variable,20));
    return 0;  
}  

static void __exit hello_exit(void)  
{  
    printk(KERN_INFO "Goodbye world!\n");  
}  

module_init(hello_init); //insmod
module_exit(hello_exit); //rmmod
