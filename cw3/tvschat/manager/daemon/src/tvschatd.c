/*
 * implements an echo daemon using unix domain stream sockets
 */

#include <systemd/sd-daemon.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/un.h>
#include <sys/socket.h>

#include <syslog.h>
#include <signal.h>
#include <stdbool.h>
#include <pthread.h>
#include <stdlib.h>
#include "request_parser.h"



// size of pending connections queue
#define BACKLOG 128

// globals
int srv_sock;
volatile bool interrupted = false;

void init_log() {
	//openlog("echod", LOG_PID, LOG_DAEMON);
}

void sigterm_handler(int sig) {
	interrupted = true;
	close(srv_sock);
}

void init() {
	// save daemon pid
#ifdef WITH_PIDFILE
	char pidfile[128];
	sprintf(pidfile, "echo %d > %sechod.pid",  getpid(), ECHO_DIR);
	system(pidfile); 
#endif

	init_log();
	
	//handling sigterm
	signal(SIGTERM, sigterm_handler);
}



void process_connection(int cfd) {
	char req[128] = {0};
	
 	 read(cfd, req, sizeof(req)-1);
	 
	 script_info_t info = parseRequest(req);

	  if (strlen(info.script) == 0) {
        write(cfd, "Invalid\n", 8);
        close(cfd);
        return;
    }

	pid_t pid = fork();
    	if (pid == 0) {

        dup2(cfd,STDOUT_FILENO); 
        dup2(cfd,STDERR_FILENO);

    close(cfd);
    if (strlen(info.arg) > 0) {
        char *argv[] = { info.script, info.arg, NULL };
        execv(info.script, argv);
    } else {
        char *argv[] = { info.script, NULL };
        execv(info.script, argv);
    }
    perror("execv failed"); 
    exit(1);
} else if(pid > 0){
        close(cfd);
}else{
	perror("fork"); 
    close(cfd);
    }	 
}

void* dispatch_connection(void *arg) {
	int cfd = (int)(size_t) arg;
	process_connection(cfd);
	return NULL;
}


void run(void) {
    fprintf(stderr, "tvschatd: server started!\n");

    while (!interrupted) {
        struct sockaddr_un cli_addr;
        socklen_t addrlen = sizeof(cli_addr);
        int cli_sock = accept(srv_sock, (struct sockaddr *)&cli_addr, &addrlen);
        if (cli_sock == -1) {
            if (errno == EINTR) continue;
            perror("accept");
            /* In case of persistent errors we break to allow supervised restart */
            break;
        }

        pthread_t cthread;
        if (pthread_create(&cthread, NULL, dispatch_connection, (void *)(size_t)cli_sock) != 0) {
            perror("pthread_create");
            close(cli_sock);
            continue;
        }
        pthread_detach(cthread);
    }

    fprintf(stderr, "tvschatd: server terminated!\n");
}

int main(int argc, char *argv[]) {
	
	init();
	
	
	int nfd = sd_listen_fds(0); // aqui este fd é recebido quando é estabelecido uma ligação ao socket , consequentemente ativa o daemon
	
	if (nfd != 1) {
		fprintf(stderr, "echod: none or too many file descriptors\n");
		exit(1);
	}
 
	// server socket from systemd launcher
	srv_sock = SD_LISTEN_FDS_START;
	
	// run the daemon
	run();
	
	// cleanup

	close(srv_sock);
	closelog();
	
}


 
