////////////////////////////////////////////
//
// ISEL - System Virtualization Techniques
// Autumn/Winter 2025/26
//
// Coursework Assignment #1
//

#include "chuta.h"
#include <unistd.h>	   // fork()
#include <sys/types.h> // pid_t
#include <sys/wait.h>  // waitpid, WIF*, WEXITSTATUS, WTERMSIG
#include <signal.h>	   // sinais
#include <stdlib.h>	   // exit, EXIT_SUCCESS/FAILURE
#include <stdio.h>	   // printf
#include <stdbool.h>   // bool

static int run_test(test_function tfunc);

void run_function_tests(test_function tests[], size_t num_tests, bool stop_at_first_failure)
{
	/* (to be implemented) */
	for (int i = 0; i < num_tests; i++)
	{
		pid_t pid = fork();
		if (pid == 0)
		{ // Verifica se é filho
			tests[i]();
			_exit(EXIT_SUCCESS); // se a função acabar normalmente → SUCCESS
		}
		else
		{ // Verifica se é Pai
			int status;
			waitpid(pid, &status, 0);	// Espera para que o filho acabe
			if (WIFEXITED(status))
			{ // Terminou normalmente
				int code = WEXITSTATUS(status);
				if (code == EXIT_SUCCESS){	// Verifica Success
					printf("[%zu] SUCCESS\n", i);
				}
				else if (code == EXIT_FAILURE){		// Verifica Fracasso
					printf("[%zu] ASSERTION_FAILED\n", i);
				}
			}
			else if (WIFSIGNALED(status))			
			{	// Foi morto
				printf("[%zu] TERMINATED(%d)\n", i, WTERMSIG(status));
			}
		}
	}
}

static int run_test(test_function tfunc)
{
	/* (to be implemented) */
	return -1;
}
