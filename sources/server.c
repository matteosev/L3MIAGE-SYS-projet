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

int read_trains_from_file(char *filename, Train *trains, int maxTrains) ;
int count_trains(char *filename);
int filter_train_from_array(Train trains[], int nb_train, Train **trains_filtered, char* city_from, char* city_to, Time time_from_1, Time time_from_2, Request_type type);
int check_filter(Train train, char* city_from, char* city_to, Time time_from, Time time_to, Request_type type);
int timecmp(Time t1, Time t2);

void end_child() {
    wait(NULL);
}
/*
void handle_sigsegv() {
    printf("Erreur de segmentation\n");
    exit(1);
}
*/
int main(int argc, char **argv) {
    
    int sock_listen;
    struct sockaddr_in sockaddr;
    struct sigaction ac;

    ac.sa_handler = end_child;
    ac.sa_flags = SA_RESTART;
    sigaction(SIGCHLD, &ac, NULL);

    //signal(SIGSEGV, handle_sigsegv);
    
    sockaddr.sin_family = AF_INET;
    sockaddr.sin_port = 5000;
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
                    read(sock_service, &req, sizeof(req));

                    printf("Requête reçue : ");
                    print_request(req);

                    int nb_train_filtered = filter_train_from_array(trains, nb_train, &filtered_trains, req.city_from, req.city_to, req.time_from_1, req.time_from_2, req.type);

                    printf("Nombre de trains trouvés : %d\n", nb_train_filtered);

                    if (write(sock_service, &nb_train_filtered, sizeof(nb_train_filtered)) == -1)
                        perror("write du nombre de train");
                    
                    for(int i = 0; i < nb_train_filtered; i++){
                        
                        printf("%d : ", i);
                        print_train(filtered_trains[i]);
                        write(sock_service, &filtered_trains[i], sizeof(filtered_trains[i]));
                    }

                } while(req.type != FIN);
                
                printf("Connexion fermée normalement par le processus %d\n", getpid());

                close(sock_service);
                exit(0);

            default: // père
                close(sock_service);
                printf("Connexion établie et gérée par le processus %d\n", pid);
                break;
        }
    }

    close(sock_listen);

    return 0;
}

/**
 * Extraie les trains qui valident les critères de filtrage.
 * @param trains Train[] Le tableau de trains qui doit être filtré
 * @param nb_train int La taille du tableau trains
 * @param trains_filtered Train ** Un pointeur vers le pointeur qui pointera vers le tableau de trains filtré
 * @param req Request Les critères de filtrage
 * @return La taille du tableau de trains filtré
 */
int filter_train_from_array(Train trains[], int nb_train, Train **trains_filtered, char* city_from, char* city_to, Time time_from, Time time_to, Request_type type){

    // Nombre de Trains qui valident les critères de recherche (aucun au début)
    int nb_train_filtered = 0;
    // Tableau de Trains qui valident les critères de recherche (vide pour l'instant)
    *trains_filtered = NULL;

    for(int i = 0; i < nb_train; i++){
        // Si le Train n° i valide les critères de filtrage
        if(check_filter(trains[i], city_from, city_to, time_from, time_to, type) == 1){

            // On agrandit le tableau de Train, puis on pointe vers celui ci
            *trains_filtered = realloc(*trains_filtered, sizeof(Train) * (nb_train_filtered + 1));

            if (*trains_filtered == NULL)
                perror("realloc");

            // On copie le train qui valide les critères de recherche à la fin du tableau de Train
            memcpy(&(*trains_filtered)[nb_train_filtered], &trains[i], sizeof(Train));

            nb_train_filtered++;
        }
    }
    return nb_train_filtered;
}

int check_filter(Train train, char* city_from, char* city_to, Time time_from_1, Time time_from_2, Request_type type){

    if(strcmp(train.city_from, city_from) != 0)
        return 0;

    if(strcmp(train.city_to, city_to) != 0)
        return 0;

    switch (type) {

        case HORAIRE:

            break;
        
        case PLAGE:
            print_train(train);
            int tot = timecmp(train.time_from, time_from_1);
            int tard = timecmp(train.time_from, time_from_2);
            printf("tot=%d tard=%d\n", tot, tard);
            if ( tot == -1 || tard == 1)
                return 0;
            break;

        case JOURNEE:
            // Horaire de départ inutile
            break;

        case FIN:
            break;
        
        default:
    }
    
    return 1;
}

/* REMPLACER LES 2 FONCTIONS AU DESSUS PAR CA MAIS CA MARCHE pas
 *
int filter_trains(Train trains[], int nb_train, Train **trains_filtered, Request req) {

    // Nombre de Trains qui valident les critères de recherche (aucun au début)
    int nb_train_filtered = 0;
    // Tableau de Trains qui valident les critères de recherche (vide pour l'instant)
    Train *t = NULL;

    for(int i = 0; i < nb_train; i++){

        // Si le Train n° i valide les critères de filtrage
        if(1 || check_train(trains[i], req) == 1){

            // On agrandit le tableau de Train, puis on fait pointer le pointeur passé en paramètre vers celui ci
            *trains_filtered = (Train *)realloc(t, sizeof(Train) * (nb_train_filtered + 1));
            // On copie le train qui valide les critères de recherche à la fin du tableau de Train
            memcpy(trains_filtered[nb_train_filtered], &trains[i], sizeof(Train));

            nb_train_filtered++;
        }
    }
    return nb_train_filtered;
}

int check_train(Train train, Request req) {

    if(strcmp(train.city_from, req.city_from) != 0)
        return 0;

    if(strcmp(train.city_to, req.city_to) != 0)
        return 0;


    // Si le premier horaire de la plage horaire de la requête est valide
    if (req.time_from_1.hour < 24 && req.time_from_1.minute < 60) {

        // Si on cherche un train le + tôt possible à partir d'un horaire
        if (timecmp(req.time_from_1, req.time_from_2) == 0) {
            // Si le train est trop tôt, on retourne faux
            if (timecmp(train.time_from, req.time_from_1) == -1)
                return 0;
        }
        // Sinon on cherche un train dans la plage horaire
        // Si le train est en dehors de la plage horaire, on retourne faux
        if (timecmp(train.time_from, req.time_from_1) == -1 || timecmp(train.time_from, req.time_from_2) == 1)
            return 0;
    }

    if(train.time_from.hour != req.time_from_1.hour && train.time_from.minute != req.time_from_2.minute)
        return 0;


    return 1;
}
*/

/**
 * Compare 2 structures Time
 * @param t1 Time 1er temps comparé
 * @param t2 Time 2e temps comparé
 * @return -1 si t1 < t2 (plus tôt), 0 si t1 == t2 (même heure), 1 si t1 > t2 (plus tard)
 */
int timecmp(Time t1, Time t2) {

    if (t1.hour == t2.hour) {

        if (t1.minute == t2.minute)
            return 0;

        return t1.minute > t2.minute ? 1 : -1;
    }
    return t1.hour > t2.hour ? 1 : -1;
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