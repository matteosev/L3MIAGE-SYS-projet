
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



int main(int argc, char **argv) {

    struct hostent *host;
    struct in_addr ip_server;
    struct sockaddr_in addr_server;
    int sock_server;
    Request req;

    host = gethostbyname("localhost");

    ip_server.s_addr = *host->h_addr_list[0];

    addr_server.sin_family = AF_INET;
    addr_server.sin_port = 5000;
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
        

    do{
        Train train;
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

            printf("Villes disponibles : Paris, Montelimar, Grenoble, Valence \n");
            printf("Ville de départ : ");
            scanf("%99s", req.city_from);
            printf("Ville d'arrivée : ");
            scanf("%99s", req.city_to);
        }

        req.type = choix;

        switch(choix){

            case HORAIRE :
                req.time_from_1.hour = 10;
                req.time_from_1.minute = 07;
                req.time_from_2.hour = 12;
                req.time_from_2.minute = 19;
                
                break;
            
            case PLAGE :
                req.time_from_1.hour = 8;
                req.time_from_1.minute = 0;
                req.time_from_2.hour = 18;
                req.time_from_2.minute = 0;
                break;

            case JOURNEE :
                req.time_from_1.hour = 25;
                req.time_from_1.minute = 60;
                req.time_from_2.hour = 25;
                req.time_from_2.minute = 60;
                break;

            case FIN :
                printf("Merci et bonne journée !\n");
                break;

            default :
        }

        if (write(sock_server, &req, sizeof(req)) == -1)
            perror("write de la requête");

        if(choix == HORAIRE || choix == PLAGE  || choix == JOURNEE){

            int nb_train;

            if (read(sock_server, &nb_train, sizeof(nb_train)) == -1)
                perror("read nombre de train");

            printf("Nombre de train : %d\n", nb_train);

            for(int i = 0; i < nb_train && i < 10; i++){

                if (read(sock_server, &train, sizeof(train)) == -1)
                    perror("read train");

                printf("%d : ", i);
                print_train(train);
            }
            printf("\n");
        }

    } while(req.type != FIN);

    return 0;
}
