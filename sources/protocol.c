#include <unistd.h>
#include <stdio.h>

#include "../headers/commons.h"

void send_train(int socket, Train train){

    train.price *= train.reduc == 1 ? 0.8 : 1;
    train.price *= train.suppl == 1 ? 1.1 : 1;

    if (
        write(socket, &train.id, sizeof(train.id)) == -1 ||
        write(socket, &train.city_from, sizeof(train.city_from)) == -1 ||
        write(socket, &train.city_to, sizeof(train.city_to)) == -1 ||
        write(socket, &train.time_from.hour, sizeof(train.time_from.hour)) == -1 ||
        write(socket, &train.time_from.minute, sizeof(train.time_from.minute)) == -1 ||
        write(socket, &train.time_to.hour, sizeof(train.time_from.hour)) == -1 ||
        write(socket, &train.time_to.minute, sizeof(train.time_from.minute)) == -1 ||
        write(socket, &train.price, sizeof(train.price)) == -1 ||
        write(socket, &train.reduc, sizeof(train.reduc)) == -1 ||
        write(socket, &train.suppl, sizeof(train.suppl)) == -1
    )

        perror("Erreur envoi train");
}

void receive_train(int socket, Train *train){

    if(
        read(socket, &train->id, sizeof(train->id)) <= 0 ||
        read(socket, &train->city_from, sizeof(train->city_from)) <= 0 ||
        read(socket, &train->city_to, sizeof(train->city_to)) <= 0 ||
        read(socket, &train->time_from.hour, sizeof(train->time_from.hour)) <= 0 ||
        read(socket, &train->time_from.minute, sizeof(train->time_from.minute)) <= 0 ||
        read(socket, &train->time_to.hour, sizeof(train->time_from.hour)) <= 0 ||
        read(socket, &train->time_to.minute, sizeof(train->time_from.minute)) <= 0 ||
        read(socket, &train->price, sizeof(train->price)) <= 0 ||
        read(socket, &train->reduc, sizeof(train->reduc)) <= 0 ||
        read(socket, &train->suppl, sizeof(train->suppl)) <= 0
    )

        perror("Erreur réception train");
}

void send_request(int socket, Request query) {

    int type = (int)query.type;
    if(
        write(socket, &type, sizeof(type))  == -1 ||
        write(socket, &query.city_from, sizeof(query.city_from))  == -1 ||
        write(socket, &query.city_to, sizeof(query.city_to))  == -1 ||
        write(socket, &query.time_from_1.minute, sizeof(int))  == -1 ||
        write(socket, &query.time_from_1.hour, sizeof(int))  == -1 ||
        write(socket, &query.time_from_2.minute, sizeof(int))  == -1 ||
        write(socket, &query.time_from_2.hour, sizeof(int))  == -1
    )
        perror("Erreur envoi requête");
}   

void receive_request(int socket, Request *query) {

    if(
        read(socket, &query->type, sizeof(query->type)) <= 0 ||
        read(socket, &query->city_from, sizeof(query->city_from)) <= 0 ||
        read(socket, &query->city_to, sizeof(query->city_to)) <= 0 ||
        read(socket, &query->time_from_1.minute, sizeof(int)) <= 0 ||
        read(socket, &query->time_from_1.hour, sizeof(int)) <= 0 ||
        read(socket, &query->time_from_2.minute, sizeof(int)) <= 0 ||
        read(socket, &query->time_from_2.hour, sizeof(int)) <= 0
    )
        perror("Erreur reception requête");
}