#include <unistd.h>
#include <stdio.h>

#include "../headers/commons.h"

void print_train(Train t) {
    printf("{ city_from=%s city_to=%s time_from=%02d:%02d time_to=%02d:%02d price=%.2f reduc=%d suppl=%d }\n", t.city_from, t.city_to, t.time_from.hour, t.time_from.minute, t.time_to.hour, t.time_to.minute, t.price, t.reduc, t.suppl);
}

void print_request(Request r) {
    printf("{ city_from=%s city_to=%s time_from_1=%02d:%02d time_from_2=%02d:%02d last=%d }\n", r.city_from, r.city_to, r.time_from_1.hour, r.time_from_1.minute, r.time_from_2.hour, r.time_from_2.minute, r.last);
}
