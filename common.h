/**
 * @file    common.h
 * @author  Kieran Hillier
 * @date    August 15, 2023
 * @version 0.1
 * 
 * @brief   Header file with common includes and defines for the project.
 * 
 * @details This file includes necessary headers for the project and defines 
 *          common constants.
 */

#ifndef COMMON_H
#define COMMON_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <semaphore.h>
#include <time.h>

#define MAX_THREADS 10
#define MAX_STRING 100
#define NUM_FUNC 3
#define READ_OP 0
#define INCR_OP 1
#define DECR_OP -1

#endif /* COMMON_H */
