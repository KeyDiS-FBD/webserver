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

char *get_filetype(char *path) {
    char *filetype;

    filetype = strtok(path, " ");
    filetype = strtok(NULL, " .");

    return filetype;
}

char *scan_file(char *path) {
    FILE *fp = NULL;
    fp = fopen(path + 1, "rb");
    if (fp == NULL) {
        puts("ERROR WITH FILE OPEN");
        return NULL;
    }
    char line[256];
    size_t max_line_size = 256;
    size_t line_size;
    char *filetext = NULL;
    size_t filetext_size = 0;

    while (fgets(line, max_line_size, fp) != NULL) {
        line_size = strlen(line);
        filetext_size += line_size;
        filetext = realloc(filetext, (filetext_size + 1) * sizeof(char));
        strncat(filetext, line, line_size);
    }
    filetext[filetext_size] = '\0';
    fclose(fp);
    return filetext;
}
