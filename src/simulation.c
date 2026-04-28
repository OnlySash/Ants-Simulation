#include "../include/simulation.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static void simulation_assign_food_to_leaves(
    simulation_t *simulation,
    int food_per_leaf
) {
    simulation->total_food = 0;

    for (int i = 0; i < simulation->tree->node_count; i++) {
        node_t *node = simulation->tree->nodes[i];

        if (node_is_leaf(node)) {
            node->food = food_per_leaf;
            simulation->total_food += food_per_leaf;
        } else {
            node->food = 0;
        }
    }

    simulation->tree->initial_food = simulation->total_food;
}

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

    simulation->tree = tree_create_random(node_count, max_children);
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
    simulation->total_food = 0;

    unsigned int base_seed = (unsigned int)time(NULL);

    for (int i = 0; i < ant_count; i++) {
        simulation->ants[i].id = i;
        simulation->ants[i].collected_food = 0;
        simulation->ants[i].seed = base_seed + (i * 97);
    }

    simulation_assign_food_to_leaves(simulation, food_per_leaf);

    return simulation;
}

int simulation_run(simulation_t *simulation) {
    if (!simulation) return 0;

    while (simulation->total_food > 0) {
        for (int i = 0; i < simulation->ant_count; i++) {
            if (simulation->total_food <= 0) {
                break;
            }

            ant_t *ant = &simulation->ants[i];
            node_t *leaf = tree_walk_random_leaf(
                simulation->tree,
                &ant->seed
            );

            if (leaf && leaf->food > 0) {
                leaf->food--;
                ant->collected_food++;
                simulation_decrease_total_food(simulation);

                printf("Hormiga %d comio en hoja %d. Comida restante: %d\n",
                       ant->id,
                       leaf->id,
                       simulation->total_food);
            }
        }
    }

    return simulation_get_collected_food(simulation);
}

int simulation_get_total_food(simulation_t *simulation) {
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

    printf("\nResultados de la simulacion:\n");
    printf("Hormigas: %d\n", simulation->ant_count);
    printf("Comida inicial: %d\n", simulation->tree->initial_food);
    printf("Comida restante: %d\n", simulation->total_food);
    printf("Comida recolectada: %d\n",
           simulation_get_collected_food(simulation));

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