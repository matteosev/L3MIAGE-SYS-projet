/**
 * \file server.c
 * \brief Point d'entrée du serveur
 *
 * Ecoute les requêtes effectuées sur un port et répond
 *
 */

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
#include "../headers/filter.h"
#include "../headers/parsing.h"
#include "../headers/protocol.h"
#include "../headers/time.h"

// 100 connexions simultanées maximum
int children[100];
int nb_child;

/**
 * Gère l'interruption du processus père (serveur)
 * Envoie un signal d'interruption (SIGINT) à chacun des processus fils encore en vie
 */
void handle_sigint_father() {
    printf("\nSIGINT reçu par père\n%d fils encore en vie\n", nb_child);
    // Parcours des pid des fils mémorisés
    for (int i = 0; i < nb_child; i++) {
        int pid = children[i];
        // Si pid valide, envoi d'un signal d'interruption
        if (pid > 0) {
            kill(pid, SIGINT);
        }
    }
    exit(0);
}

/**
 * Gère l'interruption d'un processus fils
 * Informe l'administrateur de sa mort
 */
void handle_sigint_child() {
    printf("SIGINT reçu par %d\n", getpid());
    exit(0);
}

void handle_sigchld() {
    // pid de l'enfant mort
    int pid = wait(NULL);
    printf("Processus %d est mort\n", pid);
    // Recherche du pid dans les enfants mémorisés
    int i = 0;
    while(i < nb_child && children[i] != pid)
        i++;
    // Oubli du pid
    children[i] = 0;
    nb_child--;
}

/**
 * 
 * @param argv[1] Chemin vers le fichier csv qui sert de base de données
 * @param argv[2] Numéro de port écouté par le serveur
 * @return 0 normalement, 1 en cas d'erreur
*/
int main(int argc, char **argv) {
    
    if (argc != 3) {
        printf("Le serveur a besoin du chemin vers le fichier csv et d'un numéro de port\n");
        exit(1);
    }

    int sock_listen;
    struct sockaddr_in sockaddr;
    char *db_filename = argv[1];

    signal(SIGINT, handle_sigint_father);
    signal(SIGCHLD, handle_sigchld);
    
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = atoi(argv[2]);
    sockaddr.sin_addr.s_addr = INADDR_ANY;  // sin_addr est de type 'struct in_addr'

    socklen_t size_addr = sizeof(sockaddr);
    
    sock_listen = socket(AF_INET, SOCK_STREAM, 0);
    bind(sock_listen, (const struct sockaddr *)&sockaddr, size_addr);
    listen(sock_listen, 0);

    // Lecture du fichier csv "base de données"
    // argv[1] : nom du fichier csv, paramètre 1 de la ligne de commande
    const int nb_train = count_trains(db_filename);
    Train trains[nb_train];
    
    char cities[MAX_CITIES][MAX_CITY_NAME_LENGTH];
    int city_count = 0;

    read_trains_from_file(db_filename, trains, nb_train, cities, &city_count);

    nb_child = 0;
    
    while(1) {

        Train *filtered_trains = NULL;

        int sock_service = accept(sock_listen, (struct sockaddr *)&sockaddr, &size_addr);
        int pid = fork();

        switch(pid) {

            case -1:
            
                perror("fork");
                break;

            case 0: // fils
                // Définition du comportement en cas de SIGINT reçu (envoyé par le père)
                signal(SIGINT, handle_sigint_child);
                close(sock_listen);
                Request req;
                if (write(sock_service, &city_count, sizeof(city_count)) == -1) {
                    perror("write city count");
                    exit(1);
                }

                
                for (int i = 0; i < city_count; i++) {
                    if (write(sock_service, cities[i], sizeof(cities[i])) == -1) {
                        perror("write city data");
                        exit(1);
                    }
                }

                do{
                    
                    receive_request(sock_service, &req);
                    print_request(req);
                    int nb_train_filtered = filter_train_from_array(trains, nb_train, &filtered_trains, req);

                    if (write(sock_service, &nb_train_filtered, sizeof(nb_train_filtered)) == -1)
                        perror("write du nombre de train");
                    
                    for(int i = 0; i < nb_train_filtered; i++){
                        send_train(sock_service, filtered_trains[i]);}
                        
                } while(req.type != FIN);
                
                printf("Connexion avec %s fermée normalement par le processus %d\n", inet_ntoa(sockaddr.sin_addr), getpid());
                close(sock_service);
                exit(0);

            default: // père

                close(sock_service);
                printf("Connexion établie avec %s et gérée par le processus %d\n", inet_ntoa(sockaddr.sin_addr), pid);
                int i = 0;
                // Recherche d'une place libre dans le tableau de mémorisation des fils
                while (i < nb_child && children[i] < 1) i++;
                // Mémorisation du fils
                children[i] = pid;
                nb_child++;
                break;
        }
    }

    close(sock_listen);

    return 0;
}
