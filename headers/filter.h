int filter_train_from_array(Train trains[], int nb_train, Train **trains_filtered, Request req);
int check_filter(Train train, Request req);
int soonest_train(Train trains[], Train **filter_result, int nb_train);