CC=gcc
CFLAGS=-Wall -Werror -lm -g -Iinclude
LIB = include/init_socket.c include/file.c include/http.c

.PHONY: all, clean

all: bin bin/server

bin:
	mkdir bins

bin/server: src/server.c
	$(CC) $(CFLAGS) $(LIB) -o bin/server

clean:
	rm bin/server
	rmdir bin
