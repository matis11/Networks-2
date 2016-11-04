#include <iostream>
#include <sys/socket.h>
#include <cstring>
#include <netdb.h>

#include <sys/types.h>
#include <netinet/in.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <zconf.h>
#include <arpa/inet.h>


int main() {
    std::cout << "Hello, World!" << std::endl;
    int socket_desc;
    ssize_t bytes_read;
    char buf0[30];
    int nBind, nListen;
    int nFoo = 1, nTmp;
    struct sockaddr_in stAddr, stClientAddr;

    memset(&stAddr, 0, sizeof(struct sockaddr));
    stAddr.sin_family = AF_INET;
    stAddr.sin_addr.s_addr = htonl(INADDR_ANY);
    stAddr.sin_port = htons(1234);

    socket_desc = socket(AF_INET, SOCK_STREAM, 0);

    setsockopt(socket_desc, SOL_SOCKET, SO_REUSEADDR,  (char*)&nFoo, sizeof(nFoo));

    int bindResult = bind(socket_desc, (struct sockaddr*)&stAddr, sizeof(struct sockaddr));
    if (bindResult == -1) {
        std::cout<<"ERROR BIND"<<std::endl;
    }
    int listenResult = listen(socket_desc, 5);
    if (listenResult == -1) {
        std::cout<<"ERROR Listen"<<std::endl;
    }

    while(1)
    {
        /* block for connection request */
        int nTmp = sizeof(struct sockaddr);
        int client = accept(socket_desc, (struct sockaddr*)&stClientAddr, (socklen_t *) &nTmp);
        if (client < 0)
        {
            std::cout<<"ERROR ACCEPT"<<std::endl;
            exit(1);
        }

            printf("[connection from %s]\n",
                    inet_ntoa((struct in_addr)stClientAddr.sin_addr));
            write(client, (const iovec *) "WITAM\n", 6);
            close(client);
            //exit(1);

        //exit(0);

    }
    return 0;
}