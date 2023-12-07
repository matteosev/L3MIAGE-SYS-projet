
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
    //struct hostent *host = gethostbyname("localhost");
    //ip_server.s_addr = *host->h_addr_list[0];

    // Connexion distante
    ip_server.s_addr = inet_addr("152.77.82.87");

    addr_server.sin_family = AF_INET;
    addr_server.sin_port = PORT;
    addr_server.sin_addr = ip_server;

    sock_server = socket(AF_INET, SOCK_STREAM, 0);

    int connect_tried = 0;
    while (connect(sock_server, (struct sockaddr *)&addr_server, sizeof(addr_server)) == -1 && connect_tried < 10) {
        printf("Tentative de connexion n°%d échouée\n", connect_tried);
        connect_tried++;
        sleep(1);
    }

    if (connect_tried == 10) {
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

        //Train train;
        int choix;

        printf("------------------------------------------------------------\n");
        printf("Chercher, entre 2 villes ...\n");
        printf("%d. LE train disponible le + tôt à partir d'un horaire\n", HORAIRE);
        printf("%d. TOUS les trains dans une plage horaire\n", PLAGE);
        printf("%d. TOUS les trains de la journée\n", JOURNEE);
        printf("%d. RIEN, bye\n", FIN);
        printf("------------------------------------------------------------\n");
        scanf("%d", &choix);

        if(choix == HORAIRE || choix == PLAGE  || choix == JOURNEE){

            
            // Print the cities
            printf("Villes disponibles:\n");
            for (int i = 0; i < city_count; i++) {
                printf("%d. %s\n", i + 1, cities[i]);
            }

            printf("Ville de départ : ");
            scanf("%99s", req.city_from);
            printf("Ville d'arrivée : ");
            scanf("%99s", req.city_to);
        }

        req.type = choix;

        switch(choix){

            case HORAIRE :

                printf("Horaire de départ hh:mm : ");
                scanf("%d:%d", &req.time_from_1.hour, &req.time_from_1.minute);
                printf("\n");       
                break;
            
            case PLAGE :

                printf("Horaire de départ n°1 hh:mm : ");
                scanf("%d:%d", &req.time_from_1.hour, &req.time_from_1.minute);
                printf("\n");  

                printf("Horaire de départ n°2 hh:mm : ");
                scanf("%d:%d", &req.time_from_2.hour, &req.time_from_2.minute);
                printf("\n");         
                break;

            case JOURNEE :

                break;

            case FIN :
                printf("Merci et bonne journée !\n");
                break;

            default :
        }

        /*
        if (write(sock_server, &req, sizeof(req)) == -1)
            perror("write de la requête");
        */
        send_request(sock_server, req);

        if(choix == HORAIRE || choix == PLAGE  || choix == JOURNEE){

            int nb_train;

            if (read(sock_server, &nb_train, sizeof(nb_train)) == -1)
                perror("read nombre de train");

            printf("Nombre de train : %d\n", nb_train);

            Train trains[nb_train];

            for(int i = 0; i < nb_train; i++){

                /*
                if (read(sock_server, &train, sizeof(train)) == -1)
                    perror("read train");
                */
               
                receive_train(sock_server, &trains[i]);
                display_train(trains[i]);
                
            }

            if(choix == HORAIRE && nb_train > 1){

                char rep = ' ';

                printf("%d options sont disponibles, souhaitez-vous le train le plus rapide ? (y/n)\n", nb_train);
                while(rep != 'y' && rep != 'n'){

                    scanf("%c", &rep);

                }

                switch(rep){

                    case 'y' :

                        Time time_cmp={23,59};
                        Train * fastest;

                        for(int i = 0; i < nb_train; i++){

                            Time duration = time_difference(trains[i].time_from,trains[i].time_to);

                            if(timecmp(duration, time_cmp)==-1){
                                memcpy(&time_cmp, &duration, sizeof(Time));
                                fastest=&trains[i];
                            }
                        }
                        display_train(*fastest);
                        break;

                    case 'n' :

                        double price_min=99999.9999;
                        Train * cheapest;

                        for(int i =0; i<nb_train;i++){

                            if(trains[i].price < price_min){

                                price_min=trains[i].price;
                                cheapest=&trains[i];
                            }
                        }
                        display_train(*cheapest);
                        break;

                    default :
                }     
            }
        }

    } while(req.type != FIN);

    return 0;
}
