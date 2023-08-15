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
#include "utilities.h"
#include "shared_data.h"

extern sem_t data_sem;      /* Semaphore for data access    */
extern sem_t reader_sem;     /* Semaphore for reader count   */

void cleanup()
{
    destroy_shared_data();
    sem_destroy(&data_sem);
    sem_destroy(&reader_sem);
}

void handle_error(const char* msg)
{
    fprintf(stderr, "%s\n", msg);
    cleanup();
    exit(EXIT_FAILURE);
}
