/**
 * @file    resources.c
 * @author  Kieran Hillier
 * @date    August 16, 2023
 * @version 1.0
 *
 * @brief   Provides management and operations for shared resources.
 * 
 * @details This file contains the implementation for initializing, allocating,
 *          and managing shared resources such as threads and semaphores.
 */

#include "common.h"
#include "resources.h"
#include "utilities.h"

static Resources* resources = NULL;
static pthread_mutex_t resource_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief   Initializes shared resources.
 * 
 * @details Allocates and sets up resources for the program. Ensures 
 *          initialization is thread-safe.
 * 
 * @return  Pointer to the initialized Resources structure.
 */
Resources* init_resources()
{
    mutex_lock(&resource_mutex);

    if (resources) {
        mutex_unlock(&resource_mutex);
        return resources;
    }

    /* Allocate memory for the resources and handle potential errors */
    resources = malloc(sizeof(Resources));
    if (!resources) {
        mutex_unlock(&resource_mutex);
        handle_error("Failed to allocate memory for resources struct");
    }

    /* Initialize resource structure fields */
    resources->threads = NULL;
    resources->readers_count = 0;
    resources->sem_initialised = 0;

    mutex_unlock(&resource_mutex);

    /* Initialize semaphores for the resources */
    init_semaphores();

    return resources;
}

/**
 * @brief   Retrieves the shared resources structure.
 * 
 * @details If resources are not initialized, it initializes them.
 * 
 * @return  Pointer to the Resources structure.
 */
Resources* get_resources()
{
    if (!resources) {
        init_resources();
    }
    return resources;
}

/**
 * @brief   Allocates memory for the threads.
 * 
 * @param   max_threads Number of threads to allocate.
 */
void alloc_threads(int max_threads)
{
    mutex_lock(&resource_mutex);

    if (!resources) {
        mutex_unlock(&resource_mutex);
        init_resources();
        mutex_lock(&resource_mutex);
    }

    /* Allocate memory for the thread pointers and handle errors */
    resources->threads = (pthread_t *)malloc(max_threads * sizeof(pthread_t));
    if (resources->threads == NULL) {
        mutex_unlock(&resource_mutex);
        handle_error("Failed to allocate memory for threads.");
    }

    mutex_unlock(&resource_mutex);
}

/**
 * @brief   Initializes the required semaphores.
 * 
 * @details Sets up data and reader semaphores for thread synchronization.
 */
void init_semaphores()
{
    if (!resources) {
        mutex_unlock(&resource_mutex);
        init_resources();
        mutex_lock(&resource_mutex);
    }

    mutex_lock(&resource_mutex);

    if(sem_init(&resources->data_sem, 0, 1) != 0) {
        mutex_unlock(&resource_mutex);
        handle_error("Error initializing data semaphore");
    }
    if(sem_init(&resources->reader_sem, 0, 1) != 0) {
        mutex_unlock(&resource_mutex);
        handle_error("Error initializing count semaphore");
    }
    resources->sem_initialised = 1;

    mutex_unlock(&resource_mutex);
}

/**
 * @brief   Frees and cleans up shared resources.
 * 
 * @details Safely deallocates memory and destroys semaphores.
 */
void destroy_resources()
{
    mutex_lock(&resource_mutex);

    if (resources) {
        if (resources->threads) {
            free(resources->threads);
            resources->threads = NULL;
        }
        if (resources->sem_initialised){
            sem_destroy(&resources->data_sem);
            sem_destroy(&resources->reader_sem);
            resources->sem_initialised = 0;
        }
        free(resources);
        resources = NULL;
    }

    mutex_unlock(&resource_mutex);
    pthread_mutex_destroy(&resource_mutex);
}

/* end resources.c */
