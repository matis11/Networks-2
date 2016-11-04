#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h>

#define BUF_SIZE 1024
int client_socket;

struct transport {
    int descriptor;
};

void exiting_server(int s) {
    printf("Caught signal %d\n", s);
    close(client_socket);
    exit(1);
}

void *sending(void *data) {
    char buf[BUF_SIZE];
    struct transport *th_data = (struct transport *) data;
    int descriptor = (*th_data).descriptor;
    while (1) {
        bzero(buf, BUF_SIZE);
        gets(buf);
        int result = (int) write(descriptor, buf, BUF_SIZE);
        if (result == -1) {
            printf("<ERROR>\n");
        }
        printf("<OK>\n");
        bzero(buf, BUF_SIZE);
    }
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = exiting_server;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;
    sigaction(SIGTERM, &sigIntHandler, NULL);

    if (argc != 3) {
        printf("No server address or port given");
        exit(1);
    }

    struct hostent *server_address;
    server_address = gethostbyname(argv[1]);
    if (!server_address) {
        printf("No server address");
        exit(1);
    }

    client_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (client_socket < 0) {
        printf("Cannot create socket. Error code: %d", client_socket);
        exit(1);
    }

    struct sockaddr_in sockaddr;
    memset(&sockaddr, 0, sizeof(struct sockaddr));
    sockaddr.sin_family = AF_INET;
    memcpy(&sockaddr.sin_addr.s_addr, server_address->h_addr, server_address->h_length);
    sockaddr.sin_port = htons(atoi(argv[2]));

    int result = connect(client_socket, (struct sockaddr *) &sockaddr, sizeof(struct sockaddr));
    if (result < 0) {
        printf("Cannot connect to server");
        exit(1);
    }
    char buf[BUF_SIZE];
    read(client_socket, buf, BUF_SIZE);

    pthread_t client;
    struct transport data;
    data.descriptor = client_socket;
    if (pthread_create(&client, NULL, sending, (void *) &data) == 1) {
        printf("Cannot create data");
        exit(1);
    }

    while (1) {
        bzero(buf, BUF_SIZE);
        int status = (int) read(client_socket, buf, BUF_SIZE);
        if (status != 0) {
            puts(buf);
        } else {
            printf("Serwer został wyłączony\n");
            break;
        }
    }
    close(client_socket);

    return 0;
}