/**
 * @file a3q2.c
 * @author Mohammad El-Hassan (elha7950@mylaurier.ca)
 * @brief
 * @version 0.1
 * @date 2025-11-07
 *
 * @copyright Copyright (c) 2025
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_CHAIRS 5
#define NUM_CUSTOMERS 15

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t barber_cond = PTHREAD_COND_INITIALIZER;
pthread_cond_t customer_cond = PTHREAD_COND_INITIALIZER;

int waitingCustomers = 0;
int barberSleeping = 0;

/**
 * @brief
 *
 * @param arg
 * @return void*
 */
void *barber(void *arg)
{
    while (1)
    {
        // Acquire the lock because waitingCustomers is a shared variable
        // And it is a critical section.
        pthread_mutex_lock(&mutex);

        // WHILE is important because of waking up
        // While there are no waiting customers, the barber should sleep.
        while (waitingCustomers == 0)
        {
            printf("Barber is going to sleep.\n");
            barberSleeping = 1;
            pthread_cond_wait(&barber_cond, &mutex);
        }

        // Once the Barber has been woken, set sleeping barbers to 0
        // This will be signaled from a customer.
        barberSleeping = 0;
        waitingCustomers--;

        printf("Barber is cutting hair\n");

        // Free the lock
        pthread_mutex_unlock(&mutex);
    }
    pthread_exit(NULL);
}

/**
 * @brief
 *
 * @param arg
 * @return void*
 */
void *customer(void *arg)
{
    int id = *((int *)arg);

    // Acquire lock
    pthread_mutex_lock(&mutex);

    if (waitingCustomers < NUM_CHAIRS)
    {
        waitingCustomers++;
        printf("Customer %d is waiting.\n", id);

        if (barberSleeping)
        {
            pthread_cond_signal(&barber_cond);
        }

        pthread_cond_wait(&customer_cond, &mutex);

        pthread_mutex_unlock(&mutex);

        printf("Customer %d is getting a haircut\n", id);
    }
    else
    {
        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main()
{
    pthread_t barber_thread;
    pthread_t customer_threads[NUM_CUSTOMERS];

    pthread_create(&barber_thread, NULL, barber, NULL);

    int ids[NUM_CUSTOMERS];

    for (int i = 0; i < NUM_CUSTOMERS; i++)
    {
        ids[i] = i + 1;
        pthread_create(&customer_threads[i], NULL, customer, &ids[i]);
    }

    for (int i = 0; i < NUM_CUSTOMERS; i++)
    {
        pthread_join(customer_threads[i], NULL);
    }

    pthread_cancel(barber_thread);
    pthread_join(barber_thread, NULL);

    pthread_mutex_destroy(&mutex);
    pthread_cond_destroy(&barber_cond);
    pthread_cond_destroy(&customer_cond);

    return 0;
}
