/**
 * @file    resources.c
 * @author  Kieran Hillier
 * @date    August 15, 2023
 * @version 0.1
 * 
 * @brief   
 * 
 * @details 
 */

#include "common.h"
#include "resources.h"
#include "utilities.h"

static Resources* resources = NULL;
static pthread_mutex_t resource_mutex = PTHREAD_MUTEX_INITIALIZER;

Resources* init_resources()
{
    mutex_lock(&resource_mutex);

    if (resources) {
        mutex_unlock(&resource_mutex);
        return resources;
    }

    resources = malloc(sizeof(Resources));
    if (!resources) {
        mutex_unlock(&resource_mutex);
        handle_error("Failed to allocate memory for resources struct");
    }

    resources->threads = NULL;
    resources->readers_count = 0;
    resources->sem_initialised = 0;

    mutex_unlock(&resource_mutex);
    
    init_semaphores();

    return resources;
}

Resources* get_resources()
{
    if (!resources) {
        init_resources();
    }
    return resources;
}

void alloc_threads(int max_threads)
{
    mutex_lock(&resource_mutex);

    if (!resources) {
        mutex_unlock(&resource_mutex);
        init_resources();
        mutex_lock(&resource_mutex);
    }

    resources->threads = (pthread_t *)malloc(max_threads * sizeof(pthread_t));
    if (resources->threads == NULL) {
        mutex_unlock(&resource_mutex);
        handle_error("Failed to allocate memory for threads.");
    }

    mutex_unlock(&resource_mutex);
}

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
