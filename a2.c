/**
 * @file    a2.c
 * @author  Kieran Hillier
 * @date    August 15, 2023
 * @version 0.1
 * 
 * @brief   A program to demonstrate readers and writers synchronization.
 * 
 * @details A multi-threaded program that creates a random number of 
 *          incrementer, decrementer, and reader threads. Shared data 
 *          is protected using POSIX semaphores.
 */

#include "arg_parser.h"
#include "common.h"
#include "resources.h"
#include "utilities.h"
#include "shared_data.h"
#include "thread_operations.h"

/**
 * @brief   Prints final state of the program.
 * 
 * @param   num_incrementers number of incrementer threads.
 * @param   num_decrementers number of decrementer threads.
 * @param   num_readers number of reader threads.
 */
void print_result(int num_incrementers,int num_decrementers, int num_readers)
{
    SharedData *data = get_shared_data();
    printf("There were %d readers, %d incrementers and %d decrementers\n",
            num_readers, num_incrementers, num_decrementers);
    printf("The final state of the data is:\n"
            "\tlast incrementer %d\n"
            "\tlast decrementer %d\n"
            "\ttotal writers %d\n"
            "\tsum %d\n", 
            data->last_incr_id, 
            data->last_decr_id, 
            data->num_writers, 
            data->sum);
}

/**
 * @brief   Main function to create threads and print the final state.
 * 
 * @return  Does not return a value; instead, exits with status 
 *          `EXIT_SUCCESS` to indicate successful execution.
 */
int main(int argc, char *argv[])
{
    int max_threads = DEFAULT_THREADS; /* Number of threads to create       */
    int num_incrementers;       /* Number of incrementer threads.           */
    int num_decrementers;       /* Number of decrementer threads.           */
    int num_readers;            /* Number of reader threads.                */
    int count = 0;              /* Count of all threads created so far      */

    /* Parse arguments */
    parse_args(argc, argv, &max_threads);

    /* Initialise resources */
    Resources* rsc = init_resources();

    /* Allocate memory for the threads */
    alloc_threads(max_threads);

    /* Initialise shared_data */
    init_shared_data();

    /* Seed random number generator with current time */
    srand(time(NULL));

    /* Randomly decide number of threads to be created */
    num_incrementers = rand() % (max_threads / 2) + 1;
    num_decrementers = rand() % (max_threads / 2) + 1;
    num_readers = max_threads - (num_incrementers + num_decrementers);

    /* Create threads */
    count = create_threads(rsc->threads, max_threads, count, num_incrementers,
                            incrementer, "incrementer");
    count = create_threads(rsc->threads, max_threads, count, num_decrementers,
                            decrementer, "decrementer");
    count = create_threads(rsc->threads, max_threads, count, num_readers,
                            reader, "reader");

    /* Join all threads */
    join_threads(rsc->threads, count);

    /* Print the final state */
    print_result(num_incrementers, num_decrementers, num_readers);

    /* Destroy resources */
    cleanup();
    exit(EXIT_SUCCESS);
}
