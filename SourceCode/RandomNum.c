#include <linux/module.h>
#include <linux/version.h>
#include <linux/kernel.h>
#include <linux/types.h>
#include <linux/kdev_t.h>
#include <linux/random.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/fs.h>
#include <linux/uaccess.h>

#define DRIVER_AUTHOR "19127041_19127246_19127250"
#define DRIVER_DESC "Generate a random number"
#define MAX_RAND 500

static dev_t first; // Global variable for the first device number
static struct cdev c_dev; // Global variable for the character device structure
static struct class *cl; // Global variable for the device class

static int _open(struct inode *, struct file *);

static int _close(struct inode *, struct file *);

static ssize_t _read(struct file *, char *, size_t, loff_t *);

static int _open(struct inode *i, struct file *f)
{
	printk(KERN_INFO "Driver: open()\n");
	return 0;
}

static int _close(struct inode *i, struct file *f)
{
	printk(KERN_INFO "Driver: Close()\n");
	return 0;
}

static ssize_t _read(struct file *f, char __user *buf, size_t len, loff_t *off)
{
	int res;
    	get_random_bytes(&res, sizeof(res));
	res %= MAX_RAND;

    	if (copy_to_user(buf, &res, sizeof(res))) 
	{	
		return -EFAULT;
	}
	else
	{
		return 0;
	}
}


static struct file_operations RandomNum_fops =
{
	.owner = THIS_MODULE,
	.open = _open,
 	.release = _close,
	.read = _read,
};

/* Constructor */
static int __init ORRNCD_init(void) 
{
	printk(KERN_INFO "Hello: Open and Read random number CD registered");

	if (alloc_chrdev_region(&first, 0, 1, "RandomNumber") < 0)
	{
 		return -1;
 	}
	
	
 	if ((cl = class_create(THIS_MODULE, "randNum")) == NULL)
 	{
 		unregister_chrdev_region(first, 1);
 		return -1;
 	}
 
	if (device_create(cl, NULL, first, NULL, "randomNum") == NULL)
 	{
 		class_destroy(cl);
 		unregister_chrdev_region(first, 1);
 		return -1;
 	}

 	cdev_init(&c_dev, &RandomNum_fops);

 	if (cdev_add(&c_dev, first, 1) == -1)
 	{
 		device_destroy(cl, first);
 		class_destroy(cl);
 		unregister_chrdev_region(first, 1);
 		return -1;
 	}
   	return 0;
}

/* Destructor */
static void __exit ORRNCD_exit(void) 
{
 	cdev_del(&c_dev);
 	device_destroy(cl, first);
 	class_destroy(cl);
 	unregister_chrdev_region(first, 1);
 	printk(KERN_INFO "Driver: ORRNCD unregistered");
}

module_init(ORRNCD_init);
module_exit(ORRNCD_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR(DRIVER_AUTHOR);
MODULE_DESCRIPTION(DRIVER_DESC);
