#ifndef ROPE_NODE_H
#define ROPE_NODE_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct rope_node {
    int weight;
    char *word;
    struct rope_node *left_child;
    struct rope_node *right_child;
} rope_node;

void rope_node_create(rope_node *self);
void rope_node_destroy(rope_node *self);

void rope_destroy_nodes(rope_node *root);

void rope_node_initialize(rope_node *self);

void rope_node_create_leaf(rope_node* self, char *str);
void rope_node_initialize_leaf(rope_node *self, char *str);


#endif /* NODE_H */

