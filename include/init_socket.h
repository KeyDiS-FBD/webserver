#ifndef INIT_SOCKET_H
#define INIT_SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
// #include <http.h>

typedef struct HTTPreq {
    char *method; // GET or POST
    char *path; // page which need, size is maximal in Chrome browser
    char *protocol; // HTTP/1.1
    int client_socket;
} HTTPreq;

int init_socket(char *address, const int mode);
int socket_accept(int server_socket);
void socket_send_message(int client_socket, char *message);
int parse_address(char *address, char *ipv4, char *port);

HTTPreq *my_recv(int client_socket);

#endif
