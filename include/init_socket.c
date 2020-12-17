#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#include <unistd.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <init_socket.h>

#define MAX_CONNECTION 10

int init_socket(char *address, const int mode);
int parse_address(char *address, char *ipv4, char *port);
int socket_accept(int server_socket);
void socket_send_message(int client_socket, char *message);

int init_socket(char *address, const int mode) {
    // open socket, return socket descriptor
    // mode == 1 is client mode, mode == 0 is server mode
    int server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (server_socket < 0) {
        perror("Fail: open socket");
        exit(1);
    }
    struct sockaddr_in server_address;
    char ipv4[16];
    char port[6];

    if (parse_address(address, ipv4, port) != 0) {
        puts("Incorrect argument");
        exit(1);
    }
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(atoi(port));
    server_address.sin_addr.s_addr = inet_addr(ipv4);

    if (mode != 0) {
        struct hostent *host = gethostbyname(ipv4);
        memcpy(&server_address.sin_addr, host->h_addr_list[0],
               (socklen_t)sizeof(server_address.sin_addr));

        //connection
        if (connect(server_socket, (struct sockaddr*) &server_address,
            (socklen_t)sizeof(server_address)) < 0) {

            perror("Error: connect");
            exit(1);
        }
    } else {
        int socket_option = 1;
        setsockopt(server_socket, SOL_SOCKET, SO_REUSEADDR, &socket_option,
                   sizeof(socket_option));
        if (server_socket < 0) {
            perror("Fail: set socket options");
            exit(1);
        }

        if (bind(server_socket, (struct sockaddr *) &server_address,
                 sizeof(server_address)) < 0) {

            perror("Fail: bind socket address");
            exit(1);
        }

        if (listen(server_socket, MAX_CONNECTION) < 0) {
            perror("Fail: listen socket");
            exit(1);
        }
    }
    return server_socket;
}

int parse_address(char *address, char *ipv4, char *port) {
    size_t i = 0, j = 0;

    while (address[i] != ':') {
        if (address[i] == '\0') {
            return 1;

        }
        if (i >= 15) {
            return 2;
        }
        ipv4[i] = address[i];
        i++;
    }
    ipv4[i] = '\0';
    i++;
    while (address[i] != '\0') {
        if (j >= 5) {
            return 3;
        }
        port[j] = address[i];
        i++;
        j++;
    }
    port[j] = '\0';
    return 0;
}

int socket_accept(int server_socket) {
    int client_socket;
    struct sockaddr_in client_address;
    socklen_t size = sizeof(struct sockaddr_in);

    client_address.sin_family = AF_INET;
    client_address.sin_addr.s_addr = INADDR_ANY;

    if ((client_socket = accept(server_socket,
        (struct sockaddr *) &client_address,
        &size)) < 0) {

            perror("Error accept");
            exit(1);
        }
        printf("Connected:\n  ip:%s\n  port:%d\n",
        inet_ntoa(client_address.sin_addr),
        ntohs(client_address.sin_port));

    return client_socket;
}

char *socket_scan_request(int client_socket) {
    char *request = NULL;
    char ch;
    int request_len = 0;
    size_t bytes = 0;
    int flag = 0;

    do {
        if (read(client_socket, &ch, 1) <= 0) {
            close(client_socket);
            free(request);
            exit(1);
        }
        bytes = (request_len + 1) * sizeof(char);
        request = realloc(request, bytes);
        if (ch == '\r' || ch == '\n') {
            flag++;
            if (flag >= 3) {
                break;
                request[request_len] = '\0';
            }
        } else {
            flag = 0;
        }
        request[request_len] = ch;
        request_len++;
    } while (ch != '\0');

    return request;
}

void socket_send_message(int client_socket, char *message) {
    int message_len = strlen(message);

    if (write(client_socket, message, message_len * sizeof(char)) <= 0) {
        free(message);
        exit(0);
    }

}

HTTPreq *my_recv(int client_socket) {
    HTTPreq *req = malloc(sizeof(HTTPreq));
    char *buf = malloc(65536 * sizeof(char));
    int rcvd; // size of read data

    rcvd = recv(client_socket, buf, 65536 * sizeof(char), 0);
    if (rcvd <= 0) {
        // perror("");
        exit(1);
    }
    buf[rcvd] = '\0';
    puts(buf);
    req->method = strtok(buf, " \r\n");
    req->path = strtok(NULL, " ");
    req->protocol = strtok(NULL, " \r\n");
    return req;
}
