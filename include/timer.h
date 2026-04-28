#ifndef TIMER_H
#define TIMER_H

#include <time.h>

typedef struct timer {
    struct timespec start;
    struct timespec end;
} timer_t;

void timer_start(timer_t *timer);
void timer_stop(timer_t *timer);
double timer_elapsed_seconds(const timer_t *timer);

#endif