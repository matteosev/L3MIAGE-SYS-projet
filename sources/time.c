#include "../headers/commons.h"

Time time_difference(Time t1,Time t2){

    Time ret;
    ret.hour = t1.hour > t2.hour ? t1.hour-t2.hour : t2.hour-t1.hour;
    ret.minute = t1.minute > t2.minute ? t1.minute-t2.minute : t2.minute-t1.minute;
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