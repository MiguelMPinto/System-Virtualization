////////////////////////////////////////////
//
// ISEL - System Virtualization Techniques
// Autumn/Winter 2025/26
//
// Coursework Assignment #1
//

#include "chuta.h"
#include "ftests.h"

///////////////////////////////
// 
// DECLARE ALL FUNCTION TESTS
//
test_function all_function_tests[] = {
	test_add,
	test_sub,
	test_strsize,
};

//////////////////////////////
// 
// DECLARE ALL PROGRAM TESTS
//
//prog_function all_program_tests[] = {
//	/* ... */
//};

////////////////////////
// 
// RUN ALL TESTS
//
// (do not modify, except to add the call to run_stdio_program_tests)
//

#define ARRLEN(arr) (sizeof(arr)/sizeof(arr[0]))

const size_t num_function_tests = ARRLEN(all_function_tests);
//const size_t num_program_tests = ARRLEN(all_program_tests);

int main() {
	
	run_function_tests(all_function_tests, num_function_tests, false);
	
	//run_stdio_program_tests(...);
	
	return 0;
}
