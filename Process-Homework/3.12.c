#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int main()
{
    __pid_t rc;
    rc = fork();
    if (rc == -1)
    {
        perror("fork!");
        exit(EXIT_FAILURE);
    }
    if (rc == 0)
    {
        printf("child process:%d\n", (int)getpid());
        exit(0);
    }
    else
    {
        printf("I am father(pid:%d)\n", (int)getpid());
        sleep(60);
        /*实现僵尸进程的关键：1.子进程退出时父进程未调用wait 2.父进程一直活着，子进程不会被Init回收*/
    }
}