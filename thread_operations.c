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
#include "utilities.h"
#include "thread_operations.h"

extern sem_t data_sem;      /* Semaphore for data access    */
extern sem_t reader_sem;    /* Semaphore for reader count   */
extern int readers_count;   /* Count of threads currently reading */

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

    /* retrieve the shared data */
    SharedData *data = get_shared_data();

    if (increment == 0) {  /* Read operation */

        sem_wait(&reader_sem);
        readers_count++;
        /* If first reader, lock for reading */
        if (readers_count == 1) {
            sem_wait(&data_sem);
        }
        sem_post(&reader_sem);

        /* Read the value of the shared data */
        int value = read_shared_data();
        printf("Reader %d got %d\n", id, value);

        sem_wait(&reader_sem);
        readers_count--;
        /* If last reader, unlock for writers */
        if (readers_count == 0) {
            sem_post(&data_sem);
        }
        sem_post(&reader_sem);

    } else {  /* Write operation */
    
        sem_wait(&data_sem);
        
        /* Modify the shared data based on the increment value */
        modify_shared_data(increment, id);
        if (increment > 0) {
            printf("Incrementer %d set sum = %d\n", id, data->sum);
        } else {
            printf("Decrementer %d set sum = %d\n", id, data->sum);
        }

        sem_post(&data_sem);
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
int create_threads( pthread_t threads[], int start_index, 
                    int num_threads, void *(*thread_type)(void *), 
                    const char *error_msg)
{
    if (start_index + num_threads - 1 > MAX_THREADS) {
        handle_error("Error, exceeded maximum threads");
    }
    for (int i = 0; i < num_threads; i++) {
        int* thread_id = malloc(sizeof(int));
        if (!thread_id) {
            handle_error("Error allocating memory for thread ID");
        }
        *thread_id = i;
        if (pthread_create(&threads[start_index + i], 
            NULL, thread_type, thread_id)) {
            free(thread_id);
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
void join_threads(pthread_t threads[], int count)
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
