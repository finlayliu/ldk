/*************************************************************************
	> File Name: helloworld.c
	> Author: finlay
	> Mail: liufy.thu@gmail.com 
	> Created Time: Sun 20 Jul 2014 11:24:28 HKT
 ************************************************************************/

#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <asm/thread_info.h>
#include <linux/kthread.h>
#include <linux/kfifo.h>

static void print_taskinfo(struct task_struct *task)
{
	printk(KERN_ALERT "PID:%d\n", task->pid);
	printk(KERN_ALERT "state:%ld\n", task->state);
}

static int list_parent_task(struct task_struct *task)
{
	
	print_taskinfo(task);
	if (task->pid > 1)
		list_parent_task(task->parent);			
	return 0;
}

static int list_child_task(struct task_struct *task, struct list_head *list)
{
	struct task_struct *task1;
	struct pid *p1;
	p1 = find_get_pid(1);
	task1 = get_pid_task(p1, PIDTYPE_PGID);
	print_taskinfo(task1);

	list_for_each(list, &task1->children)
	{
		task = list_entry(list, struct task_struct, sibling);
		print_taskinfo(task);
	}
	return 0;
}

static void print_task(void)
{
	printk(KERN_ALERT "sizeof task_struct is %ld\n", sizeof(struct task_struct));
	printk(KERN_ALERT "sizeof thread_info is %ld\n", sizeof(struct thread_info));
}

static void test_task(void)
{
	struct task_struct *test_task;
	struct list_head *list;
	printk(KERN_ALERT "I bear a charmed life!\n");

	print_task();
	
	test_task = current_thread_info()->task;
	print_taskinfo(test_task);
	//printk(KERN_ALERT "TASK_RUNNING is %p\n", TASK_RUNNING);
	set_task_state(test_task, TASK_INTERRUPTIBLE);
	printk(KERN_ALERT "after set:\n");
	print_taskinfo(test_task);
	
	set_task_state(test_task, TASK_RUNNING);
	printk(KERN_ALERT "after set:\n");
	print_taskinfo(test_task);
#ifdef PARENT_CHILDREN_PID
	printk(KERN_ALERT "parent pid:\n");
	list_parent_task(current);
	printk(KERN_ALERT "children pid:\n");
	list_child_task(test_task, list);
#endif	
}

//2014-9-1
static void test_task1(void)
{
	struct task_struct *task;
	for (task = current; task != &init_task; task = task->parent)
		print_taskinfo(task);
}

static void test_task2(void)
{
	struct task_struct *task;
	for_each_process(task)
		printk("%s[%d]\n", task->comm, task->pid);
	printk("current:%s[%d]\n", current->comm, current->pid);
}

static int test_threadfn(void *data)
{
	char *k = data;
	printk("test_threadfn enter!\n");
	printk("%s\n", k);

	struct kfifo fifo1;
	//typeof(fifo1) ttt;
	struct kfifo ttt;

	printk("kfifo:0x%x,type:0x%x\n", &fifo1, &ttt);
	return 0;
	
}

static void test_kthread(void)
{
	char *a = "yes,I'm in";
	struct task_struct *k;
	k =	kthread_create(test_threadfn, a, "test_finlay");
	printk("wake up process!\n");
	wake_up_process(k);
}


static int hello_init(void)
{
	test_kthread();	
	return 0;
}

static void hello_exit(void)
{
	printk(KERN_ALERT "Out, out, brief candle!\n");
}

module_init(hello_init);
module_exit(hello_exit);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Shakespeare");
MODULE_DESCRIPTION("A Hello, World Module");
