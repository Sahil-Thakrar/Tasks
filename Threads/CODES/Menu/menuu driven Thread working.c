#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define MAX_NUM_THREADS         5
#define INFINITE	

static pthread_t thread_list[MAX_NUM_THREADS];
static unsigned int thread_list_counter=0;

void* thread_func(void *arg)
{
#ifdef INFINITE
    while(1)
    {
        sleep(1);
        //printf("thread [%lu] is running...\n", pthread_self());
    }
#else
    printf("thread [%lu] is running...\n", pthread_self());
#endif
    return NULL;
}

void create_threads()
{
    printf("THREAD COUNTER -> %d\n",thread_list_counter);
    if(thread_list_counter >= MAX_NUM_THREADS)
    {
        printf("Can't create more than %d threads\n", MAX_NUM_THREADS);
    }
    else
    {
        int result = pthread_create(&thread_list[thread_list_counter], NULL, thread_func, NULL);
        if(result)
        {
            printf("Error while creating thread #%d...\n", thread_list_counter);
        }
        else
        {
            printf("Thread [%lu] created successfully...\n", thread_list[thread_list_counter]);
            thread_list_counter++;
        }
    }
}

void print_thread_status()
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

void join_thread(pthread_t tid)
{
    printf("THREAD COUNTER -> %d\n",thread_list_counter);
    int result = pthread_join(tid, NULL);
    if(result)
    {
        printf("Error while joining thread [%lu]...\n", tid);
    }
    else
    {
        printf("Thread [%lu] joined successfully...\n", tid);
    }
}

void join_threads()
{
    printf("THREAD COUNTER -> %d\n",thread_list_counter);
    for(int i=0; i<thread_list_counter; i++)
    {
        int result = pthread_cancel(thread_list[i]);
        if(result)
        {
            printf("Error while joining thread [%lu]...\n", thread_list[i]);
        }
        else
        {
            printf("Thread [%lu] joined successfully...\n", thread_list[i]);
        }
    }
}

void cancel_threads()
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
            printf("Error while cancel thread [%lu]...\n", thread_list[cancel_index]);
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
                create_threads();
                break;
            case 3:
                cancel_threads();
                break;
            case 4:
            default:
                goto exit;
                break;
        }
    }
exit:
    join_threads();
    return 0;
}

