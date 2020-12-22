#ifndef HTTP_H
#define HTTP_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct HTTPreq {
    char *method; // GET or POST
    char *path; // page which need, size is maximal in Chrome browser
    char *protocol; // HTTP/1.1
    char *request;
    int client_socket;
} HTTPreq;

HTTPreq *parse_request(char *request);

#endif
