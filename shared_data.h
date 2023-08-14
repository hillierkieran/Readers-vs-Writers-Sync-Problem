/**
 * @file    shared_data.h
 * @author  Kieran Hillier
 * @date    August 14, 2023
 * @version 0.1
 * 
 * @brief   Header file defining the SharedData structure for
 *          the multi-threaded addition/subtraction problem.
 */

/* include guard */
#ifndef SHARED_DATA_H
#define SHARED_DATA_H

/* Structure representing the shared data between threads. */
typedef struct {
    int sum;               /* The current sum */
    int last_incr_id;      /* ID of the last incrementer thread */
    int last_decr_id;      /* ID of the last decrementer thread */
    int num_writers;       /* Total number of writer threads */
} SharedData;

#endif /* end include guard */
