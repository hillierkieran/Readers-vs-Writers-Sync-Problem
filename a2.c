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
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#include "shared_data.h"

#define MAX_THREADS 10

SharedData data = {0, -1, -1, 0};   /* Shared data structure                */
int readers_count = 0;              /* Count of threads currently reading   */
sem_t data_sem;                     /* Semaphore for data access            */
sem_t count_sem;                    /* Semaphore for reader count           */
pthread_t threads[MAX_THREADS];     /* Array of thread IDs                  */
int ids[MAX_THREADS];               /* Array of numerical IDs               */


/**
 * @brief   Handles error messages.
 * 
 * @param   msg The error message to be displayed.
 */
void handle_error(const char* msg)
{
    fprintf(stderr, "%s\n", msg);
    exit(EXIT_FAILURE);
}

/**
 * @brief   Shared data operation, either read or modify based on increment.
 * 
 * @param   arg A pointer to the thread ID.
 * @param   increment The value to add to the global sum 
 *          (0 for read, positive or negative for write).
 * @return  NULL
 */
void* shared_data_operation(void* arg, int increment)
{
    /* retrieve the thread ID */
    int id = *(int*)arg;

    if (increment == 0) {  /* Read operation */
        int value;

        /* Acquire the count semaphor to safely increment the readers_count */
        sem_wait(&count_sem);
        
        readers_count++;  /* Increase the count of current active readers */

        /* If this is the first reader, lock the data for reading */
        if (readers_count == 1) {
            sem_wait(&data_sem);
        }
        
        /* Release the count semaphore */
        sem_post(&count_sem);

        /* Read the value of the shared data */
        value = data.sum;
        printf("Reader %d got %d\n", id, value);

        /* Acquire the count semaphore to safely decrement the readers_count */
        sem_wait(&count_sem);
        
        readers_count--;  /* Decrease the count of current active readers */

        /* If this is the last reader, release the data semaphore for writers */
        if (readers_count == 0) {
            sem_post(&data_sem);
        }

        /* Release the count semaphore */
        sem_post(&count_sem);

    } else {  /* Write operation */
        /* Acquire the data semaphore to ensure exclusive access for writing */
        sem_wait(&data_sem);
        
        /* Modify the shared data based on the increment value */
        data.sum += increment;

        /* Update the last thread IDs and print respective messages */
        if (increment > 0) {
            data.last_incr_id = id;
            printf("Incrementer %d set sum = %d\n", id, data.sum);
        } else {
            data.last_decr_id = id;
            printf("Decrementer %d set sum = %d\n", id, data.sum);
        }

        data.num_writers++;  /* Update the number of writers */

        /* Release the data semaphore to allow other operations */
        sem_post(&data_sem);
    }

    return NULL;
}

/** 
 * @brief   Increment the global sum by 1.
 * 
 * @param   arg A pointer to the thread ID.
 * @return  NULL
 */
void* incrementer(void* arg)
{
    return shared_data_operation(arg, 1);
}

/** 
 * @brief   Decrement the global sum by 1.
 * 
 * @param   arg A pointer to the thread ID.
 * @return  NULL
 */
void* decrementer(void* arg)
{
    return shared_data_operation(arg, -1);
}

/**
 * @brief   Read the global sum.
 * 
 * @param   arg A pointer to the thread ID.
 * @return  NULL
 */
void* reader(void* arg)
{
    return shared_data_operation(arg, 0);  // 0 indicates a read operation
}

/**
 * @brief   Create threads of a specific type.
 * 
 * @param   start_index The starting index in the threads and ids arrays.
 * @param   num_threads Number of threads to create.
 * @param   thread_type Function pointer to the thread's main function.
 * @param   error_msg Error message to print if thread creation fails.
 * 
 * @return  The new index after all threads are created.
 */
int create_threads( int start_index, int num_threads, 
                    void *(*thread_type)(void *), const char *error_msg)
{
    for (int i = 0; i < num_threads; i++) {
        ids[start_index + i] = i;
        if (pthread_create(&threads[start_index + i], 
            NULL, thread_type, &ids[start_index + i])) {
            handle_error(error_msg);
        }
    }
    return start_index + num_threads;
}

/**
 * @brief   Joins created threads.
 * 
 * @param   count count of threads created
 */
void join_threads(int count)
{
    for (int i = 0; i < count; i++) {
        int err = pthread_join(threads[i], NULL);
        if (err != 0) {
            char buf[256];
            snprintf(buf, sizeof(buf), "Error joining thread: %s", 
                        strerror(err));
            handle_error(buf);
        }
    }
}

/**
 * @brief   Prints final state of the program.
 * 
 * @param   num_incrementers number of incrementer threads.
 * @param   num_decrementers number of decrementer threads.
 * @param   num_readers number of reader threads.
 */
void print_result(int num_incrementers,int num_decrementers, int num_readers)
{
    printf("There were %d readers, %d incrementers and %d decrementers\n",
            num_readers, num_incrementers, num_decrementers);
    printf("The final state of the data is:\n"
            "\tlast incrementer %d\n"
            "\tlast decrementer %d\n"
            "\ttotal writers %d\n"
            "\tsum %d\n", 
            data.last_incr_id, data.last_decr_id, data.num_writers, data.sum);
}

/**
 * @brief   Main function to create threads and print the final state.
 * 
 * @return  Does not return a value; instead, exits with status 
 *          `EXIT_SUCCESS` to indicate successful execution.
 */
int main()
{
    int num_incrementers;   /* Number of incrementer threads.       */
    int num_decrementers;   /* Number of decrementer threads.       */
    int num_readers;        /* Number of reader threads.            */
    int count = 0;          /* Count of all threads created so far  */

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
    num_incrementers = rand() % (MAX_THREADS / 2) + 1;
    num_decrementers = rand() % (MAX_THREADS / 2) + 1;
    num_readers = MAX_THREADS - (num_incrementers + num_decrementers);

    /* Create threads */
    count = create_threads(count, num_incrementers, incrementer, 
                            "Error creating incrementer thread");
    count = create_threads(count, num_decrementers, decrementer, 
                            "Error creating decrementer thread");
    count = create_threads(count, num_readers, reader, 
                            "Error creating reader thread");

    /* Join all threads */
    join_threads(count);

    /* Print the final state */
    print_result(num_incrementers, num_decrementers, num_readers);

    /* Destroy the semaphores */
    sem_destroy(&data_sem);
    sem_destroy(&count_sem);

    exit(EXIT_SUCCESS);
}
