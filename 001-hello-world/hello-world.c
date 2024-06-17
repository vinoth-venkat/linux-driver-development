#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>

static int __init hello_world_init(void)
{
	printk(KERN_INFO "This is the sample Hello World module\n");
	printk(KERN_INFO "The module is now loaded\n");
	return 0;
}

static void __exit hello_world_exit(void)
{
	printk("The module is now unloaded\n");
}

module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vinoth");
MODULE_DESCRIPTION("A sample Hello world module");
MODULE_VERSION("2:1.0");
