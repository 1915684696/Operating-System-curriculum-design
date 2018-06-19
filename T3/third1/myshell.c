#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <ctype.h>
#include <pwd.h>

void GetLoginName()
{
    struct passwd* pass;
    pass = getpwuid(getuid());
    printf("%s@",pass->pw_name);
}
void GetHostName()
{
    char name[128];
    gethostname(name,sizeof(name));	//如果函数成功，则返回0。如果发生错误则返回-1。
    printf("%s",name);
}
void GetDir()
{
    char pwd[255];
    getcwd(pwd,255);
    printf(":%s@",pwd);
}


char *cmdStr [4]= {"exit","./cmd1","./cmd2","./cmd3"};


//对比所有命令参数，如果有一样的，就返回对应数字，用于后面执行
int getCmdIndex(char *cmd)
{
    int i;
    for(i=0;i<4;i++)
    {
        if (strcmp(cmd,cmdStr[i])==0)
        {
            return i;
        }
    }
    return -1;
}

void myFork(int cmdIndex)
{
    pid_t pid;

    if((pid = fork())<0)
    {
        printf("创建子进程错误\n");
        exit(0);
    }
    else if (pid == 0)
    {
        int execl_status = -1;

        switch(cmdIndex)
        {
            case 1:
                execl_status = execl("./cmd1","",NULL);
                break;
            case 2:
                execl_status = execl("./cmd2","",NULL);
                break;
            case 3:
                execl_status = execl("./cmd3","",NULL);
                break;
            default:
                printf("无此命令！！！\n");
                break;
        }
        if(execl_status<0)
        {
            printf("创建错误\n");
            exit(0);
        }

        printf("运行完毕！\n");
        exit(0);
    }
    else{
        return;
    }

}

//运行cmd
void runCMD(int cmdIndex)
{
    switch(cmdIndex)
    {
        case -1:
            printf("Command Not Found \n"); //没有找到该命令
            break;
        case 0: //exit命令返回0
            exit(0);
            break;
        default:
            myFork(cmdIndex); //创建子进程运行
            break;
    }
}

int main()
{
    pid_t pid;

    char cmdStr[30]; //命令数组（最长30）

    int cmdIndex; //用于显示运行哪个数据
    while(1)
    {
    	GetLoginName();
    	GetHostName();
    	GetDir();
	printf(" ");
        scanf("%s",cmdStr);
        cmdIndex = getCmdIndex(cmdStr);
        runCMD(cmdIndex); //根据数字运行不同的cmd
        wait(0);
    }
}
