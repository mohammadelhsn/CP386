#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define MAX_BUFFER 5
#define TOTAL_ITEMS 10

int queue[MAX_BUFFER];
int writePos = 0;
int readPos = 0;
int itemsInBuffer = 0;

/** Edge case: There's 8 items consumed, but less than 3 in buffer, and it will never finish. */
int consumer_started = 0;

sem_t spaceAvailable;
sem_t itemsAvailable;
pthread_mutex_t lock;

void *producer_func(void *arg)
{
    for (int product = 1; product <= TOTAL_ITEMS; product++)
    {
        sem_wait(&spaceAvailable);
        pthread_mutex_lock(&lock);

        queue[writePos] = product;
        writePos = (writePos + 1) % MAX_BUFFER;
        itemsInBuffer++;

        printf("Produced %d\n", product);

        if (!consumer_started && itemsInBuffer >= 3)
            consumer_started = 1;

        pthread_mutex_unlock(&lock);
        sem_post(&itemsAvailable);
    }

    pthread_exit(NULL);
}

void *consumer_func(void *arg)
{
    int itemsConsumed = 0;

    while (1)
    {
        pthread_mutex_lock(&lock);
        if (consumer_started)
        {
            pthread_mutex_unlock(&lock);
            break;
        }
        pthread_mutex_unlock(&lock);
    }

    while (itemsConsumed < TOTAL_ITEMS)
    {
        sem_wait(&itemsAvailable);
        pthread_mutex_lock(&lock);

        int item = queue[readPos];
        readPos = (readPos + 1) % MAX_BUFFER;
        itemsInBuffer--;
        itemsConsumed++;

        printf("Consumed %d\n", item);

        pthread_mutex_unlock(&lock);
        sem_post(&spaceAvailable);
    }

    pthread_exit(NULL);
}

int main()
{
    pthread_t producerThread;
    pthread_t consumerThread;

    sem_init(&spaceAvailable, 0, MAX_BUFFER);
    sem_init(&itemsAvailable, 0, 0);
    pthread_mutex_init(&lock, NULL);

    pthread_create(&producerThread, NULL, producer_func, NULL);
    pthread_create(&consumerThread, NULL, consumer_func, NULL);

    pthread_join(producerThread, NULL);
    pthread_join(consumerThread, NULL);

    sem_destroy(&spaceAvailable);
    sem_destroy(&itemsAvailable);
    pthread_mutex_destroy(&lock);

    return 0;
}
