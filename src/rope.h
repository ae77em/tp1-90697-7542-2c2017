#ifndef SRC_ROPE_ROPE_H_
#define SRC_ROPE_ROPE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "string_utils.h"

typedef struct rope_node {
    int lenght;
    char *word;
    struct rope_node *left_child;
    struct rope_node *right_child;
} rope_node;

typedef struct {
    rope_node* left_tree;
    rope_node* right_tree;
} splitted_rope;

typedef struct {
    char *left_string;
    char *right_string;
} splitted_string;

/* start and end of life definition methods */
void rope_node_create(rope_node* self);
void rope_leaf_create(rope_node* self, char *str);
void rope_node_destroy(rope_node* self);

void splitted_rope_create(splitted_rope* self);
void splitted_rope_destroy(splitted_rope* self);

void splitted_string_create(splitted_string* self);
void splitted_string_destroy(splitted_string* self);

/* behavior definition methods */
void split_string_at_pos(splitted_string* self, int index, char* word);
void split(splitted_rope* self, int index, rope_node* node);
void join(rope_node* self, rope_node* r1, rope_node* r2);
void print_rope(rope_node *node);

#ifdef __cplusplus
}
#endif

#endif
