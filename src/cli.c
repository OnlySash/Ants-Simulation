#include "../include/cli.h"

#include <stdio.h>
#include <stdlib.h>

void cli_print_usage(const char *program_name) {
    fprintf(stderr, "Uso: %s [hormigas] [nodos] [max_hijos] [comida_por_hoja]\n",
            program_name);
    fprintf(stderr, "Ejemplo: %s 10 50 4 5\n", program_name);
}

int cli_parse_args(int argc, char *argv[], cli_options_t *options) {
    if (!options) return 0;

    options->ant_count = 5;
    options->node_count = 20;
    options->max_children = 3;
    options->food_per_leaf = 4;

    if (argc >= 2) options->ant_count = atoi(argv[1]);
    if (argc >= 3) options->node_count = atoi(argv[2]);
    if (argc >= 4) options->max_children = atoi(argv[3]);
    if (argc >= 5) options->food_per_leaf = atoi(argv[4]);

    if (argc > 5) {
        cli_print_usage(argv[0]);
        return 0;
    }

    if (
        options->ant_count <= 0 ||
        options->node_count <= 0 ||
        options->max_children <= 0 ||
        options->food_per_leaf < 0
    ) {
        cli_print_usage(argv[0]);
        return 0;
    }

    return 1;
}