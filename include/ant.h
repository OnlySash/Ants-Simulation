#ifndef ANT_H
#define ANT_H

#include "node.h"

struct simulation;

typedef struct ant {
    int id;
    int collected_food;
    unsigned int seed;
    struct simulation *simulation;
} ant_t;

// Funcion que ejecuta el hilo de una hormiga.
void *ant_run(void *arg);

// Intenta recolectar comida de una hoja protegida con mutex.
int ant_try_collect(ant_t *ant, node_t *leaf);

#endif
