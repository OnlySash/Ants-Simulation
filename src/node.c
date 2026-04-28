#include "node.h"
#include <stdlib.h>

node_t *node_create(int id, int child_capacity) {
    node_t *node = malloc(sizeof(node_t));
    if (!node) return NULL;

    node->id = id;
    node->food = 0;
    node->child_count = 0;
    node->child_capacity = child_capacity;
    node->children = NULL;

    if (child_capacity > 0) {
        node->children = malloc(sizeof(node_t *) * child_capacity);
        if (!node->children) {
            free(node);
            return NULL;
        }
    }

    return node;
}

int node_add_child(node_t *parent, node_t *child) {
    if (!parent || !child) return 0;

    if (parent->child_count >= parent->child_capacity) {
        return 0;
    }

    parent->children[parent->child_count++] = child;
    return 1;
}

int node_is_leaf(const node_t *node) {
    return node && node->child_count == 0;
}

void node_destroy(node_t *node) {
    if (!node) return;

    free(node->children);
    free(node);
}