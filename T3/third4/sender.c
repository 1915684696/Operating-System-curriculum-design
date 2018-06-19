#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define SIZE 128	// 共享内存大小
char *MUTEX_NAME = "mutex";		// 自定义有名信号量
char *SYNC_NAME = "sync";		// 同步信号量

int main()
{
	key_t key;		//标志共享内存的自定义键值
	key = 2018;

	sem_t *mutex;		// 互斥信号量
	sem_t *sync;		// 同步信号量
	//创建互斥信号量
	mutex = sem_open(MUTEX_NAME, O_CREAT, 0644, 1);//O_CREATE没有则创建,0644权限
	if(mutex == SEM_FAILED)
	{
		printf("Sender创建互斥信号量失败!\n");
		exit(0);
	}


	//创建同步信号量
	sync = sem_open(SYNC_NAME, O_CREAT, 0644, 0);
	if(sync == SEM_FAILED)
	{
		printf("Sender创建同步信号量失败!\n");
		exit(0);
	}


	//创建共享内存
	int shmid;
	shmid = shmget(key, SIZE, IPC_CREAT|0666);//key>0ftok()函数生成,size大小,IPC_CREATE没有则创建
	if(shmid < 0)
	{
		printf("创建共享内存失败!\n");
		exit(0);
	}

	// 将共享内存映射到调用进程的虚拟地址空间
	char *shmaddr;
	shmaddr = shmat(shmid, NULL, 0);//addr,null系统找合适的位置,0读写

	if(shmaddr < 0)
	{
		printf("共享内存映射失败!\n");
		exit(0);
	}

	printf("请输入要传输的消息:\n");
	char message[SIZE];
	gets(message);
	// 拷贝到共享内存
	sem_wait(mutex);
	strcpy(shmaddr, message);
	sem_post(mutex);
	printf("已经发送：%s\n", message);

	// 等待回应
	char reply[SIZE];
	sem_wait(sync);
	strcpy(reply, shmaddr);
	printf("Sender收到回应消息%s\n", reply);

	// 删除信号量
	sem_close(mutex);
	sem_unlink(MUTEX_NAME);
	sem_close(sync);
	sem_unlink(SYNC_NAME);

	//销毁共享内存
	shmdt(shmaddr);//断开映射
	shmctl(shmid, IPC_RMID, 0);//销毁共享内存
	printf("Sender进程结束!\n");

	return 0;
}

