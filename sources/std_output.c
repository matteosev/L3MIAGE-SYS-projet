#include <stdio.h>
#include <string.h>

#include "../headers/commons.h"
#include "../headers/time.h"

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

void train_number_display(int nb_train){

    switch (nb_train){
        case 0 :
            printf("Aucun train satisfaisant votre demande\n");
            break;
        case 1 :
            printf("%d train satisfaisant votre demande\n", nb_train);
            break;
        default :
            printf("%d trains satisfaisant votre demande\n", nb_train);
    } 
}

void fastest_or_cheapest_train(int nb_train, Train trains[], char rep){

    switch (rep){

        case 'y' :
    
            Time time_cmp = {23,59};
            Train *fastest;

            for(int i = 0; i < nb_train; i++){

                Time duration = time_difference(trains[i].time_from, trains[i].time_to);

                if(timecmp(duration, time_cmp)==-1){
                    memcpy(&time_cmp, &duration, sizeof(Time));
                    fastest=&trains[i];
                }
            }
            printf("\n");
            printf("Vous avez choisi le train le plus rapide\n");
            display_train(*fastest);
            break;

        case 'n' :

            double price_min = 99999.9999;
            Train * cheapest;

            for(int i =0; i<nb_train;i++){

                if(trains[i].price < price_min){

                    price_min=trains[i].price;
                    cheapest=&trains[i];
                }
            }
            printf("\n");
            printf("Vous avez choisi le train moins cher\n");
            display_train(*cheapest);
            break;
    }
}

int choices_display() {

    int choix = -2;

    do {
        printf("------------------------------------------------------------\n");
        printf("Chercher, entre 2 villes ...\n");
        printf("%d. LE train disponible le + tôt à partir d'un horaire\n", HORAIRE);
        printf("%d. TOUS les trains dans une plage horaire\n", PLAGE);
        printf("%d. TOUS les trains de la journée\n", JOURNEE);
        printf("%d. RIEN, bye\n", FIN);
        printf("------------------------------------------------------------\n");

        if (scanf("%d", &choix) == 1 && choix > 0 && choix < 5) {
            return choix;

        } else {

            printf("Merci d'entrer une option possible\n");
            while (getchar() != '\n');
        }

    } while (1);

    return -1;
}

void display_cities(char cities[250][100], int nb_city){

    for(int i = 0; i < nb_city; i++){
        printf("%d. %s\n", i+1, cities[i]);
    }
}