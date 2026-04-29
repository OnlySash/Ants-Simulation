#include "../include/timer.h"
#include <stdio.h>

void timer_start(sim_timer_t *timer) {
    if (timer) {
        gettimeofday(&timer->start, NULL);
    }
}

void timer_stop(sim_timer_t *timer) {
    if (timer) {
        gettimeofday(&timer->end, NULL);
    }
}

double timer_elapsed_seconds(const sim_timer_t *timer) {
    if (!timer) return 0.0;

    long seconds = timer->end.tv_sec - timer->start.tv_sec;
    long microseconds = timer->end.tv_usec - timer->start.tv_usec;

    return (double)seconds + (double)microseconds / 1000000.0;
}