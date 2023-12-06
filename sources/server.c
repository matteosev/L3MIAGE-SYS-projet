#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <signal.h>
#include <sys/wait.h>

#include "../headers/commons.h"



void end_child() {
    wait(NULL);
}

void handle_sigsegv() {
    printf("Erreur de segmentation\n");
    exit(1);
}

int main(int argc, char **argv) {
    
    int sock_listen;
    struct sockaddr_in sockaddr;
    struct sigaction ac;

    ac.sa_handler = end_child;
    ac.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &ac, NULL);
    signal(SIGSEGV, handle_sigsegv);
    
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = PORT;
    sockaddr.sin_addr.s_addr = INADDR_ANY;  // sin_addr est de type 'struct in_addr'

    socklen_t size_addr = sizeof(sockaddr);
    
    sock_listen = socket(AF_INET, SOCK_STREAM, 0);
    bind(sock_listen, (const struct sockaddr *)&sockaddr, size_addr);
    listen(sock_listen, 0);

    char *csv = "db.csv";
    const int nb_train = count_trains(csv);
    Train trains[nb_train];
    read_trains_from_file(csv, trains, nb_train);
    
    while(1) {

        Train *filtered_trains = NULL;

        int sock_service = accept(sock_listen, (struct sockaddr *)&sockaddr, &size_addr);
        int pid = fork();

        switch(pid) {

            case -1:
            
                perror("fork");
                break;

            case 0: // fils

                close(sock_listen);
                Request req;

                do{
                    
                    receive_request(sock_service, &req);

                    int nb_train_filtered = filter_train_from_array(trains, nb_train, &filtered_trains, req);

                    if (write(sock_service, &nb_train_filtered, sizeof(nb_train_filtered)) == -1)
                        perror("write du nombre de train");
                    
                    for(int i = 0; i < nb_train_filtered; i++)
                        send_train(sock_service, filtered_trains[i]);
                        
                } while(req.type != FIN);
                
                printf("Connexion fermée normalement par le processus %d\n", getpid());
                close(sock_service);
                exit(0);

            default: // père

                close(sock_service);
                printf("Connexion établie avec %d et gérée par le processus %d\n", sockaddr.sin_addr.s_addr, pid);
                break;
        }
    }

    close(sock_listen);

    return 0;
}