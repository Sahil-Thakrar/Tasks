#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <signal.h>
#define BufferSize 5

static int BufferIndex=0,count=1;
static char BUFFER[BufferSize];
static pthread_cond_t Buffer_Full=PTHREAD_COND_INITIALIZER;
static pthread_cond_t Buffer_Empty=PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mutex=PTHREAD_MUTEX_INITIALIZER;
int err;

static void *Producer()
{
    while(1)
    {
        err=pthread_mutex_lock(&mutex);
        if(err!=0)
        {
        	perror("Mutex Lock of producer Failed ");
        }
        while(BufferIndex == 5)
        {
        	err=pthread_cond_wait(&Buffer_Empty,&mutex);
        	if(err!=0)
        	{
        		perror("Conditional waiting for Empty Signal got failed ");
        	}
        }
        for(int i=0;i<BufferSize;i++)
        {
            BUFFER[i]=count;
            printf("Produce %d : %d\n",i+1,BUFFER[i]);
            BufferIndex++;
            count++;
        }
        printf("Buffer is full now\n");
        sleep(2);
        err=pthread_mutex_unlock(&mutex);
        if(err!=0)
        {
        	perror("Mutex Unlock of producer Failed ");
        }
        err=pthread_cond_signal(&Buffer_Full);
        if(err!=0)
        {
        	perror("Signalling Full got an error ");
        }
    }
    return NULL;
}
static void *Consumer()
{
    while(1)
    {
        err=pthread_mutex_lock(&mutex);
        if(err!=0)
        {
        	perror("Mutex Lock of Consumer Failed ");
        }
        while(BufferIndex == 0)
        {
        	err=pthread_cond_wait(&Buffer_Full,&mutex);
        	if(err!=0)
        	{
        		perror("Conditional waiting for Full Signal got failed ");
        	}
        }
        for(int i=0;i<BufferSize;i++)
        {
            printf("Consume %d : %d\n",i+1,BUFFER[i]);
            BufferIndex--;
        }
        printf("Buffer is empty now\n");
        sleep(2);
        err=pthread_mutex_unlock(&mutex);
        if(err!=0)
        {
        	perror("Mutex Unlock of consumer Failed ");
        }
        err=pthread_cond_signal(&Buffer_Empty);
        if(err!=0)
        {
        	perror("Signalling of Empty got failed ");
        }
    }
    return NULL;
}
int main()
{
    pthread_t pro,con;
    err=pthread_create(&pro,NULL,Producer,NULL);
    if(err!=0)
    {
        perror("Creation of Producer Thread Failed ");
    }
    err=pthread_create(&con,NULL,Consumer,NULL);
    if(err!=0)
    {
        perror("Creation of Consumer Thread Failed ");
    }
    err=pthread_join(pro,NULL);
    if(err!=0)
    {
        perror("Joining of Producer Thread Failed ");
    }
    err=pthread_join(con,NULL);
    if(err!=0)
    {
        perror("Joining of Consumer Thread Failed ");
    }
    return 0;
}
