#include <unistd.h>
#include <stdio.h>

#include "../headers/commons.h"

void print_train(Train t) {
    printf("{ id=%d city_from=%s city_to=%s time_from=%02d:%02d time_to=%02d:%02d price=%.2f reduc=%d suppl=%d }\n", t.id, t.city_from, t.city_to, t.time_from.hour, t.time_from.minute, t.time_to.hour, t.time_to.minute, t.price, t.reduc, t.suppl);
}

void print_request(Request r) {
    printf("{ city_from=%s city_to=%s time_from_1=%02d:%02d time_from_2=%02d:%02d type=%d }\n", r.city_from, r.city_to, r.time_from_1.hour, r.time_from_1.minute, r.time_from_2.hour, r.time_from_2.minute, r.type);
}

void display_train(Train train){

    printf("\n");
    printf("**********************************");
    printf("\n");
    printf("Train n°%d\n", train.id);
    printf("Destination : %s\n", train.city_to);
    printf("Heure de départ : %d:%d\n", train.time_from.hour, train.time_from.minute);
    printf("Heure d'arrivé : %d:%d\n", train.time_to.hour, train.time_to.minute);
    printf("Prix du billet : %.2f euros\n", train.price);
    printf("\n");
    printf("**********************************");
    printf("\n");
}

Time time_difference(Time t1,Time t2){
    Time ret;
    ret.hour=t1.hour>t2.hour ? t1.hour-t2.hour : t2.hour-t1.hour;
    ret.minute=t1.minute>t2.minute ? t1.minute-t2.minute : t2.minute-t1.minute;
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

    write(socket, &train.id, sizeof(train.id));
    write(socket, &train.city_from, sizeof(train.city_from));
    write(socket, &train.city_to, sizeof(train.city_to));
    write(socket, &train.time_from.hour, sizeof(train.time_from.hour));
    write(socket, &train.time_from.minute, sizeof(train.time_from.minute));
    write(socket, &train.time_to.hour, sizeof(train.time_from.hour));
    write(socket, &train.time_to.minute, sizeof(train.time_from.minute));
    write(socket, &train.price, sizeof(train.price));
    write(socket, &train.reduc, sizeof(train.reduc));
    write(socket, &train.suppl, sizeof(train.suppl));
}

void receive_train(int socket,Train *train){

    read(socket, &train->id, sizeof(train->id));
    read(socket, &train->city_from, sizeof(train->city_from));
    read(socket, &train->city_to, sizeof(train->city_to));
    read(socket, &train->time_from.hour, sizeof(train->time_from.hour));
    read(socket, &train->time_from.minute, sizeof(train->time_from.minute));
    read(socket, &train->time_to.hour, sizeof(train->time_from.hour));
    read(socket, &train->time_to.minute, sizeof(train->time_from.minute));
    read(socket, &train->price, sizeof(train->price));
    read(socket, &train->reduc, sizeof(train->reduc));
    read(socket, &train->suppl, sizeof(train->suppl));
}
 Request read_request(int socket) {
    Request  query;
    read(socket, &query.type, sizeof(query.type));
    read(socket, &query.city_from, sizeof(query.city_from));
    read(socket, &query.city_to, sizeof(query.city_to));
    read(socket, &query.time_from_1.minute, sizeof(int));
    read(socket, &query.time_from_1.hour, sizeof(int));
    read(socket, &query.time_from_2.minute, sizeof(int));
    read(socket, &query.time_from_2.hour, sizeof(int));
    return query;
}
void write_request(int socket,Request query) {
    int type = (int)query.type; 
    write(socket, &type, sizeof(type));
    write(socket, &query.city_from, sizeof(query.city_from));
    write(socket, &query.city_to, sizeof(query.city_to));
    write(socket, &query.time_from_1.minute, sizeof(int));
    write(socket, &query.time_from_1.hour, sizeof(int));
    write(socket, &query.time_from_2.minute, sizeof(int));
    write(socket, &query.time_from_2.hour, sizeof(int));

}
