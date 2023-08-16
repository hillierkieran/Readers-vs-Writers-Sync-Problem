/**
 * @file    resources.h
 * @author  Kieran Hillier
 * @date    August 15, 2023
 * @version 0.1
 * 
 * @brief   
 * 
 * @details 
 */

#ifndef RESOURCES_H
#define RESOURCES_H

#include "common.h"

typedef struct {
    pthread_t* threads;
    int readers_count;
    sem_t data_sem;                     /* Semaphore for data access            */
    sem_t reader_sem;                   /* Semaphore for reader count           */
    int sem_initialised;
} Resources;

Resources* init_resources();
Resources* get_resources();
void alloc_threads(int max_threads);
void init_semaphores();
void destroy_resources();

#endif /* RESOURCES_H */
