#include "tree.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

tree_t *tree_create_random(int node_count, int max_children) {
    if (node_count <= 0 || max_children <= 0) return NULL;

    tree_t *tree = malloc(sizeof(tree_t));
    if (!tree) return NULL;

    tree->nodes = malloc(sizeof(node_t *) * node_count);
    if (!tree->nodes) {
        free(tree);
        return NULL;
    }

    tree->node_count = node_count;
    tree->initial_food = 0;
    tree->max_children = max_children;

    for (int i = 0; i < node_count; i++) {
        tree->nodes[i] = node_create(i, max_children);
        if (!tree->nodes[i]) {
            for (int j = 0; j < i; j++) {
                node_destroy(tree->nodes[j]);
            }
            free(tree->nodes);
            free(tree);
            return NULL;
        }
    }

    tree->root = tree->nodes[0];

    unsigned int seed = (unsigned int)time(NULL);

    for (int i = 1; i < node_count; i++) {
        node_t *parent = NULL;

        do {
            int parent_index = rand_r(&seed) % i;
            parent = tree->nodes[parent_index];
        } while (parent->child_count >= parent->child_capacity);

        node_add_child(parent, tree->nodes[i]);
    }

    tree_assign_food_to_leaves(tree);

    return tree;
}

void tree_assign_food_to_leaves(tree_t *tree) {
    if (!tree) return;

    unsigned int seed = (unsigned int)time(NULL);
    tree->initial_food = 0;

    for (int i = 0; i < tree->node_count; i++) {
        node_t *node = tree->nodes[i];

        if (node_is_leaf(node)) {
            node->food = 1 + (rand_r(&seed) % 10);
            tree->initial_food += node->food;
        } else {
            node->food = 0;
        }
    }
}

node_t *tree_walk_random_leaf(tree_t *tree, unsigned int *seed) {
    if (!tree || !tree->root || !seed) return NULL;

    node_t *current = tree->root;

    while (!node_is_leaf(current)) {
        int index = rand_r(seed) % current->child_count;
        current = current->children[index];
    }

    return current;
}

void tree_destroy(tree_t *tree) {
    if (!tree) return;

    if (tree->nodes) {
        for (int i = 0; i < tree->node_count; i++) {
            node_destroy(tree->nodes[i]);
        }

        free(tree->nodes);
    }

    free(tree);
}

void tree_print_summary(const tree_t *tree) {
    if (!tree) return;

    int leaf_count = 0;
    int total_food = 0;

    for (int i = 0; i < tree->node_count; i++) {
        node_t *node = tree->nodes[i];

        if (node_is_leaf(node)) {
            leaf_count++;
            total_food += node->food;
        }
    }

    printf("Tree summary:\n");
    printf("Nodes: %d\n", tree->node_count);
    printf("Leaves: %d\n", leaf_count);
    printf("Max children per node: %d\n", tree->max_children);
    printf("Initial food: %d\n", tree->initial_food);
    printf("Current food: %d\n", total_food);
}