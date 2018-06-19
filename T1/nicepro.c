#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#define __NR_nhsysnice 333 //系统调用号
int main() {
    	pid_t tid;
    	int nicevalue = 0;
    	int prio = 0;
    	int nice = 0;
	int flag;
	printf("请输入pid值:");
	while(scanf("%d",&tid)){
		printf("请输入flag:");
		scanf("%d",&flag);
		if(flag==1){
			printf("请输入新的nice值:");
			scanf("%d",&nicevalue);
		}
		syscall(__NR_nhsysnice,tid,flag,nicevalue,&prio,&nice);
		if(flag==0){
			printf("pid: %d\tprio: %d\tnice: %d\n\n", tid, prio,nice);
		}
		else{
			printf("进程号为%d进程的nice值已被修改为：%d\n\n",tid,nicevalue);
		}
		printf("若继续操作请重新输入pid值,按ctrl+c结束\n");
	}        
	return 0;
}
