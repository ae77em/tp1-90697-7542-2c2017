#ifndef SRC_ROPE_ROPE_H_
#define SRC_ROPE_ROPE_H_

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>

typedef struct rope_node {
    int weight;
    char *word;
    struct rope_node *left_child;
    struct rope_node *right_child;
} rope_node;

typedef struct {
    rope_node* left_tree;
    rope_node* right_tree;
} splitted_rope;

rope_node* rope_node_create();
void rope_node_initialize(rope_node *self);
rope_node* rope_node_create_leaf(char *str);
void rope_node_destroy(rope_node *self);

splitted_rope* splitted_rope_create();
void splitted_rope_destroy(splitted_rope *self);

splitted_rope *split(int index, rope_node* node);
void join(rope_node *parent, rope_node* left_child, rope_node* right_child);

int calculate_weight(rope_node *subtree);
void print(rope_node *self);

#endif
