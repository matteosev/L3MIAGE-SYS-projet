#include <unistd.h>
#include <stdio.h>

#include "../headers/commons.h"

/*
void write_size_then_msg(int socket, void *msg, size_t size) {

    write(socket, &size, sizeof(size));
    write(socket, msg, size);
}

void read_size_then_msg(int socket, void *msg) {

    size_t size;
    read(socket, &size, sizeof(size));
    read(socket, msg, size);
}
*/

void print_train(Train *train){
    printf("id: %d\n",train->id);
    printf("city_from_train_came: %d\n",train->city_from);
    printf("city_that_train_will_go_to: %d\n",train->city_to);
    printf("Time_From: %02d:%02d\n", train->time_from.hour, train->time_from.minute);
    printf("Time_To: %02d:%02d\n", train->time_to.hour, train->time_to.minute);
    printf("the_price_is: %lf\n",train->price);
    printf("the_reduc: %d\n",train->reduc);
    printf("the_supp: %d\n",train->suppl);

}

City char_to_city(char c){

    switch(c){

        case 'P' :

            return PARIS;

        case 'G' :

            return GRENOBLE;

        case 'V' :

            return VALENCE;

        case 'M' : 

            return MONTELIMAR;

        default : 

            return PARIS;
    }
}