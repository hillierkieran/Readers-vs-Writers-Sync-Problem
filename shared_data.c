/**
 * @file    shared_data.c
 * @author  Kieran Hillier
 * @date    August 15, 2023
 * @version 0.1
 * 
 * @brief   Source file for operations on shared data.
 * 
 * @details Implements the functions for initializing, getting, 
 *          reading, modifying, and destroying the shared data.
 */

#include "common.h"
#include "utilities.h"
#include "shared_data.h"

static SharedData* global_data = NULL;
static pthread_mutex_t data_mutex = PTHREAD_MUTEX_INITIALIZER;

SharedData* init_shared_data()
{
    pthread_mutex_lock(&data_mutex);

    if (!global_data) {
        global_data = malloc(sizeof(SharedData));
        if (!global_data) {
            handle_error("Error allocating memory for shared data");
        }
        
        global_data->sum = 0;
        global_data->last_incr_id = -1;
        global_data->last_decr_id = -1;
        global_data->num_writers = 0;
    }

    pthread_mutex_unlock(&data_mutex);
    return global_data;
}

SharedData* get_shared_data()
{
    return global_data;
}

int read_shared_data()
{
    return global_data->sum;
}

void modify_shared_data(int increment, int thread_id)
{
    pthread_mutex_lock(&data_mutex);

    if (!global_data) {
        pthread_mutex_unlock(&data_mutex);
        handle_error("Shared data not initialized");
    }

    global_data->sum += increment;
    if (increment > 0) {
        global_data->last_incr_id = thread_id;
    } else if (increment < 0) {
        global_data->last_decr_id = thread_id;
    }
    global_data->num_writers++;

    pthread_mutex_unlock(&data_mutex);
}

void destroy_shared_data()
{
    pthread_mutex_lock(&data_mutex);

    if (global_data) {
        free(global_data);
        global_data = NULL;
    }

    pthread_mutex_unlock(&data_mutex);
    pthread_mutex_destroy(&data_mutex);
}
