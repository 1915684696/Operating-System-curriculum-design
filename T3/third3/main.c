#include <pthread.h>
#include <semaphore.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>
#include <sys/msg.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/stat.h>
#include <errno.h>

#define TRUE 1
#define BUF_SIZE 255
#define PERM S_IRUSR|S_IWUSR // 文件主读|文件主写权限 
#define KEY_NUM 2000


struct msgbuf{
	long mtype; //消息类型 
	char mtext[BUF_SIZE]; //消息内容 
};


// 信号量 
sem_t full;		// 满信号 
sem_t empty;	// 空信号
sem_t mutex;	// 互斥访问 
sem_t sender_over;	//完 


// 两个线程
pthread_t sender_tid;
pthread_t receiver_tid;


// 消息队列标识符 
int msgid;

//初始化 
void Init()
{
	// 初始化信号量 
	sem_init(&full,0,0);// ，共享，值 
	sem_init(&empty,0,1);
	sem_init(&mutex,0,1);
	sem_init(&sender_over,0,0);

	// 创建消息队列 msgget(关键字,操作)
	// IPC_CREATE创建/打开
	if((msgid = msgget(KEY_NUM, PERM|IPC_CREAT)) == -1)//给了读写权限，IPC_CREAT若存在则返回标识符
	{
		printf("创建消息队列错误\n");
	}

}

//sender线程 
void *SenderThread(void *arg)
{
	char input[BUF_SIZE];
	struct msgbuf msg;
	msg.mtype = 1;
	int length = sizeof(struct msgbuf) - sizeof(long);

	while(TRUE)
	{
		sem_wait(&empty);
		sem_wait(&mutex);
		//p(empty) p(mutex)
		printf("\n当前为Sender线程: 输入消息:\n");
		gets(input);

		// 退出
		if(strcmp(input, "exit") == 0)	
		{
			strncpy(msg.mtext, "end", BUF_SIZE);
			if(msgsnd(msgid, &msg, length, 0)==-1){//length表示消息正文的长度，0表示队列满时阻塞
				printf("发送消息错误\n");
			}
			printf("Sender线程已将%s写入\n", msg.mtext);
			//v(mutex)v(full)
			sem_post(&mutex);
			sem_post(&full);
			break;
		}

		strncpy(msg.mtext, input, BUF_SIZE);
		if(msgsnd(msgid, &msg, length, 0) == -1)//将消息写入到消息队列
			printf("发送消息错误\n");
		printf("Sender线程已写入消息队列:  %s\n", msg.mtext);
		//v(mutex)v(full)
		sem_post(&mutex);		
		sem_post(&full);
		
	}
	//p(sernder_over,full,mutex)
	sem_wait(&sender_over);
	sem_wait(&full);
	sem_wait(&mutex);
	if(msgrcv(msgid, &msg, length, 1, 0) == -1)//1>0消息类型(接收第一个类型是msgtyp的消息)，0没有消息则阻塞 
		printf("接收信息错误\n");
	if(strcmp(msg.mtext, "over") == 0) 
		printf("Sender线程已收到:%s\n", msg.mtext);
	//v(mutex,empty)
	sem_post(&mutex);
	sem_post(&empty);

	// 移除信息 
	if(msgctl(msgid, IPC_RMID, 0) == -1) //从系统内核中移走消息队列 
	{
		printf("错误!\n");
		exit(0);
	}
	pthread_exit(0);
}


//接收者 
void *ReceiverThread(void *arg)
{
	struct msgbuf msg;
	msg.mtype = 1;
	int length = sizeof(struct msgbuf) - sizeof(long);
	while(TRUE)
	{
		//p(full)p(mutex)
		sem_wait(&full);
		sem_wait(&mutex);
		// 取走信息
		if(msgrcv(msgid, &msg, length, 1, 0) == -1)
			printf("接收信息错误\n");

		if(strcmp(msg.mtext, "end") == 0)  
		{
			printf("Receiver线程收到: %s，即将回应..\n", msg.mtext);
			strncpy(msg.mtext, "over", BUF_SIZE);
			if(msgsnd(msgid, &msg, length, 0) == -1)//将消息写入到消息队列
				printf("发送消息错误\n");
			printf("Receiver已回应 %s\n", msg.mtext);
			//v(sernder_over,full,mutex)
			sem_post(&sender_over);
			sem_post(&mutex);
			sem_post(&full);
			break;
		}
		printf("Receiver打印:%s\n", msg.mtext);
		//v(empty)v(mutex)
		sem_post(&mutex);
		sem_post(&empty);
	}
	pthread_exit(0);
}



int main()
{
	//初始化
	Init();

	// 创建线程
	if(pthread_create(&sender_tid, NULL, SenderThread, NULL) == -1)	//创建一个新的线程，并将线程加入当前进程
	{
		printf("创建线程失败!\n");
		exit(0);
	}

	if(pthread_create(&receiver_tid, NULL, ReceiverThread, NULL) == -1)
	{
		printf("创建线程失败!\n");
		exit(0);
	}


	// 等待线程结束 
	pthread_join(sender_tid, NULL);//会一直阻塞调用线程，直到指定的线程tid终止
	pthread_join(receiver_tid, NULL);

	sem_destroy(&full);
	sem_destroy(&empty);
	sem_destroy(&mutex);
	sem_destroy(&sender_over);

	printf("消息队列通信模拟结束!\n");

	return 0;
}

