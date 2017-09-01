#include "rope_node.h"

void rope_node_create(rope_node *self) {
    rope_node_initialize(self);
}

void rope_node_destroy(rope_node *self) {
    if (self->word != NULL) {
        free(self->word);
    }
    free(self);
}

void rope_destroy_nodes(rope_node* self) {
    if (self != NULL) {
        rope_destroy_nodes(self->left_child);
        rope_destroy_nodes(self->right_child);

        rope_node_destroy(self);
    }
}

void rope_node_initialize(rope_node* self) {
    self->left_child = NULL;
    self->right_child = NULL;
    self->word = NULL;
    self->weight = 0;
}

void rope_node_initialize_leaf(rope_node* self, char *str) {
    size_t lenght_of_word = strlen(str);
    self->word = (char*) malloc(sizeof (char) * (lenght_of_word + 1));
    strncpy(self->word, str, lenght_of_word + 1);
    self->weight = lenght_of_word;
}

void rope_node_create_leaf(rope_node *self, char *str) {
    self = (rope_node*) malloc(sizeof (rope_node));
    rope_node_create(self);
    rope_node_initialize_leaf(self, str);
}
