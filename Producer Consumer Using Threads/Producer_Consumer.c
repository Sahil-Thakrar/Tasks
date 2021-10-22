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

static void *Producer()
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        while(BufferIndex == 5)
        {
        	pthread_cond_wait(&Buffer_Empty,&mutex);
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
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&Buffer_Full);
    }
    return NULL;
}
static void *Consumer()
{
    while(1)
    {
        pthread_mutex_lock(&mutex);
        while(BufferIndex == 0)
        {
        	pthread_cond_wait(&Buffer_Full,&mutex);
        }
        for(int i=0;i<BufferSize;i++)
        {
            printf("Consume %d : %d\n",i+1,BUFFER[i]);
            BufferIndex--;
        }
        printf("Buffer is empty now\n");
        sleep(2);
        pthread_mutex_unlock(&mutex);
        pthread_cond_signal(&Buffer_Empty);
    }
    return NULL;
}
int main()
{
    pthread_t pro,con;
    pthread_create(&pro,NULL,Producer,NULL);
    pthread_create(&con,NULL,Consumer,NULL);
    pthread_join(pro,NULL);
    pthread_join(con,NULL);
    return 0;
}
