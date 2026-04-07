#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/mman.h>
#include <sys/wait.h>

int main(int argc, char *argv[])
{
    if (argc != 2)
    {
        perror("argument should 2! \n");
        exit(EXIT_FAILURE);
    }

    const int SIZE = 4096;
    char *name = "Collatz";

    int shm_fd;
    void *ptr;

    shm_fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if (shm_fd == -1)
    {
        perror("shm_open!");
        exit(EXIT_FAILURE);
    }

    if (ftruncate(shm_fd, SIZE) == -1)
    {
        perror("ftruncate! \n");
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
        ptr = mmap(0, SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
        int start_number = atoi(argv[1]);
        int len = sprintf(ptr, "%d ", start_number); /*sprintf返回写入缓冲区的字符数*/
        ptr += len;
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
            sprintf(ptr, "%d ", start_number);
            len = sprintf(ptr, "%d ", start_number);
            ptr += len;
        }
        munmap(ptr, SIZE);
        close(shm_fd);
    }
    else
    {
        wait(NULL);
        ptr = mmap(0, SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
        printf("%s ", (char *)ptr);
        shm_unlink(name);
    }
}