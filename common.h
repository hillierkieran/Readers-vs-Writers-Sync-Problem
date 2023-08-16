/**
 * @file    common.h
 * @author  Kieran Hillier
 * @date    August 16, 2023
 * @version 1.0
 *
 * @brief   Common includes and defines for the project.
 * 
 * @details This header includes necessary system headers and defines constants 
 *          commonly used across multiple files in the project.
 */

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define DEFAULT_THREADS 10
#define MINIMUM_THREADS 3
#define MAX_STRING 100
#define NUM_FUNC 3
#define READ_OP 0
#define INCR_OP 1
#define DECR_OP -1

#endif /* COMMON_H */
