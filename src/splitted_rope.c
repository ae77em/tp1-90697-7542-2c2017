#include "splitted_rope.h"

splitted_rope* splitted_rope_create() {
    splitted_rope *new_splitted_rope
            = (splitted_rope*) malloc(sizeof (splitted_rope));

    return new_splitted_rope;
}

void splitted_rope_destroy(splitted_rope *sr) {
    rope_destroy_nodes(sr->left_tree);
    rope_destroy_nodes(sr->right_tree);
}
