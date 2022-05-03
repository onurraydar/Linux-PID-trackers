#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>

int myint = 3;
module_param(myint, int, 0);

int init_module(void)
{
	char pid[] = "PID";
	char name[] = "NAME";
	char prio[] = "PRIO";
	char policy[] = "policy";
	char schedN[] = "SCHED_NORMAL";
	char schedF[] = "SCHED_FIFO";
	char schedRR[] = "SCHED_RR";
	char schedB[] = "SCHED_BATCH";
	char schedI[] = "SCHED_ISO";
	char schedID[] = "SCHED_IDLE";
	char schedD[] = "SCHED_DEADLINE";
	struct task_struct *task;
	printk(KERN_INFO "%-6s %-20s %-6s %-15s", pid, name, prio, policy);
	for_each_process(task) {
		if(task->cred->uid.val == myint) {
			if(task->policy == 0) {
				printk(KERN_INFO "%-6d %-20s %-6d %-15s\n", task->pid, task->comm, task->prio, schedN);
			}
			if(task->policy == 1) {
				printk(KERN_INFO "%-6d %-20s %-6d %-15s\n", task->pid, task->comm, task->prio, schedF);
			}
			if(task->policy == 2) {
				printk(KERN_INFO "%-6d %-20s %-6d %-15s\n", task->pid, task->comm, task->prio, schedRR);
			}
			if(task->policy == 3) {
				printk(KERN_INFO "%-6d %-20s %-6d %-15s\n", task->pid, task->comm, task->prio, schedB);
			}
			if(task->policy == 4) {
				printk(KERN_INFO "%-6d %-20s %-6d %-15s\n", task->pid, task->comm, task->prio, schedI);
			}
			if(task->policy == 5) {
				printk(KERN_INFO "%-6d %-20s %-6d %-15s\n", task->pid, task->comm, task->prio, schedID);
			}
			if(task->policy == 6) {
				printk(KERN_INFO "%-6d %-20s %-6d %-15s\n", task->pid, task->comm, task->prio, schedD);
			}
		}
	}
	return 0;
}

void cleanup_module(void)
{
	printk(KERN_INFO "Done\n");
}
