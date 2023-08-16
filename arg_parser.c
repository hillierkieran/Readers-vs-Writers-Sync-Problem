/**
 * @file    arg_parser.c
 * @author  Kieran Hillier
 * @date    August 16, 2023
 * @version 1.0
 *
 * @brief   Provides parsing of command line arguments.
 * 
 * @details This file implements the function to parse the command line
 *          arguments for the program. It checks argument validity and sets 
 *          the number of threads to be used.
 */

#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include "arg_parser.h"
#include "common.h"
#include "utilities.h"

/**
 * @brief   Parses the command line arguments.
 * 
 * @details Checks argument count and validity. If arguments are invalid, 
 *          prints an error message and exits.
 * 
 * @param   argc Count of command line arguments.
 * @param   argv Array of command line arguments.
 * @param   num_threads Pointer to the number of threads to be used.
 */
void parse_args(int argc, char* argv[], int* num_threads)
{
    /* Check for excess arguments */
    if (argc > 2) {
        char errorMsg[MAX_STRING];
        snprintf(errorMsg, sizeof(errorMsg),
                "Invalid number of arguments.\n"
                "Usage: %s [num_threads]", argv[0]);
        handle_error(errorMsg);

    /* Check if the number of threads specified is below the minimum */
    } else if (argc == 2 && (*num_threads = atoi(argv[1])) < MINIMUM_THREADS) {
        char errorMsg[MAX_STRING];
        snprintf(errorMsg, sizeof(errorMsg),
                "Invalid number of threads.\n"
                "Must be greater than %d.", MINIMUM_THREADS);
        handle_error(errorMsg);
    }
}

/* end arg_parser.c */
