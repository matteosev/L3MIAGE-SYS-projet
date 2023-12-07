
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "../headers/commons.h"
#include "../headers/protocol.h"
#include "../headers/std_input.h"
#include "../headers/std_output.h"
#include "../headers/time.h"

int main(int argc, char **argv) {

    struct in_addr ip_server;
    struct sockaddr_in addr_server;
    int sock_server;
    Request req;
    char cities[250][100];
    int city_count = 0;

    // Connexion locale
    struct hostent *host = gethostbyname("localhost");
    ip_server.s_addr = *host->h_addr_list[0];

    // Connexion distante (pc de la salle 215 = 152.77.82.8x)
    //ip_server.s_addr = inet_addr(argv[1]);

    addr_server.sin_family = AF_INET;
    addr_server.sin_port = PORT;
    addr_server.sin_addr = ip_server;

    sock_server = socket(AF_INET, SOCK_STREAM, 0);

 
    if(connect(sock_server, (struct sockaddr *)&addr_server, sizeof(addr_server)) == -1) {
        perror("connexion");
        exit(1);
    }

    if (read(sock_server, &city_count, sizeof(city_count)) == -1) {
        perror("read city count");
        exit(1);
    }

    for (int i = 0; i < city_count; i++) {
        if (read(sock_server, cities[i], sizeof(cities[i])) == -1) {
            perror("Error reading city data");
            exit(1);
        }
    }

    do{

        int choix = choices_display();

        if(choix == HORAIRE || choix == PLAGE  || choix == JOURNEE){

            char input_city_from[100];
            char input_city_to[100];

            city_check(input_city_from, cities, 1);
            strcpy(req.city_from, input_city_from);

            city_check(input_city_to, cities, 2);
            strcpy(req.city_to, input_city_to);
        }

        req.type = choix;

        switch(choix){

            Time choosen_time;

            case HORAIRE :

                check_time(&choosen_time, 1);
                req.time_from_1.hour = choosen_time.hour;
                req.time_from_1.minute = choosen_time.minute;
                break;
            
            case PLAGE :

                check_time(&choosen_time, 2);
                req.time_from_1.hour = choosen_time.hour;
                req.time_from_1.minute = choosen_time.minute;
                
                check_time(&choosen_time, 3);
                req.time_from_2.hour = choosen_time.hour;
                req.time_from_2.minute = choosen_time.minute;
                break;

            case JOURNEE :

                break;

            case FIN :
            
                printf("Merci et bonne journée !\n");
                break;

            default :
        }

        send_request(sock_server, req);

        if(choix == HORAIRE || choix == PLAGE  || choix == JOURNEE){

            int nb_train;

            if (read(sock_server, &nb_train, sizeof(nb_train)) == -1)
                perror("read nombre de train");

            train_number_display(nb_train);
            
            if(nb_train > 0){

                Train trains[nb_train];

                for(int i = 0; i < nb_train; i++){

                    receive_train(sock_server, &trains[i]);
                    display_train(trains[i]);
                }

                if(choix == HORAIRE){

                    char rep = yes_or_no_verification(nb_train);
                    fastest_or_cheapest_train(nb_train, trains, rep);
                }
            }
        }

    } while(req.type != FIN);

    return 0;
}
