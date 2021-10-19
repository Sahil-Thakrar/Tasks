#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <syslog.h>
#include <signal.h>

#define MAX_NUM_THREADS         5	

FILE *filePointer;
char buff[50];
static pthread_t thread_list[MAX_NUM_THREADS];
static unsigned int thread_list_counter=0;
static int Infinite;
static int log;

static void sigintHandler(int sig_num)
{
    printf("\n You Cannot terminate the process with keyboard Interrupt \n");
    fflush(stdout);
}

static void logs(long unsigned int id)
{
    syslog(LOG_INFO,"thread [%lu] is running...\n",id);
    syslog(LOG_DEBUG,"A Debug...");
}

static void file_content(char *data,FILE *pointer)
{
    fputs(data,pointer);
}

void* thread_func(void *arg)
{
    sprintf(buff,"thread [%lu] is running...\n", pthread_self());
    if(Infinite == 1)
    {
        while(1)
        {
            (log == 0) ? file_content(buff,filePointer):logs(pthread_self());
            sleep(1);
        }
    }
    else
    {
            (log == 0) ? file_content(buff,filePointer):logs(pthread_self());
            sleep(1);
    }
    return NULL;
}

static void create_threads()
{
    printf("THREAD COUNTER -> %d\n",thread_list_counter);
    if(thread_list_counter >= MAX_NUM_THREADS)
    {
        syslog(LOG_ERR,"Can't create more than %d threads\n", MAX_NUM_THREADS);
    }
    else
    {
        int result = pthread_create(&thread_list[thread_list_counter], NULL, thread_func, NULL);
        if(result)
        {
            syslog(LOG_ERR,"Error while creating thread #%d...\n", thread_list_counter);
        }
        else
        {
            printf("Thread [%lu] created successfully...\n", thread_list[thread_list_counter]);
            thread_list_counter++;
        }
    }
}

static void print_thread_status()
{
    printf("THREAD COUNTER -> %d\n",thread_list_counter);
    printf("Below is the list of threads created by user...\n");
    printf("=====================================\n");
    printf("Thread[Index] - [pthread_t ID]\n");
    for(int i=0; i<thread_list_counter; i++)
    {
        printf("Thread[%d]     - [%lu]\n", i, thread_list[i]);
    }
    printf("=====================================\n");
}

static void join_thread(pthread_t tid)
{
    printf("THREAD COUNTER -> %d\n",thread_list_counter);
    int result = pthread_join(tid, NULL);
    (result) ? syslog(LOG_ERR,"Error while joining thread [%lu]...\n", tid) : printf("Thread [%lu] joined successfully...\n", tid);
}

static void join_threads()
{
    printf("THREAD COUNTER -> %d\n",thread_list_counter);
    for(int i=0; i<thread_list_counter; i++)
    {
        int result = pthread_cancel(thread_list[i]);
        (result) ? syslog(LOG_ERR,"Error while joining thread [%lu]...\n", thread_list[i]) : NULL;//printf("Thread [%lu] joined successfully...\n", thread_list[i]);
    }
}

static void cancel_threads()
{
    int cancel_index = 0;
    print_thread_status();
    printf("Pick index to cancel corresponding thread -> ");
    scanf(" %d",&cancel_index);
    if(cancel_index <= thread_list_counter)
    {
        int result = pthread_cancel(thread_list[cancel_index]);
        if(result)
        {
            syslog(LOG_ERR,"Error while cancel thread [%lu]...\n", thread_list[cancel_index]);
        }
        else
        {
            // This part will only be execute when thread is infinite
            printf("Thread [%lu] cancel requested successfully...\n", thread_list[cancel_index]);
            // As we are cancel thread we also need to join that thread right away to remove the entire instance of that thread
            join_thread(thread_list[cancel_index]);
            // remove pthread_t ID from thread_list array and decrement thread_list_counter value by 1 
            // to make contigueuos array with one free index
            for(int i=cancel_index; i<thread_list_counter; i++)
            {
                thread_list[i] = thread_list[i+1];
            }
            thread_list_counter--;
        }
    }
    else
    {    
        printf("Invalid index choose by user\n");
    }
}
int main()
{
    signal(SIGINT, sigintHandler);   
    filePointer=fopen("menu.txt", "w+");
    openlog("Thread in syslog", LOG_PID|LOG_CONS, LOG_USER);
    int choice;
    while(1)
    {
        printf("#### Menu ####\n");
        printf("Enter 1 for display thread status\n");
        printf("Enter 2 for create thread\n");
        printf("Enter 3 for cancel thread\n");
        printf("Enter 4 for exit\n");
        printf("\nChoice -> ");
        scanf(" %d",&choice);
        switch(choice)
        {
            case 1:
                print_thread_status();
                break;
            case 2:
                printf("If You Want\n0:Single Thread\n1:Infinite Thread\n");
                scanf("%d",&Infinite);
                printf("You Want logs in\n0:File\n1:Syslog\n");
                scanf("%d",&log);
                create_threads();
                break;
            case 3:
                cancel_threads();
                break;
            case 4:
            default:
                closelog();
                fclose(filePointer);
                filePointer=NULL;
                goto exit;
                break;
        }
    }
    exit:join_threads();
    return 0;
}
