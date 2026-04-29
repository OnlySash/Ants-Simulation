#include "tree.h"

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Crea un arbol aleatorio donde las hormigas van a buscar comida.
tree_t *tree_create_random(int node_count, int max_children, int food_per_leaf) {
    tree_t *tree = calloc(1, sizeof(tree_t));
    if (tree == NULL) {
        fprintf(stderr, "Error: no se pudo reservar memoria para el arbol\n");
        return NULL;
    }

    tree->nodes = calloc((size_t)node_count, sizeof(node_t *));
    if (tree->nodes == NULL) {
        fprintf(stderr, "Error: no se pudo reservar memoria para la lista de nodos\n");
        free(tree);
        return NULL;
    }

    tree->node_count = node_count;
    tree->max_children = max_children;
    tree->food_per_leaf = food_per_leaf;

    tree->root = node_create(0, max_children);
    if (tree->root == NULL) {
        tree_destroy(tree);
        return NULL;
    }
    tree->nodes[0] = tree->root;

    int *available = calloc((size_t)node_count, sizeof(int));
    if (available == NULL) {
        fprintf(stderr, "Error: no se pudo reservar memoria para padres disponibles\n");
        tree_destroy(tree);
        return NULL;
    }

    unsigned int seed = (unsigned int)time(NULL);
    int available_count = 1;
    available[0] = 0;

    for (int i = 1; i < node_count; ++i) {
        node_t *new_node = node_create(i, max_children);
        if (new_node == NULL) {
            free(available);
            tree_destroy(tree);
            return NULL;
        }

        int window = available_count;
        if (window > max_children) {
            window = max_children;
        }

        int candidate_index = rand_r(&seed) % window;
        int parent_index = available[candidate_index];
        node_t *parent = tree->nodes[parent_index];

        if (node_add_child(parent, new_node) == 0) {
            fprintf(stderr, "Error: no se pudo agregar el nodo %d al padre %d\n", i, parent_index);
            node_destroy(new_node);
            free(available);
            tree_destroy(tree);
            return NULL;
        }

        tree->nodes[i] = new_node;

        if (parent->child_count == parent->child_capacity) {
            for (int j = candidate_index; j < available_count - 1; ++j) {
                available[j] = available[j + 1];
            }
            available_count--;
        }

        available[available_count] = i;
        available_count++;
    }

    free(available);
    tree_assign_food_to_leaves(tree, food_per_leaf);

    return tree;
}

// Pone comida solo en las hojas del arbol.
void tree_assign_food_to_leaves(tree_t *tree, int food_per_leaf) {
    if (tree == NULL) {
        return;
    }

    tree->leaf_count = 0;
    tree->initial_food = 0;

    for (int i = 0; i < tree->node_count; ++i) {
        node_t *node = tree->nodes[i];
        if (node_is_leaf(node)) {
            node->food = food_per_leaf;
            tree->leaf_count++;
            tree->initial_food += food_per_leaf;
        } else {
            node->food = 0;
        }
    }
}

// Camina desde la raiz hasta una hoja aleatoria.
node_t *tree_walk_random_leaf(tree_t *tree, unsigned int *seed) {
    if (tree == NULL) {
        return NULL;
    }
    if (tree->root == NULL) {
        return NULL;
    }
    if (seed == NULL) {
        return NULL;
    }

    node_t *current = tree->root;
    while (current->child_count > 0) {
        int index = rand_r(seed) % current->child_count;
        current = current->children[index];
    }

    return current;
}

// Libera todos los nodos y el arbol.
void tree_destroy(tree_t *tree) {
    if (tree == NULL) {
        return;
    }

    if (tree->nodes != NULL) {
        for (int i = 0; i < tree->node_count; ++i) {
            node_destroy(tree->nodes[i]);
        }
    }

    free(tree->nodes);
    free(tree);
}

// Imprime datos generales del arbol.
void tree_print_summary(const tree_t *tree) {
    if (tree == NULL) {
        return;
    }

    printf("Cantidad de nodos: %d\n", tree->node_count);
    printf("Maximo de hijos por nodo: %d\n", tree->max_children);
    printf("Cantidad de hojas: %d\n", tree->leaf_count);
    printf("Comida por hoja: %d\n", tree->food_per_leaf);
}
