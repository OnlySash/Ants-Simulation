#ifndef CLI_H
#define CLI_H

typedef struct cli_options {
    int ant_count;
    int node_count;
    int max_children;
    int food_per_leaf;
} cli_options_t;

int cli_parse_args(int argc, char *argv[], cli_options_t *options);
void cli_print_usage(const char *program_name);

#endif