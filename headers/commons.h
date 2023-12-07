/**
 * \file commons.h
 * \brief Structures
 */

#ifndef __COMMONS_H__
#define __COMMONS_H__

#define MAX_CITIES 250
#define MAX_CITY_NAME_LENGTH 100

/**
 * Type de requête
 */
typedef enum 
{
    HORAIRE = 1,    /** les trains à partir d'un horaire **/
    PLAGE = 2,      /** les trains dans une plage horaire **/
    JOURNEE = 3,    /** les trains de toute la journée **/
    FIN = 4         /** fermer la connexion **/

} Request_type;

/**
 * \struct Time
 * \brief Modélise un horaire en heures et minutes
 */
typedef struct  {
    int hour;
    int minute;
} Time;

/**
 * \struct Train
 * \brief Modélise un train
 */
typedef struct {
    int id;
    char city_from[100];
    char city_to[100];
    Time time_from;
    Time time_to;
    double price;
    int reduc;
    int suppl;
} Train;

/**
 * \struct Request
 * \brief Modélise une requête envoyée par le client
 */
typedef struct {
    Request_type type;
    char city_from[100];
    char city_to[100];
    Time time_from_1;
    Time time_from_2;
} Request;

void print_train(Train t);
void print_request(Request r);

#endif