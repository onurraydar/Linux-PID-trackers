#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/hrtimer.h>
#include <linux/ktime.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/sched/signal.h>
#include <linux/sched.h>
#include <linux/mm.h>

unsigned long timer_interval_ns = 5e9;
static struct hrtimer hr_timer;

int pid = 3;
module_param(pid, int, 0);

// Whatever we want to do periodically
void my_function(void){
    int tester = 0;
    struct task_struct *task;
    struct task_struct *target_task;
    for_each_process(task) {
        if(task->pid == pid) {
            target_task = task;
            tester = 1;
        }
    }
    if(tester == 0) {
        printk(KERN_INFO "No pid matched\n");
        return;
    }
    //printk(KERN_INFO "[PID]  : [WSS]\n");
    //printk(KERN_INFO "[%d] : [1200 kB]\n", target_task->pid); 
    struct vm_area_struct *mm = target_task->mm->mmap;
    unsigned long i = 0;
    int sum = 0;
    while(mm != NULL) {
        for(i = mm->vm_start; i < mm->vm_end; i+=PAGE_SIZE) {
            pgd_t *pgd;
            p4d_t *p4d;
            pmd_t *pmd;
            pud_t *pud;
            pte_t *ptep, pte;
    
            pgd = pgd_offset(target_task->mm, i);                    
            if (pgd_none(*pgd) || pgd_bad(*pgd)){           
                break;
            }

            p4d = p4d_offset(pgd, i);                   
            if (p4d_none(*p4d) || p4d_bad(*p4d)){          
                break;
            }

            pud = pud_offset(p4d, i);                   
            if (pud_none(*pud) || pud_bad(*pud)){          
                break;
            }

            pmd = pmd_offset(pud, i);               
            if (pmd_none(*pmd) || pmd_bad(*pmd)){       
                break;
            } 

            ptep = pte_offset_map(pmd, i);      
            if (!ptep){
                break;
            }                                         
            pte = *ptep;
            if(pte_young(pte) > 0) {
                sum+=PAGE_SIZE;
            }
            pte_mkold(pte);

        }
        mm = mm->vm_next;
    }
    sum = sum / 1024;
    printk(KERN_INFO "[PID]  : [WSS]\n");
    printk(KERN_INFO "[%d] : [%d kB]\n", target_task->pid, sum);



}

// Callback executed periodically, reset timer
enum hrtimer_restart timer_callback( struct hrtimer *timer_for_restart )
{
    ktime_t currtime , interval;
    currtime  = ktime_get();
    interval = ktime_set(0,timer_interval_ns);
    hrtimer_forward(timer_for_restart, currtime , interval);
    my_function();
    return HRTIMER_RESTART;
}

// Initialize timer with callback
static int timer_init(void) {
    ktime_t ktime;
    ktime = ktime_set( 0, timer_interval_ns );
    hrtimer_init( &hr_timer, CLOCK_MONOTONIC, HRTIMER_MODE_REL );
    hr_timer.function = &timer_callback;
    hrtimer_start( &hr_timer, ktime, HRTIMER_MODE_REL );  // Start timer
    return 0;
}

// Remove timer
static void timer_exit(void) {
    int ret;
    ret = hrtimer_cancel( &hr_timer );
    if (ret) printk("Timer was still in use!\n");
    printk("HR Timer removed\n");
}


MODULE_LICENSE("GPL");
module_init(timer_init);
module_exit(timer_exit);