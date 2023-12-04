
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "../commons.h"

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
        
        memset(msg_sent, 0, sizeof(msg_sent));
        memset(msg_received, 0, sizeof(msg_received));

        scanf("%s", msg_sent);

        printf("envoyé : %s\n", msg_sent);

        write_size_then_msg(sock_server_fd, msg_sent);

        read_size_then_msg(sock_server_fd, &msg_received);

        printf("recu : %s\n", msg_received);
    }

    return 0;
}