#ifndef UTILS_H
#define UTILS_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

int Rec_message(void *msg, int port);
int Send_message(void *msg, int msg_len, char * IP, int port);

#endif