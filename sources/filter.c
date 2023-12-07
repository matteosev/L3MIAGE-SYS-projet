/**
 * \file filter.c
 * \brief Fonctions de filtrage d'un tableau de trains
 *
 * 
 *
 */

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "../headers/commons.h"
#include "../headers/time.h"

/**
 * Vérifie si un train correspond aux critères de filtrage d'une requête.
 * 
 * @param train Le train à vérifier.
 * @param req La requête contenant les critères de filtrage.
 * @return 1 si le train correspond aux critères, 0 sinon.
 */
int check_filter(Train train, Request req){

    // Vérifie si la ville de départ correspond
    if(strcmp(train.city_from, req.city_from) != 0)
        return 0;

    // Vérifie si la ville d'arrivée correspond
    if(strcmp(train.city_to, req.city_to) != 0)
        return 0;

    switch (req.type) {

        case HORAIRE:
            // Vérifie si l'heure de départ du train est après l'heure spécifiée
            if (timecmp(train.time_from, req.time_from_1) == -1)
                return 0;
            break;
        
        case PLAGE:
            // Vérifie si l'heure de départ du train est dans la plage horaire spécifiée
            if (timecmp(train.time_from, req.time_from_1) == -1 || timecmp(train.time_from, req.time_from_2) == 1)
                return 0;
            break;

        case JOURNEE:
            // Pas de critère d'heure spécifique pour le filtrage journalier
            break;

        case FIN:
            // Cas spécifique 'FIN', pas de filtrage
            break;
        
        default:
            // Type de requête non reconnu
    }
    
    return 1; // Le train correspond aux critères
}

/**
 * Sélectionne le train le plus proche dans le temps parmi une liste de trains.
 * 
 * @param trains Tableau de trains à examiner.
 * @param filter_result Pointeur vers le tableau résultant.
 * @param nb_train Nombre de trains dans le tableau original.
 * @return Nombre de trains dans le tableau résultant.
 */
int soonest_train(Train trains[], Train **filter_result, int nb_train){

    Time time_cmp = {23, 59}; // Heure de comparaison initiale (très tardive)
    *filter_result = NULL;
    int nb_train_filtered = 0;

    for(int i = 0; i < nb_train; i++){

        // Si le train part plus tôt ou à la même heure que time_cmp
        if(timecmp(trains[i].time_from, time_cmp) <= 0){

            // Agrandissement du tableau de résultat
            *filter_result = realloc(*filter_result, sizeof(Train) * (nb_train_filtered + 1));

            if (*filter_result == NULL)
                perror("realloc");

            // Copie du train dans le tableau de résultat
            memcpy(&(*filter_result)[nb_train_filtered], &trains[i], sizeof(Train));
            memcpy(&time_cmp, &trains[i].time_from, sizeof(Time));
            nb_train_filtered++;
        }
    }

    return nb_train_filtered; // Nombre de trains dans le tableau résultant
}

/**
 * Filtre les trains selon les critères d'une requête.
 * 
 * @param trains Le tableau initial de trains.
 * @param nb_train La taille du tableau initial.
 * @param trains_filtered Pointeur vers le tableau de trains filtrés.
 * @param req La requête de filtrage.
 * @return Le nombre de trains dans le tableau filtré.
 */
int filter_train_from_array(Train trains[], int nb_train, Train **trains_filtered, Request req){

    int nb_train_filtered = 0;
    *trains_filtered = NULL;

    for(int i = 0; i < nb_train; i++){
        if(check_filter(trains[i], req) == 1){

            *trains_filtered = realloc(*trains_filtered, sizeof(Train) * (nb_train_filtered + 1));

            if (*trains_filtered == NULL)
                perror("realloc");

            memcpy(&(*trains_filtered)[nb_train_filtered], &trains[i], sizeof(Train));
            nb_train_filtered++;
        }
    }

    if(req.type == HORAIRE){
        Train *result = NULL;
        nb_train_filtered = soonest_train(*trains_filtered, &result, nb_train_filtered);
        *trains_filtered = result;
    }

    return nb_train_filtered;
}
