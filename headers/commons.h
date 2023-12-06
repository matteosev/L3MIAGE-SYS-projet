#define PORT 5000

typedef enum 
{
    HORAIRE = 1,
    PLAGE = 2,
    JOURNEE = 3,
    FIN = 4

} Request_type;

typedef struct  {
    int hour;
    int minute;
} Time;

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

typedef struct {
    Request_type type;
    char city_from[100];
    char city_to[100];
    Time time_from_1;
    Time time_from_2;
} Request;

void print_train(Train t);
void print_request(Request r);
void display_train(Train train);
int timecmp(Time t1, Time t2);
void send_train(int socket, Train train);
void receive_train(int socket,Train *train);
void send_request(int socket, Request request);
void receive_request(int socket,Request *request);
Time time_difference(Time t1,Time t2);
int read_trains_from_file(char *filename, Train *trains, int maxTrains) ;
int count_trains(char *filename);
int filter_train_from_array(Train trains[], int nb_train, Train **trains_filtered, Request req);
int check_filter(Train train, Request req);
int soonest_train(Train trains[], Train **filter_result, int nb_train);