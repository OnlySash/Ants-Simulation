#include "simulation.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

static int tree_actual_remaining_food(const tree_t *tree, int *has_negative_food) {
    int total = 0;
    *has_negative_food = 0;

    for (int i = 0; i < tree->node_count; ++i) {
        int food = tree->nodes[i]->food;
        if (food < 0) {
            *has_negative_food = 1;
        }
        total += food;
    }

    return total;
}

simulation_t *simulation_create(
    int ant_count,
    int node_count,
    int max_children,
    int food_per_leaf,
    int use_mutex
) {
    simulation_t *simulation = calloc(1, sizeof(*simulation));
    if (simulation == NULL) {
        fprintf(stderr, "Error: unable to allocate simulation\n");
        return NULL;
    }

    simulation->ant_count = ant_count;
    simulation->use_mutex = use_mutex;

    simulation->tree = tree_create_random(node_count, max_children, food_per_leaf);
    if (simulation->tree == NULL) {
        simulation_destroy(simulation);
        return NULL;
    }
    simulation->total_food = simulation->tree->initial_food;

    if (pthread_mutex_init(&simulation->total_food_mutex, NULL) != 0) {
        fprintf(stderr, "Error: unable to initialize total food mutex\n");
        simulation_destroy(simulation);
        return NULL;
    }
    simulation->total_food_mutex_initialized = 1;

    simulation->ants = calloc((size_t)ant_count, sizeof(*simulation->ants));
    if (simulation->ants == NULL) {
        fprintf(stderr, "Error: unable to allocate ants\n");
        simulation_destroy(simulation);
        return NULL;
    }

    simulation->threads = calloc((size_t)ant_count, sizeof(*simulation->threads));
    if (simulation->threads == NULL) {
        fprintf(stderr, "Error: unable to allocate thread handles\n");
        simulation_destroy(simulation);
        return NULL;
    }

    unsigned int base_seed = (unsigned int)time(NULL);
    for (int i = 0; i < ant_count; ++i) {
        simulation->ants[i].id = i;
        simulation->ants[i].collected_food = 0;
        simulation->ants[i].seed = base_seed ^ ((unsigned int)i * 2654435761u);
        simulation->ants[i].simulation = simulation;
    }

    return simulation;
}

int simulation_run(simulation_t *simulation) {
    if (simulation == NULL) {
        return 0;
    }

    int created = 0;
    for (int i = 0; i < simulation->ant_count; ++i) {
        if (pthread_create(&simulation->threads[i], NULL, ant_run, &simulation->ants[i]) != 0) {
            fprintf(stderr, "Error: unable to create ant thread %d\n", i);
            for (int j = 0; j < created; ++j) {
                if (pthread_join(simulation->threads[j], NULL) != 0) {
                    fprintf(stderr, "Error: unable to join ant thread %d after create failure\n", j);
                }
            }
            return 0;
        }
        created++;
    }

    int ok = 1;
    for (int i = 0; i < created; ++i) {
        if (pthread_join(simulation->threads[i], NULL) != 0) {
            fprintf(stderr, "Error: unable to join ant thread %d\n", i);
            ok = 0;
        }
    }

    return ok;
}

int simulation_get_total_food(simulation_t *simulation) {
    if (simulation == NULL) {
        return 0;
    }

    if (!simulation->use_mutex) {
        return simulation->total_food;
    }

    if (pthread_mutex_lock(&simulation->total_food_mutex) != 0) {
        fprintf(stderr, "Error: unable to lock total food mutex\n");
        return 0;
    }

    int total = simulation->total_food;

    if (pthread_mutex_unlock(&simulation->total_food_mutex) != 0) {
        fprintf(stderr, "Error: unable to unlock total food mutex\n");
    }

    return total;
}

void simulation_decrease_total_food(simulation_t *simulation) {
    if (simulation == NULL) {
        return;
    }

    if (!simulation->use_mutex) {
        simulation->total_food--;
        return;
    }

    if (pthread_mutex_lock(&simulation->total_food_mutex) != 0) {
        fprintf(stderr, "Error: unable to lock total food mutex\n");
        return;
    }

    simulation->total_food--;

    if (pthread_mutex_unlock(&simulation->total_food_mutex) != 0) {
        fprintf(stderr, "Error: unable to unlock total food mutex\n");
    }
}

int simulation_get_collected_food(const simulation_t *simulation) {
    if (simulation == NULL || simulation->ants == NULL) {
        return 0;
    }

    int total = 0;
    for (int i = 0; i < simulation->ant_count; ++i) {
        total += simulation->ants[i].collected_food;
    }

    return total;
}

void simulation_print_results(const simulation_t *simulation, double elapsed_seconds) {
    if (simulation == NULL || simulation->tree == NULL) {
        return;
    }

    int collected = simulation_get_collected_food(simulation);
    int remaining = simulation->total_food;
    int expected = collected + remaining;
    int has_negative_food = 0;
    int actual_remaining = tree_actual_remaining_food(simulation->tree, &has_negative_food);
    int consistent = simulation->tree->initial_food == expected &&
                     remaining == actual_remaining &&
                     !has_negative_food;

    printf("=== Ant Foraging Simulation ===\n");
    printf("Mode: %s\n\n", simulation->use_mutex ? "mutex" : "nomutex");

    printf("Ant count: %d\n", simulation->ant_count);
    tree_print_summary(simulation->tree);
    printf("\n");

    printf("Initial food: %d\n", simulation->tree->initial_food);
    printf("Collected food: %d\n", collected);
    printf("Remaining food: %d\n\n", remaining);

    printf("Execution time: %.6f seconds\n\n", elapsed_seconds);

    printf("Per-ant collection:\n");
    for (int i = 0; i < simulation->ant_count; ++i) {
        printf("Ant %d: %d\n", simulation->ants[i].id, simulation->ants[i].collected_food);
    }

    printf("\nConsistency check: %s\n", consistent ? "OK" : "FAILED");
}

void simulation_destroy(simulation_t *simulation) {
    if (simulation == NULL) {
        return;
    }

    if (simulation->tree != NULL) {
        tree_destroy(simulation->tree);
    }

    if (simulation->total_food_mutex_initialized &&
        pthread_mutex_destroy(&simulation->total_food_mutex) != 0) {
        fprintf(stderr, "Warning: unable to destroy total food mutex\n");
    }

    free(simulation->ants);
    free(simulation->threads);
    free(simulation);
}
