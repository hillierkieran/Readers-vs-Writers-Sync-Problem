/**
 * @file    thread_operations.c
 * @author  Kieran Hillier
 * @date    August 16, 2023
 * @version 1.0
 * 
 * @brief   Implements thread-related operations.
 * 
 * @details Contains functions related to thread operations such as 
 *          creating, joining, reading, incrementing, and decrementing.
 */

#include "common.h"
#include "resources.h"
#include "utilities.h"
#include "thread_operations.h"

/**
 * @brief   Performs shared data operations based on increment.
 * 
 * @param   arg Pointer to the thread ID.
 * @param   increment Value indicating operation type (read/modify).
 * 
 * @return  NULL
 */
void* shared_data_operation(void* arg, int increment)
{
    /* Retrieve the thread ID from argument */
    int id = *(int*)arg;
    free(arg);

    /* Access the resources */
    Resources* rsc = get_resources();

    /* Access the shared data */
    SharedData *data = get_shared_data();

    if (increment == 0) {  /* Read operation */

        /* Lock to increment reader count */
        sem_lock(&rsc->reader_sem);
        rsc->readers_count++;

        /* If first reader, lock access to data */
        if (rsc->readers_count == 1) {
            sem_lock(&rsc->data_sem);
        }
        sem_unlock(&rsc->reader_sem);

        /* Read the shared data value */
        int value = data->sum;
        printf("Reader %d got %d\n", id, value);

        /* Lock to decrement reader count */
        sem_lock(&rsc->reader_sem);
        rsc->readers_count--;

        /* If last reader, unlock access to data */
        if (rsc->readers_count == 0) {
            sem_unlock(&rsc->data_sem);
        }
        sem_unlock(&rsc->reader_sem);

    } else {  /* Write operation */

        /* Lock to ensure exclusive data access */
        sem_lock(&rsc->data_sem);

        /* Modify shared data and print updates */
        modify_shared_data(increment, id);
        if (increment > 0) {
            printf("Incrementer %d set sum = %d\n", id, data->sum);
        } else {
            printf("Decrementer %d set sum = %d\n", id, data->sum);
        }

        /* Unlock to allow access to other threads */
        sem_unlock(&rsc->data_sem);
    }

    return NULL;
}

/**
 * @brief   Increments the global sum by 1.
 * 
 * @param   arg Pointer to the thread ID.
 * 
 * @return  NULL
 */
void* incrementer(void* arg)
{
    return shared_data_operation(arg, INCR_OP);
}

/**
 * @brief   Decreases the global sum by 1.
 * 
 * @param   arg Pointer to the thread ID.
 * 
 * @return  NULL
 */
void* decrementer(void* arg)
{
    return shared_data_operation(arg, DECR_OP);
}

/**
 * @brief   Reads the global sum.
 * 
 * @param   arg Pointer to the thread ID.
 * 
 * @return  NULL
 */
void* reader(void* arg)
{
    return shared_data_operation(arg, READ_OP);
}

/**
 * @brief   Creates a specified number of threads of a certain type.
 * 
 * @param   threads          Array to hold thread IDs.
 * @param   max_threads      Maximum allowable threads.
 * @param   start_index      Starting index in threads array.
 * @param   num_threads      Number of threads to create.
 * @param   thread_type      Pointer to thread's main function.
 * @param   thread_type_name Name representing thread type.
 * 
 * @return  Updated index after all threads are created.
 */
int create_threads (pthread_t threads[], int max_threads, int start_index, 
                    int num_threads, void *(*thread_type)(void *), 
                    const char *thread_type_name)
{
    /* Ensure that the number of threads doesn't exceed the maximum */
    if (start_index + num_threads - 1 > max_threads) {
        handle_error("Error, exceeded maximum threads");
    }

    /* Loop to create and initialize threads */
    for (int i = 0; i < num_threads; i++) {
        int* thread_id = malloc(sizeof(int));
        if (!thread_id) {
            char errorMsg[MAX_STRING];
            snprintf(errorMsg, sizeof(errorMsg),
                    "Error allocating memory for %s thread %d",
                    thread_type_name, start_index);
            handle_error(errorMsg);
        }
        *thread_id = i;

        /* Create the thread and handle errors */
        if (pthread_create(&threads[start_index + i], 
            NULL, thread_type, thread_id)) {
            free(thread_id);
            char errorMsg[MAX_STRING];
            snprintf(errorMsg, sizeof(errorMsg),
                    "Error creating %s thread", thread_type_name);
            handle_error(errorMsg);
        }
    }

    /* Return the updated index */
    return start_index + num_threads;
}

/**
 * @brief   Waits for specified threads to terminate.
 * 
 * @param   threads Array holding thread IDs.
 * @param   count   Number of threads to join.
 */
void join_threads(pthread_t threads[], int count)
{
    /* Loop to join each thread and handle errors */
    for (int i = 0; i < count; i++) {
        int err = pthread_join(threads[i], NULL);
        if (err != 0) {
            char errorMsg[MAX_STRING];
            snprintf(errorMsg, sizeof(errorMsg),
                    "Error joining thread: %s", strerror(err));
            handle_error(errorMsg);
        }
    }
}

/* end thread_operations.c */
