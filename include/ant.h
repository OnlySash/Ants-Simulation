#ifndef ANT_H
#define ANT_H

struct simulation;

typedef struct ant {
    int id;
    int collected_food;
    unsigned int seed;
    struct simulation *simulation;
} ant_t;

void ant_init(ant_t *ant, int id, unsigned int seed, struct simulation *simulation);
int ant_step(ant_t *ant);

#endif