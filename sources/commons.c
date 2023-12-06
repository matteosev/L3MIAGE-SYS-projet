#include <unistd.h>
#include <stdio.h>

#include "../headers/commons.h"

void print_train(Train t) {
    printf("{ id=%d city_from=%s city_to=%s time_from=%02d:%02d time_to=%02d:%02d price=%.2f reduc=%d suppl=%d }\n", t.id, t.city_from, t.city_to, t.time_from.hour, t.time_from.minute, t.time_to.hour, t.time_to.minute, t.price, t.reduc, t.suppl);
}

void print_request(Request r) {
    printf("{ city_from=%s city_to=%s time_from_1=%02d:%02d time_from_2=%02d:%02d type=%d }\n", r.city_from, r.city_to, r.time_from_1.hour, r.time_from_1.minute, r.time_from_2.hour, r.time_from_2.minute, r.type);
}


Time time_difference(Time t1,Time t2){
    Time ret;
    ret.hour=t1.hour>t2.hour ? t1.hour-t2.hour : t2.hour-t1.hour;
    ret.minute=t1.minute>t2.minute ? t1.minute-t2.minute : t2.minute-t1.minute;
    return ret;

}

/**
 * Compare 2 structures Time
 * @param t1 Time 1er temps comparé
 * @param t2 Time 2e temps comparé
 * @return -1 si t1 < t2 (plus tôt), 0 si t1 == t2 (même heure), 1 si t1 > t2 (plus tard)
 */
int timecmp(Time t1, Time t2) {

    if (t1.hour == t2.hour) {

        if (t1.minute == t2.minute)
            return 0;

        return t1.minute > t2.minute ? 1 : -1;
    }
    return t1.hour > t2.hour ? 1 : -1;
}
