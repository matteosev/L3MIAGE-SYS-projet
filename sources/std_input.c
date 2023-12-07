#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../headers/commons.h"
#include "../headers/std_input.h"

/**
 * Permet de s'assurer de la saisie de l'utilisateur

 * @param nb_train Nombre de trains dans le tableau original.
 * @return Réponse 'y' ou 'n'
 */
char yes_or_no_verification(int nb_train) {

    char rep;
    printf("\n");
    printf("%d options sont disponibles\n", nb_train);
    printf("Appuyez sur 'y' pour le plus rapide ou sur 'n' pour le moins cher :");
    scanf(" %c", &rep);

    // Utiliser un boucle pour s'assurer que l'utilisateur entre 'y' ou 'n'
    while (rep != 'y' && rep != 'n') {

        scanf(" %c", &rep); // Utiliser un espace dans le format de scanf pour consommer les caractères de nouvelle ligne
        printf("Merci de simple saisir y ou n \n");
        
        // Ignorer les caractères restants dans le flux d'entrée jusqu'à la prochaine nouvelle ligne
        while (getchar() != '\n') {/*Ignore les caractères restants*/}
    }
    return rep;
}

/**
 * Permet de s'assurer de 3 points :
 *   - de la présence d'une chaine de caractères uniquement
 *   - si oui, mise au format Ville peut importe l'entrée
 *   - enfin de s'assurer de la présence de la ville dans la liste des villes disponibles
 *
 * @param input_city pointeur vers la ville à comparer
 * @param cities Tableau de villes à examiner.
 * @return Si la ville est dans le tableau (1) sinon (0)
 */
int cities_check(char *input_city, char cities[][100]){

    if (only_letters(input_city) == 0) {

        return -1;
    }

    to_correct_format(input_city);
    if(in_array(input_city, cities) == 0){

        return 0;
    }

    return 1;
}

/**
 * Permet de vérifier la composition de la chaine  de caractères pointée
 *
 * @param input_city pointeur vers la ville à vérifier
 * @return Si la saisie est bien composée uniquement de lettres (1) sinon (0)
 */
int only_letters(char* input_city){

    // Vérifier que la saisie ne contient que des lettres
    int valid_input = 1;
    for (int i = 0; input_city[i] != '\0'; ++i) {
        if (!isalpha(input_city[i])) {
            valid_input = 0;
            break;
        }
    }

    return valid_input;
}

/**
 * Permet de formatter la ville pointée au format Ville
 * Facilite la vie de l'utilisateur : PARIS devient Paris, sTrasBOURG devient Strasbourg
 *
 * @param input_city pointeur vers la ville à vérifier
 */
void to_correct_format(char* input_city){

    // Formater la ville saisie (première lettre en majuscule, le reste en minuscules)
    for (int i = 0; input_city[i] != '\0'; ++i) {
        input_city[i] = tolower(input_city[i]);
    }
    input_city[0] = toupper(input_city[0]);
}

/**
 * Permet de vérifier la présente de la ville pointée dans la liste donnée
 *
 * @param input_city pointeur vers la ville à vérifier
 * @param cities liste de villes
 * @return 1 si la ville est présente dans la liste, 0 sinon
 */
int in_array(char input_city[100], char cities[][100]){

    for (int i = 0; cities[i][0] != '\0'; ++i) {
        if (strcmp(cities[i], input_city) == 0) {
            return 1;
        }
    }
    return 0;
}

/**
 * Permet de vérifier la saisie d'utilitateur et de garantir un Time au format hh:mm
 *
 * @param time pointeur vers un Time
 * @param numeroMessage numero du message à afficher
 */
void check_time(Time* time, int numeroMessage){ 

    int retour;
    int validation;
    int hour;
    int minute;
    char message[100];
    switch (numeroMessage) {
        case 1:
            strcpy(message, "Horaire de départ hh:mm : ");
            break;

        case 2:
            strcpy(message, "Horaire de départ n°1 hh:mm :");
            break;

        case 3:
            strcpy(message, "Horaire de départ n°2 hh:mm :");
            break;
    }

    do {
        printf("%s", message);
        retour = scanf("%d:%d", &hour, &minute);
        if (retour != 2) {
            printf("Heure non valide. Veuillez entrer une heure au format hh:mm.\n");
            // Nettoyer le tampon d'entrée en cas d'erreur
            while ((retour = getchar()) != '\n' && retour != EOF) {}
        } else {
            // Vérifier si l'heure et les minutes sont valides
            retour = valid_time(hour, minute);

            if (retour != 1) {
                printf("Heure non valide. Veuillez entrer des valeurs valides.\n");
                // Nettoyer le tampon d'entrée en cas d'erreur
                while ((retour = getchar()) != '\n' && retour != EOF) {}
            } else {
                time->hour = hour;
                time->minute = minute;
                validation = 1;
            }
        }
    } while (validation != 1);
}

/**
 * Permet de vérifier la saisie d'utilitateur et de garantir la cohérence des données envoyées au serveur
 *
 * @param input_city un pointeur vers une ville donnée en paramètre
 * @param cities la liste des villes disponibles
 * @param numeroMessage le numéro du message à afficher suivant l'utilisation
 */
void city_check(char *input_city, char cities[][100], int numeroMessage) {

    int retour = -2;
    char input_buffer[100];
    char message[100];

    switch (numeroMessage) {
        case 1:
            strcpy(message, "Ville de départ : ");
            break;

        case 2:
            strcpy(message, "Ville d'arrivée :");
            break;
    }

    do {
        if (retour == -1)
            printf("Entrée non valide\n");

        if (retour == 0)
            printf("Ville non présente dans la liste\n");

        printf("%s", message);
        scanf("%99s", input_buffer);
        retour = cities_check(input_buffer, cities);

    } while (retour != 1);

    strcpy(input_city, input_buffer);
}

/**
 * Permet de vérifier la saisie d'utilitateur et de garantir la cohérence des données envoyées au serveur
 *
 * @param hour une heure
 * @param minute une minute
 * @return 1 si duo valide, sinon 0
 */
int valid_time(int hour, int minute) {

    if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
        return 0;
    } else {
        return 1;
    }
}