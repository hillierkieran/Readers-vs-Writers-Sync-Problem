/**
 * @file    utilities.c
 * @author  Kieran Hillier
 * @date    August 16, 2023
 * @version 1.0
 * 
 * @brief   Utility functions for the program.
 * 
 * @details Contains functions for handling errors, cleanup, mutex 
 *          locking/unlocking, and semaphore locking/unlocking.
 */

#include "common.h"
#include "resources.h"
#include "utilities.h"
#include "shared_data.h"

/**
 * @brief   Locks the provided mutex.
 * 
 * @param   mutex Pointer to the mutex to be locked.
 */
void mutex_lock(pthread_mutex_t *mutex)
{
    pthread_mutex_lock(mutex);
}

/**
 * @brief   Unlocks the provided mutex.
 * 
 * @param   mutex Pointer to the mutex to be unlocked.
 */
void mutex_unlock(pthread_mutex_t *mutex)
{
    pthread_mutex_unlock(mutex);
}

/**
 * @brief   Locks the provided semaphore.
 * 
 * @param   semaphore Pointer to the semaphore to be locked.
 */
void sem_lock(sem_t *semaphore)
{
    sem_wait(semaphore);
}

/**
 * @brief   Unlocks the provided semaphore.
 * 
 * @param   semaphore Pointer to the semaphore to be unlocked.
 */
void sem_unlock(sem_t *semaphore)
{
    sem_post(semaphore);
}

/**
 * @brief   Performs cleanup of resources and shared data.
 */
void cleanup()
{
    destroy_resources();    /* Destroy system resources */
    destroy_shared_data();  /* Destroy shared data structure */
}

/**
 * @brief   Handles error messages and initiates cleanup.
 * 
 * @param   msg String containing the error message to be displayed.
 */
void handle_error(const char* msg)
{
    /* Display given error message */
    fprintf(stderr, "%s\n", msg);

    /* Clean up allocated resources and exit. */
    cleanup();
    exit(EXIT_FAILURE);
}

/* end utilities.c*/
