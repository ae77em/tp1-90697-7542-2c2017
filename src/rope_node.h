#ifndef ROPE_NODE_H
#define ROPE_NODE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct rope_node_t {
    int id;
    int weight;
    char *word;
    struct rope_node_t *left_child;
    struct rope_node_t *right_child;
} rope_node_t;

void rope_node_create(rope_node_t *self);
void rope_node_destroy(rope_node_t *self);

void rope_destroy_subtree(rope_node_t *root);

void rope_node_initialize(rope_node_t *self);

void rope_node_initialize_leaf(rope_node_t *self, char *str);

#endif /* NODE_H */

