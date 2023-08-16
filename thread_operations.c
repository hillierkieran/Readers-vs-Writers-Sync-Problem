/**
 * @file    thread_operations.c
 * @author  Kieran Hillier
 * @date    August 15, 2023
 * @version 0.1
 * 
 * @brief   Source file for thread operations.
 * 
 * @details Implements the functions related to threads, like creating, 
 *          joining, reading, incrementing, and decrementing.
 */

#include "common.h"
#include "resources.h"
#include "utilities.h"
#include "thread_operations.h"

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

    /* Get resources pointer */
    Resources* rsc = get_resources();

    /* retrieve the shared data */
    SharedData *data = get_shared_data();

    if (increment == 0) {  /* Read operation */

        sem_lock(&rsc->reader_sem);
        rsc->readers_count++;
        /* If first reader, lock for reading */
        if (rsc->readers_count == 1) {
            sem_lock(&rsc->data_sem);
        }
        sem_unlock(&rsc->reader_sem);

        /* Read the value of the shared data */
        int value = data->sum;
        printf("Reader %d got %d\n", id, value);

        sem_lock(&rsc->reader_sem);
        rsc->readers_count--;
        /* If last reader, unlock for writers */
        if (rsc->readers_count == 0) {
            sem_unlock(&rsc->data_sem);
        }
        sem_unlock(&rsc->reader_sem);

    } else {  /* Write operation */
    
        sem_lock(&rsc->data_sem);
        
        /* Modify the shared data based on the increment value */
        modify_shared_data(increment, id);
        if (increment > 0) {
            printf("Incrementer %d set sum = %d\n", id, data->sum);
        } else {
            printf("Decrementer %d set sum = %d\n", id, data->sum);
        }

        sem_unlock(&rsc->data_sem);
    }

    free(arg);
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
    return shared_data_operation(arg, INCR_OP);
}

/** 
 * @brief   Decrement the global sum by 1.
 * 
 * @param   arg A pointer to the thread ID.
 * @return  NULL
 */
void* decrementer(void* arg)
{
    return shared_data_operation(arg, DECR_OP);
}

/**
 * @brief   Read the global sum.
 * 
 * @param   arg A pointer to the thread ID.
 * @return  NULL
 */
void* reader(void* arg)
{
    return shared_data_operation(arg, READ_OP);
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
int create_threads (pthread_t threads[], int max_threads, int start_index, 
                    int num_threads, void *(*thread_type)(void *), 
                    const char *thread_type_name)
{
    if (start_index + num_threads - 1 > max_threads) {
        /* Overflow. Trying to create more than maximum threads*/
        handle_error("Error, exceeded maximum threads");
    }
    for (int i = 0; i < num_threads; i++) {
        int* thread_id = malloc(sizeof(int));
        if (!thread_id) {
            /* Failed to allocating memory */
            char errorMsg[MAX_STRING];
            snprintf(errorMsg, sizeof(errorMsg),
                    "Error allocating memory for %s thread %d",
                    thread_type_name, start_index);
            handle_error(errorMsg);
        }
        *thread_id = i;
        if (pthread_create(&threads[start_index + i], 
            NULL, thread_type, thread_id)) {
            /* Failed to create thread */
            free(thread_id);
            char errorMsg[MAX_STRING];
            snprintf(errorMsg, sizeof(errorMsg),
                    "Error creating %s thread", thread_type_name);
            handle_error(errorMsg);
        }
    }
    return start_index + num_threads;
}

/**
 * @brief   Joins created threads.
 * 
 * @param   count count of threads created
 */
void join_threads(pthread_t threads[], int count)
{
    for (int i = 0; i < count; i++) {
        int err = pthread_join(threads[i], NULL);
        if (err != 0) {
            /* Failed to join thread */
            char errorMsg[MAX_STRING];
            snprintf(errorMsg, sizeof(errorMsg),
                    "Error joining thread: %s", strerror(err));
            handle_error(errorMsg);
        }
    }
}
