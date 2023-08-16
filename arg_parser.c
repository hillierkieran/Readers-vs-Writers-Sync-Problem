/**
 * @file    arg_parser.c
 * @author  Kieran Hillier
 * @date    August 16, 2023
 * @version 0.1
 * @brief   This file provides the definition of parse_args() function.
 */

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include "arg_parser.h"
#include "common.h"
#include "utilities.h"

/**
 * @brief Parses the command line arguments.
 *
 * @details This function checks the number of arguments and their validity. 
 *          If the arguments are not valid, it prints an error message and 
 *          returns -1.
 *
 * @param argc The count of arguments provided in the command line.
 * @param argv The array of arguments provided in the command line.
 * @param num_threads A pointer to the number of threads to be used.
 */
void parse_args(int argc, char* argv[], int* num_threads)
{
    if (argc > 2) {
        /* Invalid number of arguments */
        char errorMsg[MAX_STRING];
        snprintf(errorMsg, sizeof(errorMsg),
                "Invalid number of arguments.\n"
                "Usage: %s [num_threads]", argv[0]);
        handle_error(errorMsg);

    } else if (argc == 2 && (*num_threads = atoi(argv[1])) < MINIMUM_THREADS) {
        /* Invalid number of threads */
        char errorMsg[MAX_STRING];
        snprintf(errorMsg, sizeof(errorMsg),
                "Invalid number of threads.\n"
                "Must be greater than %d.", MINIMUM_THREADS);
        handle_error(errorMsg);
    }
}
