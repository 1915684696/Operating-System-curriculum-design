#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <stdio.h>
#define __NR_nhsysnice 333 //ϵͳ���ú�
int main() {
    	pid_t tid;
    	int nicevalue = 0;
    	int prio = 0;
    	int nice = 0;
	int flag;
	printf("������pidֵ:");
	while(scanf("%d",&tid)){
		printf("������flag:");
		scanf("%d",&flag);
		if(flag==1){
			printf("�������µ�niceֵ:");
			scanf("%d",&nicevalue);
		}
		syscall(__NR_nhsysnice,tid,flag,nicevalue,&prio,&nice);
		if(flag==0){
			printf("pid: %d\tprio: %d\tnice: %d\n\n", tid, prio,nice);
		}
		else{
			printf("���̺�Ϊ%d���̵�niceֵ�ѱ��޸�Ϊ��%d\n\n",tid,nicevalue);
		}
		printf("��������������������pidֵ,��ctrl+c����\n");
	}        
	return 0;
}
