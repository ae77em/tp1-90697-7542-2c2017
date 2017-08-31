#include "rope.h"

/* helpers functions declaration */
static int calculate_weight(rope_node *subtree);
static void print2(rope_node *subtree);

/* members functions */
void rope_create(rope* self) {
    rope_node *root = (rope_node *) malloc(sizeof (rope_node));
    rope_node_initialize(root);
    self->root = root;
}

void rope_destroy(rope *tree) {
    rope_destroy_nodes(tree->root);
    free(tree);
}

splitted_rope *split(int index, rope_node* node) {
    splitted_rope *pair = splitted_rope_create();

    if (node != NULL) {
        if (node->left_child == NULL) {
            assert(index >= 0 && index <= node->weight);
            if (index == 0) {
                pair->left_tree = NULL;
                pair->right_tree = node;
            } else if (index == node->weight) {
                pair->left_tree = node;
                pair->right_tree = NULL;
            } else {
                int str_lenght = node->weight;
                char left_str[index + 1];
                strncpy(left_str, node->word, index);
                left_str[index] = '\0';

                char right_str[str_lenght - index + 1];
                strncpy(right_str, node->word + index, str_lenght - index);
                right_str[str_lenght - index] = '\0';

                pair->left_tree = (rope_node*) malloc(sizeof (rope_node));
                pair->right_tree = (rope_node*) malloc(sizeof (rope_node));

                rope_node_initialize_leaf(pair->left_tree, left_str);
                rope_node_initialize_leaf(pair->right_tree, right_str);
            }
        } else if (index == node->weight) {
            pair->left_tree = node->left_child;
            pair->right_tree = node->right_child;
        } else if (index < node->weight) {
            splitted_rope *tmp = split(index, node->left_child);

            pair->left_tree = tmp->left_tree;

            pair->right_tree = (rope_node*) malloc(sizeof (rope_node));
            rope_node_create(pair->right_tree);

            join(pair->right_tree, tmp->right_tree, node->right_child);

            free(tmp);
        } else {
            splitted_rope *tmp = split(index - node->weight, node->right_child);

            pair->left_tree = (rope_node*) malloc(sizeof (rope_node));
            rope_node_create(pair->left_tree);

            pair->right_tree = tmp->right_tree;

            join(pair->left_tree, node->left_child, tmp->left_tree);

            free(tmp);
        }
    }

    return pair;
}

void join(rope_node *parent, rope_node* left_child, rope_node* right_child) {
    parent->left_child = left_child;
    parent->right_child = right_child;
    parent->weight = calculate_weight(left_child);
    parent->word = NULL;
}

static int calculate_weight(rope_node *subtree) {
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

void insert(rope *self, int pos, char *str) {
    if (is_empty(self)) {
        self->root->left_child = (rope_node*) malloc(sizeof (rope_node));
        rope_node_initialize_leaf(self->root->left_child, str);
        self->root->weight = self->root->left_child->weight;
    } else {
        splitted_rope *sr = split(pos, self->root);
        rope_node *new_leaf = (rope_node*) malloc(sizeof (rope_node));
        rope_node *new_root = (rope_node*) malloc(sizeof (rope_node));
        rope_node *parent_first_join = (rope_node*) malloc(sizeof (rope_node));
        rope_node *parent_second_join = (rope_node*) malloc(sizeof (rope_node));

        rope_node_initialize_leaf(new_leaf, str);
        rope_node_initialize(new_root);
        rope_node_initialize(parent_first_join);
        rope_node_initialize(parent_second_join);

        join(parent_first_join, sr->left_tree, new_leaf);
        join(parent_second_join, parent_first_join, sr->right_tree);
        join(new_root, parent_second_join, NULL);

        self->root = new_root;

        free(sr);
    }
}

void delete(rope_node *tree, int start, int end) {

}

void append(rope *tree, char *word) {
    insert(tree, tree->root->weight, word);
}

void print(rope *tree) {
    print2(tree->root);
    puts("");
}

static void print2(rope_node *self) {
    if (self == NULL) {
        return;
    }

    print2(self->left_child);
    print2(self->right_child);

    if (self->word != NULL) {
        printf("%s", self->word);
    }
}

int is_empty(rope* self) {
    return self->root->left_child == NULL && self->root->left_child == NULL;
}
