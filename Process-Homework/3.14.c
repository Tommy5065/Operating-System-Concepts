#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        perror("argument should 2!");
        exit(EXIT_FAILURE);
    }
    __pid_t rc;
    rc = fork();
    if (rc < 0)
    {
        perror("rc!");
        exit(EXIT_FAILURE);
    }
    else if (rc == 0)
    {
        int start_number = atoi(argv[1]);
        while (start_number != 1)
        {
            if (start_number % 2 == 0)
            {
                start_number /= 2;
            }
            else
            {
                start_number = start_number * 3 + 1;
            }
            printf("%d ", start_number);
        }
    }
    else
    {
        wait(NULL);
        printf("I am father");
    }
}