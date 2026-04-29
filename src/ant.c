#include "ant.h"

#include "simulation.h"

#include <stdio.h>

// Funcion que ejecuta cada hilo de hormiga.
void *ant_run(void *arg) {
    ant_t *ant = arg;
    if (ant == NULL) {
        return NULL;
    }
    if (ant->simulation == NULL) {
        return NULL;
    }

    simulation_t *simulation = ant->simulation;

    while (simulation_get_total_food(simulation) > 0) {
        node_t *leaf = tree_walk_random_leaf(simulation->tree, &ant->seed);
        if (leaf == NULL) {
            break;
        }

        ant_try_collect(ant, leaf);
    }

    return NULL;
}

// Intenta tomar comida de una hoja usando el mutex de esa hoja.
int ant_try_collect(ant_t *ant, node_t *leaf) {
    if (ant == NULL) {
        return 0;
    }
    if (leaf == NULL) {
        return 0;
    }

    if (pthread_mutex_lock(&leaf->mutex) != 0) {
        fprintf(stderr, "Error: la hormiga %d no pudo bloquear la hoja %d\n", ant->id, leaf->id);
        return 0;
    }

    int collected = 0;
    if (leaf->food > 0) {
        leaf->food--;
        ant->collected_food++;
        simulation_decrease_total_food(ant->simulation);
        collected = 1;
    }

    if (pthread_mutex_unlock(&leaf->mutex) != 0) {
        fprintf(stderr, "Error: la hormiga %d no pudo desbloquear la hoja %d\n", ant->id, leaf->id);
    }

    return collected;
}
