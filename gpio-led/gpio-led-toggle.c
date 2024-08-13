#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/gpio.h>
#include <linux/cdev.h>
#include <linux/uaccess.h>

#define GPIO_PIN 534
#define DRIVER_NAME "led_gpio_driver"
#define DRIVER_CLASS "drv_gpio_class"

/* Variables for device and device class */
static dev_t led_device_nr;
static struct class *drv_gpio_class;
static struct cdev led_device;

/** Read data out for buffer **/
static ssize_t led_read(struct file *File, char *user_buffer, size_t count, loff_t *offs)
{
	int to_copy, not_copied, delta;
	char tmp[3]=" \n";

	/** Get the data to copy **/
	to_copy = min(count, sizeof(tmp));
	
	/** Copy data to user **/
	not_copied = copy_to_user(user_buffer, &tmp, to_copy);

	/** Calculate data **/
	delta = to_copy - not_copied;

	return delta;
}

/** Write data to buffer **/
static ssize_t led_write(struct file *File, const char *user_buffer, size_t count, loff_t *offs)
{
	int to_copy, not_copied, delta;
	char value;

	/** Get amount of data to copy **/
	to_copy = min(count, sizeof(value));

	/** Copy data to user **/
	not_copied = copy_from_user(&value, user_buffer, to_copy);

	/** LED setting **/
	switch(value)
	{
		case '0':
			gpio_set_value(GPIO_PIN, 0);
			break;
		case '1':
			gpio_set_value(GPIO_PIN, 1);
			break;
		default:
			printk(KERN_INFO "Invalid input\n");
			break;
	}

	/** Calculate data **/
	delta = to_copy - not_copied;

	return delta;
}

/** Device file open Function **/
static int led_open(struct inode *device_file, struct file *instance)
{
	printk(KERN_INFO "Device open Function called\n");
	return 0;
}

/** Device file closed Function **/
static int led_close(struct inode *device_file, struct file *instance)
{
	printk(KERN_INFO "Device close Function called\n");
        return 0;
}

/** File operation Section **/
static struct file_operations fops = 
{
	.owner = THIS_MODULE,
	.open = led_open,
	.release = led_close,
	.read = led_read,
	.write = led_write
};


/** Module Init Function **/
static int __init led_init(void)
{
	printk(KERN_INFO "Hello, Kernel!!! This is GPIO LED");

	/** Allocate the device tree number **/
	if( alloc_chrdev_region(&led_device_nr, 0, 1, DRIVER_NAME) < 0) {
		printk("Device number could not be allocated!\n");
		return -1;
	}

	printk("Device number Major: %d, Minor: %d was registered!\n", led_device_nr >> 20, led_device_nr && 0xfffff);
	
	/** Create Device class **/
	if((drv_gpio_class = class_create(DRIVER_CLASS)) == NULL) {
		printk("Device class can not be created!\n");
		goto led_class;
	}

	/** Create Device file **/
	if(device_create(drv_gpio_class, NULL, led_device_nr, NULL, DRIVER_NAME) == NULL) {
		printk("Can not create device file!\n");
		goto led_device;
	}

	/** Initalize the device file **/
	cdev_init(&led_device, &fops);

	/** registering device to the kernel **/
	if(cdev_add(&led_device, led_device_nr, 1) == -1) {
		printk("Registering of device to kernel failed!\n");
		goto led_dev_add;
	}

	/** GPIO init **/
	if(gpio_request(GPIO_PIN, "GPIO_PIN")) {
		printk("Can not allocate GPIO %d\n", GPIO_PIN);
		goto led_dev_add;
	}

	/** Set GPIO direction **/
	if(gpio_direction_output(GPIO_PIN, 0)) {
		printk("Can not set GPIO %d to output!\n", GPIO_PIN);
		goto led_gpio;
	}

	return 0;

led_gpio:
	gpio_free(GPIO_PIN);
led_dev_add:
	device_destroy(drv_gpio_class, led_device_nr);
led_device:
	class_destroy(drv_gpio_class);
led_class:
	unregister_chrdev_region(led_device_nr, 1);
	return -1;
}

static void __exit led_exit (void)
{
	gpio_set_value(GPIO_PIN, 0);
	gpio_free(GPIO_PIN);
	cdev_del(&led_device);
	device_destroy(drv_gpio_class, led_device_nr);
	class_destroy(drv_gpio_class);
	unregister_chrdev_region(led_device_nr, 1);
	printk(KERN_INFO "Goodbye, Kernel\n");
}

module_init(led_init);
module_exit(led_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Vinoth Venkatraman");
MODULE_DESCRIPTION("The simple GPIO driver to toggle the LED");
MODULE_VERSION("1.00");
