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

    pthread_mutex_t total_food_mutex;
} simulation_t;

// Reserva memoria y prepara los datos iniciales de la simulacion.
simulation_t *simulation_create(
    int ant_count,
    int node_count,
    int max_children,
    int food_per_leaf
);

// Lanza los hilos de las hormigas y espera a que terminen.
int simulation_run(simulation_t *simulation);

// Lee la comida total protegida con mutex.
int simulation_get_total_food(simulation_t *simulation);

// Resta una comida del total protegido con mutex.
void simulation_decrease_total_food(simulation_t *simulation);

// Suma lo recolectado por todas las hormigas.
int simulation_get_collected_food(const simulation_t *simulation);

void simulation_print_results(const simulation_t *simulation, double elapsed_seconds);

void simulation_destroy(simulation_t *simulation);

#endif
