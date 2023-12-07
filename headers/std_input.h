/**
 * \file std_input.h
 */

char yes_or_no_verification(int nb_train);
int cities_check(char *input_city, char cities[][100]);
void to_correct_format(char* input_city);
int only_letters(char* input_city);
int in_array(char input_city[100], char cities[][100]);
int valid_time(int hour, int minute);
void check_time(Time* time, int numeroMessage);
void city_check(char *input_city, char cities[][100], int numeroMessage);