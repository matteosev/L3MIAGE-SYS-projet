#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "../headers/commons.h"

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

int read_trains_from_file(char *filename, Train *trains, int maxTrains, char cities[MAX_CITIES][MAX_CITY_NAME_LENGTH], int *city_count) {

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

        add_city_if_new(cities, city_count, trains[i].city_from);


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



void add_city_if_new(char cities[MAX_CITIES][MAX_CITY_NAME_LENGTH], int *city_count, const char *city) {
    for (int i = 0; i < *city_count; i++) {
        if (strcmp(cities[i], city) == 0) {
            return; 
        }
    }

    
    if (*city_count < MAX_CITIES) {
        strncpy(cities[*city_count], city, MAX_CITY_NAME_LENGTH);
        cities[*city_count][MAX_CITY_NAME_LENGTH - 1] = '\0'; // Ensure null termination
        (*city_count)++;
    } else {
        fprintf(stderr, "City limit reached, unable to add more cities.\n");
    }
}
