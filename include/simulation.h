#ifndef SIMULATION_H
#define SIMULATION_H

#include "ant.h"
#include "tree.h"

#include <pthread.h>

typedef struct simulation {
    tree_t *tree;
    ant_t *ants;
    pthread_t *threads;

    int ant_count;
    int total_food;
    int use_mutex;
    int total_food_mutex_initialized;

    pthread_mutex_t total_food_mutex;
} simulation_t;

simulation_t *simulation_create(
    int ant_count,
    int node_count,
    int max_children,
    int food_per_leaf,
    int use_mutex
);

int simulation_run(simulation_t *simulation);
int simulation_get_total_food(simulation_t *simulation);
void simulation_decrease_total_food(simulation_t *simulation);
int simulation_get_collected_food(const simulation_t *simulation);
void simulation_print_results(const simulation_t *simulation, double elapsed_seconds);
void simulation_destroy(simulation_t *simulation);

#endif
