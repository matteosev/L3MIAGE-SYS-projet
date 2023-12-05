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
        
        int pid = fork();
        switch(pid) {
            case -1:
                perror("Erreur");
                break;
            case 0:

                //fils
                close(sock_listen);
                
                Request req;

                do{
                    
                    read(sock_service, &req, sizeof(req));
            
                    int nb_train = 2;

                    Train monTrain;
                    Time horaire1;
                    Time horaire2;

                    horaire1.hour = 10;
                    horaire1.minute = 30;

                    horaire2.hour = 11;
                    horaire2.minute = 30;

                    monTrain.id = 10;
                    strcpy(monTrain.city_from, "MONTELIMAR");
                    strcpy(monTrain.city_to, "PARIS");
                    monTrain.time_from = horaire1;
                    monTrain.time_to = horaire2;
                    monTrain.price = 9.80;
                    monTrain.reduc = 0;
                    monTrain.suppl = 0;

                    write(sock_service, &nb_train, sizeof(nb_train));
                    
                    for(int i = 0; i < nb_train; i++){

                        write(sock_service, &monTrain, sizeof(monTrain));
                            
                    }

                }while(req.last == 0);
                
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

int filter_train_from_array(Train trains[], int nb_train, Train *trains_filtered, char* city_from, char* city_to, Time time_from, Time time_to){

    trains_filtered = malloc(0);
    int nb_train_filtered;

    for(int i = 0; i < nb_train; i++){

        if(check_filter(trains[i], city_from, city_to, time_from, time_to) == 1){
            
            trains_filtered = realloc(trains_filtered, sizeof(trains_filtered) + sizeof(Train));
            trains_filtered[nb_train_filtered] = trains[i];
            nb_train_filtered++;
        }
    }

    return nb_train_filtered;
}

int check_filter(Train train, char* city_from, char* city_to, Time time_from, Time time_to){

    if(strcmp(train.city_from, city_from) != 0)
        return 0;

    if(strcmp(train.city_to, city_to) != 0)
        return 0;

    if(train.time_from.hour != time_from.hour || train.time_from.minute != time_from.minute)
        return 0;

    if(train.time_to.hour != time_to.hour || train.time_to.minute != time_to.minute)
        return 0;

    return 1;
}