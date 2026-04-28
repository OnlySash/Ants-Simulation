#include "../include/timer.h"

void timer_start(timer_t *timer) {
    if (!timer) return;

    clock_gettime(CLOCK_MONOTONIC, &timer->start);
}

void timer_stop(timer_t *timer) {
    if (!timer) return;

    clock_gettime(CLOCK_MONOTONIC, &timer->end);
}

double timer_elapsed_seconds(const timer_t *timer) {
    if (!timer) return 0.0;

    double seconds = (double)(timer->end.tv_sec - timer->start.tv_sec);
    double nanoseconds = (double)(timer->end.tv_nsec - timer->start.tv_nsec) / 1000000000.0;

    return seconds + nanoseconds;
}