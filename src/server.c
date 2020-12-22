#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <arpa/inet.h>

//Пользовательские заголовочные файлы
#include <file.h>
#include <http.h>
#include <init_socket.h>


#define OK 200
#define NOT_FOUND 404

// void send_index_page(int client, HTTPreq *request);
void send_response_404(int client_socket);
void send_response(HTTPreq *req);
void serve(int client_socket);

// char *read_from_fd(int fd) {
//     char *message;
//     int message_size = 0;
//     int rcvd;
//
//     do {
//         message = realloc(message, (message_size + 1) * sizeof(char));
//
//         if ((rcvd = read(fd, &message[message_size], 1)) < 0) {
//             free(message);
//             exit(1);
//         }
//         message_size++;
//     } while (rcvd != 0);
//     message[message_size] = '\0';
//
//     return message;
// }
//
// int get_binary(HTTPreq *req) {
//     char *message = NULL;
//     char *header_http ="HTTP/1.1 200 OK\r\n";
//     char *header_type = "Content-type: text/html\r\n";
//     const char header_length_template[] = "Content-length: %ld\r\n\r\n";
//     char *header_length = malloc(sizeof(header_length_template));
//     int pid1 = fork();
//     if (pid1 == 0) {
//         int fd[2];
//         pipe(fd);
//         int pid2 = fork();
//
//         if (pid2 == 0) {
//             dup2(fd[1], 1);
//             close(fd[0]);
//             close(fd[1]);
//             execvp(req->path + 1, NULL);
//             exit(0);
//         }
//         close(fd[1]);
//         message = read_from_fd(fd[0]);
//         sprintf(header_length, header_length_template, strlen(message));
//         socket_send_message(req->client_socket, header_http);
//         socket_send_message(req->client_socket, header_type);
//         socket_send_message(req->client_socket, header_length);
//         socket_send_message(req->client_socket, message);
//         free(message);
//         free(header_length);
//         close(fd[0]);
//         waitpid(pid2, NULL, 0);
//     }
//     waitpid(pid1, NULL, 0);
//     return 0;
// }

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
    char *header_http ="HTTP/1.1 200 OK\r\n";
    const char header_type_template[] = "Content-type: %s\r\n";
    const char header_length_template[] = "Content-length: %ld\r\n\r\n";
    char *header_type = malloc(sizeof(header_type_template));
    char *header_length = malloc(sizeof(header_length_template));
    char *filedata = NULL;
    char *filetype = NULL;
    // int header_size = 0;
    size_t file_size = 0;
    struct stat ck_file;

    filetype = get_filetype(req->path);
    if (strcmp(filetype, "html") == 0 || strcmp(filetype, "txt") == 0) {
        sprintf(header_type, header_type_template, "text/html");
    } else if (strcmp(filetype, "png") == 0) {
        sprintf(header_type, header_type_template, "image/png");
    
    // } else if (access((req->path) + 1, 1) == 0) {
        // free(header_type);
        // free(header_length);
        // free(filetype);
        // get_binary(req);
        // return;
    }
    stat(req->path + 1, &ck_file);
    file_size = ck_file.st_size;
    filedata = scan_file(req->path);
    if (filedata == NULL) {
        puts("File not found");
        send_response_404(req->client_socket);
        return;
    }

    sprintf(header_length, header_length_template, file_size);

    // send header with status code
    socket_send_message(req->client_socket, header_http);
    socket_send_message(req->client_socket, header_type); // buf in this moment is type
    socket_send_message(req->client_socket, header_length); // buf in this moment is len

    // send(req->client_socket, response, strlen(response) * sizeof(char), 0);
    // send(req->client_socket, filetext, filetext_len * sizeof(char), 0);
    socket_send_message(req->client_socket, filedata);

    // free after using
    free(filetype);
    free(filedata);
    free(header_type);
    free(header_length);

}

void serve(int client_socket) {
    HTTPreq *req;
    char *request = NULL;
    while (1) {
        request = get_request(client_socket);
        req = parse_request(request);
        req->client_socket = client_socket;
        puts("SERVE:");
        puts(req->method);
        puts(req->protocol);
        puts(req->path);
        // puts(req->request);
        send_response(req);

        free(req);
        free(request);
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
    int max_client_num = 5; // set max client connection number
    // int curr_client_num = 0;
    int pid;
    // char *error_message = "Max connection error, reconnect later";
    // a function to track the death of child processes
    // void child_hadler(int sig) {
    //     curr_client_num--;
    //     puts("Client left");
    // }
    // signal(SIGCHLD, child_hadler);

    for (int i = 0; i < max_client_num; i ++) {


    // while (1) {
        // if (curr_client_num == max_client_num) {
        //     // write(client_socket, error_message, strlen(error_message));
        //     puts(error_message);
        //     close(client_socket);
        //     wait(NULL);
        // } else {
            // puts("Client accepted");
            // curr_client_num++;
            // printf("Client number:%d\n", curr_client_num);
            pid = fork();
            if (pid == 0) {
                // while(1) {}
                client_socket = socket_accept(server_socket);
                close(server_socket);
                serve(client_socket);
                exit(0);
            } else if (pid < 0) {
                perror("Error fork");
                exit(1);
            }
            // close(client_socket);
            // wait(NULL);
        // }
    }
    for (int i = 0; i < max_client_num; i++) {
        wait(NULL);

    }

    close(server_socket);
    return 0;
}
