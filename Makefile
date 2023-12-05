all: server client clean

server: server.o commons.o
	gcc -Wall server.o commons.o -g -o bin/server

client: client.o commons.o
	gcc -Wall client.o commons.o -g -o bin/client

server.o: sources/server.c
	gcc -Wall -c sources/server.c -g -o server.o

client.o: sources/client.c
	gcc -Wall -c sources/client.c -g -o client.o

commons.o: sources/commons.c
	gcc -Wall -c sources/commons.c -g -o commons.o

clean:
	rm -f *.o
