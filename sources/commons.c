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

void print_train(Train t) {
    printf("{ id=%d city_from=%s city_to=%s time_from=%02d:%02d time_to=%02d:%02d price=%.2f reduc=%d suppl=%d }\n", t.id, t.city_from, t.city_to, t.time_from.hour, t.time_from.minute, t.time_to.hour, t.time_to.minute, t.price, t.reduc, t.suppl);
}

void print_request(Request r) {
    printf("{ city_from=%s city_to=%s time_from_1=%02d:%02d time_from_2=%02d:%02d type=%d }\n", r.city_from, r.city_to, r.time_from_1.hour, r.time_from_1.minute, r.time_from_2.hour, r.time_from_2.minute, r.type);
}

void display_train(Train train){

    printf("**********************************");
    printf("\n");
    printf("Train n°%d\n", train.id);
    printf("Destination : %s\n", train.city_to);
    printf("Heure de départ : %d:%d\n", train.time_from.hour, train.time_from.minute);
    printf("Heure d'arrivé : %d:%d\n", train.time_to.hour, train.time_to.minute);
    if(train.suppl == 1){

        printf("Prix du billet : %.2f euros (tarif augmenté)\n", train.price);

    }else{

        if(train.reduc == 1){

            printf("Prix du billet : %.2f euros (tarif augmenté)\n", train.price);

        }else{

            printf("Prix du billet : %.2f euros (tarif normal)\n", train.price);
        }
    }

    printf("\n");
    printf("**********************************");
}

Time time_difference(Time t1,Time t2){

    Time ret;
    ret.hour = t1.hour > t2.hour ? t1.hour-t2.hour : t2.hour-t1.hour;
    ret.minute = t1.minute > t2.minute ? t1.minute-t2.minute : t2.minute-t1.minute;
    return ret;
}

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

void send_train(int socket, Train train){

    train.price *= train.reduc == 1 ? 0.8 : 1;
    train.price *= train.suppl == 1 ? 1.1 : 1;

    if (
        write(socket, &train.id, sizeof(train.id)) == -1 ||
        write(socket, &train.city_from, sizeof(train.city_from)) == -1 ||
        write(socket, &train.city_to, sizeof(train.city_to)) == -1 ||
        write(socket, &train.time_from.hour, sizeof(train.time_from.hour)) == -1 ||
        write(socket, &train.time_from.minute, sizeof(train.time_from.minute)) == -1 ||
        write(socket, &train.time_to.hour, sizeof(train.time_from.hour)) == -1 ||
        write(socket, &train.time_to.minute, sizeof(train.time_from.minute)) == -1 ||
        write(socket, &train.price, sizeof(train.price)) == -1 ||
        write(socket, &train.reduc, sizeof(train.reduc)) == -1 ||
        write(socket, &train.suppl, sizeof(train.suppl)) == -1
    )

        perror("Erreur envoi train");
}

void receive_train(int socket, Train *train){

    if(
        read(socket, &train->id, sizeof(train->id)) <= 0 ||
        read(socket, &train->city_from, sizeof(train->city_from)) <= 0 ||
        read(socket, &train->city_to, sizeof(train->city_to)) <= 0 ||
        read(socket, &train->time_from.hour, sizeof(train->time_from.hour)) <= 0 ||
        read(socket, &train->time_from.minute, sizeof(train->time_from.minute)) <= 0 ||
        read(socket, &train->time_to.hour, sizeof(train->time_from.hour)) <= 0 ||
        read(socket, &train->time_to.minute, sizeof(train->time_from.minute)) <= 0 ||
        read(socket, &train->price, sizeof(train->price)) <= 0 ||
        read(socket, &train->reduc, sizeof(train->reduc)) <= 0 ||
        read(socket, &train->suppl, sizeof(train->suppl)) <= 0
    )

        perror("Erreur réception train");
}

