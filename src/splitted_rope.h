#ifndef SPLITTED_ROPE_H
#define SPLITTED_ROPE_H

#include "rope_node.h"

typedef struct {
    rope_node_t* left;
    rope_node_t* right;
} splitted_rope_t;

splitted_rope_t* splitted_rope_create();
void splitted_rope_destroy(splitted_rope_t *self);

#endif /* SPLITTED_ROPE_H */

