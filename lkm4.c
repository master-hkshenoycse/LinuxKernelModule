


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/kthread.h>
#include <linux/init.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/rcupdate.h>


#define page_to_pa(page)	(page_to_pfn(page) << PAGE_SHIFT)
#define page_to_phys(page)	page_to_pa(page)

int given_pid;
module_param(given_pid, int , 0);

long given_va;
module_param(given_va,long,0);

MODULE_DESCRIPTION("My kernel module lkm4");
MODULE_AUTHOR("Me");
MODULE_LICENSE("GPL");

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
    printk("%s","LOADING MODULE LKM4\n");    
      
    struct task_struct *task;

    //unsigned long given_pid=1773;
    //unsigned long given_va=139774394277890; 
    unsigned long vpage;
    

    rcu_read_lock();

    for_each_process(task) {

        if(task->pid==given_pid){    
            if (task->mm && task->mm->mmap){
                struct vm_area_struct *vma = 0;
                for (vma = task->mm->mmap; vma; vma = vma->vm_next){
                    //printk("%lu - %lu\n",vma->vm_start,vma->vm_end);
                    if(given_va>=(vma->vm_start) && given_va<=(vma->vm_end)){
                        //printk("%lu - %lu",vma->vm_start,vma->vm_end);
                        
                        printk("Page size is :%lu\n",PAGE_SIZE);
                        for (vpage = vma->vm_start; vpage < vma->vm_end; vpage += PAGE_SIZE){
                                if(given_va>=vpage && given_va<vpage+PAGE_SIZE){
                                    unsigned long phys = virt2phys(task->mm,vpage);
                                    printk("pid is: %lu \n virtual addres is: %lu\n physical adress is:%lu\n",given_pid,given_va,phys);
                                }
                        }
                    }
                    
                } 
            }
        }
    }   

    rcu_read_unlock();

 

    return 0;

}               
    
void cleanup_exit(void)        
{


    printk("%s","REMOVING MODULE LKM4\n");

}               

module_init(iterate_init);    
module_exit(cleanup_exit);    