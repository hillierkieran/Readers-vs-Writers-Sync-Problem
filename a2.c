/**
 * @file    a2.c
 * @author  Kieran Hillier
 * @date    August 14, 2023
 * @version 0.1
 * 
 * @brief   A program to demonstrate readers and writers synchronization.
 * 
 * @details A multi-threaded program that creates a random number of 
 *          incrementer, decrementer, and reader threads. Shared data 
 *          is protected using POSIX semaphores.
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#include "shared_data.h"

#define MAX_THREADS 10

SharedData data = {0, -1, -1, 0};
sem_t data_sem;     /* Semaphore for data access    */
sem_t count_sem;    /* Semaphore for reader count   */
int readers_count = 0;

//---------------------------------------------------------------80-Char-Limit-|

/**
 * @brief   Handles error messages.
 * 
 * @param   msg The error message to be displayed.
 */
void handle_error(const char* msg) {
    perror(msg);
    exit(EXIT_FAILURE);
}

/**
 * @brief   Modify the global sum by a given increment value.
 * 
 * @param   arg A pointer to the thread ID.
 * @param   increment The value to add to the global sum (can be negative).
 * @return  NULL
 */
void* modify_value(void* arg, int increment) {
    int id = *(int*)arg;

    sem_wait(&data_sem);
    
    data.sum += increment;
    if (increment > 0) {
        data.last_incr_id = id;
    } else {
        data.last_decr_id = id;
    }
    data.num_writers += 1;
    
    printf(increment > 0 ? 
            "Incrementer %d set sum = %d\n" : 
            "Decrementer %d set sum = %d\n"
            , id, data.sum);

    sem_post(&data_sem);
    return NULL;
}

/** 
 * @brief   Increment the global sum by 1.
 * 
 * @param   arg A pointer to the thread ID.
 * @return  NULL
 */
void* incrementer(void* arg) {
    return modify_value(arg, 1);
}

/** 
 * @brief   Decrement the global sum by 1.
 * 
 * @param   arg A pointer to the thread ID.
 * @return  NULL
 */
void* decrementer(void* arg) {
    return modify_value(arg, -1);
}

/**
 * @brief   Read the global sum.
 * 
 * @param   arg A pointer to the thread ID.
 * @return  NULL
 */
void* reader(void* arg) {
    int id = *(int*)arg;
    int value;

    sem_wait(&count_sem);
    readers_count++;
    if (readers_count == 1) {
        sem_wait(&data_sem);
    }
    sem_post(&count_sem);

    value = data.sum;
    printf("Reader %d got %d\n", id, value);

    sem_wait(&count_sem);
    readers_count--;
    if (readers_count == 0) {
        sem_post(&data_sem);
    }
    sem_post(&count_sem);

    return NULL;
}

/**
 * @brief   Main function to create threads and print the final state.
 * 
 * @return  Does not return a value; instead, exits with status 
 *          `EXIT_SUCCESS` to indicate successful execution.
 */
int main() {
    pthread_t threads[MAX_THREADS]; /* Array of thread IDs */
    int ids[MAX_THREADS];           /* Array of numerical IDs */

    /* Seed random number generator with current time */
    srand(time(NULL));

    /* Initialize the semaphores */
    if(sem_init(&data_sem, 0, 1) != 0) {
        handle_error("Error initializing data semaphore");
    }
    if(sem_init(&count_sem, 0, 1) != 0) {
        handle_error("Error initializing count semaphore");
    }

    /* Randomly decide number of threads to be created */
    int num_incrementers = rand() % (MAX_THREADS / 2) + 1;
    int num_decrementers = rand() % (MAX_THREADS / 2) + 1;
    int num_readers = MAX_THREADS - (num_incrementers + num_decrementers);

    /* Create incrementer threads */
    for (int i = 0; i < num_incrementers; i++) {
        ids[i] = i;
        if (pthread_create(&threads[i], NULL, incrementer, &ids[i])) {
            handle_error("Error creating incrementer thread");
        }
    }

    /* Create decrementer threads */
    for (int i = 0; i < num_decrementers; i++) {
        ids[num_incrementers + i] = i;
        if (pthread_create(&threads[num_incrementers + i],
            NULL,decrementer, &ids[num_incrementers + i])) {
            handle_error("Error creating decrementer thread");
        }
    }

    /* Create reader threads */
    for (int i = 0; i < num_readers; i++) {
        ids[num_incrementers + num_decrementers + i] = i;
        if (pthread_create(&threads[num_incrementers + num_decrementers + i],
            NULL, reader, &ids[num_incrementers + num_decrementers + i])) {
            handle_error("Error creating reader thread");
        }
    }

    /* Join all threads */
    for (int i = 0; i < MAX_THREADS; i++) {
        if (pthread_join(threads[i], NULL)) {
            handle_error("Error joining thread");
        }
    }

    /* Print the final state */
    printf("There were %d readers, %d incrementers and %d decrementers\n",
            num_readers, num_incrementers, num_decrementers);
    printf("The final state of the data is:\n"
            "\tlast incrementer %d\n"
            "\tlast decrementer %d\n"
            "\ttotal writers %d\n"
            "\tsum %d\n", 
            data.last_incr_id, data.last_decr_id, data.num_writers, data.sum);

    /* Destroy the semaphores */
    sem_destroy(&data_sem);
    sem_destroy(&count_sem);

    exit(EXIT_SUCCESS);
}