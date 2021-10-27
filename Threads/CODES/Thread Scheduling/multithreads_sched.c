#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sched.h>

void *myThreadFun(void *arg)
{
    int *no = (int *)arg;
    while(1)
    {
        printf("==============\n");
        printf("We are Printing from Thread no %d \n",*no);
        printf("==============\n");
        printf("\n");
        sleep(10);
    }
    return NULL;
}
int main()
{
    int err;
    char buff[16];
    int j[]={6,8,5,7,12};
    int z[]={0,1,2,3,4};
    pthread_t thread_id[5];
    pthread_attr_t attr[5];
    struct sched_param param[5];
    for(int i=0;i<5;i++)
    {
        param[i].sched_priority = j[i];
        err=pthread_attr_init(&attr[i]);
        if(err)
        {
            printf("Attributes cannot be initialized\n");
        }
        err=pthread_attr_setinheritsched(&attr[i],PTHREAD_EXPLICIT_SCHED);
        if(err)
        {
            printf("Inheritance attribute got failed\n");
        }
        err=pthread_attr_setschedpolicy(&attr[i],SCHED_FIFO);
        if(err)
        {
            printf("Scheduling Policy Attribute cannot be initialized\n");
        }
        err=pthread_attr_setschedparam(&attr[i],&param[i]);
        if(err)
        {
            printf("Setting of the scheduling parameters cannot be initialized\n");
        }
    }
    for(int i=0;i<5;i++)
    {
        printf("Before Thread %d\n\n",i);
        err=pthread_create(&thread_id[i],&attr[i],myThreadFun,(void *)&z[i]);
        if(err)
        {
            printf("Thread cannot be created\n");
        }
        sprintf(buff,"IT IS THREAD %d",i);
        err=pthread_setname_np(thread_id[i],buff);
        if(err)
        {
            printf("Name cannot be given to thread %d\n",i);
        }
    }
    for(int i=0;i<5;i++)
    {
        err=pthread_join(thread_id[i], NULL);
        if(err)
        {
            printf("Thread cannot be joined\n");
        }
        printf("After Thread %d\n",i);
    }
    pthread_exit(0);
    return 0;
}
