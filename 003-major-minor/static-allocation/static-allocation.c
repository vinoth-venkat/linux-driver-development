#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

dev_t dev = MKDEV(235, 0);

/************** Module Init function *************/
static int __init hello_world_init(void)
{
	register_chrdev_region(dev, 1, "linux_dev");
	printk(KERN_INFO "Major = %d & Minor = %d\n", MAJOR(dev), MINOR(dev));
	printk(KERN_INFO "The module is now loaded\n");
	return 0;
}

/************ Module Exit Function *************/
static void __exit hello_world_exit(void)
{
	unregister_chrdev_region(dev, 1);
	printk("The module is now unloaded\n");
}

module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vinoth");
MODULE_DESCRIPTION("A sample driver to test the major and minor number allocation statically");
MODULE_VERSION("1.0");
