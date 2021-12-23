#include <linux/init.h>
#include <linux/ktime.h>
#include <linux/list.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/printk.h>
#include <linux/slab.h>
#include <linux/stat.h>
#include <linux/timekeeping.h>

MODULE_AUTHOR("Dmytrii Karpeka");
MODULE_DESCRIPTION("Hello, world in Linux Kernel Training");
MODULE_LICENSE("Dual BSD/GPL");

static uint n = 1;
module_param(n, uint, S_IRUGO);
MODULE_PARM_DESC(n, " the number of repetitions of the 'Hello, world!' line");

struct ktime_struct
{
    struct list_head head;
    ktime_t data;
};

static struct list_head ktime_head = LIST_HEAD_INIT(ktime_head);

static int __init hello_init(void)
{
	int i;
	BUG_ON(n > 10);
	if (n == 0)
	{
		printk(KERN_WARNING "Warning: the value of parameter equals to zero.\n");
	}
	else if ((n >= 5) && (n <= 10))
	{
		printk(KERN_WARNING "Warning: the value of parameter is between 5 and 10.\n");
	}
	for (i = 0; i < n; i++)
	{
		struct ktime_struct *node;
		node = kmalloc(sizeof(struct ktime_struct *), GFP_KERNEL);
		if (i == 7) 
		{
			node = NULL;
		}
		node->data = ktime_get();
		list_add(&node->head, &ktime_head);
		printk(KERN_INFO "Hello, world!\n");
	}
	return 0;
}

static void __exit hello_exit(void)
{
	struct ktime_struct *cursor;
	struct ktime_struct *temp_data;
	list_for_each_entry_safe(cursor, temp_data, &ktime_head, head)
	{
		printk(KERN_INFO "Time: %llxns\n", cursor->data);
		list_del(&cursor->head);
		kfree(cursor);
	}
	BUG_ON(!list_empty(&ktime_head));
}

module_init(hello_init);
module_exit(hello_exit);
