#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>
#define NAME "/shm_buff"
#define SIZE 10
#define SPACE (SIZE * sizeof(int))
int count=1;
int main() 
{
    while(1)
    {
    const char *semName = "buffer";
    int fd = shm_open(NAME, O_CREAT | O_EXCL | O_RDWR,0666);
    if (fd < 0)
    {
        perror("shm_open is having error ");
        return 1;
    }
    sem_t *sem_id = sem_open(semName, O_CREAT, 0666, 0);
    if (sem_id == SEM_FAILED)
    {
        perror("sem open Failed\n");
        return 1;
    }
    ftruncate(fd, SPACE);
    int *data = (int *)mmap(0, SPACE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    while(1)
    {
        for(int i = 0; i < SIZE;i++)
        {
            data[i] = count;
            printf("The data %d is ready,which is %d\n",i+1,data[i]);
            count++;
        }
        sem_post(sem_id);
        sleep(10);
    }
    sem_close(sem_id);
    sem_unlink(semName);
    munmap(data,SPACE);
    close(fd);
    }
    return 0;
}
