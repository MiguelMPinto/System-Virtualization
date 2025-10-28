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
#include <fcntl.h>     // open()
#include <string.h>    // snprintf

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
    pid_t pid = fork();
    if (pid == 0) {
        // Filho: executa o teste
        tfunc();
        _exit(EXIT_SUCCESS);
    }
    else if (pid > 0) {
        int status;
        if (waitpid(pid, &status, 0) == -1)
            return -1; // erro no waitpid
        if (WIFEXITED(status))
            return WEXITSTATUS(status);
        else
            return EXIT_FAILURE; // terminou por sinal
    }
    else {
        // erro no fork
        return -1;
    }
}



// Função auxiliar para comparar ficheiros
static int compare_files(const char* f1, const char* f2) {
    FILE *fp1 = fopen(f1, "r");
    FILE *fp2 = fopen(f2, "r");
    if (!fp1 || !fp2) {
        if (fp1) fclose(fp1);
        if (fp2) fclose(fp2);
        return -1; // erro ao abrir ficheiros
    }

    int c1, c2;
    do {
        c1 = fgetc(fp1);
        c2 = fgetc(fp2);
        if (c1 != c2) {
            fclose(fp1);
            fclose(fp2);
            return 1; // diferentes
        }
    } while (c1 != EOF && c2 != EOF);

    fclose(fp1);
    fclose(fp2);
    return 0; // iguais
}


void run_stdio_program_tests(prog_function tests[], size_t num_tests, bool stop_at_first_failure) {
    size_t executed = 0, success = 0, failures = 0;

    // Loop por cada teste definido no array `tests`.
    // Cada tests[i] deve ter:
    //   - name: nome do teste/programa
    //   - command: comando a executar (ex: "./prog arg1 arg2")
    //   - input_file: ficheiro que vai alimentar o STDIN do programa
    //   - expected_output_file: ficheiro com o output esperado (para comparação)
    for (size_t i = 0; i < num_tests; i++) {

        pid_t pid = fork(); // cria um processo filho para correr o programa a testar

        if (pid == 0) {
            // --- Código executado no processo filho ---

            // Abre o ficheiro de input definido para este teste
            // Este ficheiro vai ser usado como stdin do programa testado
            int fd_in = open(tests[i].input_file, O_RDONLY);
            if (fd_in < 0) _exit(EXIT_FAILURE); // se falhar a abrir input -> falha logo o teste

            // dup2(fd_in, STDIN_FILENO) faz com que o descritor fd_in
            // passe a ser o stdin (fd 0) do processo.
            // Ou seja, quando o programa fizer scanf/gets/read(STDIN), vai ler deste ficheiro.
            dup2(fd_in, STDIN_FILENO);
            close(fd_in); // já não precisamos do descritor antigo, fica só o STDIN_FILENO ativo

            // Vamos criar um ficheiro temporário para capturar o stdout do programa.
            // Isto é para depois o pai conseguir comparar com o expected_output.
            char tmp_file[64];
            snprintf(tmp_file, sizeof(tmp_file), "/tmp/chuta_out_%zu.txt", i);

            // Abre/cria o ficheiro temporário para escrita
            int fd_out = open(tmp_file, O_CREAT | O_WRONLY | O_TRUNC, 0644);
            if (fd_out < 0) _exit(EXIT_FAILURE);

            // Redireciona STDOUT (fd 1) para esse ficheiro.
            // Ou seja, tudo o que o programa imprimir com printf/puts/write(1, ...)
            // vai parar ao tmp_file.
            dup2(fd_out, STDOUT_FILENO);
            close(fd_out);

            // Agora executa o comando do teste usando /bin/sh -c "comando"
            // Isto permite que `command` seja algo tipo "ls -l | wc -l" etc.
            execlp("/bin/sh", "sh", "-c", tests[i].command, NULL);

            // Se execlp falhar, chegamos aqui. _exit para não voltar ao código do pai.
            _exit(EXIT_FAILURE);
        }
        else if (pid > 0) {
            // --- Código executado no processo pai ---

            executed++; // já tentámos correr um teste (mesmo que dê fork fail ou crash depois)

            int status;
            // Espera que o filho acabe e guarda informação de saída em `status`
            waitpid(pid, &status, 0);

            if (WIFEXITED(status)) {
                // O filho terminou normalmente com exit() ou return main()
                int exit_code = WEXITSTATUS(status);

                if (exit_code == EXIT_SUCCESS) {
                    // Se o programa correu e devolveu 0, agora vamos validar o output
                    char tmp_file[64];
                    snprintf(tmp_file, sizeof(tmp_file), "/tmp/chuta_out_%zu.txt", i);

                    // compare_files(tmp_file, expected_output) deve devolver 0 se iguais
                    if (compare_files(tmp_file, tests[i].expected_output_file) == 0) {
                        success++;
                        printf("Program [%s] -> SUCCESS\n", tests[i].name);
                    } else {
                        failures++;
                        printf("Program [%s] -> FAILED (output mismatch)\n", tests[i].name);

                        // se queremos parar logo no 1º fail, saímos do loop
                        if (stop_at_first_failure) break;
                    }
                } else {
                    // O programa acabou mas devolveu código diferente de 0
                    // (normalmente significa erro)
                    failures++;
                    printf("Program [%s] -> FAILED (exit code %d)\n", tests[i].name, exit_code);

                    if (stop_at_first_failure) break;
                }
            }
            else if (WIFSIGNALED(status)) {
                // O processo filho acabou porque levou um sinal (ex: segfault = SIGSEGV)
                failures++;
                printf("Program [%s] -> TERMINATED(%d)\n", tests[i].name, WTERMSIG(status));

                if (stop_at_first_failure) break;
            }
            // Nota: se não WIFEXITED nem WIFSIGNALED, podias ter outros casos (stopped, etc.)
            // mas aqui não estão a ser tratados.
        }
        else {
            // fork() devolveu < 0 -> erro a criar processo filho
            executed++;
            failures++;
            printf("Program [%s] -> FORK_FAILED\n", tests[i].name);

            if (stop_at_first_failure) break;
        }
    }

    // No fim, imprime estatísticas gerais
    printf("Resumo programas: total=%zu, executed=%zu, success=%zu, failures=%zu\n",
           num_tests, executed, success, failures);
}


