#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/rcupdate.h>
#include <linux/sched.h>

MODULE_DESCRIPTION("My kernel module lkm2");
MODULE_AUTHOR("Me");
MODULE_LICENSE("GPL");


int iterate_init(void)                 
{
    printk("%s","LOADING MODULE LKM2\n");    
      
    struct task_struct *task_list;
    long int pid_largest=0,heap_largest=0;

    rcu_read_lock();

    for_each_process(task_list) {

        if(task_list->mm != NULL ){
            if(heap_largest < (task_list->mm->brk)-(task_list->mm->start_brk)){
                heap_largest=(task_list->mm->brk)-(task_list->mm->start_brk);
                pid_largest=task_list->pid;
            }
        }
    }   

    rcu_read_unlock(); 

    
    printk("Process with PID:[%ld] has largest heap space which =%lu Bytes\t", pid_largest, heap_largest); 
    

    return 0;

}               
    
void cleanup_exit(void)        
{


    printk("%s","REMOVING MODULE LKM2\n");

}               

module_init(iterate_init);    
module_exit(cleanup_exit);    