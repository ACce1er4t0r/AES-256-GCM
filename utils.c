#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "utils.h"

int Rec_message(void * msg, int port) {
    int sockfd, clientfd;
    struct sockaddr_in addr, client;
    size_t msg_len;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1) {
        printf("socket failed : %d\n", errno);
        exit(-1);
    }

    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    memset(&(addr.sin_zero), '\0', 8);

    if(bind(sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr)) < 0) {
        printf("bind error\n");
        exit(-1);
    }

    listen(sockfd, 1);
    printf("Waiting for connections.\n");

    while(1) {
        int s = sizeof(struct sockaddr_in);
        clientfd = accept(sockfd, (struct sockaddr*)&client, &s);
        if(clientfd == -1) {
            printf("Connect failed : %d\n", errno);
        } else {
            printf("Connect Accepted.\n");
            msg_len = recv(clientfd, msg, 65536, 0);
            printf("Inbound file.\n");
            break;
        }
    }
    return msg_len;
}

int Send_message(void *msg, int msg_len, char * IP, int port) {
    int sockfd;
    struct sockaddr_in addr;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if(sockfd == -1) {
        printf("socket failed : %d\n", errno);
        return -1;
    }

    addr.sin_addr.s_addr = inet_addr(IP);
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);

    memset(&(addr.sin_zero), '\0', 8);

    if(connect(sockfd, (struct sockaddr*)&addr, sizeof(struct sockaddr)) == -1) {
        printf("connect failed : %d\n", errno);
        return -1;
    } else {
        printf("Transmitting to %s:%d\n", IP, port);
        send(sockfd, msg, msg_len, 0);
        printf("Successfully received\n");
    }
    return 0;
}