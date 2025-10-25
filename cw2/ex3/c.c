#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Alínea c: reduzir Pss da .bss para cerca de 1MB durante 30s mantendo Rss
// Implementação: fork(); o filho dorme 30s e termina. Enquanto existir o filho,
// as páginas previamente tocadas na .bss estarão partilhadas (COW) entre pai e filho.

void do_alinea_c(void) {

    pid_t pid = fork();

    if (pid < 0) {
        perror("do_alinea_c: fork");
        return;

    } else if (pid == 0) {
        // filho
        printf("[filho] PID=%d: vou dormir 30s para manter partilha de páginas (.bss)\n", getpid());
        sleep(30);

        printf("[filho] PID=%d: a sair agora.\n", getpid());
        _exit(0);
        
    } else {
        // pai
        printf("[pai] PID=%d: filho PID=%d. Durante os próximos ~30s as páginas da .bss estarão partilhadas (ver /proc/%d/smaps).\n",
               getpid(), pid, getpid());
        // Espera pelo filho (logo, o pai ficará bloqueado até o filho terminar).
        waitpid(pid, NULL, 0);
        printf("[pai] Filho terminou — a partilha acabou.\n");
    }
}