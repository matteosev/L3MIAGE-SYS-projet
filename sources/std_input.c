#include <stdio.h>
#include <string.h>
#include <ctype.h>

#include "../headers/commons.h"
#include "../headers/std_input.h"

/*Permet de s'assurer que l'utilisateur ne puisse rien saisir d'autre que 'y' ou 'n'*/
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

/*Permet de s'assurer :
- de la présence d'une chaine de caractères uniquement
- si oui, mise au format Ville peut importe l'entrée
- enfin de s'assurer de la présence de la ville dans la liste des villes disponibles
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

/*Permet de garantir l'absence de chiffres ou de caractères spéciaux*/
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

/*Permet de formatter une entrée au format Ville, afin de sécuriser la recherche côté serveur
et de faciliter la vie de l'utilisateur pour qu'il puisse taper le nom de la ville comme il veut*/
void to_correct_format(char* input_city){

    // Formater la ville saisie (première lettre en majuscule, le reste en minuscules)
    for (int i = 0; input_city[i] != '\0'; ++i) {
        input_city[i] = tolower(input_city[i]);
    }
    input_city[0] = toupper(input_city[0]);
}

/*Permet de vérifier la présence d'une ville dans la liste de villes disponibles*/
int in_array(char input_city[100], char cities[][100]){

    for (int i = 0; cities[i][0] != '\0'; ++i) {
        if (strcmp(cities[i], input_city) == 0) {
            return 1;
        }
    }
    return 0;
}

/*Permet de s'assurer d'avoir un nouveau Time qui sera au format HH:MM
Permet également d'afficher un message suivant le numéro saisi en paramètre, 
utilise pour factorirer le main client*/
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

/*Permet de mettre dans input_city une ville qui sera forcément dans les villes disponibles*/
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

/*Permet d'avoir une heure valide avec 0 < H < 23 && 0 < M < 59*/
int valid_time(int hour, int minute) {

    if (hour < 0 || hour > 23 || minute < 0 || minute > 59) {
        return 0;
    } else {
        return 1;
    }
}