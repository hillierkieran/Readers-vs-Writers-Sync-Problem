/**
 * @file    utilities.h
 * @author  Kieran Hillier
 * @date    August 16, 2023
 * @version 1.0
 * 
 * @brief   Header for utility functions.
 * 
 * @details Provides declarations for utility functions like error 
 *          handling, mutex operations, semaphore operations, and cleanup.
 */

#ifndef UTILITIES_H
#define UTILITIES_H

/**
 * @brief   Locks the provided mutex.
 * 
 * @param   mutex Pointer to the mutex to be locked.
 */
void mutex_lock(pthread_mutex_t *mutex);

/**
 * @brief   Unlocks the provided mutex.
 * 
 * @param   mutex Pointer to the mutex to be unlocked.
 */
void mutex_unlock(pthread_mutex_t *mutex);

/**
 * @brief   Locks the provided semaphore.
 * 
 * @param   semaphore Pointer to the semaphore to be locked.
 */
void sem_lock(sem_t *semaphore);

/**
 * @brief   Unlocks the provided semaphore.
 * 
 * @param   semaphore Pointer to the semaphore to be unlocked.
 */
void sem_unlock(sem_t *semaphore);

/**
 * @brief   Performs cleanup of resources and shared data.
 */
void cleanup();

/**
 * @brief   Handles error messages and initiates cleanup.
 * 
 * @param   msg String containing the error message.
 */
void handle_error(const char* msg);

#endif /* UTILITIES_H */
