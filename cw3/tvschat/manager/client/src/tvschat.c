#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <fcntl.h>

#define TVS_SOCKET_PATH "/run/isel/tvschat/request"

int create_client_socket() {
    return socket(AF_UNIX, SOCK_STREAM, 0);
}

int main(int argc, const char *argv[]) {

    // DON'T EDIT
    close(0); open("/dev/null", O_RDONLY);

    if (argc < 2) {
        fprintf(stderr, "Usage: tvschat <run|stop|status> [arg]\n");
        return 1;
    }

    const char *cmd = argv[1];

   // validação do comando
    if (strcmp(cmd, "run") != 0 &&
        strcmp(cmd, "stop") != 0 &&
        strcmp(cmd, "status") != 0) {

        fprintf(stderr, "Invalid command: %s (must be run | stop | status)\n", cmd);
        return 1;
    }

    // construir o pedido
    char request[128];
    memset(request, 0, sizeof(request));

    strcpy(request, cmd);

    // status não leva argumentos
    if (strcmp(cmd, "status") != 0 && argc > 2) {

        const char *arg = argv[2];

        if (strcmp(arg, "private") != 0 &&
            strcmp(arg, "prod") != 0 &&
            strcmp(arg, "dev") != 0 &&
            strcmp(arg, "-llm") != 0) {

            fprintf(stderr, "Invalid argument: %s (must be private | prod | dev | -llm)\n", arg);
            return 1;
        }

        strcat(request, " ");
        strcat(request, arg);
    }

    // ---- criar socket ----
    int cli_sock = create_client_socket();
    if (cli_sock < 0) {
        perror("error creating socket");
        return 2;
    }

    struct sockaddr_un srv_address;
    memset(&srv_address, 0, sizeof(struct sockaddr_un));

    srv_address.sun_family = AF_UNIX;
    strncpy(srv_address.sun_path, TVS_SOCKET_PATH,
            sizeof(srv_address.sun_path) - 1);

    if (connect(cli_sock, (struct sockaddr*)&srv_address,
                sizeof(struct sockaddr_un)) == -1) {
        perror("Error connecting socket");
        close(cli_sock);
        return 3;
    }

    // pedido
    if (write(cli_sock, request, strlen(request) + 1) < 0) {
        perror("error sending request");
        close(cli_sock);
        return 4;
    }
	//resposta
    char buf[4096];
    ssize_t n;

    while ((n = read(cli_sock, buf, sizeof(buf))) > 0) {
        write(STDOUT_FILENO, buf, n);
    }

    close(cli_sock);
    return 0;
}
