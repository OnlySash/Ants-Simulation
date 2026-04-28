#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#include "tree.h"

int main(int argc, char *argv[]) {
    int node_count = 20;
    int max_children = 3;
    int walks = 10;

    if (argc >= 2) {
        node_count = atoi(argv[1]);
    }

    if (argc >= 3) {
        max_children = atoi(argv[2]);
    }

    if (argc >= 4) {
        walks = atoi(argv[3]);
    }

    if (node_count <= 0 || max_children <= 0 || walks <= 0) {
        fprintf(stderr, "Uso: %s [node_count] [max_children] [walks]\n", argv[0]);
        return EXIT_FAILURE;
    }

    tree_t *tree = tree_create_random(node_count, max_children);
    if (!tree) {
        fprintf(stderr, "Error creando el árbol\n");
        return EXIT_FAILURE;
    }

    tree_print_summary(tree);

    unsigned int seed = (unsigned int)time(NULL);

    printf("\nRandom walks:\n");

    for (int i = 0; i < walks; i++) {
        node_t *leaf = tree_walk_random_leaf(tree, &seed);

        if (leaf) {
            printf("Walk %d reached leaf %d with food %d\n",
                   i + 1,
                   leaf->id,
                   leaf->food);

            if (leaf->food > 0) {
                leaf->food--;
            }
        }
    }

    printf("\nAfter walks:\n");
    tree_print_summary(tree);

    tree_destroy(tree);

    return EXIT_SUCCESS;
}