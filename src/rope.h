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

typedef struct {
    char *first_half;
    char *second_half;
} splitted_string;

void rope_node_create(rope_node *self);
void rope_node_create_leaf(rope_node *self, char *word);
void rope_node_destroy(rope_node *self);

void splitted_rope_create(splitted_rope *self);
void splitted_rope_destroy(splitted_rope *self);

void split(splitted_rope *sr, int index, rope_node* node);
void join(rope_node *parent, rope_node* left_child, rope_node* right_child);

int calculate_weight(rope_node *subtree);
void print(rope_node *self, char *direction);

#endif
