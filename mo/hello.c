#include <linux/module.h>
#include <linux/init.h>

MODULE_LICENSE("GPL");

static int hello_init(void)
{
	printk("<0>hello loujunkai!!\n");
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_EMERG "byebye loujunkai!!\n");
}

module_init(hello_init);
module_exit(hello_exit);
