
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

typedef enum 
{
    CHOIX_1 = 1,
    CHOIX_2 = 2,
    CHOIX_3 = 3,
    QUITTER = 4

} Menu;

int main(int argc, char **argv) {

    struct hostent *h = gethostbyname("localhost");

    struct in_addr ip_server = { *h->h_addr_list[0] };
    
    struct sockaddr_in addr_server;
    int flag = 0;

    

    addr_server.sin_family = AF_INET;
    addr_server.sin_port = 5000;
    addr_server.sin_addr = ip_server;

    int sock_server_fd = socket(AF_INET, SOCK_STREAM, 0);
    connect(sock_server_fd, (struct sockaddr *)&addr_server, sizeof(addr_server));
    
    Request req;

    do{
        req.last = 0;
        Train train;
        int rep; 
        
        //memset(msg_sent, 0, sizeof(msg_sent));
        //memset(&train, 0, sizeof(train));

        printf("Que voulez-vous faire ?\n");
        printf("%d. Demander le premier train disponible entre deux villes à partir d'une heure donnée\n", CHOIX_1);
        printf("%d. Demander tous les trains disponibles entre deux villes durant une plage horaire\n", CHOIX_2);
        printf("%d. Demander tous les trains disponibles entre deux villes\n", CHOIX_3);
        printf("%d. Quitter\n", QUITTER);
        scanf("%d", &rep);

        if(rep == CHOIX_1 || rep == CHOIX_2  || rep == CHOIX_3){

            printf("Villes disponibles : PARIS, MONTELIMAR, GRENOBLE, VALENCE \n");
            printf("Ville numero 1 : ");
            scanf("%s", &req.city_from);
     
            printf("Ville numero 2 : ");
            scanf("%s", &req.city_to);
        }
    
        switch(rep){

            case CHOIX_1 :

                req.time_from_1.hour = 10;
                req.time_from_1.minute = 0;
                req.time_from_2.hour = 25;
                req.time_from_2.minute = 60;

                break;
            
            case CHOIX_2 :

                req.time_from_1.hour = 10;
                req.time_from_1.minute = 0;
                req.time_from_2.hour = 12;
                req.time_from_2.minute = 0;

                break;

            case CHOIX_3 :

                req.time_from_1.hour = 25;
                req.time_from_1.minute = 60;
                req.time_from_2.hour = 25;
                req.time_from_2.minute = 60;

                break;

            case QUITTER :

                req.last = 1;
                printf("Merci et bonne journée !\n");
                break;

            default : 

                break;
        }

        write(sock_server_fd, &req, sizeof(req));

        if(rep == CHOIX_1 || rep == CHOIX_2  || rep == CHOIX_3){

            int nb_train;

            read(sock_server_fd, &nb_train, sizeof(nb_train));

            printf("Nombre de train : %d\n", nb_train);

            for(int i = 0; i < nb_train; i++){

                printf("Train numero %d\n", i);
                read(sock_server_fd, &train, sizeof(train));
                print_train(&train);
            }
        }

    } while(req.last == 0);

    return 0;
}
