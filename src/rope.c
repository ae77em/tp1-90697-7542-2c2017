#include "rope.h"

/* helpers functions declaration */
static int calculate_weight(rope_node *subtree);
static int calculate_positive_position(int pos, int last_pos);
static void print2(rope_node *subtree);
static void initialize_empty_node(rope_node *node);
static void insert2(rope *self, int pos, char *str);
static void delete2(rope *tree, int start, int end);

static const int MAX_BUFFER_SIZE = 4096; // pongo esto porque me lo sugiere
//                                       // el cpplint

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

splitted_rope *split(rope_node* node, int index) {
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

                char left_str[MAX_BUFFER_SIZE];
                strncpy(left_str, node->word, index);
                left_str[index] = '\0';

                char right_str[MAX_BUFFER_SIZE];
                strncpy(right_str, node->word + index, str_lenght - index);
                right_str[str_lenght - index] = '\0';

                pair->left_tree = (rope_node*) malloc(sizeof (rope_node));
                initialize_empty_node(pair->left_tree);
                pair->right_tree = (rope_node*) malloc(sizeof (rope_node));
                initialize_empty_node(pair->right_tree);

                rope_node_initialize_leaf(pair->left_tree, left_str);
                rope_node_initialize_leaf(pair->right_tree, right_str);
            }
        } else if (index == node->weight) {
            pair->left_tree = node->left_child;
            pair->right_tree = node->right_child;
        } else if (index < node->weight) {
            splitted_rope *tmp = split(node->left_child, index);

            pair->left_tree = tmp->left_tree;
            //pair->left_tree->weight = calculate_weight(pair->left_tree->left_child);

            pair->right_tree = (rope_node*) malloc(sizeof (rope_node));
            initialize_empty_node(pair->right_tree);
            //rope_node_create(pair->right_tree);

            join(pair->right_tree, tmp->right_tree, node->right_child);

            free(tmp);
        } else {
            splitted_rope *tmp = split(node->right_child, index - node->weight);

            pair->left_tree = (rope_node*) malloc(sizeof (rope_node));
            initialize_empty_node(pair->left_tree);

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

    int pos2 = calculate_positive_position(pos, self->root->weight);

    assert(pos2 <= self->root->weight);

    insert2(self, pos2, str);
}

static void insert2(rope *self, int pos, char *str) {
    if (is_empty(self)) {
        rope_node *lc = (rope_node*) malloc(sizeof (rope_node));
        initialize_empty_node(lc);
        rope_node_initialize_leaf(lc, str);

        self->root->left_child = lc;
        self->root->weight = self->root->left_child->weight;
    } else {
        splitted_rope *sr = split(self->root, pos);
        rope_node *new_leaf = (rope_node*) malloc(sizeof (rope_node));
        initialize_empty_node(new_leaf);
        rope_node *new_root = (rope_node*) malloc(sizeof (rope_node));
        initialize_empty_node(new_root);

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

            rope_node *new_root2 = (rope_node*) malloc(sizeof (rope_node));
            initialize_empty_node(new_root2);

            join(new_root2, new_root, sr->right_tree);
            join(self->root, new_root2, NULL);
        }

        free(sr);
    }
}

void delete(rope *self, int start, int end) {
    int start2 = calculate_positive_position(start, self->root->weight);
    int end2 = calculate_positive_position(end, self->root->weight);

    assert(start2 <= end2 && end2 <= self->root->weight);

    delete2(self, start2, end2);
}

static void delete2(rope *tree, int start, int end) {
    assert(start >= 0 && end <= tree->root->weight);

    if (!is_empty(tree)) {
        splitted_rope *sr1 = split(tree->root, start);
        splitted_rope *sr2 = split(sr1->right_tree, end - start);

        rope_node *new_root = (rope_node*) malloc(sizeof (rope_node));

        join(new_root, sr1->left_tree, sr2->right_tree);
        join(tree->root, new_root, NULL);

        free(sr2);
        free(sr1);
    }
}

void append(rope *tree, char *word) {
    insert(tree, tree->root->weight, word);
}

void print(rope *tree) {
    if (tree != NULL) {
        print2(tree->root);
        puts("");
    }
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

    if (self->weight == 0) {
        //puts("nodo con peso 0!!");
    }
}

static void initialize_empty_node(rope_node *node) {
    node->left_child = NULL;
    node->right_child = NULL;
    node->word = NULL;
    node->weight = 0;
}

int is_empty(rope* self) {
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
