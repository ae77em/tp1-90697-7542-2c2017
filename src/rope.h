#ifndef SRC_ROPE_ROPE_H_
#define SRC_ROPE_ROPE_H_

#include <stdlib.h>

typedef struct rope_node {
    int weight;
    char *word;
    struct rope_node *left_child;
    struct rope_node *right_child;
} rope_node;

typedef struct {
    rope_node* left_tree;
    rope_node* right_tree;
} splited_rope;

rope_node *create_rope();
splited_rope split(int, rope_node);
rope_node *join(rope_node*, rope_node*);
void print_rope(rope_node*);
void destroy_rope(rope_node*);

#endif
