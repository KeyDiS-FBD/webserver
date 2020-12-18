#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <arpa/inet.h>

//Пользовательские заголовочные файлы
#include <init_socket.h>
#include <file.h>
// #include <http.h>

#define OK 200
#define NOT_FOUND 404

// void send_index_page(int client, HTTPreq *request);
void send_response_404(int client_socket);
void send_response(HTTPreq *req);
void serve(int client_socket);

void send_response_404(int client_socket) {
    char *response_404 = "HTTP/1.1 404\r\nContent-type: text/html\r\nContent-length: 50\r\n\r\n";
    char *not_found = "<!DOCTYPE html><html><h1>404-NOT FOUND</h1></html>";
    printf("%s", response_404);
    // send(client_socket, response_404, strlen(response_404), 0);
    // send(client_socket, not_found, strlen(not_found), 0);
    socket_send_message(client_socket, response_404);
    socket_send_message(client_socket, not_found);

}

void send_response(HTTPreq *req) {
    const char RESPONSE_TEMPLATE[] = {
        "HTTP/1.1 200 OK\r\nContent-type: text/html\r\nContent-length: %d\r\n\r\n"
    };
    char *response = NULL;
    char *filetext = NULL;
    size_t response_size = 0;
    int filetext_len = 0;
    // char *filetype;

    // printf("status:%d\n", status);
    // filetype = get_filetype(req->path);
    filetext = scan_file(req->path);
    if (filetext == NULL) {
        send_response_404(req->client_socket);
        return;
    }
    response_size = sizeof(RESPONSE_TEMPLATE);
    response = malloc(response_size);

    filetext_len = strlen(filetext);
    snprintf(response, response_size, RESPONSE_TEMPLATE, filetext_len);

    send(req->client_socket, response, strlen(response) * sizeof(char), 0);
    free(response);
    send(req->client_socket, filetext, filetext_len * sizeof(char), 0);
    free(filetext);
}

void serve(int client_socket) {
    HTTPreq *req;
    char *filetype;
    while (1) {
        req = my_recv(client_socket);
        req->client_socket = client_socket;
        filetype = get_filetype(req->path);
        puts(filetype);
        
        send_response(req);

        free(req);
    }

}

int check_argc(int argc) {
    if (argc != 2) {
        puts("Incorrect args.");
        puts("./server <ip:port>");
        puts("Example:");
        puts("./server 127.0.0.1:5000");
        return 1;
    }
    return 0;
}

int main(int argc, char** argv) {
    if (check_argc(argc)) {
        return 1;
    }
    // argv[1] is address line in format <ip:port>
    int server_socket = init_socket(argv[1], 0); // second argument is mode
    int client_socket;
    int max_client_num = 3;
    // int conn_client_num = 0;
    int pid;

    for (int i = 0; i < max_client_num; i++) {
        // if (conn_client_num < max_client_num) {
        //     client_socket = socket_accept(server_socket);
        //     conn_client_num++;
        //
        // }
        pid = fork();
        if (pid == 0) {
            client_socket = socket_accept(server_socket);
            close(server_socket);
            serve(client_socket);
            // printf("Method:%s\nPath:%s\nProtocol:%s\n", req->method, req->path, req->protocol);

            exit(0);
        } else if (pid < 0) {
            perror("Error fork");
            exit(1);
        }
    }
    for (int i = 0; i < max_client_num; i++) {
        wait(NULL);
    }
    close(server_socket);
    return 0;
}
