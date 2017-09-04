#include "server_rope.h"
#include "common_socket.h"

char *SPACE = " ";
char *NEWLINE = "\n";

/* helpers functions declaration */
static int calculate_weight(rope_node_t *subtree);
static int calculate_positive_position(int pos, int last_pos);
static void print2(rope_node_t *subtree);
static void sprint2(rope_node_t *subtree, socket_t *socket);
static void initialize_empty_node(rope_node_t *node);
static void insert2(rope_t *self, int pos, char *str);
static void delete2(rope_t *tree, int start, int end);

static const int MAX_BUFFER_SIZE = 4096; // pongo esto porque me lo sugiere
//                                       // el cpplint

/* members functions */
void rope_create(rope_t* self) {
    rope_node_t *root = (rope_node_t *) malloc(sizeof (rope_node_t));

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
                pair->left_tree = NULL;
                pair->right_tree = node;
            } else if (index == node->weight) {
                pair->left_tree = node;
                pair->right_tree = NULL;
            } else {
                int str_lenght = node->weight;

                char left_str[MAX_BUFFER_SIZE];
                strncpy(left_str, node->word, index);
                left_str[index] = '\0';

                char right_str[MAX_BUFFER_SIZE];
                strncpy(right_str, node->word + index, str_lenght - index);
                right_str[str_lenght - index] = '\0';

                pair->left_tree = (rope_node_t*) malloc(sizeof (rope_node_t));
                initialize_empty_node(pair->left_tree);

                pair->right_tree = (rope_node_t*) malloc(sizeof (rope_node_t));
                initialize_empty_node(pair->right_tree);

                rope_node_initialize_leaf(pair->left_tree, left_str);
                rope_node_initialize_leaf(pair->right_tree, right_str);
            }
        } else if (index == node->weight) {
            pair->left_tree = node->left_child;
            pair->right_tree = node->right_child;
        } else if (index < node->weight) {
            splitted_rope_t *tmp = split(node->left_child, index);

            pair->left_tree = tmp->left_tree;

            pair->right_tree = (rope_node_t*) malloc(sizeof (rope_node_t));

            initialize_empty_node(pair->right_tree);

            join(pair->right_tree, tmp->right_tree, node->right_child);

            free(tmp);
        } else {
            splitted_rope_t *tmp =
                    split(node->right_child, index - node->weight);

            pair->left_tree = (rope_node_t*) malloc(sizeof (rope_node_t));

            initialize_empty_node(pair->left_tree);

            pair->right_tree = tmp->right_tree;

            join(pair->left_tree, node->left_child, tmp->left_tree);

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
        initialize_empty_node(lc);
        rope_node_initialize_leaf(lc, str);

        self->root->left_child = lc;
        self->root->weight = self->root->left_child->weight;
    } else {
        splitted_rope_t *sr = split(self->root, pos);
        rope_node_t *new_leaf = (rope_node_t*) malloc(sizeof (rope_node_t));
        initialize_empty_node(new_leaf);
        rope_node_t *new_root = (rope_node_t*) malloc(sizeof (rope_node_t));

        rope_node_initialize(new_root);

        rope_node_initialize_leaf(new_leaf, str);

        if (sr->right_tree == NULL) {
            free(sr->right_tree);
            join(new_root, sr->left_tree, new_leaf);
            join(self->root, new_root, NULL);
        } else if (sr->left_tree == NULL) {
            free(sr->left_tree);
            join(new_root, new_leaf, sr->right_tree);
            join(self->root, new_root, NULL);
        } else {
            join(new_root, sr->left_tree, new_leaf);

            rope_node_t *new_root2 =
                    (rope_node_t*) malloc(sizeof (rope_node_t));
            initialize_empty_node(new_root2);

            join(new_root2, new_root, sr->right_tree);
            join(self->root, new_root2, NULL);
        }

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
        splitted_rope_t *sr2 = split(sr1->right_tree, end - start);

        if (sr1->left_tree == NULL) {
            free(sr1->left_tree);
            join(tree->root, sr2->right_tree, NULL);
        } else if (sr2->right_tree == NULL) {
            free(sr2->right_tree);
            join(tree->root, sr1->left_tree, NULL);
        } else {
            rope_node_t *new_root = (rope_node_t*) malloc(sizeof (rope_node_t));
            join(new_root, sr1->left_tree, sr2->right_tree);
            join(tree->root, new_root, NULL);
        }

        rope_destroy_subtree(sr2->left_tree);
        free(sr2);
        free(sr1);
    }
}

void space(rope_t *tree, int pos) {
    insert(tree, pos, SPACE);
}

void newline(rope_t *tree, int pos) {
    insert(tree, pos, NEWLINE);
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

void sprint(rope_t *tree, socket_t *socket) {
    if (tree != NULL) {
        sprint2(tree->root, socket);
        puts("");
    }
}

static void sprint2(rope_node_t *self, socket_t *socket) {
    if (self == NULL) {
        return;
    }

    print2(self->left_child);
    print2(self->right_child);

    if (self->word != NULL) {
        socket_send(socket, self->word, self->weight);
        //printf("%s", self->word);
    }
}

static void initialize_empty_node(rope_node_t *node) {
    node->left_child = NULL;
    node->right_child = NULL;
    node->word = NULL;
    node->weight = 0;
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
