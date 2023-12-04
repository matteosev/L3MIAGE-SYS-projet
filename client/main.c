
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

int main(int argc, char **argv) {

    struct hostent *h = gethostbyname("localhost");

    struct in_addr ip_server = { *h->h_addr_list[0] };
    
    struct sockaddr_in addr_server;

    // Taille max trame ethernet = 1500o
    char msg_sent[1000] = "";
    char msg_received[1000] = "";

    addr_server.sin_family = AF_INET;
    addr_server.sin_port = 5000;
    addr_server.sin_addr = ip_server;

    int sock_server_fd = socket(AF_INET, SOCK_STREAM, 0);
    connect(sock_server_fd, (struct sockaddr *)&addr_server, sizeof(addr_server));
    
    while(strcmp(msg_sent, "finito") != 0) {
        scanf("%s", msg_sent);
        int size_msg_sent = strlen(msg_sent);
        int size_msg_received = 0;

        printf("envoyé : %s\n", msg_sent);

        write(sock_server_fd, &size_msg_sent, sizeof(size_msg_sent));

        printf("1\n");

        write(sock_server_fd, msg_sent, size_msg_sent);
        
        printf("2\n");

        read(sock_server_fd, &size_msg_received, sizeof(size_msg_received));

        printf("3\n");

        read(sock_server_fd, msg_received, size_msg_received);

        printf("recu : %s\n", msg_received);
    }

    return 0;
}