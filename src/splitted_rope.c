#include "splitted_rope.h"

splitted_rope* splitted_rope_create() {
    splitted_rope *pair
            = (splitted_rope*) malloc(sizeof (splitted_rope));

    pair->left_tree = NULL;
    pair->right_tree = NULL;

    return pair;
}

void splitted_rope_destroy(splitted_rope *sr) {
    rope_destroy_nodes(sr->left_tree);
    rope_destroy_nodes(sr->right_tree);
}
