#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <signal.h>
#include <pthread.h>

#define BUF_SIZE 1024
#define SERVER_PORT 12345
#define QUEUE_SIZE 5

int clients[3];
int client_quantity = 0;

struct thread_data_t {
    int number;
    int descriptor;
};

void send_to_all(int socket, char *buf) {
    for (int i = 0; i < 3; i++) {
        if (clients[i] != socket) {
            write(clients[i], buf, sizeof(buf));
        }
    }
}

void *reading(void *client) {
    char myBuffer[BUF_SIZE];
    struct thread_data_t *th_data = (struct thread_data_t *) client;
    int descriptor = (*th_data).descriptor;
    int nBytes;
    printf("Start reading from socket descriptor\n");

    while (1) {
        bzero(myBuffer, BUF_SIZE);
        nBytes = read(descriptor, myBuffer, BUF_SIZE);
        if (nBytes == 0) {
            printf("Client został wyłączony\n");
            for (int i = 0; i < 3; i++)
                if (clients[i] == descriptor) {
                    clients[i] == 0;
                    client_quantity--;
                }
            printf("Klienci %d\n", client_quantity);
            pthread_exit(NULL);
        } else {
            printf("Otrzymano: \t %d", nBytes);
            puts(myBuffer);
            send_to_all(descriptor, myBuffer);
        }
    }

}

int main(int argc, char *argv[]) {
    struct sockaddr_in sockaddr;

    memset(&sockaddr, 0, sizeof(struct sockaddr));
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    sockaddr.sin_port = htons((uint16_t) SERVER_PORT);

    int socket_desc = socket(AF_INET, SOCK_STREAM, 0);
    if (socket_desc < 0) {
        printf("Cannot create socket. Error code: %d", socket_desc);
        exit(1);
    }

    char temp = 1;
    setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR, &temp, sizeof(temp));
    printf("Socket created... \n");

    int myBind = bind(socket_desc, (struct sockaddr *) &sockaddr, sizeof(struct sockaddr));
    if (myBind < 0) {
        fprintf(stderr, "ERROR: Binding IP and port to socket\n");
        exit(1);
    }
    printf("Socket binded...\n");

    int myListen = listen(socket_desc, QUEUE_SIZE);
    if (myListen < 0) {
        printf("Cannot listen to socket. Error code: %d", myListen);
        exit(1);
    }

    int connection;

    char buf[BUF_SIZE];

    for (int i = 0; i < 3; i++) {
        clients[i] = 0;
    }

    while (1) {
        connection = accept(socket_desc, NULL, NULL);
        if (client_quantity < 3) {
            if (connection < 0) {
                printf("Cannot connect to client. Error code: %d", connection);
                exit(1);
            }

            write(connection, buf, sizeof(buf));
            printf("Connection socket created... \n");

            pthread_t client;
            struct thread_data_t data;
            data.descriptor = connection;
            for (int i = 0; i < 3; i++) {
                if (clients[i] == 0) {
                    data.number = i;
                }
            }

            if (pthread_create(&client, NULL, reading, (void *) &data) == 1) {
                fprintf(stderr, "ERROR: Creating thread\n");
                exit(1);
            }
            clients[data.number] = data.descriptor;
            client_quantity++;
        } else {
            printf("Another client tried to connect to the server - waiting for a free slot");
        }
    }

    return 0;
}