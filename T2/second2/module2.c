#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/moduleparam.h>

static pid_t pid=1;
module_param(pid,int,0644);

static int hello_init(void)
{
    struct task_struct *p;
    struct list_head *pp;
    struct task_struct *psibling;

    // 当前进程的 PID
    p = pid_task(find_get_pid(pid), PIDTYPE_PID);
    printk("当前进程pid: %d 名称 %20s 状态 %ld\n", p->pid, p->comm,p->state);

    // 父进程
    if(p->parent == NULL) {
        printk("没有父进程！\n");
    }
    else {
        printk("父进程pid %d 名称 %20s 状态 %ld\n", p->parent->pid, p->parent->comm,p->state);
    }

    // 兄弟进程
    list_for_each(pp, &p->parent->children)
    {
        psibling = list_entry(pp, struct task_struct, sibling);
	if(psibling->pid!=pid){
        	printk("兄弟进程pid %d 名称 %20s 状态 %ld\n", psibling->pid, psibling->comm,p->state);
	}
    }

    // 子进程
    list_for_each(pp, &p->children)
    {
        psibling = list_entry(pp, struct task_struct, sibling);
        printk("子进程pid %d 名称 %20s 状态 %ld\n", psibling->pid, psibling->comm,p->state);
    }
    
    return 0;
}

static void hello_exit(void)
{
    printk(KERN_ALERT"goodbye!\n");
}

module_init(hello_init);
module_exit(hello_exit);
                
MODULE_LICENSE("GPL");

