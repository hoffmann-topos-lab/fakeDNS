
#ifndef SOCKET_H
#define SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#define DNS_PORT 53

void setup_socket(int *sockfd, struct sockaddr_in *server_addr);

#endif



