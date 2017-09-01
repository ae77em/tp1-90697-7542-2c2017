#ifndef SRC_ROPE_ROPE_H_
#define SRC_ROPE_ROPE_H_

#define MIN(a,b) (((a)<(b))?(a):(b))
#define MAX(a,b) (((a)>(b))?(a):(b))

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include "rope_node.h"
#include "splitted_rope.h"

typedef struct rope {
  rope_node *root;
} rope;

void rope_create(rope* self);
void rope_destroy(rope *self);

/* basic operations */
splitted_rope *split(rope_node* node, int index);
void join(rope_node *parent, rope_node* left_child, rope_node* right_child);
void insert(rope *tree, int pos, char *word);
void delete(rope *tree, int start, int end);

void append(rope *tree, char *word);
void print(rope *self);

/* auxiliar operations */
int is_empty(rope *self);

#endif
