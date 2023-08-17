#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>
#include <asm/processor.h>
#include <asm/types.h>



MODULE_DESCRIPTION("My kernel module lkm3");
MODULE_AUTHOR("Me");
MODULE_LICENSE("GPL");
#define task_thread_info(tsk)	((struct thread_info *) ((char *) (tsk) + TASK_SIZE))



int iterate_init(void)                 
{
    printk("%s","LOADING MODULE LKM3\n");    
      
    struct task_struct *task_list;

    rcu_read_lock();


    for_each_process(task_list) {

        if(task_list->pid==1){
            
            printk("Kernel Stack pointer points at:%ul\n",task_list->thread_info);
            printk("Stack pointer points at:%ul\n",task_list->thread.sp);
        }

    }   

    rcu_read_unlock(); 

 

    return 0;

}               
    
void cleanup_exit(void)        
{


    printk("%s","REMOVING MODULE LKM3\n");

}               

module_init(iterate_init);    
module_exit(cleanup_exit);    
