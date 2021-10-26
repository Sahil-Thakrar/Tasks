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
	    printf("\n\n");
       sleep(10);
    }
    return NULL;
}
int main()
{
    int err;
    //char buff[100];
    //int j[]={77,55,88,66,99};
    int j[]={6,8,5,7,12};
    int z[]={0,1,2,3,4};
    pthread_t thread_id[5];
    pthread_attr_t attr[5];
    struct sched_param param[5];
    int a=sched_get_priority_max(SCHED_FIFO);
    int b=sched_get_priority_min(SCHED_FIFO);
    printf("MAX:: %d\n",a);
    printf("MIN:: %d\n",b);
    for(int i=0;i<5;i++)
    {
        //sprintf(buff,"Thread %d",i);
        //pthread_setname_np(thread_id[i],"HIh");
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
        printf("Before Thread %d\n",i);
    }
 
    for(int i=0;i<5;i++)
    {
    	err=pthread_create(&thread_id[i],&attr[i],myThreadFun,(void *)&z[i]);
        if(err)
        {
            printf("Thread cannot be created\n");
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
