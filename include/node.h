#ifndef NODE_H
#define NODE_H

#include <pthread.h>

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
int node_is_leaf(const node_t *node);
void node_destroy(node_t *node);

#endif
