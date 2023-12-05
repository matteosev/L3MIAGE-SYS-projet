typedef struct 
{
    int hour;
    int minute;
} Time;

typedef struct 
{
    int id;
    char city_from[100];
    char city_to[100];
    Time time_from;
    Time time_to;
    double price;
    int reduc;
    int suppl;
} Train;

typedef struct 
{
    char city_from[100];
    char city_to[100];
    Time time_from_1;
    Time time_from_2;
    int last; 

} Request;

void print_train(Train train);
void print_request(Request req);
