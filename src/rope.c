#include "rope.h"

/* helpers functions declaration */
static int calculate_weight(rope_node_t *subtree);
static int calculate_positive_position(int pos, int last_pos);
static void print2(rope_node_t *subtree);
static void insert2(rope_t *self, int pos, char *str);
static void delete2(rope_t *tree, int start, int end);

static const int MAX_BUFFER_SIZE = 4096;

/* members functions */
void rope_create(rope_t* self) {
    rope_node_t *root = (rope_node_t*) malloc(sizeof (rope_node_t));
    rope_node_initialize(root);
    self->root = root;
}

void rope_destroy(rope_t *tree) {
    rope_destroy_subtree(tree->root);
    free(tree);
}

splitted_rope_t *split(rope_node_t* node, int index) {
    splitted_rope_t *pair = splitted_rope_create();

    if (node != NULL) {
        if (node->left_child == NULL) {
            assert(index >= 0 && index <= node->weight);
            if (index == 0) {
                pair->left = NULL;
                pair->right = node;
            } else if (index == node->weight) {
                pair->left = node;
                pair->right = NULL;
            } else {
                int str_lenght = node->weight;

                char left_str[MAX_BUFFER_SIZE];
                strncpy(left_str, node->word, index);
                left_str[index] = '\0';

                char right_str[MAX_BUFFER_SIZE];
                strncpy(right_str, node->word + index, str_lenght - index);
                right_str[str_lenght - index] = '\0';

                pair->left = (rope_node_t*) malloc(sizeof (rope_node_t));
                rope_node_initialize(pair->left);

                pair->right = (rope_node_t*) malloc(sizeof (rope_node_t));
                rope_node_initialize(pair->right);

                rope_node_initialize_leaf(pair->left, left_str);
                rope_node_initialize_leaf(pair->right, right_str);

                rope_node_destroy(node); // la hoja vieja la boleteamos.
            }
        } else if (index == node->weight) {
            pair->left = node->left_child;
            pair->right = node->right_child;
            rope_node_destroy(node); // queda este nodo huérfano, lo borramos.
        } else if (index < node->weight) {
            splitted_rope_t *tmp = split(node->left_child, index);

            pair->left = tmp->left;
            pair->right = (rope_node_t*) malloc(sizeof (rope_node_t));
            rope_node_initialize(pair->right);
            join(pair->right, tmp->right, node->right_child);
            rope_node_destroy(node);

            free(tmp);
        } else {
            splitted_rope_t *tmp =
                    split(node->right_child, index - node->weight);

            pair->left = (rope_node_t*) malloc(sizeof (rope_node_t));
            rope_node_initialize(pair->left);
            pair->right = tmp->right;
            join(pair->left, node->left_child, tmp->left);
            rope_node_destroy(node);

            free(tmp);
        }
    }

    return pair;
}

void join(rope_node_t *p, rope_node_t* l, rope_node_t* r) {
    p->left_child = l;
    p->right_child = r;
    p->weight = calculate_weight(l);
    p->word = NULL;
}

static int calculate_weight(rope_node_t *subtree) {
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

void insert(rope_t *self, int pos, char *str) {
    int pos2 = calculate_positive_position(pos, self->root->weight);
    assert(pos2 <= self->root->weight);

    insert2(self, pos2, str);
}

static void insert2(rope_t *self, int pos, char *str) {
    if (is_empty(self)) {
        rope_node_t *lc = (rope_node_t*) malloc(sizeof (rope_node_t));

        rope_node_initialize_leaf(lc, str);

        self->root->left_child = lc;
        self->root->weight = self->root->left_child->weight;
    } else {
        splitted_rope_t *sr = split(self->root, pos);

        rope_node_t *new_leaf = (rope_node_t*) malloc(sizeof (rope_node_t));
        rope_node_initialize_leaf(new_leaf, str);

        rope_node_t *join_root = (rope_node_t*) malloc(sizeof (rope_node_t));
        rope_node_initialize(join_root);

        rope_node_t *new_root = (rope_node_t*) malloc(sizeof (rope_node_t));
        rope_node_initialize(new_root);

        if (sr->right == NULL) {
            join(join_root, sr->left, new_leaf);
            join(new_root, join_root, NULL);
        } else if (sr->left == NULL) {
            join(join_root, new_leaf, sr->right);
            join(new_root, join_root, NULL);
        } else {
            join(join_root, sr->left, new_leaf);

            rope_node_t *join_root2 =
                    (rope_node_t*) malloc(sizeof (rope_node_t));
            rope_node_initialize(join_root2);

            join(join_root2, join_root, sr->right);
            join(new_root, join_root2, NULL);
        }

        self->root = new_root;

        free(sr);
    }
}

void delete(rope_t *self, int start, int end) {
    int start2 = calculate_positive_position(start, self->root->weight);
    int end2 = calculate_positive_position(end, self->root->weight);

    assert(start2 <= end2 && end2 <= self->root->weight);

    delete2(self, start2, end2);
}

static void delete2(rope_t *tree, int start, int end) {
    assert(start >= 0 && end <= tree->root->weight);

    if (!is_empty(tree)) {
        splitted_rope_t *sr1 = split(tree->root, start);
        splitted_rope_t *sr2 = split(sr1->right, end - start);

        // En las operaciones previas se destruye el root. Lo creo de nuevo.
        tree->root = (rope_node_t*) malloc(sizeof (rope_node_t));
        rope_node_initialize(tree->root);

        if (sr1->left == NULL) {
            join(tree->root, sr2->right, NULL);
        } else if (sr2->right == NULL) {
            join(tree->root, sr1->left, NULL);
        } else {
            rope_node_t *new_root = (rope_node_t*) malloc(sizeof (rope_node_t));

            rope_node_initialize(new_root);
            join(new_root, sr1->left, sr2->right);
            join(tree->root, new_root, NULL);
        }

        rope_destroy_subtree(sr2->left);
        free(sr2);
        free(sr1);
    }
}

void append(rope_t *tree, char *word) {
    insert(tree, tree->root->weight, word);
}

void print(rope_t *tree) {
    if (tree != NULL) {
        print2(tree->root);
        puts("");
    }
}

static void print2(rope_node_t *self) {
    if (self == NULL) {
        return;
    }

    print2(self->left_child);
    print2(self->right_child);

    if (self->word != NULL) {
        printf("%s", self->word);
    }
}

int is_empty(rope_t* self) {
    return self->root != NULL
            && self->root->left_child == NULL
            && self->root->left_child == NULL;
}

static int calculate_positive_position(int pos, int last_pos) {
    int pos2 = pos;

    if (pos2 < 0) {
        pos2 = last_pos + pos2 + 1;
    }

    return pos2;
}
