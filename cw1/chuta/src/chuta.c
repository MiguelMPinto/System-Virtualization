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

	size_t tests_executed = 0;
	size_t sucess = 0;
	size_t failures = 0;


	for (size_t i = 0; i < num_tests; i++)
	{
     	int fds[2];        
		pipe(fds);  
		pid_t processoTeste = fork();
		if (processoTeste == 0)
		{ // Verifica se é filho
			close(fds[0]);
			if (dup2(fds[1], STDERR_FILENO) == -1) {    
    			_exit(127);                              
			}
			close(fds[1]);
			tests[i](); 
			_exit(EXIT_SUCCESS); // se a função acabar normalmente → SUCCESS
		}

		else if (processoTeste > 0)
		{ // Verifica se é Pai
			int status;
			tests_executed++;
			close(fds[1]);
  

			waitpid(processoTeste, &status, 0);	// Espera para que o filho acabe
			if (WIFEXITED(status))
			{ // Terminou normalmente

				int exit_code = WEXITSTATUS(status); // verifica qual o código do exit
				if (exit_code == EXIT_SUCCESS){	// bem sucedido
				
					sucess++;
					printf("[%zu] SUCCESS\n", i);
				}
				else if (exit_code == EXIT_FAILURE){		// Verifica Fracasso
					failures++;
					printf("[%zu] ASSERTION_FAILED\n", i);

				}
			}

			else if (WIFSIGNALED(status))			
			{	// Foi morto
				failures++;
				printf("[%zu] TERMINATED(%d)\n", i, WTERMSIG(status));
			}
			char buf[256];
			ssize_t n;
			while ((n = read(fds[0], buf, sizeof buf)) > 0) {
    			fwrite(buf, 1, n, stdout);
			}
			close(fds[0]);                     

			if (stop_at_first_failure &&       // parar no 1.º falhado/terminado
    			(!WIFEXITED(status) || WEXITSTATUS(status) != EXIT_SUCCESS)) break;
		}

		

		else{ // caso tenha existido um erro na realização do fork (processoTeste == -1 )
			tests_executed++;
			failures++;
			printf("[%zu] FORK_FAILED\n", i);
			close(fds[0]);
			close(fds[1]);
			if(stop_at_first_failure) break;
		}
	}
printf("Resumo: total_tests=%zu, tests_executed=%zu, success=%zu, failures=%zu\n",num_tests,tests_executed,sucess,failures);
}

static int run_test(test_function tfunc)
{
	/* (to be implemented) */
	return -1;
}
