
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {

    int ip_server = gethostbyname("localhost");

    const struct sockaddr_in addr_server = {
        AF_INET,
        5000,
        ip_server,
        { 0, 0, 0, 0, 0, 0, 0, 0 }
    };

    int sock_server_fd;
    connect(&sock_server_fd, (struct sockaddr *)&addr_server, sizeof(addr_server));
    char msg[] = "Bonjour";
    write(sock_server_fd, msg, sizeof(msg));

    return 0;
}