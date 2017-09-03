#include "splitted_rope.h"

splitted_rope_t* splitted_rope_create() {
    splitted_rope_t *pair
            = (splitted_rope_t*) malloc(sizeof (splitted_rope_t));

    pair->left_tree = NULL;
    pair->right_tree = NULL;

    return pair;
}

void splitted_rope_destroy(splitted_rope_t *sr) {
    rope_destroy_subtree(sr->left_tree);
    rope_destroy_subtree(sr->right_tree);
}
