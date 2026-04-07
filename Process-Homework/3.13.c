#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#define MIN_PID 300
#define MAX_PID 5000
#define SIZE (sizeof(unsigned int) * MAX_PID / 32) + 1

unsigned int *bitmap;
int allocate_map(void);
int allocate_pid(void);
void release_pid(int pid);

int main()
{
    int pid;
    allocate_map();
    for (size_t i = MIN_PID; i <= MAX_PID; ++i)
    {
        pid = allocate_pid();
        printf("%d ", pid);
    }
    if (pid > 0)
        release_pid(pid);
}

int allocate_map(void)
{
    bitmap = (unsigned int *)malloc(SIZE);
    if (bitmap == NULL)
    {
        perror("bitmap!");
        return -1;
    }
    memset(bitmap, 0, SIZE);
    return 1;
}

int allocate_pid(void)
{
    for (size_t i = MIN_PID; i <= MAX_PID; ++i)
    {
        size_t a = i / 32;
        size_t b = i % 32; /*位运算，实现位偏移，在一个字节中是第几位*/
        if (((bitmap[a] >> b) & 1) == 0)
        {
            bitmap[a] |= (1 << b); /*1<<b为了生成只有第b位是1的掩码,然后与bitmap[a]进行按位或、按位与*/
            /*按位或的特点是该是1就置1:x|1=1,x|0=0(不变)*/
            return i;
        }
    }
    return -1;
}

void release_pid(int pid)
{
    size_t a = pid / 32;
    size_t b = pid % 32;
    bitmap[a] &= ~(1 << b); /*按位与的特点是该位是0就置0:x|0=0,x|1=1(不变)*/
}