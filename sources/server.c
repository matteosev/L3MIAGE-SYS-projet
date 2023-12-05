#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "../headers/commons.h"

int read_trains_from_file(char *filename, Train *trains, int maxTrains) ;
int count_trains(char *filename);
int filter_train_from_array(Train trains[], int nb_train, Train *trains_filtered, char* city_from, char* city_to, Time time_from, Time time_to);
int check_filter(Train train, char* city_from, char* city_to, Time time_from, Time time_to);

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

    char *csv = "db.csv";
    
    const int nb_train = count_trains(csv);
    
    Train trains[nb_train];

    read_trains_from_file(csv, trains, nb_train);
    /*
    for (int i = 0; i < nb_train; i++) {

        print_train(&trains[i]);
    }
    */
    
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
                    
                    printf("trains filtrés : %d\n", nb_train_filtered);
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

int count_trains(char *filename) {
    FILE *file = fopen(filename, "r");
    int nbtrains = 0;

    if (file == NULL)
        exit(1);

    char c;
    while(!feof(file)) {
        fscanf(file, "%c", &c);
        if (c == '\n')
            nbtrains++;
    }

    fclose(file);
    
    return nbtrains + 1;
}

int read_trains_from_file(char *filename, Train *trains, int maxTrains) {
    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Error opening file");
        return 0;
    }

    char line[256];
    int i = 0;
    while (fgets(line, sizeof(line), file) && i < maxTrains) {
        
        char *token;
        char *rest = line;

        token = strtok_r(rest, ";", &rest); // ID
        trains[i].id = atoi(token);

        token = strtok_r(NULL, ";", &rest); // City From
        strcpy(trains[i].city_from, token);

        token = strtok_r(NULL, ";", &rest); // City To
        strcpy(trains[i].city_to, token);

        token = strtok_r(NULL, ":", &rest); // Hour Time_From
        trains[i].time_from.hour = atoi(token);

        token = strtok_r(NULL, ";", &rest); // Minute Time_From
        trains[i].time_from.minute = atoi(token);

        token = strtok_r(NULL, ":", &rest); // Hour Time_To
        trains[i].time_to.hour = atoi(token);

        token = strtok_r(NULL, ";", &rest); // Minute Time_To
        trains[i].time_to.minute = atoi(token);

        token = strtok_r(NULL, ";", &rest); // Price
        trains[i].price = strtod(token, NULL);

        token = strtok_r(NULL, "\n", &rest); // Reduc/Suppl
        if (token != NULL) {

            if (strcmp(token, "REDUC") == 0) {
                trains[i].reduc = 1;
            } else if (strcmp(token, "SUPPL") == 0) {
                trains[i].suppl = 1;
            }

        }else{

            trains[i].reduc = 0;
            trains[i].suppl = 0;
        }

        i++;
    }

    fclose(file);
    return i; // Number of trains read
}