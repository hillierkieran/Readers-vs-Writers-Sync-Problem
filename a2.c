/**
 * @file    a2.c
 * @author  Kieran Hillier
 * @date    August 15, 2023
 * @version 0.1
 * 
 * @brief   A program to demonstrate readers and writers synchronization.
 * 
 * @details A multi-threaded program that creates a random number of 
 *          incrementer, decrementer, and reader threads. Shared data 
 *          is protected using POSIX semaphores.
 */

#include "common.h"
#include "utilities.h"
#include "shared_data.h"
#include "thread_operations.h"

#define MAX_THREADS 10

int readers_count = 0;              /* Count of threads currently reading   */
sem_t data_sem;                     /* Semaphore for data access            */
sem_t count_sem;                    /* Semaphore for reader count           */
pthread_t threads[MAX_THREADS];     /* Array of thread IDs                  */
int ids[MAX_THREADS];               /* Array of numerical IDs               */


/**
 * @brief   Prints final state of the program.
 * 
 * @param   num_incrementers number of incrementer threads.
 * @param   num_decrementers number of decrementer threads.
 * @param   num_readers number of reader threads.
 */
void print_result(int num_incrementers,int num_decrementers, int num_readers)
{
    SharedData *data = get_shared_data();
    printf("There were %d readers, %d incrementers and %d decrementers\n",
            num_readers, num_incrementers, num_decrementers);
    printf("The final state of the data is:\n"
            "\tlast incrementer %d\n"
            "\tlast decrementer %d\n"
            "\ttotal writers %d\n"
            "\tsum %d\n", 
            data->last_incr_id, 
            data->last_decr_id, 
            data->num_writers, 
            data->sum);
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

    /* Initialise shared_data */
    init_shared_data();

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
    count = create_threads(threads, ids, count, num_incrementers, incrementer, 
                            "Error creating incrementer thread");
    count = create_threads(threads, ids, count, num_decrementers, decrementer, 
                            "Error creating decrementer thread");
    count = create_threads(threads, ids, count, num_readers, reader, 
                            "Error creating reader thread");

    /* Join all threads */
    join_threads(threads, count);

    /* Print the final state */
    print_result(num_incrementers, num_decrementers, num_readers);

    /* Free shared_data memory allocation */
    destroy_shared_data();

    /* Destroy the semaphores */
    sem_destroy(&data_sem);
    sem_destroy(&count_sem);

    exit(EXIT_SUCCESS);
}
