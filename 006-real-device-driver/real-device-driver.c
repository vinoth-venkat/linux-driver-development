/*************************************
 ******** Simple device driver *******
 ************************************/

#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/err.h>
#include <linux/kdev_t.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>
#include <linux/slab.h>

#define mem_size	1024

dev_t dev = 0;
static struct class *dev_class;
static struct cdev linux_cdev;
uint8_t *kernel_buffer;

/**************** Function Prototype *****************/
static int __init lin_driver_init(void);
static void __exit lin_driver_exit(void);
static int lin_open(struct inode *inode, struct file *file);
static int lin_release(struct inode *inode, struct file *file);
static ssize_t lin_read(struct file *flip, char __user *buf, size_t len, loff_t * off);
static ssize_t lin_write(struct file *flip, const char *buf, size_t len, loff_t * off);

/**************** File Operation ******************/
static struct file_operations fops =
{
	.owner = THIS_MODULE,
	.read = lin_read,
	.write = lin_write,
	.open = lin_open,
	.release = lin_release,
};

/***** This function will called when we open the device file *****/
static int lin_open(struct inode *inode, struct file *file)
{
	pr_info("Driver open function is called...\n");
	return 0;
}

/***** This function will called when we open the device file *****/
static int lin_release(struct inode *inode, struct file *file)
{
	pr_info("Driver release function is called...\n");
	return 0;
}

/***** This function will called when we open the device file *****/
static ssize_t lin_read(struct file *flip, char __user *buf, size_t len, loff_t * off)
{
	int ret = copy_to_user(buf, kernel_buffer, mem_size);
	if(ret)
	{
		pr_err("Data read error\n");
	}
	pr_info("Driver read function is called...\n");
	return 0;
}

/***** This function will called when we open the device file *****/
static ssize_t lin_write(struct file *flip, const char *buf, size_t len, loff_t * off)
{
	int ret = copy_from_user(kernel_buffer, buf, len);
	if (ret)
	{
		pr_err("Data write error\n");
	}
	pr_info("Driver write function is called");
	return len;
}

/**************** Module Init Function ***************/
static int __init lin_driver_init(void)
{
	/*** Allocating the major and minor number ***/
	int rev = alloc_chrdev_region(&dev, 0, 1, "Linux_dev");
	if(rev < 0)
	{
		printk(KERN_INFO "Cannot allocate the major number to the device\n");
		return -1;
	}
	pr_info("Major = %d & minor = %d\n", MAJOR(dev), MINOR(dev));

	/*** creating cdev structure ***/
	cdev_init(&linux_cdev, &fops);

	/*** Adding character device to the system ***/
	if ((cdev_add(&linux_cdev, dev, 1)) < 0)
	{
		pr_err("Cannot add the device to the system\n");
		goto r_class;
	}

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

	/*** Creating physcial memory ***/
	if((kernel_buffer = kmalloc(mem_size, GFP_KERNEL)) == 0)
	{
		pr_err("Cannot allocate memory in kernel\n");
		goto r_device;
	}

	pr_info("The module is now loaded\n");
	return 0;

r_device:
	class_destroy(dev_class);
r_class:
	unregister_chrdev_region(dev, 1);
	return -1;
}

/************* Module Exit Function **************/
static void __exit lin_driver_exit(void)
{
	device_destroy(dev_class, dev);
	class_destroy(dev_class);
	unregister_chrdev_region(dev, 1);
	printk("The module is now unloaded\n");
}

module_init(lin_driver_init);
module_exit(lin_driver_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vinoth");
MODULE_DESCRIPTION("A sample real linux driver");
MODULE_VERSION("1.4");
