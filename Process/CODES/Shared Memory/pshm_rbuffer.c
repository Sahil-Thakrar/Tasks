#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#define NAME "/shm_buff"
#define SIZE 10
#define SPACE (SIZE * sizeof(int))

int main()
{
    
    const char *semName = "buffer";
    int fd = shm_open(NAME, O_RDONLY,0666);
    if (fd < 0)
    {
        perror("shm_open is having error ");
        return 1;
    }
    sem_t *sem_id= sem_open(semName, O_CREAT, 0666,0);
    if (sem_id == SEM_FAILED)
    {
        perror("sem open Failed\n");
        return 1;
    }
    int *data = (int *)mmap(0, SPACE, PROT_READ, MAP_SHARED, fd, 0);
    while(1)
    {
        sem_wait(sem_id);
        for (int i = 0; i < SIZE; i++)
        {
            printf("The Data %d got is %d\n",i+1,data[i]);
        }
    }
    munmap(data,SPACE);
    shm_unlink(NAME);
    sem_close(sem_id);
    close(fd);
    return 0;
}
