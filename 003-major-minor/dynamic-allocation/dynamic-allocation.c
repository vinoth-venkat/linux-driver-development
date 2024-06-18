#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

dev_t dev = 0;

/**************** Module Init Function ***************/
static int __init hello_world_init(void)
{
	int rev = alloc_chrdev_region(&dev, 0, 1, "Linux_dev");
	if(rev < 0)
	{
		printk(KERN_INFO "Cannot allocate the major number to the device\n");
		return -1;
	}
	printk(KERN_INFO "Major = %d & minor = %d\n", MAJOR(dev), MINOR(dev));
	printk(KERN_INFO "The module is now loaded\n");
	return 0;
}

/************* Module Exit Function **************/
static void __exit hello_world_exit(void)
{
	unregister_chrdev_region(dev, 1);
	printk("The module is now unloaded\n");
}

module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vinoth");
MODULE_DESCRIPTION("A driver for the major and minor number allocation dynamically");
MODULE_VERSION("1.1");
