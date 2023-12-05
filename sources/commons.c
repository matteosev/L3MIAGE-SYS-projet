#include <unistd.h>
#include <stdio.h>

#include "../headers/commons.h"

void print_train(Train *train){

    printf("id: %d\n",train->id);
    printf("city_from_train_came: %s\n",train->city_from);
    printf("city_that_train_will_go_to: %s\n",train->city_to);
    printf("Time_From: %02d:%02d\n", train->time_from.hour, train->time_from.minute);
    printf("Time_To: %02d:%02d\n", train->time_to.hour, train->time_to.minute);
    printf("the_price_is: %.2lf\n",train->price);
    printf("the_reduc: %d\n",train->reduc);
    printf("the_supp: %d\n",train->suppl);

}
