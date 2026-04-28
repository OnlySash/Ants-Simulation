#include "../include/ant.h"
#include "../include/simulation.h"

#include <stdio.h>

void ant_init(ant_t *ant, int id, unsigned int seed, struct simulation *simulation) {
    if (!ant) return;

    ant->id = id;
    ant->collected_food = 0;
    ant->seed = seed;
    ant->simulation = simulation;
}

int ant_step(ant_t *ant) {
    if (!ant || !ant->simulation || !ant->simulation->tree) {
        return 0;
    }

    node_t *leaf = tree_walk_random_leaf(
        ant->simulation->tree,
        &ant->seed
    );

    if (!leaf || leaf->food <= 0) {
        return 0;
    }

    leaf->food--;
    ant->collected_food++;
    simulation_decrease_total_food(ant->simulation);

    printf("Hormiga %d comio en hoja %d. Comida restante: %d\n",
           ant->id,
           leaf->id,
           simulation_get_total_food(ant->simulation));

    return 1;
}