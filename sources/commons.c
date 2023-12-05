#include <unistd.h>
#include <stdio.h>

#include "../headers/commons.h"

void print_train(Train *train){
    
    printf("id: %d\n",train->id);
    printf("city_from_train_came: %s\n",train->city_from);
    printf("city_that_train_will_go_to: %s\n",train->city_to);
    printf("Time_From: %02d:%02d\n", train->time_from.hour, train->time_from.minute);
    printf("Time_To: %02d:%02d\n", train->time_to.hour, train->time_to.minute);
    printf("the_price_is: %lf\n",train->price);
    printf("the_reduc: %d\n",train->reduc);
    printf("the_supp: %d\n",train->suppl);

}



int readTrainsFromFile(const char *filename, Train *trains, int maxTrains) {
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
        strcpy(trains[i].city_from,token);


        token = strtok_r(NULL, ";", &rest); // City To
        strcpy(trains[i].city_to,token);


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
        }

        i++;
    }

    fclose(file);
    return i; // Number of trains read
}
