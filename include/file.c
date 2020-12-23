#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <netdb.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <netinet/in.h>
#include <arpa/inet.h>

char *get_filetype(char *path) {
    char *filetype;
    int i = 0;
    puts("GET_FILETYPE");
    puts(path);
    filetype = strtok(path, " \r\n");

    for(i = 0; filetype[i] != '.'; i++) {
        if (filetype[i] == '\0') {
            return "binary";
        }
    }
    return &filetype[i + 1];
}

// char *scan_file(char *path) {
//     // FILE *fp = NULL;
//     int fd;
//     char *filedata = NULL;
//     size_t file_size = 0;
//     int rcvd = 0;
//
//     fd = open(path + 1, O_RDONLY, 0);
//     // fp = fopen(path + 1, "rb");
//     // if (fp == NULL) {
//     if (fd < 0) {
//         puts("ERROR WITH FILE OPEN");
//         return NULL;
//     }
//     while (1) {
//         filedata = realloc(filedata, (file_size + 1) * sizeof(char));
//         filedata[file_size] = '\0';
//         // fread(&filedata[file_size], sizeof(char), 1, fp);
//         rcvd = read(fd, &filedata[file_size], sizeof(char));
//         if (rcvd < 0) {
//         // if (!feof(fp)) {
//             puts("SCAN_FILE");
//             puts(filedata);
//             puts("File read error");
//             free(filedata);
//             return NULL;
//         } else if (rcvd == 0) {
//             break;
//         }
//         file_size++;
//     }
//     filedata[file_size] = '\0';
//     // fclose(fp);
//     close(fd);
//     return filedata;
// }
