SYSCALL_DEFINE5(nhsyscall, pid_t, pid, int, flag, int, nicevalue, void __user *, prio, void __user *, nice)
{
	         struct pid * kpid;
	         struct task_struct * task;
	         int nicebef; 
	         int priobef;
	         kpid = find_get_pid(pid);   // ·µ»Ø pid
	         task = pid_task(kpid, PIDTYPE_PID);  // ·µ»Ø task_struct
	         nicebef = task_nice(task);
	         priobef = task_prio(task);
	   
	         if(flag == 1){
		    set_user_nice(task, nicevalue);
	printk("nice before modification£º%d\tnice after modification£º%d\n",  nicebef, nicevalue);
		     return 0;
	          }
	          else if(flag == 0){
		      copy_to_user(nice, (const void*)&nicebef, sizeof(nicebef));
		      copy_to_user(prio, (const void*)&priobef, sizeof(priobef));
		      printk("nice is£º%d\n", nicebef);
		      printk("prio is£º%d\n", priobef);
		      return 0;
	           }

	          printk("flag is error£¬please input it again!\n");
	          return EFAULT;
}
