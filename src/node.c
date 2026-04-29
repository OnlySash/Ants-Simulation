#include "node.h"

#include <stdio.h>
#include <stdlib.h>

// Crea un nodo con espacio para sus hijos y con su propio mutex.
node_t *node_create(int id, int child_capacity) {
    node_t *node = calloc(1, sizeof(node_t));
    if (node == NULL) {
        fprintf(stderr, "Error: no se pudo reservar memoria para el nodo %d\n", id);
        return NULL;
    }

    node->id = id;
    node->child_capacity = child_capacity;

    node->children = calloc((size_t)child_capacity, sizeof(node_t *));
    if (node->children == NULL) {
        fprintf(stderr, "Error: no se pudo reservar memoria para los hijos del nodo %d\n", id);
        free(node);
        return NULL;
    }

    if (pthread_mutex_init(&node->mutex, NULL) != 0) {
        fprintf(stderr, "Error: no se pudo iniciar el mutex del nodo %d\n", id);
        free(node->children);
        free(node);
        return NULL;
    }

    return node;
}

// Agrega un hijo al nodo padre si todavia hay espacio.
int node_add_child(node_t *parent, node_t *child) {
    if (parent == NULL) {
        return 0;
    }
    if (child == NULL) {
        return 0;
    }

    if (parent->child_count >= parent->child_capacity) {
        return 0;
    }

    parent->children[parent->child_count] = child;
    parent->child_count++;
    return 1;
}

// Revisa si un nodo no tiene hijos.
int node_is_leaf(const node_t *node) {
    if (node == NULL) {
        return 0;
    }

    if (node->child_count == 0) {
        return 1;
    }

    return 0;
}

// Destruye el mutex del nodo y libera su memoria.
void node_destroy(node_t *node) {
    if (node == NULL) {
        return;
    }

    if (pthread_mutex_destroy(&node->mutex) != 0) {
        fprintf(stderr, "Advertencia: no se pudo destruir el mutex del nodo %d\n", node->id);
    }

    free(node->children);
    free(node);
}
