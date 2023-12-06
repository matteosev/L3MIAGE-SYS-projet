#define PORT 5000
#define NB_CLIENT_MAX 1000

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
int timecmp(Time t1, Time t2);
Time time_difference(Time t1,Time t2);