void send_request(int socket, Request query) {

    int type = (int)query.type;
    if(
        write(socket, &type, sizeof(type))  == -1 ||
        write(socket, &query.city_from, sizeof(query.city_from))  == -1 ||
        write(socket, &query.city_to, sizeof(query.city_to))  == -1 ||
        write(socket, &query.time_from_1.minute, sizeof(int))  == -1 ||
        write(socket, &query.time_from_1.hour, sizeof(int))  == -1 ||
        write(socket, &query.time_from_2.minute, sizeof(int))  == -1 ||
        write(socket, &query.time_from_2.hour, sizeof(int))  == -1
    )
        perror("Erreur envoi requête");
}   

void receive_request(int socket, Request *query) {

    if(
        read(socket, &query->type, sizeof(query->type)) <= 0 ||
        read(socket, &query->city_from, sizeof(query->city_from)) <= 0 ||
        read(socket, &query->city_to, sizeof(query->city_to)) <= 0 ||
        read(socket, &query->time_from_1.minute, sizeof(int)) <= 0 ||
        read(socket, &query->time_from_1.hour, sizeof(int)) <= 0 ||
        read(socket, &query->time_from_2.minute, sizeof(int)) <= 0 ||
        read(socket, &query->time_from_2.hour, sizeof(int)) <= 0
    )
        perror("Erreur reception requête");
}

/**
 * Extraie les trains qui valident les critères de filtrage.
 * @param trains Train[] Le tableau de trains qui doit être filtré
 * @param nb_train int La taille du tableau trains
 * @param trains_filtered Train ** Un pointeur vers le pointeur qui pointera vers le tableau de trains filtré
 * @param req Request Les critères de filtrage
 * @return La taille du tableau de trains filtré
 */
int filter_train_from_array(Train trains[], int nb_train, Train **trains_filtered, Request req){

    // Nombre de Trains qui valident les critères de recherche (aucun au début)
    int nb_train_filtered = 0;
    // Tableau de Trains qui valident les critères de recherche (vide pour l'instant)
    *trains_filtered = NULL;

    for(int i = 0; i < nb_train; i++){
        // Si le Train n° i valide les critères de filtrage
        if(check_filter(trains[i], req) == 1){

            // On agrandit le tableau de Train, puis on pointe vers celui ci
            *trains_filtered = realloc(*trains_filtered, sizeof(Train) * (nb_train_filtered + 1));

            if (*trains_filtered == NULL)
                perror("realloc");

            // On copie le train qui valide les critères de recherche à la fin du tableau de Train
            memcpy(&(*trains_filtered)[nb_train_filtered], &trains[i], sizeof(Train));

            nb_train_filtered++;
        }
    }

    if(req.type == HORAIRE){

        Train *result = NULL;
        nb_train_filtered= soonest_train(*trains_filtered, &result, nb_train_filtered);
        *trains_filtered = result;
    }

    return nb_train_filtered;
}

int soonest_train(Train trains[], Train **filter_result, int nb_train){

    Time time_cmp = {23, 59};
    *filter_result = NULL;
    int nb_train_filtered = 0;

    for(int i = 0; i < nb_train; i++){

        // Heure plus tôt ou égale
        if(timecmp(trains[i].time_from, time_cmp) <= 0){

            // On agrandit le tableau de Train, puis on pointe vers celui ci
            *filter_result = realloc(*filter_result, sizeof(Train) * (nb_train_filtered + 1));

            if (*filter_result == NULL)
                perror("realloc");

            // On copie le train qui valide les critères de recherche à la fin du tableau de Train
            memcpy(&(*filter_result)[nb_train_filtered], &trains[i], sizeof(Train));
            memcpy(&time_cmp, &trains[i].time_from, sizeof(Time));
            nb_train_filtered++;
        }
    }

    return nb_train_filtered;
}

int check_filter(Train train, Request req){

    if(strcmp(train.city_from, req.city_from) != 0)
        return 0;

    if(strcmp(train.city_to, req.city_to) != 0)
        return 0;

    switch (req.type) {

        case HORAIRE:

            if ( timecmp(train.time_from, req.time_from_1) == -1)
                return 0;

            break;
        
        case PLAGE:

            if ( timecmp(train.time_from, req.time_from_1) == -1 || timecmp(train.time_from, req.time_from_2) == 1)
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