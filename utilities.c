/**
 * @file    utilities.c
 * @author  Kieran Hillier
 * @date    August 15, 2023
 * @version 0.1
 * 
 * @brief   Source file for utility functions.
 * 
 * @details Implements utility functions like error handling.
 */

#include "common.h"
#include "resources.h"
#include "utilities.h"
#include "shared_data.h"

void mutex_lock(pthread_mutex_t *mutex)
{
    pthread_mutex_lock(mutex);
}

void mutex_unlock(pthread_mutex_t *mutex)
{
    pthread_mutex_unlock(mutex);
}

void sem_lock(sem_t *semaphore)
{
    sem_wait(semaphore);
}

void sem_unlock(sem_t *semaphore)
{
    sem_post(semaphore);
}

void cleanup()
{
    destroy_resources();
    destroy_shared_data();
}

void handle_error(const char* msg)
{
    fprintf(stderr, "%s\n", msg);
    cleanup();
    exit(EXIT_FAILURE);
}
