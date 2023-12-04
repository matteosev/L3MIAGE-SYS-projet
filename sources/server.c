#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../headers/commons.h"

int main(int argc, char **argv) {
    int sock_listen;
    struct sockaddr_in sockaddr;
    struct in_addr ip = {INADDR_ANY};
    
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = 5000;
    sockaddr.sin_addr = ip;

    socklen_t size_addr = sizeof(sockaddr);
    
    sock_listen = socket(AF_INET, SOCK_STREAM, 0);
    bind(sock_listen, (const struct sockaddr *)&sockaddr, size_addr);
    listen(sock_listen, 0);

    while(1) {
        int sock_service = accept(sock_listen, (struct sockaddr *)&sockaddr, &size_addr);
        
        // Taille max trame ethernet = 1500o
        char msg_received[1000] = "";

        int pid = fork();
        switch(pid) {
            case -1:
                perror("Erreur");
                break;
            case 0:
                //fils
                close(sock_listen);

                while(strcmp(msg_received, "finito") != 0) {

                    char msg_sent[1000];
                    memset(msg_received, 0, sizeof(msg_received));

                    read_size_then_msg(sock_service, msg_received);

                    printf("Msg recu : %s\n", msg_received);

                    if (strcmp(msg_received, "finito") == 0)
                        strcpy(msg_sent, "finito");
                    else
                        strcpy(msg_sent, "no finito");

                    write_size_then_msg(sock_service, msg_sent);

                    printf("Msg envoyé : %s\n", msg_sent);
                }
                
                printf("Connexion fermée FINITO\n");

                close(sock_service);
                exit(0);
            default:
                // père
                close(sock_service);
                break;
        }
    }

    close(sock_listen);

    return 0;
}
