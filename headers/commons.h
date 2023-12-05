typedef enum 
{
    VALENCE = 'V',
    GRENOBLE = 'G',
    MONTELIMAR = 'M',
    PARIS = 'P'
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
    int last; 

} Request;


/*
void write_size_then_msg(int socket, void *msg, size_t size);
void read_size_then_msg(int socket, void *msg);
*/
void print_train(Train *train);
City char_to_city(char c);
