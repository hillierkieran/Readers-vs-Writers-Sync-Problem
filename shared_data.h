/**
 * @file    shared_data.h
 * @author  Kieran Hillier
 * @date    August 15, 2023
 * @version 0.1
 * 
 * @brief   Header file defining the SharedData structure.
 * 
 * @details This file contains the definition for the SharedData 
 *          structure which is used as the shared resource between 
 *          multiple threads in the readers-writers problem.
 */

#ifndef SHARED_DATA_H
#define SHARED_DATA_H

/** Structure representing the shared data between threads. */
typedef struct {
    int sum;               /* The current sum */
    int last_incr_id;      /* ID of the last incrementer thread */
    int last_decr_id;      /* ID of the last decrementer thread */
    int num_writers;       /* Total number of writer threads */
} SharedData;

SharedData* init_shared_data();
SharedData* get_shared_data();
int read_shared_data();
void modify_shared_data(int increment, int thread_id);
void destroy_shared_data();

#endif /* SHARED_DATA_H */
