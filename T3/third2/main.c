#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h> 

#define READ 0 //filedes[0]用于读

#define WRITE 1 //filedes[1]用于写

int main() {

    int filedes[2];

    pid_t pid1,pid2,pid3;

    char buf[256];
    pipe(filedes);
    int status;

    if((pid1 = fork()) == -1) { //创建子进程
        printf("fork error(pid1)!\n");
        exit(1);
    }
    if(pid1 == 0) {
	sleep(1);
        printf("正在产生子进程pid1:%d\n",getpid());
        //子进程向父进程写数据，关闭管道的读端
        close(filedes[READ]);
        lockf(filedes[1],1,0);
        if(write(filedes[WRITE], "pid111111\n", strlen("pid111111\n"))!=strlen("pid111111\n")){
        	printf("pid1写入错误\n");
        }
		sleep(1);
		if(write(filedes[WRITE], "pid111111\n", strlen("pid111111\n"))!=strlen("pid111111\n")){
        	printf("pid1写入错误\n");
        }
        if(write(filedes[WRITE], "pid111111\n", strlen("pid111111\n"))!=strlen("pid111111\n")){
        	printf("pid1写入错误\n");
        }
        if(write(filedes[WRITE], "pid111111\n", strlen("pid111111\n"))!=strlen("pid111111\n")){
        	printf("pid1写入错误\n");
        }
		lockf(filedes[1],0,0);
        exit(0);
    }



    if ((pid2 = fork()) == -1) {
        printf("fork error(pid2)\n");
        exit(1);
    }
    if (pid2 == 0) {
		sleep(1);
        printf("正在产生子进程pid2:%d\n",getpid());
        close(filedes[READ]);
        lockf(filedes[1],1,0);
        write(filedes[WRITE], "pid222222\n", strlen("pid222222\n"));
        write(filedes[WRITE], "pid222222\n", strlen("pid222222\n"));
		sleep(1);
        write(filedes[WRITE], "pid222222\n", strlen("pid222222\n"));
        write(filedes[WRITE], "pid222222\n", strlen("pid222222\n"));
		lockf(filedes[1],0,0);
        exit(0);
    }


    if ((pid3 = fork()) == -1) {
        printf("fork error(pid3)\n");
        exit(1);
    }
    if (pid3 == 0) {
		sleep(1);
        printf("正在产生子进程pid3:%d\n",getpid());
        close(filedes[READ]);
        lockf(filedes[1],1,0);
        write(filedes[WRITE], "pid333333\n", strlen("pid333333\n"));
        write(filedes[WRITE], "pid333333\n", strlen("pid333333\n"));
        write(filedes[WRITE], "pid333333\n", strlen("pid333333\n"));
		sleep(1);
        write(filedes[WRITE], "pid333333\n", strlen("pid333333\n"));
		lockf(filedes[1],0,0);
        exit(0);
    }
    else {
        //waitpid()会暂时停止目前进程的执行，直到有信号来或者子进程结束
        pid1 = waitpid(pid1, NULL, WUNTRACED);
        pid2 = waitpid(pid2, NULL, WUNTRACED);
        pid3 = waitpid(pid3, NULL, WUNTRACED);
        printf("父进程的pid: %d\n",getpid());
        printf("三个子进程的pid: %d %d %d 返回信息\n",pid1,pid2,pid3);
        /*父进程从管道读取子进程写的数据，关闭管道的写端*/
        close(filedes[WRITE]);
        lockf(filedes[1],1,0);
        //read()：读取的数据保存在缓冲区buf
        read(filedes[READ], buf, sizeof(buf));
        printf("3个子进程传输的数据为：\n%s\n", buf);
		lockf(filedes[1],0,0);
    }
    return 0;
}
