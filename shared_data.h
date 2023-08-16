/**
 * @file    shared_data.h
 * @author  Kieran Hillier
 * @date    August 16, 2023
 * @version 1.0
 * 
 * @brief   Header defining the SharedData structure and operations.
 * 
 * @details This header provides declarations for the SharedData structure 
 *          and functions for operations on shared data among threads.
 */

#ifndef SHARED_DATA_H
#define SHARED_DATA_H

/**
 * @struct  SharedData
 * 
 * @brief   Structure representing shared data between threads.
 * 
 * @details Holds the current sum, last incrementer/decrementer thread IDs,
 *          and the total number of writer threads so far.
 */
typedef struct {
    int sum;               /* The current sum */
    int last_incr_id;      /* ID of the last incrementer thread */
    int last_decr_id;      /* ID of the last decrementer thread */
    int num_writers;       /* Total number of writer threads */
} SharedData;

/**
 * @brief   Initializes the shared data.
 * 
 * @return  Pointer to the initialized SharedData structure.
 */
SharedData* init_shared_data();

/**
 * @brief   Retrieves the shared data structure.
 * 
 * @return  Pointer to the SharedData structure.
 */
SharedData* get_shared_data();

/**
 * @brief   Reads the current value of the sum from shared data.
 * 
 * @return  The current value of the sum.
 */
int read_shared_data();

/**
 * @brief   Updates shared data based on increment value and thread ID.
 * 
 * @param   increment Value to adjust the sum.
 * @param   thread_id ID of the thread performing the update.
 */
void modify_shared_data(int increment, int thread_id);

/**
 * @brief   Frees and cleans up the shared data structure.
 */
void destroy_shared_data();

#endif /* SHARED_DATA_H */
