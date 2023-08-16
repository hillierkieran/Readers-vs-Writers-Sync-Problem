/**
 * @file    thread_operations.h
 * @author  Kieran Hillier
 * @date    August 16, 2023
 * @version 1.0
 * 
 * @brief   Header for thread operations.
 * 
 * @details Contains declarations for functions related to thread operations
 *          such as creation, joining, reading, incrementing, and decrementing.
 */

#ifndef THREAD_OPERATIONS_H
#define THREAD_OPERATIONS_H

#include <pthread.h>
#include "shared_data.h"

/**
 * @brief   Thread function to increment the shared data sum by 1.
 * 
 * @param   arg Pointer to the thread ID.
 * @return  NULL
 */
void* incrementer(void* arg);

/**
 * @brief   Thread function to decrement the shared data sum by 1.
 * 
 * @param   arg Pointer to the thread ID.
 * @return  NULL
 */
void* decrementer(void* arg);

/**
 * @brief   Thread function to read the shared data sum.
 * 
 * @param   arg Pointer to the thread ID.
 * @return  NULL
 */
void* reader(void* arg);

/**
 * @brief   Creates a specified number of threads of a certain type.
 * 
 * @param   threads          Array to hold thread IDs.
 * @param   max_threads      Maximum allowable threads.
 * @param   start_index      Starting index in threads array.
 * @param   num_threads      Number of threads to create.
 * @param   thread_type      Pointer to thread's main function.
 * @param   thread_type_name Name representing thread type.
 * @return  Updated index after all threads are created.
 */
int create_threads (pthread_t threads[], int max_threads, int start_index, 
                    int num_threads, void *(*thread_type)(void *), 
                    const char *thread_type_name);

/**
 * @brief   Joins created threads.
 * 
 * @param   threads Array containing created thread IDs.
 * @param   count Count of threads to be joined.
 */
void join_threads(pthread_t threads[], int count);

#endif /* THREAD_OPERATIONS_H */
