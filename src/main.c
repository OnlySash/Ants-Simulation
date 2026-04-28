#include <stdio.h>
#include <stdlib.h>

#include "../include/cli.h"
#include "../include/simulation.h"
#include "../include/timer.h"

int main(int argc, char *argv[]) {
    cli_options_t options;

    if (!cli_parse_args(argc, argv, &options)) {
        return EXIT_FAILURE;
    }

    simulation_t *simulation = simulation_create(
        options.ant_count,
        options.node_count,
        options.max_children,
        options.food_per_leaf
    );

    if (!simulation) {
        fprintf(stderr, "Error creando la simulacion\n");
        return EXIT_FAILURE;
    }

    tree_print_summary(simulation->tree);

    timer_t timer;

    timer_start(&timer);
    simulation_run(simulation);
    timer_stop(&timer);

    simulation_print_results(simulation);

    printf("\nTiempo de ejecucion: %.6f segundos\n",
           timer_elapsed_seconds(&timer));

    simulation_destroy(simulation);

    return EXIT_SUCCESS;
}