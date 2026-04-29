#ifndef TIMER_H
#define TIMER_H

#include <sys/time.h>

// Guarda el inicio y final de una medicion de tiempo.
typedef struct sim_timer {
    struct timeval start;
    struct timeval end;
} sim_timer_t;

// Marca el tiempo inicial.
void timer_start(sim_timer_t *timer);

// Marca el tiempo final.
void timer_stop(sim_timer_t *timer);

// Devuelve el tiempo transcurrido en segundos.
double timer_elapsed_seconds(const sim_timer_t *timer);

#endif