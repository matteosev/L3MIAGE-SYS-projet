all: server client clean

server: server.o commons.o
	gcc server.o commons.o -o server/server

client: client.o commons.o
	gcc client.o commons.o -o client/client

server.o: server/main.c
	gcc -c server/main.c -o server.o

client.o: client/main.c
	gcc -c client/main.c -o client.o

commons.o: commons.c
	gcc -c commons.c -o commons.o

clean:
	rm -f server/*.o client/*.o commons.o server/server client/client