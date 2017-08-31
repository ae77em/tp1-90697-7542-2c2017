#ifndef SPLITTED_ROPE_H
#define SPLITTED_ROPE_H

#include "rope_node.h"

typedef struct {
    rope_node* left_tree;
    rope_node* right_tree;
} splitted_rope;

splitted_rope* splitted_rope_create();
void splitted_rope_destroy(splitted_rope *self);

#endif /* SPLITTED_ROPE_H */

