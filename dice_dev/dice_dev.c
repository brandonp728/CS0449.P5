/*
 * "dice, world!" minimal kernel module - /dev version
 *
 * Valerie Henson <val@nmt.edu>
 *
 */

#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/module.h>
#include <linux/random.h>
#include <asm/uaccess.h>

/*
 * dice_read is the function called when a process calls read() on
 * /dev/dice.  It writes "dice, world!" to the buffer passed in the
 * read() call.
 */

static ssize_t dice_read(struct file* file, char* buf,
			  size_t count, loff_t *ppos)
{
	unsigned char mybuf[32];
  get_random_bytes(mybuf, sizeof(mybuf));
  int i;
  for(i = 0; i < sizeof(mybuf); i++)
  {
    mybuf[i] = mybuf[i] % 6; // makes it 0-5
  }
  int len = 32;
  if(count<32)
  {
    len = count;
  }
	/*
	 * Besides copying the string to the user provided buffer,
	 * this function also checks that the user has permission to
	 * write to the buffer, that it is mapped, etc.
	 */
  int j=0;
  while(j < count)
  {
    get_random_bytes(mybuf, sizeof(mybuf));
    for(i = 0; i < sizeof(mybuf); i++)
    {
      mybuf[i] = mybuf[i] % 6; // makes it 0-5
    }
	  if (copy_to_user(&buf[j], mybuf, len))
    {
		  return -EINVAL;
    }
    j+=len;
    if(j+len > count)
    {
      len = count-j;
    }
  }
  /*
	 * Tell the user how much data we wrote.
	 */
  *ppos = *ppos + count;
	return count;
}

/*
 * The only file operation we care about is read.
 */

static const struct file_operations dice_fops = {
	.owner		= THIS_MODULE,
	.read		= dice_read,
};

static struct miscdevice dice_dev = {
	/*
	 * We don't care what minor number we end up with, so tell the
	 * kernel to just pick one.
	 */
	MISC_DYNAMIC_MINOR,
	/*
	 * Name ourselves /dev/dice.
	 */
	"dice",
	/*
	 * What functions to call when a program performs file
	 * operations on the device.
	 */
	&dice_fops
};

static int __init
dice_init(void)
{
	int ret;

	/*
	 * Create the "dice" device in the /sys/class/misc directory.
	 * Udev will automatically create the /dev/dice device using
	 * the default rules.
	 */
	ret = misc_register(&dice_dev);
	if (ret)
		printk(KERN_ERR
		       "Unable to register \"Dice roll\" misc device\n");

	return ret;
}

module_init(dice_init);

static void __exit
dice_exit(void)
{
	misc_deregister(&dice_dev);
}

module_exit(dice_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Brandon Palonis <bmp59@pitt.edu>");
MODULE_DESCRIPTION("\"Dice roll\" minimal module");
MODULE_VERSION("dev");
