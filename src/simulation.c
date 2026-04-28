#include "../include/simulation.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

simulation_t *simulation_create(
    int ant_count,
    int node_count,
    int max_children,
    int food_per_leaf
) {
    if (ant_count <= 0 || node_count <= 0 || max_children <= 0 || food_per_leaf < 0) {
        return NULL;
    }

    simulation_t *simulation = malloc(sizeof(simulation_t));
    if (!simulation) return NULL;

    simulation->tree = tree_create_random(node_count, max_children, food_per_leaf);
    if (!simulation->tree) {
        free(simulation);
        return NULL;
    }

    simulation->ants = malloc(sizeof(ant_t) * ant_count);
    if (!simulation->ants) {
        tree_destroy(simulation->tree);
        free(simulation);
        return NULL;
    }

    simulation->ant_count = ant_count;
    simulation->total_food = simulation->tree->initial_food;

    unsigned int base_seed = (unsigned int)time(NULL);

    for (int i = 0; i < ant_count; i++) {
        ant_init(
            &simulation->ants[i],
            i,
            base_seed + (i * 97),
            simulation
        );
    }

    return simulation;
}

int simulation_run(simulation_t *simulation) {
    if (!simulation) return 0;

    while (simulation->total_food > 0) {
        for (int i = 0; i < simulation->ant_count; i++) {
            if (simulation->total_food <= 0) {
                break;
            }

            ant_step(&simulation->ants[i]);
        }
    }

    return simulation_get_collected_food(simulation);
}

int simulation_get_total_food(const simulation_t *simulation) {
    if (!simulation) return 0;
    return simulation->total_food;
}

void simulation_decrease_total_food(simulation_t *simulation) {
    if (!simulation) return;

    if (simulation->total_food > 0) {
        simulation->total_food--;
    }
}

int simulation_get_collected_food(const simulation_t *simulation) {
    if (!simulation) return 0;

    int collected = 0;

    for (int i = 0; i < simulation->ant_count; i++) {
        collected += simulation->ants[i].collected_food;
    }

    return collected;
}

void simulation_print_results(const simulation_t *simulation) {
    if (!simulation) return;

    int collected = simulation_get_collected_food(simulation);

    printf("\nResultados de la simulacion:\n");
    printf("Hormigas: %d\n", simulation->ant_count);
    printf("Nodos: %d\n", simulation->tree->node_count);
    printf("Hojas: %d\n", simulation->tree->leaf_count);
    printf("Comida por hoja: %d\n", simulation->tree->food_per_leaf);
    printf("Comida inicial: %d\n", simulation->tree->initial_food);
    printf("Comida restante: %d\n", simulation->total_food);
    printf("Comida recolectada: %d\n", collected);

    if (collected + simulation->total_food == simulation->tree->initial_food) {
        printf("Verificacion: OK\n");
    } else {
        printf("Verificacion: ERROR\n");
    }

    printf("\nComida por hormiga:\n");

    for (int i = 0; i < simulation->ant_count; i++) {
        printf("Hormiga %d: %d\n",
               simulation->ants[i].id,
               simulation->ants[i].collected_food);
    }
}

void simulation_destroy(simulation_t *simulation) {
    if (!simulation) return;

    free(simulation->ants);
    tree_destroy(simulation->tree);
    free(simulation);
}