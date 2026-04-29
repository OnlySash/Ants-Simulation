#include "cli.h"

#include "simulation.h"
#include "timer.h"

#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

// Muestra como se debe ejecutar el programa desde la terminal.
static void print_usage(const char *program_name) {
    fprintf(stderr,
            "Uso: %s <cantidad_hormigas> <cantidad_nodos> <max_hijos> <comida_por_hoja>\n",
            program_name);
}

// Lee un numero positivo desde texto.
static int read_number(const char *text, const char *name) {
    int value = atoi(text);

    if (value <= 0) {
        fprintf(stderr, "Error: %s debe ser positivo\n", name);
        return 0;
    }

    return value;
}

// Punto de entrada de la interfaz de consola.
int cli_run(int argc, char **argv) {
    if (argc != 5) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    int ant_count = read_number(argv[1], "cantidad_hormigas");
    int node_count = read_number(argv[2], "cantidad_nodos");
    int max_children = read_number(argv[3], "max_hijos");
    int food_per_leaf = read_number(argv[4], "comida_por_hoja");

    if (ant_count == 0) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }
    if (node_count == 0) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }
    if (max_children == 0) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }
    if (food_per_leaf == 0) {
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    if (max_children > node_count) {
        fprintf(stderr, "Error: max_hijos no puede ser mayor que cantidad_nodos\n");
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    if (food_per_leaf > INT_MAX / node_count) {
        fprintf(stderr, "Error: cantidad_nodos y comida_por_hoja son demasiado grandes\n");
        print_usage(argv[0]);
        return EXIT_FAILURE;
    }

    simulation_t *simulation = simulation_create(
        ant_count,
        node_count,
        max_children,
        food_per_leaf
    );
    if (simulation == NULL) {
        return EXIT_FAILURE;
    }

    sim_timer_t timer;
    timer_start(&timer);
    int ok = simulation_run(simulation);
    timer_stop(&timer);

    simulation_print_results(simulation, timer_elapsed_seconds(&timer));
    simulation_destroy(simulation);

    if (ok) {
        return EXIT_SUCCESS;
    }
    return EXIT_FAILURE;
}
