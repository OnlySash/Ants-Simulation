#ifndef TREE_H
#define TREE_H

#include "node.h"

typedef struct tree {
    node_t *root;
    node_t **nodes;
    int node_count;
    int leaf_count;
    int initial_food;
    int max_children;
    int food_per_leaf;
} tree_t;

tree_t *tree_create_random(int node_count, int max_children, int food_per_leaf);
void tree_assign_food_to_leaves(tree_t *tree, int food_per_leaf);
node_t *tree_walk_random_leaf(tree_t *tree, unsigned int *seed);
void tree_destroy(tree_t *tree);
void tree_print_summary(const tree_t *tree);

#endif
