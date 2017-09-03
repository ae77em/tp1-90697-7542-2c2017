#ifndef SRC_ROPE_ROPE_H_
#define SRC_ROPE_ROPE_H_

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "rope_node.h"
#include "splitted_rope.h"

typedef struct rope_t {
  rope_node_t *root;
} rope_t;

void rope_create(rope_t* self);
void rope_destroy(rope_t *self);

/* basic operations */
splitted_rope_t *split(rope_node_t* node, int index);
void join(rope_node_t *p, rope_node_t* l, rope_node_t* r);
void insert(rope_t *tree, int pos, char *word);
void delete(rope_t *tree, int start, int end);

void append(rope_t *tree, char *word);
void print(rope_t *self);

/* auxiliar operations */
int is_empty(rope_t *self);

#endif
