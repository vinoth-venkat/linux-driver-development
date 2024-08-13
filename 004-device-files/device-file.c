/*******************
 ** Device file **
 ******************/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/kdev_t.h>

dev_t dev = 0;
static struct class *dev_class;

/**************** Module Init Function ***************/
static int __init hello_world_init(void)
{
	/*** Allocating the major and minor number ***/
	int rev = alloc_chrdev_region(&dev, 0, 1, "Linux_dev");
	if(rev < 0)
	{
		printk(KERN_INFO "Cannot allocate the major number to the device\n");
		return -1;
	}
	pr_info("Major = %d & minor = %d\n", MAJOR(dev), MINOR(dev));

	/*** creating class ***/
	dev_class = class_create("linux_class");
	if(IS_ERR(dev_class))
	{
		pr_err("Cannot create the struct class for the device\n");
		goto r_class;
	}

	/**** creating device ***/
	if(IS_ERR(device_create(dev_class, NULL, dev, NULL, "linux_device")))
	{
		pr_err("cannot create the device\n");
		goto r_device;
	}

	pr_info("The module is now loaded\n");
	return 0;

r_device:
	class_destroy(dev_class);
r_class:
	device_destroy(dev, 1);
	return -1;
}

/************* Module Exit Function **************/
static void __exit hello_world_exit(void)
{
	device_destroy(dev_class, dev);
	class_destroy(dev_class);
	unregister_chrdev_region(dev, 1);
	printk("The module is now unloaded\n");
}

module_init(hello_world_init);
module_exit(hello_world_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vinoth");
MODULE_DESCRIPTION("A driver for the major and minor number allocation dynamically");
MODULE_VERSION("1.1");
