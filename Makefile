CC=gcc
CFLAGS=-Wall -Werror -lm -g -Iinclude
LIB = include/init_socket.c include/file.c include/http.c

.PHONY: all, clean

all: bin bin/server

bin:
	mkdir bins

bin/server: src/server.c
	$(CC) $(CFLAGS) $(LIB) src/server.c -o bin/server

bin/client: src/client.c
	$(CC) $(CFLAGS) -g  include/init_socket.c src/client.c -o bin/client

clean:
	rm bin/server
	rmdir bin
