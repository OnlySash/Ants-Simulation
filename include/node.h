#ifndef NODE_H
#define NODE_H

#include <pthread.h>

// Nodo del arbol. Cada nodo tiene su propio mutex para proteger su comida.
typedef struct node {
    int id;
    int food;
    int child_count;
    int child_capacity;
    struct node **children;
    pthread_mutex_t mutex;
} node_t;

node_t *node_create(int id, int child_capacity);

int node_add_child(node_t *parent, node_t *child);

// Devuelve 1 si el nodo es hoja, o 0 si no lo es.
int node_is_leaf(const node_t *node);

// Libera la memoria del nodo.
void node_destroy(node_t *node);

#endif
