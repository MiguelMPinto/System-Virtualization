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

void run_function_tests(tests_named tests[], size_t num_tests, bool stop_at_first_failure)
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
			close(fds[0]); // fecha o fd de leitura , já que es	te não é utilizado
			if (dup2(fds[1], STDERR_FILENO) == -1) {    // redireciona o fd[2] que é o stderr para o fds[1] que o fd de escrita
    			_exit(127);   // em caso de erro , da exit com status code 127                           
			}
			close(fds[1]);  // fechamos o fd de escrita porque já não precisamos dele
			tests[i].fun(); 
			_exit(EXIT_SUCCESS); // se a função acabar normalmente → SUCCESS
		}

		else if (processoTeste > 0)
		{ // Verifica se é Pai
			int status;
			tests_executed++;
			close(fds[1]); // fechamos o fd de escrita já que este não é necessário
  

			waitpid(processoTeste, &status, 0);	// Espera para que o filho acabe
			if (WIFEXITED(status))
			{ // Terminou normalmente

				int exit_code = WEXITSTATUS(status); // verifica qual o código do exit
				if (exit_code == EXIT_SUCCESS){	// bem sucedido
				
					sucess++;
					printf("Function with name [%s] and index [%zu] -> SUCCESS\n",tests[i].name, i);
				}
				else if (exit_code == EXIT_FAILURE){		// Verifica Fracasso
					failures++;
					printf("Function with name [%s] and index [%zu] ->  ASSERTION_FAILED\n",tests[i].name, i);

				}
			}

			else if (WIFSIGNALED(status))			
			{	// Foi morto
				failures++;
				printf("Function with name [%s] and index [%zu] -> TERMINATED(%d)\n", tests[i].name,i,WTERMSIG(status));
			}
			char buf[256]; // criação de um buffer temporário para armazenar os dados lidos do pipe
			ssize_t n;
			while ((n = read(fds[0], buf, sizeof buf)) > 0) {  // enquanto existir dados para ler vindos do filho , quando não existir mais dados a ler , o read retornará 0 e portanto sai do while
    			fwrite(buf, 1, n, stdout); // escrevemos no stdout esses dados
			}
			close(fds[0]);   // fecha o lado de leitura pois o filho terminou.              

			if (stop_at_first_failure &&       // parar no 1.º falhado/terminado
    			(!WIFEXITED(status) || WEXITSTATUS(status) != EXIT_SUCCESS)) break;
		}

		

		else{ // caso tenha existido um erro na realização do fork (processoTeste == -1 )
			tests_executed++;
			failures++;
			printf("[%zu] FORK_FAILED\n", i);
			close(fds[0]); // fecha o pipe
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
};
