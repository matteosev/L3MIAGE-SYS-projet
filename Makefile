all: server client clean

server: server.o commons.o filter.o parsing.o protocol.o time.o
	gcc -Wall server.o commons.o filter.o parsing.o protocol.o time.o -g -o bin/server

client: client.o commons.o protocol.o std_input.o std_output.o time.o
	gcc -Wall client.o commons.o protocol.o std_input.o std_output.o time.o -g -o bin/client

server.o: sources/server.c
	gcc -Wall -c sources/server.c -g -o server.o

client.o: sources/client.c
	gcc -Wall -c sources/client.c -g -o client.o

commons.o: sources/commons.c
	gcc -Wall -c sources/commons.c -g -o commons.o

filter.o: sources/filter.c
	gcc -Wall -c sources/filter.c -g -o filter.o

parsing.o: sources/parsing.c
	gcc -Wall -c sources/parsing.c -g -o parsing.o

protocol.o: sources/protocol.c
	gcc -Wall -c sources/protocol.c -g -o protocol.o

std_input.o: sources/std_input.c
	gcc -Wall -c sources/std_input.c -g -o std_input.o

std_output.o: sources/std_output.c
	gcc -Wall -c sources/std_output.c -g -o std_output.o

time.o: sources/time.c
	gcc -Wall -c sources/time.c -g -o time.o

clean:
	rm -f *.o
