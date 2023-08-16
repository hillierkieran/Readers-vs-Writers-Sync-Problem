/**
 * @file    utilities.h
 * @author  Kieran Hillier
 * @date    August 15, 2023
 * @version 0.1
 * 
 * @brief   Header file for utility functions.
 * 
 * @details Contains declarations for common utility functions.
 */

#ifndef UTILITIES_H
#define UTILITIES_H

void mutex_lock(pthread_mutex_t *mutex);
void mutex_unlock(pthread_mutex_t *mutex);
void sem_lock(sem_t *semaphore);
void sem_unlock(sem_t *semaphore);
void cleanup();
void handle_error(const char* msg);

#endif /* UTILITIES_H */
