#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../headers/commons.h"
#include "../headers/time.h"

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

            // On copie le train qui valide les critères de filtrage à la fin du tableau de Train
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
