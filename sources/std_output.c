#include <stdio.h>

#include "../headers/commons.h"

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