#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../headers/commons.h"

/**
 * Compte le nombre de trains dans un fichier CSV.
 * 
 * @param filename Le nom du fichier à lire.
 * @return Le nombre de lignes (trains) dans le fichier.
 */
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

/**
 * Ajoute une nouvelle ville dans le tableau de villes si elle n'y est pas déjà.
 * 
 * @param cities Tableau contenant les villes.
 * @param city_count Nombre actuel de villes dans le tableau.
 * @param city La ville à ajouter.
 */
void add_city_if_new(char cities[MAX_CITIES][MAX_CITY_NAME_LENGTH], int *city_count, const char *city) {
    for (int i = 0; i < *city_count; i++) {
        if (strcmp(cities[i], city) == 0) {
            return; 
        }
    }

    if (*city_count < MAX_CITIES) {
        strncpy(cities[*city_count], city, MAX_CITY_NAME_LENGTH);
        cities[*city_count][MAX_CITY_NAME_LENGTH - 1] = '\0'; // Assure la terminaison par un caractère nul
        (*city_count)++;
    } else {
        fprintf(stderr, "Limite de villes atteinte, impossible d'ajouter plus de villes.\n");
    }
}

/**
 * Lit les informations des trains depuis un fichier CSV et les stocke dans un tableau.
 * 
 * @param filename Nom du fichier CSV à lire.
 * @param trains Tableau de trains à remplir.
 * @param maxTrains Nombre maximum de trains à lire.
 * @param cities Tableau de villes à remplir.
 * @param city_count Nombre de villes dans le tableau.
 * @return Le nombre de trains lus.
 */
int read_trains_from_file(char *filename, Train *trains, int maxTrains, char cities[MAX_CITIES][MAX_CITY_NAME_LENGTH], int *city_count) {

    FILE *file = fopen(filename, "r");

    if (file == NULL) {
        perror("Erreur lors de l'ouverture du fichier");
        return 0;
    }

    char line[256];
    int i = 0;
    while (fgets(line, sizeof(line), file) && i < maxTrains) {
        
        char *token;
        char *rest = line;

        token = strtok_r(rest, ";", &rest); // ID
        trains[i].id = atoi(token);

        token = strtok_r(NULL, ";", &rest); // Ville de départ
        strcpy(trains[i].city_from, token);
        add_city_if_new(cities, city_count, trains[i].city_from);

        token = strtok_r(NULL, ";", &rest); // Ville d'arrivée
        strcpy(trains[i].city_to, token);
        add_city_if_new(cities, city_count, trains[i].city_to);

        token = strtok_r(NULL, ":", &rest); // Heure de départ (heure)
        trains[i].time_from.hour = atoi(token);

        token = strtok_r(NULL, ";", &rest); // Heure de départ (minutes)
        trains[i].time_from.minute = atoi(token);

        token = strtok_r(NULL, ":", &rest); // Heure d'arrivée (heure)
        trains[i].time_to.hour = atoi(token);

        token = strtok_r(NULL, ";", &rest); // Heure d'arrivée (minutes)
        trains[i].time_to.minute = atoi(token);

        token = strtok_r(NULL, ";", &rest); // Prix
        trains[i].price = strtod(token, NULL);

        token = strtok_r(NULL, "\n", &rest); // Réduction/Supplément
        if (token != NULL) {
            if (strcmp(token, "REDUC") == 0) {
                trains[i].reduc = 1;
            } else if (strcmp(token, "SUPPL") == 0) {
                trains[i].suppl = 1;
            }
        } else {
            trains[i].reduc = 0;
            trains[i].suppl = 0;
        }

        i++;
    }

    fclose(file);
    return i; // Nombre de trains lus
}
