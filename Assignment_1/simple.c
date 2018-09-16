#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/list.h>
#include <linux/types.h>
#include <linux/slab.h>

struct birthday
{
	int day;
	int month;
	int year;
	struct list_head list;
};


// Initialising the list with macro
static LIST_HEAD(birthday_list);


/* This function is called when the module is loaded. */
int simple_init(void)
{
   printk(KERN_INFO "Loading Module\n");
   // Creating the struct
	struct birthday *person;
	// Creating an element
	person = kmalloc(sizeof(*person), GFP_KERNEL); // GFP --> flag for normal kernal memory allocation
	person->day = 2;
	person->month = 8;
	person->year = 1995;
	INIT_LIST_HEAD(&person->list); // Macro to initialize the list struct

	// Macro to add an element to the list's tail
	list_add_tail(&person->list, &birthday_list);
   
   
	//Adding 5 more entries
    person = kmalloc(sizeof(*person), GFP_KERNEL);
	person->day = 24;
	person->month = 4;
	person->year = 1996;
	INIT_LIST_HEAD(&person->list);
	list_add_tail(&person->list, &birthday_list);

	person = kmalloc(sizeof(*person), GFP_KERNEL);
	person->day = 12;
	person->month = 4;
	person->year = 1997;
	INIT_LIST_HEAD(&person->list);
	list_add_tail(&person->list, &birthday_list);

	person = kmalloc(sizeof(*person), GFP_KERNEL);
	person->day = 2;
	person->month = 10;
	person->year = 1996;
	INIT_LIST_HEAD(&person->list);
	list_add_tail(&person->list, &birthday_list);

	person = kmalloc(sizeof(*person), GFP_KERNEL);
	person->day = 8;
	person->month = 6;
	person->year = 1994;
	INIT_LIST_HEAD(&person->list);
	list_add_tail(&person->list, &birthday_list);


   //Traversing the list
   // Pointer to struct that is used to create the list. This ptr will be used to traverse the list
   struct birthday *ptr;
   //Macro to traverse a list - params --> (ptr to structure being looped over, ptr to head of the list being looped over, name of variable that has the list_head structure)
   list_for_each_entry(ptr, &birthday_list, list) {
   		/* On each iteration ptr points to the next struct element */

   		printk(KERN_INFO "This person was born on %d - %d - %d", ptr->day, ptr->month, ptr->year);
   }
   return 0;
}

/* This function is called when the module is removed. */
void simple_exit(void) {

	//Delete all elements : This one is passed an additional argument that maintains the value of the next pointer of the item being deleted

	struct birthday *ptr, *next;

	list_for_each_entry_safe(ptr,next,&birthday_list, list){
		//Macro that actually deletes the element
		printk(KERN_INFO "Deleting element at address %x" , ptr);
		list_del(&ptr->list);
		//Free the memory assigned by kmalloc
		kfree(ptr);
	}

	printk(KERN_INFO "Removing Module\n");

}

/* Macros for registering module entry and exit points. */
module_init( simple_init );
module_exit( simple_exit );

MODULE_LICENSE("GPL");
MODULE_DESCRIPTION("Simple Module");
MODULE_AUTHOR("SGG");