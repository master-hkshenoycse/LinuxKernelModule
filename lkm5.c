


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>
#include <linux/mm.h>

#define page_to_pa(page)	(page_to_pfn(page) << PAGE_SHIFT)
#define page_to_phys(page)	page_to_pa(page)

MODULE_DESCRIPTION("My kernel module lkm5");
MODULE_AUTHOR("Me");
MODULE_LICENSE("GPL");


int given_pid;
module_param(given_pid, int , 0);

unsigned long virt2phys(struct mm_struct *mm,unsigned long vpage){

    pgd_t *pgd;
    p4d_t *p4d;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *pte;
    struct page *page;

    pgd = pgd_offset(mm, vpage);
    if (pgd_none(*pgd) || pgd_bad(*pgd)) return 0;
    
    p4d = p4d_offset(pgd, vpage);
    if (p4d_none(*p4d) || p4d_bad(*p4d)) return 0;

    pud = pud_offset(p4d, vpage);
    if (pud_none(*pud) || pud_bad(*pud)) return 0;


    pmd = pmd_offset(pud, vpage);
    if (pmd_none(*pmd) || pmd_bad(*pmd)) return 0;

    if (!(pte = pte_offset_map(pmd, vpage))) return 0;

    if (!(page = pte_page(*pte))) return 0;


    unsigned long physical_page_addr = page_to_phys(page);
    pte_unmap(pte);
    
    // handle unmapped page
    if (physical_page_addr==70368744173568) return 0;

    return physical_page_addr;
}


int iterate_init(void)                 
{
    printk("%s","LOADING MODULE LKM5\n");    
      
    struct task_struct *task;

    //unsigned long given_pid=1773;
    unsigned long vpage;

    unsigned long vm_space=0,physc_allot=0;

    rcu_read_lock();

    for_each_process(task) {

        if(task->pid==given_pid){    
            if (task->mm && task->mm->mmap){
                struct vm_area_struct *vma = 0;
                for (vma = task->mm->mmap; vma; vma = vma->vm_next){
                        
                    vm_space+=(vma->vm_end-vma->vm_start);
                    
                    for (vpage = vma->vm_start; vpage < vma->vm_end; vpage += PAGE_SIZE){    
                        unsigned long phys = virt2phys(task->mm,vpage);
                        if(phys != 0){
                            physc_allot+=PAGE_SIZE;
                        }
                    }
                } 
            }
        }
    }
    rcu_read_unlock();


    printk("Virtual memory alloted is :%lu Bytes \n physical memory alloted is %lu Bytes\n",vm_space,physc_allot);   

 

    return 0;

}               
    
void cleanup_exit(void)        
{


    printk("%s","REMOVING MODULE LKM5\n");

}               

module_init(iterate_init);    
module_exit(cleanup_exit);    