#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main(int argc, char **argv) {
    int sock_fd;
    char sin_zero[] = { 0, 0, 0, 0, 0, 0, 0, 0 };
    
    const struct sockaddr_in sockaddr = {
        AF_INET,
        5000,
        INADDR_ANY,
        sin_zero
    };

    int size_addr = sizeof(sockaddr);
    
    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    bind(sock_fd, &sockaddr, size_addr);
    listen(sock_fd, NULL);
    int service = accept(sock_fd, (struct sockaddr *)&sockaddr, &size_addr);

    while(1) {
        int pid = fork();
        switch(pid) {
            case -1:
            //
                break;
            case 0:
                //fils
                char msg[100];
                read(service, msg, 8);
                printf("Message recu : %s", msg);
                break;
            default:
                // père
                close(service);
                break;
        }

    }

    return 0;
}