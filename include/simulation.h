#ifndef SIMULATION_H
#define SIMULATION_H

#include "ant.h"
#include "tree.h"

typedef struct simulation {
    tree_t *tree;
    ant_t *ants;

    int ant_count;
    int total_food;
} simulation_t;

simulation_t *simulation_create(
    int ant_count,
    int node_count,
    int max_children,
    int food_per_leaf
);

int simulation_run(simulation_t *simulation);
int simulation_get_total_food(simulation_t *simulation);
void simulation_decrease_total_food(simulation_t *simulation);
int simulation_get_collected_food(const simulation_t *simulation);
void simulation_print_results(const simulation_t *simulation);
void simulation_destroy(simulation_t *simulation);

#endif