#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/sched/signal.h>

// 初始化函数
static int hello_init(void)
{
    struct task_struct *p;
    printk(KERN_ALERT"名称\t\t\t进程号\t状态\t优先级\t父进程的pid\t");
    for_each_process(p)
    {
	if(p->mm == NULL){
            printk(KERN_ALERT"%20s\t%d\t%ld\t%d\t%d\n",p->comm,p->pid,p->state,p->normal_prio,p->parent->pid);
	}
    }
    return 0;
}
// 清理函数
static void hello_exit(void)
{
    printk(KERN_ALERT"goodbye!\n");
}

// 函数注册
module_init(hello_init);  
module_exit(hello_exit);  

// 模块许可申明
MODULE_LICENSE("GPL");  
