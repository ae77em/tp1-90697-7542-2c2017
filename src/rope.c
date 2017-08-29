#include "rope.h"

rope_node* rope_node_create() {
    rope_node *new_node = (rope_node*) malloc(sizeof (rope_node));
    rope_node_initialize(new_node);
    return new_node;
}

void rope_node_initialize(rope_node* self) {
    self->left_child = NULL;
    self->right_child = NULL;
    self->word = NULL;
    self->weight = 0;
}

rope_node* rope_node_create_leaf(char *str) {
    rope_node *new_leaf = rope_node_create();
    size_t lenght_of_word = strlen(str) + 1;
    new_leaf->word = (char*) malloc(sizeof (char) * lenght_of_word);
    strncpy(new_leaf->word, str, lenght_of_word);
    new_leaf->weight = lenght_of_word;
    return new_leaf;
}

void rope_destroy_leaf(rope_node* self) {
    free(self->word);
    self->word = NULL;
}

void rope_node_destroy(rope_node* self) {
    if (self != NULL) {
        rope_node_destroy(self->left_child);
        rope_node_destroy(self->right_child);

        if (self->word != NULL) {
            rope_destroy_leaf(self);
        }
    }
}

splitted_rope* splitted_rope_create() {
    splitted_rope *new_splitted_rope
            = (splitted_rope*) malloc(sizeof (splitted_rope));
    new_splitted_rope->left_tree = rope_node_create();
    new_splitted_rope->right_tree = rope_node_create();

    return new_splitted_rope;
}

void splitted_rope_destroy(splitted_rope *sr) {
    rope_node_destroy(sr->left_tree);
    rope_node_destroy(sr->right_tree);
}

void print(rope_node *self) {
    if (self == NULL) {
        return;
    }

    print(self->left_child);
    print(self->right_child);

    if (self->word != NULL) {
        printf("%s", self->word);
    }
}

splitted_rope *split(int index, rope_node* node) {
    splitted_rope *pair = splitted_rope_create();

    if (node->left_child == NULL) {
        assert(index > 0 && index <= node->weight);
        if (index == 0) {
            pair->left_tree = NULL;
            pair->right_tree = node;
        } else if (index == node->weight) {
            pair->left_tree = node;
            pair->right_tree = NULL;
        } else {
            int str_lenght = node->weight;
            char left_part[index + 1];
            strncpy(left_part, node->word, index);
            left_part[index] = '\0';

            char right_part[str_lenght - index + 1];
            strncpy(right_part, node->word + index, str_lenght - index);
            right_part[str_lenght - index] = '\0';

            pair->left_tree = rope_node_create_leaf(left_part);
            pair->right_tree = rope_node_create_leaf(right_part);
        }
    } else if (index == node->weight) {
        pair->left_tree = node->left_child;
        pair->right_tree = node->right_child;
    } else if (index < node->weight) {
        splitted_rope *tmp = split(index, node->left_child);

        pair->left_tree = rope_node_create();
        pair->left_tree = tmp->left_tree;

        join(pair->right_tree, tmp->right_tree, node->right_child);

        free(tmp);
    } else {
        splitted_rope *tmp = split(index - node->weight, node->right_child);

        join(pair->left_tree, node->left_child, tmp->left_tree);

        pair->right_tree = rope_node_create();
        pair->right_tree = tmp->right_tree;

        free(tmp);
    }

    return pair;
}

void join(rope_node *parent, rope_node* left_child, rope_node* right_child) {
    parent->left_child = left_child;
    parent->right_child = right_child;
    parent->weight = calculate_weight(left_child);
    parent->word = NULL;
}

int calculate_weight(rope_node *subtree) {
    int current_weight = 0;

    if (subtree != NULL) {
        current_weight += calculate_weight(subtree->left_child);

        if (subtree->word != NULL) {
            current_weight += subtree->weight;
        } else {
            subtree->weight = current_weight;
        }

        current_weight += calculate_weight(subtree->right_child);
    }

    return current_weight;
}
