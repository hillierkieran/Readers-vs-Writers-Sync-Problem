/**
 * @file    a2.c
 * @author  Kieran Hillier
 * @date    August 16, 2023
 * @version 1.0
 *
 * @brief   Main file for readers-writers problem.
 * 
 * @details This program creates threads using POSIX. Incrementers and 
 *          decrementers modify a sum, while readers read its value. This file 
 *          handles the main program flow, from argument parsing to thread 
 *          creation and reporting.
 */

#include "arg_parser.h"
#include "common.h"
#include "resources.h"
#include "utilities.h"
#include "shared_data.h"
#include "thread_operations.h"

/**
 * @brief   Prints final state of the shared data and thread counts.
 * 
 * @details Accesses shared data and prints its state, including the number of 
 *          reader, incrementer, and decrementer threads created.
 * 
 * @param   num_incrementers Total incrementer threads.
 * @param   num_decrementers Total decrementer threads.
 * @param   num_readers Total reader threads.
 */
void print_result(int num_incrementers,int num_decrementers, int num_readers)
{
    /* Access the shared data */
    SharedData *data = get_shared_data();

    /* Print out the thread information. */
    printf("There were %d readers, %d incrementers and %d decrementers\n",
            num_readers, num_incrementers, num_decrementers);

    /* Display the final state of the shared data. */
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
 * @brief   Main program entry point.
 * 
 * @details Manages program flow: argument parsing, initialization, thread
 *          creation, joining, and cleanup. Begins by parsing arguments,
 *          initializing resources and data, creating threads, waiting for
 *          them to finish, and cleanup.
 * 
 * @param   argc Number of command line arguments.
 * @param   argv Array of command line arguments.
 * 
 * @return  Exits with `EXIT_SUCCESS` on success. 
 */
int main(int argc, char *argv[])
{
    int max_threads = DEFAULT_THREADS; /* Number of threads to create       */
    int num_incrementers;       /* Number of incrementer threads.           */
    int num_decrementers;       /* Number of decrementer threads.           */
    int num_readers;            /* Number of reader threads.                */
    int count = 0;              /* Count of all threads created so far      */

    /* Parse user-provided arguments. */
    parse_args(argc, argv, &max_threads);

    /* Initialize necessary resources. */
    Resources* rsc = init_resources();

    /* Allocate memory space for the threads. */
    alloc_threads(max_threads);

    /* Initialize the shared data structure. */
    init_shared_data();

    /* Seed the random number generator. */
    srand(time(NULL));

    /* Randomly decide the number of threads for each type. */
    num_incrementers = rand() % (max_threads / 2) + 1;
    num_decrementers = rand() % (max_threads / 2) + 1;
    num_readers = max_threads - (num_incrementers + num_decrementers);

    /* Create threads for incrementers, decrementers, and readers. */
    count = create_threads(rsc->threads, max_threads, count, num_incrementers,
                            incrementer, "incrementer");
    count = create_threads(rsc->threads, max_threads, count, num_decrementers,
                            decrementer, "decrementer");
    count = create_threads(rsc->threads, max_threads, count, num_readers,
                            reader, "reader");

    /* Wait for all the threads to finish execution. */
    join_threads(rsc->threads, count);

    /* Display the final state of the system. */
    print_result(num_incrementers, num_decrementers, num_readers);

    /* Clean up allocated resources and exit. */
    cleanup();
    exit(EXIT_SUCCESS);
} 

/* end a2.c */
