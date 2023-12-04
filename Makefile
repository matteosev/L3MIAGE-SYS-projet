all: server client clean

server: server.o commons.o
	gcc server.o commons.o -o bin/server

client: client.o commons.o
	gcc client.o commons.o -o bin/client

server.o: sources/server.c
	gcc -c sources/server.c -o server.o

client.o: sources/client.c
	gcc -c sources/client.c -o client.o

commons.o: sources/commons.c
	gcc -c sources/commons.c -o commons.o

clean:
	rm -f *.o
