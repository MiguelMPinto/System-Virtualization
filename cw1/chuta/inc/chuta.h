////////////////////////////////////////////
//
// ISEL - System Virtualization Techniques
// Autumn/Winter 2025/26
//
// Coursework Assignment #1
//

#ifndef CHUTA_H
#define CHUTA_H

#include <stddef.h>   // size_t
#include <stdbool.h>  // bool
#include <stdlib.h>   // EXIT_SUCCESS / EXIT_FAILURE
#include <unistd.h>   // _exit

#define CH_ASSERT(test_expression) \		
	do { if (!(test_expression)) { _exit(EXIT_FAILURE); } } while (0)

typedef void (*test_function)();

void run_function_tests(test_function tests[], size_t num_tests, bool stop_at_first_failure);

#endif // CHUTA_H
