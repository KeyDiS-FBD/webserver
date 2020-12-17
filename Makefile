CC=gcc
CFLAGS=-Wall -Werror -lm -g -Iinclude


.PHONY: all, clean

all: bin bin/server

bin:
	mkdir bins

bin/server: src/server.c include/init_socket.c
	$(CC) $(CFLAGS) src/server.c include/init_socket.c include/file.c -o bin/server

clean:
	rm bin/server
	rmdir bin
