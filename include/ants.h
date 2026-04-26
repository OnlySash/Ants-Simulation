#ifndef ANTS_H
#define ANTS_H

#include "../include/environment.h"
#include <pthread.h>

#define ant_num 5

typedef struct {
    environment_t *env;
    int ant_id;
} ant_args;

void ant_creation();
void *ant_movement(void *arg);

#endif