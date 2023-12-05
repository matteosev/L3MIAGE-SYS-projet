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

    const int nb_train = 3;
    Train trains[nb_train];
    
    for(int i = 0; i < nb_train; i++){

        trains[i].id = i;
        switch (i)
        {
            case 0 :

                strcpy(trains[i].city_from, "MONTELIMAR");
                strcpy(trains[i].city_to, "PARIS");
                break;
            
            case 1 :

                strcpy(trains[i].city_from, "GRENOBLE");
                strcpy(trains[i].city_to, "VALENCE");
                break;
            
            default :

                strcpy(trains[i].city_from, "PARIS");
                strcpy(trains[i].city_to, "VALENCE");
                break;
        
        }
        
        trains[i].time_from.hour = 10;
        trains[i].time_from.minute = 0;
        trains[i].time_to.hour = 10;
        trains[i].time_to.minute = 30;
        trains[i].price = 9.80;
        trains[i].reduc = 0;
        trains[i].suppl = 0;
    } 


    

    while(1) {

        Train *filtered_trains;

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

                    int nb_train_filtered = filter_train_from_array(trains, nb_train, filtered_trains, req.city_from, req.city_to, req.time_from_1, req.time_from_2);
                    printf("%d\n", nb_train_filtered);
                    write(sock_service, &nb_train_filtered, sizeof(nb_train_filtered));
                    
                    for(int i = 0; i < nb_train_filtered; i++){

                        write(sock_service, &filtered_trains[i], sizeof(filtered_trains[i]));
                            
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
    int nb_train_filtered = 0;

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