#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

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
    
    int boucle = 0;

    while(1 && boucle < 10) {
        int sock_service = accept(sock_listen, (struct sockaddr *)&sockaddr, &size_addr);
        boucle++;
        int pid = fork();
        switch(pid) {
            case -1:
                perror("Erreur");
                break;
            case 0:
                //fils
                close(sock_listen);
                char msg[100];
                char bj[] = "Bonjour\n";
                read(sock_service, msg, sizeof(bj));
                printf("Message recu : %s", msg);
                close(sock_service);
                exit(0);
            default:
                // père
                close(sock_service);
                break;
        }

    }

    return 0;
}