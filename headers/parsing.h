/**
 * \file parsing.h
 */

int read_trains_from_file(char *filename, Train *trains, int maxTrains, char cities[MAX_CITIES][MAX_CITY_NAME_LENGTH], int *city_count);
int count_trains(char *filename);
void add_city_if_new(char cities[MAX_CITIES][MAX_CITY_NAME_LENGTH], int *city_count, const char *city);
