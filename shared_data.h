/**
 * @file    shared_data.h
 * @author  Kieran Hillier
 * @date    August 14, 2023
 * @version 0.1
 * 
 * @brief   Header file defining the SharedData structure.
 * 
 * @details This file contains the definition for the SharedData 
 *          structure which is used as the shared resource between 
 *          multiple threads in the readers-writers problem.
 */

/* include guard */
#ifndef SHARED_DATA_H
#define SHARED_DATA_H

/** Structure representing the shared data between threads. */
typedef struct {
    int sum;               /* The current sum */
    int last_incr_id;      /* ID of the last incrementer thread */
    int last_decr_id;      /* ID of the last decrementer thread */
    int num_writers;       /* Total number of writer threads */
} SharedData;

#endif /* end include guard */
