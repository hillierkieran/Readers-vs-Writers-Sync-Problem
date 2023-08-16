/**
 * @file    arg_parser.h
 * @author  Kieran Hillier
 * @date    August 16, 2023
 * @version 1.0
 *
 * @brief   Declares the parse_args() function for argument parsing.
 * 
 * @details This header provides the declaration for the function responsible 
 *          for parsing command line arguments of the program.
 */

#ifndef ARG_PARSER_H
#define ARG_PARSER_H

/**
 * @brief   Declaration for the function that parses command line arguments.
 * 
 * @details This function checks the number of arguments and their validity.
 *          If arguments are not valid, it will print an error message.
 * 
 * @param   argc Count of command line arguments.
 * @param   argv Array of command line arguments.
 * @param   num_threads Pointer to the number of threads to be used.
 */
void parse_args(int argc, char* argv[], int* num_threads);

#endif /* ARG_PARSER_H */
