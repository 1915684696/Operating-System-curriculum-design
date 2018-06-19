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

	//打开互斥信号量
	mutex = sem_open(MUTEX_NAME, O_CREAT);
	if(mutex == SEM_FAILED)
	{
		printf("Receiver打开互斥信号量失败!\n");
		exit(0);
	}

	//打开同步信号量
	sync = sem_open(SYNC_NAME, O_CREAT);
	if(sync == SEM_FAILED)
	{
		printf("Receiver打开同步信号量失败!\n");
		exit(0);
	}


	//打开共享内存
	int shmid;
	shmid = shmget(key, SIZE, 0666);
	if(shmid < 0)
	{
		printf("Receiver打开共享内存失败!\n");
		exit(0);
	}

	//将共享内存映射到调用进程的虚拟地址空间
	char *shmaddr;
	shmaddr = shmat(shmid, NULL, 0);

	char *message;
	// Receiver接收Sender的消息
	sem_wait(mutex);
	strcpy(message, shmaddr);
	sem_post(mutex);
	printf("Receiver已受到消息: %s\n", message);

	// Receiver回应over
	char *reply = "over";
	sem_wait(mutex);
	strcpy(shmaddr, reply);
	sem_post(mutex);

	// Receiver释放关闭信号来那个
	sem_post(sync);
	sem_close(mutex);
	sem_close(sync);

	printf("Receiver进程结束!\n");

	return 0;
}

