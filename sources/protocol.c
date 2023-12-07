/**
 * \file protocol.c
 * \brief Protocole d'échange de données
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../headers/commons.h"


/**
 * Envoie un train sur un socket.
 * 
 * @param socket Le socket sur lequel envoyer le train.
 * @param train Le train à envoyer.
 */
void send_train(int socket, Train train) {
    
    // Applique les réductions et suppléments sur le prix du train

    train.price *= train.reduc == 1 ? 0.8 : 1;
    train.price *= train.suppl == 1 ? 1.1 : 1;

    // Calcule la longueur des noms de ville pour l'envoi

    int city_from_length = strlen(train.city_from) ;
    int city_to_length = strlen(train.city_to) ;

    // Envoie les données du train sur le socket
    // En cas d'erreur, affiche un message d'erreur

    if (
        write(socket, &train.id, sizeof(train.id)) == -1 ||
        write(socket, &city_from_length, sizeof(int)) == -1 ||
        write(socket, &train.city_from, sizeof(char) * city_from_length) == -1 ||
        write(socket, &city_to_length, sizeof(int)) == -1 ||
        write(socket, &train.city_to, sizeof(char) * city_to_length) == -1 ||
        write(socket, &train.time_from.hour, sizeof(train.time_from.hour)) == -1 ||
        write(socket, &train.time_from.minute, sizeof(train.time_from.minute)) == -1 ||
        write(socket, &train.time_to.hour, sizeof(train.time_to.hour)) == -1 ||
        write(socket, &train.time_to.minute, sizeof(train.time_to.minute)) == -1 ||
        write(socket, &train.price, sizeof(train.price)) == -1 ||
        write(socket, &train.reduc, sizeof(train.reduc)) == -1 ||
        write(socket, &train.suppl, sizeof(train.suppl)) == -1
    ) {
        perror("Erreur envoi train");
    }
}

/**
 * Reçoit un train depuis un socket.
 * 
 * @param socket Le socket depuis lequel recevoir le train.
 * @param train Pointeur vers le train à remplir avec les données reçues.
 */

void receive_train(int socket, Train *train) {
    int city_from_length, city_to_length;
    memset(&train->city_from, 0, sizeof(train->city_from));
    memset(&train->city_to, 0, sizeof(train->city_to));
    // Lit les données du train depuis le socket
    // En cas d'erreur, affiche un message d'erreur et retourne
    if (
        read(socket, &train->id, sizeof(train->id)) <= 0 ||
        read(socket, &city_from_length, sizeof(int)) <= 0 ||
        read(socket, &train->city_from, sizeof(char) * city_from_length) <= 0 ||
        read(socket, &city_to_length, sizeof(int)) <= 0 ||
        read(socket, &train->city_to, sizeof(char) * city_to_length) <= 0 ||
        read(socket, &train->time_from.hour, sizeof(train->time_from.hour)) <= 0 ||
        read(socket, &train->time_from.minute, sizeof(train->time_from.minute)) <= 0 ||
        read(socket, &train->time_to.hour, sizeof(train->time_to.hour)) <= 0 ||
        read(socket, &train->time_to.minute, sizeof(train->time_to.minute)) <= 0 ||
        read(socket, &train->price, sizeof(train->price)) <= 0 ||
        read(socket, &train->reduc, sizeof(train->reduc)) <= 0 ||
        read(socket, &train->suppl, sizeof(train->suppl)) <= 0
    ) {
        perror("Erreur réception train");
        return;
    }

}

/**
 * Envoie une requête sur un socket.
 * 
 * @param socket Le socket sur lequel envoyer la requête.
 * @param query La requête à envoyer.
 */

void send_request(int socket, Request query) {
    // Envoie les données de la requête sur le socket
    // En cas d'erreur, affiche un message d'erreur

    int type = (int)query.type;
    int city_from_length = strlen(query.city_from) ;
    int city_to_length = strlen(query.city_to) ;
    if(
        write(socket, &type, sizeof(type))  == -1 ||
        write(socket, &city_from_length, sizeof(city_from_length)) == 1 ||
        write(socket, &query.city_from, sizeof(char) * city_from_length)  == -1 ||
        write(socket, &city_to_length, sizeof(city_to_length)) == 1 ||
        write(socket, &query.city_to, sizeof(char) * city_to_length)  == -1 ||
        write(socket, &query.time_from_1.minute, sizeof(int))  == -1 ||
        write(socket, &query.time_from_1.hour, sizeof(int))  == -1 ||
        write(socket, &query.time_from_2.minute, sizeof(int))  == -1 ||
        write(socket, &query.time_from_2.hour, sizeof(int))  == -1
    )
        perror("Erreur envoi requête");
}   


/**
 * Reçoit une requête depuis un socket.
 * 
 * @param socket Le socket depuis lequel recevoir la requête.
 * @param query Pointeur vers la requête à remplir avec les données reçues.
 */
void receive_request(int socket, Request *query) {
    // Lit les données de la requête depuis le socket
    // En cas d'erreur, affiche un message d'erreur
    memset(&query->city_from, 0, sizeof(query->city_from));
    memset(&query->city_to, 0, sizeof(query->city_to));
    int city_from_length, city_to_length;
    if(
        read(socket, &query->type, sizeof(query->type)) <= 0 ||
        read(socket, &city_from_length, sizeof(int)) <= 0 ||
        read(socket, &query->city_from, sizeof(char) * city_from_length) <= 0 ||
        read(socket, &city_to_length, sizeof(int)) <= 0 ||
        read(socket, &query->city_to, sizeof(char) * city_to_length) <= 0 ||
        read(socket, &query->time_from_1.minute, sizeof(int)) <= 0 ||
        read(socket, &query->time_from_1.hour, sizeof(int)) <= 0 ||
        read(socket, &query->time_from_2.minute, sizeof(int)) <= 0 ||
        read(socket, &query->time_from_2.hour, sizeof(int)) <= 0
    )
        perror("Erreur reception requête");
}