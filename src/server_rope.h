#ifndef SRC_ROPE_ROPE_H_
#define SRC_ROPE_ROPE_H_

#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include "server_rope_node.h"
#include "server_splitted_rope.h"
#include "common_socket.h"

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

void space(rope_t *tree, int pos);
void newline(rope_t *tree, int pos);
void print(rope_t *self);
void sprint(rope_t *self, char *dest);

/* auxiliar operations */
int is_empty(rope_t *self);

#endif
