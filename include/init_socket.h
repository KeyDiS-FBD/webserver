#ifndef INIT_SOCKET_H
#define INIT_SOCKET_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>


int init_socket(char *address, const int mode);
int socket_accept(int server_socket);
void socket_send_message(int client_socket, char *message);
int parse_address(char *address, char *ipv4, char *port);
char *get_request(int client_socket);

#endif
