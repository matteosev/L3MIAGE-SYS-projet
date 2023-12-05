typedef enum 
{
    VALENCE,
    GRENOBLE,
    MONTELIMAR,
    PARIS
} City;

typedef struct 
{
    int hour;
    int minute;
} Time;

typedef struct 
{
    int id;
    City city_from;
    City city_to;
    Time time_from;
    Time time_to;
    double price;
    int reduc;
    int suppl;
} Train;

typedef struct 
{
    City city_from;
    City city_to;
    Time time_from_1;
    Time time_from_2;
} Request;



void write_size_then_msg(int socket, void *msg);
void read_size_then_msg(int socket, void *msg);
