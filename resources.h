/**
 * @file    resources.h
 * @author  Kieran Hillier
 * @date    August 16, 2023
 * @version 1.0
 *
 * @brief   Declares shared resources management functions.
 * 
 * @details This header defines the Resources structure and declares functions 
 *          for managing shared resources such as threads and semaphores.
 */

#ifndef RESOURCES_H
#define RESOURCES_H

#include "common.h"

/**
 * @struct  Resources
 * 
 * @brief   Structure representing shared resources among threads.
 * 
 * @details Contains an array for threads, count of readers, semaphores for 
 *          data access and reader count, and a semaphore initialization flag.
 */
typedef struct {
    pthread_t* threads;
    int readers_count;
    sem_t data_sem;
    sem_t reader_sem;
    int sem_initialised;
} Resources;

/**
 * @brief   Initializes shared resources.
 * 
 * @return  Pointer to the initialized Resources structure.
 */
Resources* init_resources();

/**
 * @brief   Retrieves the shared resources structure.
 * 
 * @return  Pointer to the Resources structure.
 */
Resources* get_resources();

/**
 * @brief   Allocates memory for the threads.
 * 
 * @param   max_threads Number of threads to allocate.
 */
void alloc_threads(int max_threads);

/**
 * @brief   Initializes the required semaphores.
 */
void init_semaphores();

/**
 * @brief   Frees and cleans up shared resources.
 */
void destroy_resources();

#endif /* RESOURCES_H */
