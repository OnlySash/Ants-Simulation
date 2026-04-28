#include <stdio.h>
#include <stdlib.h>

#include "../include/simulation.h"

int main(int argc, char *argv[]) {
    int ant_count = 5;
    int node_count = 20;
    int max_children = 3;
    int food_per_leaf = 4;

    if (argc >= 2) ant_count = atoi(argv[1]);
    if (argc >= 3) node_count = atoi(argv[2]);
    if (argc >= 4) max_children = atoi(argv[3]);
    if (argc >= 5) food_per_leaf = atoi(argv[4]);

    simulation_t *simulation = simulation_create(
        ant_count,
        node_count,
        max_children,
        food_per_leaf
    );

    if (!simulation) {
        fprintf(stderr, "Error creando la simulacion\n");
        return EXIT_FAILURE;
    }

    tree_print_summary(simulation->tree);

    simulation_run(simulation);
    simulation_print_results(simulation);

    simulation_destroy(simulation);

    return EXIT_SUCCESS;
}