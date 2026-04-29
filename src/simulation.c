#include "simulation.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Suma la comida real que queda en todos los nodos del arbol.
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

// Crea la simulacion, el arbol, las hormigas, los hilos y el mutex global.
simulation_t *simulation_create(
    int ant_count,
    int node_count,
    int max_children,
    int food_per_leaf
) {
    simulation_t *simulation = calloc(1, sizeof(simulation_t));
    if (simulation == NULL) {
        fprintf(stderr, "Error: no se pudo reservar memoria para la simulacion\n");
        return NULL;
    }

    simulation->ant_count = ant_count;

    if (pthread_mutex_init(&simulation->total_food_mutex, NULL) != 0) {
        fprintf(stderr, "Error: no se pudo iniciar el mutex de comida total\n");
        free(simulation);
        return NULL;
    }

    simulation->tree = tree_create_random(node_count, max_children, food_per_leaf);
    if (simulation->tree == NULL) {
        simulation_destroy(simulation);
        return NULL;
    }
    simulation->total_food = simulation->tree->initial_food;

    simulation->ants = calloc((size_t)ant_count, sizeof(ant_t));
    if (simulation->ants == NULL) {
        fprintf(stderr, "Error: no se pudo reservar memoria para las hormigas\n");
        simulation_destroy(simulation);
        return NULL;
    }

    simulation->threads = calloc((size_t)ant_count, sizeof(pthread_t));
    if (simulation->threads == NULL) {
        fprintf(stderr, "Error: no se pudo reservar memoria para los hilos\n");
        simulation_destroy(simulation);
        return NULL;
    }

    unsigned int base_seed = (unsigned int)time(NULL);
    for (int i = 0; i < ant_count; ++i) {
        simulation->ants[i].id = i;
        simulation->ants[i].collected_food = 0;
        simulation->ants[i].seed = base_seed + (unsigned int)i;
        simulation->ants[i].simulation = simulation;
    }

    return simulation;
}

// Crea un hilo por hormiga y espera a que todos terminen.
int simulation_run(simulation_t *simulation) {
    if (simulation == NULL) {
        return 0;
    }

    int created = 0;
    for (int i = 0; i < simulation->ant_count; ++i) {
        if (pthread_create(&simulation->threads[i], NULL, ant_run, &simulation->ants[i]) != 0) {
            fprintf(stderr, "Error: no se pudo crear el hilo de la hormiga %d\n", i);
            for (int j = 0; j < created; ++j) {
                if (pthread_join(simulation->threads[j], NULL) != 0) {
                    fprintf(stderr, "Error: no se pudo esperar el hilo %d despues del fallo\n", j);
                }
            }
            return 0;
        }
        created++;
    }

    for (int i = 0; i < created; ++i) {
        if (pthread_join(simulation->threads[i], NULL) != 0) {
            fprintf(stderr, "Error: no se pudo esperar el hilo de la hormiga %d\n", i);
            return 0;
        }
    }

    return 1;
}

// Lee la comida total usando mutex porque varios hilos la usan.
int simulation_get_total_food(simulation_t *simulation) {
    if (simulation == NULL) {
        return 0;
    }

    if (pthread_mutex_lock(&simulation->total_food_mutex) != 0) {
        fprintf(stderr, "Error: no se pudo bloquear el mutex de comida total\n");
        return 0;
    }

    int total = simulation->total_food;

    if (pthread_mutex_unlock(&simulation->total_food_mutex) != 0) {
        fprintf(stderr, "Error: no se pudo desbloquear el mutex de comida total\n");
    }

    return total;
}

// Resta una unidad de comida total usando mutex.
void simulation_decrease_total_food(simulation_t *simulation) {
    if (simulation == NULL) {
        return;
    }

    if (pthread_mutex_lock(&simulation->total_food_mutex) != 0) {
        fprintf(stderr, "Error: no se pudo bloquear el mutex de comida total\n");
        return;
    }

    simulation->total_food--;

    if (pthread_mutex_unlock(&simulation->total_food_mutex) != 0) {
        fprintf(stderr, "Error: no se pudo desbloquear el mutex de comida total\n");
    }
}

// Suma la comida recolectada por todas las hormigas.
int simulation_get_collected_food(const simulation_t *simulation) {
    if (simulation == NULL) {
        return 0;
    }
    if (simulation->ants == NULL) {
        return 0;
    }

    int total = 0;
    for (int i = 0; i < simulation->ant_count; ++i) {
        total += simulation->ants[i].collected_food;
    }

    return total;
}

// Imprime el resumen final de la simulacion.
void simulation_print_results(const simulation_t *simulation, double elapsed_seconds) {
    if (simulation == NULL) {
        return;
    }
    if (simulation->tree == NULL) {
        return;
    }

    int collected = simulation_get_collected_food(simulation);
    int remaining = simulation->total_food;
    int has_negative_food = 0;
    int actual_remaining = tree_actual_remaining_food(simulation->tree, &has_negative_food);
    int consistent = 1;

    if (simulation->tree->initial_food != collected + remaining) {
        consistent = 0;
    }
    if (remaining != actual_remaining) {
        consistent = 0;
    }
    if (has_negative_food) {
        consistent = 0;
    }

    printf("=== Simulacion de hormigas ===\n\n");

    printf("Cantidad de hormigas: %d\n", simulation->ant_count);
    tree_print_summary(simulation->tree);
    printf("\n");

    printf("Comida inicial: %d\n", simulation->tree->initial_food);
    printf("Comida recolectada: %d\n", collected);
    printf("Comida restante: %d\n\n", remaining);

    printf("Tiempo de ejecucion: %.6f segundos\n\n", elapsed_seconds);

    printf("Recoleccion por hormiga:\n");
    for (int i = 0; i < simulation->ant_count; ++i) {
        printf("Hormiga %d: %d\n", simulation->ants[i].id, simulation->ants[i].collected_food);
    }

    if (consistent) {
        printf("\nRevision de consistencia: OK\n");
    } else {
        printf("\nRevision de consistencia: FALLO\n");
    }
}

// Libera toda la memoria y destruye el mutex global.
void simulation_destroy(simulation_t *simulation) {
    if (simulation == NULL) {
        return;
    }

    if (simulation->tree != NULL) {
        tree_destroy(simulation->tree);
    }

    if (pthread_mutex_destroy(&simulation->total_food_mutex) != 0) {
        fprintf(stderr, "Advertencia: no se pudo destruir el mutex de comida total\n");
    }

    free(simulation->ants);
    free(simulation->threads);
    free(simulation);
}
