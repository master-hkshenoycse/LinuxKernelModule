#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>
#include <linux/rcupdate.h>

MODULE_DESCRIPTION("My kernel module lkm1");
MODULE_AUTHOR("Me");
MODULE_LICENSE("GPL");





int iterate_init(void)                 
{
    printk("%s","LOADING MODULE LKM1\n");    
    struct task_struct *task_list;
    

    rcu_read_lock();
    
    
    for_each_process(task_list) {
        pr_info("Process: %s\t has PID:[%d]\t", task_list->comm, task_list->pid);
    }

    rcu_read_unlock();    
    
       
    return 0;

}               
    
void cleanup_exit(void)        
{


    printk(KERN_INFO "%s","REMOVING MODULE LKM1\n");

}               

module_init(iterate_init);    
module_exit(cleanup_exit);    