#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/list.h>


/* This function is called when the module is loaded. */

struct task_struct *tasks;


void BFS(struct task_struct* start){
	struct list_head *list; 
	struct task_struct *task;

	if(start == NULL){
		return;
	}
	else{
		printk(KERN_INFO "%d\t%24s\t%8lu\n",start->pid,start->comm, start->state);
		list_for_each(list,&(start->children)){
			task = list_entry(list,struct task_struct, sibling);
			BFS(task);
		}

	}
}

// void printKids(struct list_head *start){
// 	printk(KERN_INFO "OUT %d\t%24s\t%8lu\n",tasks->pid,tasks->comm, tasks->state);
// 	list_for_each(list,start){
// 		tasks = list_entry(list,struct task_struct, sibling);
// 		printk(KERN_INFO "%d\t%24s\t%8lu\n",tasks->pid,tasks->comm, tasks->state);
// 			// BFS(tasks);
// 		}	
// }

int simple_init(void)
{
       printk(KERN_INFO "Loading Module\n");
       printk(KERN_INFO "Listing current tasks: \n");
       printk(KERN_INFO "%s\t%24s\t%8s\n","PID","NAME","STATE");
       for_each_process(tasks){
			printk(KERN_INFO "%d\t%24s\t%8lu\n",tasks->pid,tasks->comm, tasks->state);
       }
       printk(KERN_INFO "Listing BFS tree of tasks: \n");
       BFS(&init_task);
       // printKids(&(init_task.children));
       return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {
	printk(KERN_INFO "Removing Module\n");
}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");

