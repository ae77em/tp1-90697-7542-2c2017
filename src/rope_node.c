#include "rope_node.h"

int id_nodo = 0;

void rope_node_create(rope_node_t *self) {
    rope_node_initialize(self);
}

void rope_node_destroy(rope_node_t *self) {
    if (self != NULL) {
        if (self->word != NULL) {
            free(self->word);
        }
        free(self);
        self = NULL;
    }
}

void rope_destroy_subtree(rope_node_t* self) {
    if (self != NULL) {
        rope_destroy_subtree(self->left_child);
        rope_destroy_subtree(self->right_child);

        rope_node_destroy(self);
    }
}

void rope_node_initialize(rope_node_t* self) {
    self->left_child = NULL;
    self->right_child = NULL;
    self->word = NULL;
    self->weight = 0;

    id_nodo++;
}

void rope_node_initialize_leaf(rope_node_t* self, char *str) {
    rope_node_initialize(self);
    size_t lenght_of_word = strlen(str);
    self->word = (char*) malloc(sizeof (char) * (lenght_of_word + 1));
    strncpy(self->word, str, lenght_of_word + 1);
    self->weight = lenght_of_word;
}
