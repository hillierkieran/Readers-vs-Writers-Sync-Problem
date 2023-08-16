/**
 * @file    thread_operations.h
 * @author  Kieran Hillier
 * @date    August 15, 2023
 * @version 0.1
 * 
 * @brief   Header file for thread operations.
 * 
 * @details Contains declarations for functions related to thread operations.
 */

#ifndef THREAD_OPERATIONS_H
#define THREAD_OPERATIONS_H

#include <pthread.h>
#include "shared_data.h"

void* incrementer(void* arg);
void* decrementer(void* arg);
void* reader(void* arg);
int create_threads( pthread_t threads[], int max_threads, int start_index, 
                    int num_threads, void *(*thread_type)(void *), 
                    const char *error_msg);
void join_threads(pthread_t threads[], int count);

#endif /* THREAD_OPERATIONS_H */
