/**
 * @file    shared_data.c
 * @author  Kieran Hillier
 * @date    August 16, 2023
 * @version 1.0
 * 
 * @brief   Manages operations on shared data.
 * 
 * @details Contains functions for initializing, reading, modifying, and 
 *          destroying the shared data among threads.
 */

#include "common.h"
#include "utilities.h"
#include "shared_data.h"

static SharedData* global_data = NULL;
static pthread_mutex_t internal_mutex = PTHREAD_MUTEX_INITIALIZER;

/**
 * @brief   Initializes the shared data.
 * 
 * @details Allocates memory for shared data and sets the initial values.
 * 
 * @return  Pointer to the initialized SharedData structure.
 */
SharedData* init_shared_data()
{
    mutex_lock(&internal_mutex);

    /* Allocate memory for the shared data and handle potential errors */
    global_data = malloc(sizeof(SharedData));
    if (!global_data) {
        handle_error("Error allocating memory for shared data");
    }

    /* Initialize the fields of shared data structure */
    global_data->sum = 0;
    global_data->last_incr_id = -1;
    global_data->last_decr_id = -1;
    global_data->num_writers = 0;

    mutex_unlock(&internal_mutex);
    return global_data;
}

/**
 * @brief   Retrieves the global shared data.
 * 
 * @return  Pointer to the global SharedData structure.
 */
SharedData* get_shared_data()
{
    return global_data;
}

/**
 * @brief   Reads the sum from shared data.
 * 
 * @return  The current value of the sum.
 */
int read_shared_data()
{
    return global_data->sum;
}

/**
 * @brief   Modifies the shared data based on the given increment value.
 * 
 * @param   increment Value to be added or subtracted from the sum.
 * @param   thread_id ID of the thread performing the modification.
 */
void modify_shared_data(int increment, int thread_id)
{
    mutex_lock(&internal_mutex);

    if (!global_data) {
        mutex_unlock(&internal_mutex);
        handle_error("Shared data not initialized");
    }

    /* Update the fields of the shared data based on the increment value */
    global_data->sum += increment;
    if (increment > 0) {
        global_data->last_incr_id = thread_id;
    } else if (increment < 0) {
        global_data->last_decr_id = thread_id;
    }
    global_data->num_writers++;

    mutex_unlock(&internal_mutex);
}

/**
 * @brief   Destroys and frees the memory of shared data.
 */
void destroy_shared_data()
{
    mutex_lock(&internal_mutex);

    /* Deallocate memory for the shared data if it exists */
    if (global_data) {
        free(global_data);
        global_data = NULL;
    }

    mutex_unlock(&internal_mutex);
    pthread_mutex_destroy(&internal_mutex);
}

/* end shared_data.c */
