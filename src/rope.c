#include "rope.h"
#include <stdio.h>

rope_node *create_rope() {
    rope_node *to_return = (rope_node*) malloc(sizeof (rope_node));
    to_return->left_child = NULL;
    to_return->right_child = NULL;
    to_return->word = NULL;
    to_return->weight = 0;
    return to_return;
}

void destroy_rope(rope_node* node) {

    if (node == NULL) {
        return;
    }

    destroy_rope(node->left_child);
    destroy_rope(node->right_child);

    free(node);
}

void print_rope(rope_node* node) {

    if (node == NULL) {
        return;
    }

    print_rope(node->left_child);
    print_rope(node->right_child);

    if (node->word != NULL) {
        printf("%s ", node->word);
    }
}

splited_rope split(int i, rope_node r) {
    splited_rope s;
    return s;
}

rope_node* join(rope_node* r1, rope_node* r2) {
    rope_node *joined = create_rope();

    joined->left_child = r1;
    joined->right_child = r2;
    joined->weight = r1->weight + r2->weight;
    joined->word = NULL;

    return joined;
}
