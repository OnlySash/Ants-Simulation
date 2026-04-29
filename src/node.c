#include "node.h"

#include <stdio.h>
#include <stdlib.h>

node_t *node_create(int id, int child_capacity) {
    node_t *node = calloc(1, sizeof(*node));
    if (node == NULL) {
        fprintf(stderr, "Error: unable to allocate node %d\n", id);
        return NULL;
    }

    node->id = id;
    node->child_capacity = child_capacity;

    node->children = calloc((size_t)child_capacity, sizeof(*node->children));
    if (node->children == NULL) {
        fprintf(stderr, "Error: unable to allocate children for node %d\n", id);
        free(node);
        return NULL;
    }

    if (pthread_mutex_init(&node->mutex, NULL) != 0) {
        fprintf(stderr, "Error: unable to initialize mutex for node %d\n", id);
        free(node->children);
        free(node);
        return NULL;
    }

    return node;
}

int node_add_child(node_t *parent, node_t *child) {
    if (parent == NULL || child == NULL) {
        return 0;
    }

    if (parent->child_count >= parent->child_capacity) {
        return 0;
    }

    parent->children[parent->child_count] = child;
    parent->child_count++;
    return 1;
}

int node_is_leaf(const node_t *node) {
    return node != NULL && node->child_count == 0;
}

void node_destroy(node_t *node) {
    if (node == NULL) {
        return;
    }

    if (pthread_mutex_destroy(&node->mutex) != 0) {
        fprintf(stderr, "Warning: unable to destroy mutex for node %d\n", node->id);
    }

    free(node->children);
    free(node);
}
