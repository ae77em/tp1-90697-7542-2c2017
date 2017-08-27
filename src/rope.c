#include "rope.h"

void rope_node_create(rope_node* self) {
    self->left_child = NULL;
    self->right_child = NULL;
    self->word = NULL;
    self->lenght = 0;
}

void rope_leaf_create(rope_node* self, char *str) {
    rope_node_create(self);
    if (strlen(str) > 0) {
        self->word = (char*) str;
        self->lenght = strlen(str);
    }
}

void rope_node_destroy(rope_node* self) {
    if (self == NULL) {
        return;
    }

    rope_node_destroy(self->left_child);
    rope_node_destroy(self->right_child);

    free(self);
}

void splitted_rope_create(splitted_rope* self) {
    rope_node left;
    rope_node_create(&left);
    self->left_tree = &left;

    rope_node right;
    rope_node_create(&right);
    self->right_tree = &right;
}

void splitted_rope_destroy(splitted_rope* self) {
    free(self);
}

void splitted_string_create(splitted_string* self) {
    self->left_string = NULL;
    self->right_string = NULL;
}

void splitted_string_destroy(splitted_string* self) {
    free(self);
}

void print_rope(rope_node* node) {
    if (node == NULL) {
        return;
    }

    print_rope(node->left_child);
    print_rope(node->right_child);

    if (node->word != NULL) {
        printf("%s", node->word);
    }
}

void split_string_at_pos(splitted_string *self, int index, char* str) {
    size_t string_lenght = strlen(str);
    char* string_to_split = str;

    if (index < string_lenght) {
        self->left_string = malloc(sizeof (char) * (index + 1));
        strncpy(self->left_string, string_to_split, index);
        self->left_string[index] = '\0';

        self->right_string = malloc(sizeof (char) * (string_lenght - index + 1));
        strncpy(self->right_string, string_to_split + index, string_lenght - index + 1);
    } else {
        self->left_string = NULL;
        self->right_string = NULL;
    }
}

splitted_rope *split_simple_node_rope(int pos, rope_node* node) {
    splitted_rope* sr = splitted_rope_create();

    rope_node* left_node = NULL;
    rope_node* right_node = NULL;

    splitted_string* ss = split_string_at_pos(pos, node->word);

    left_node = create_rope();
    right_node = create_rope();

    left_node->word = ss->left_string;
    left_node->lenght = strlen(left_node->word);
    right_node->word = ss->right_string;
    right_node->lenght = strlen(right_node->word);

    sr->left_tree = left_node;
    sr->right_tree = right_node;

    return sr;
}

splitted_rope *split(int index, rope_node* node) {
    splitted_rope* self = NULL;

    if (node->left_child == NULL) {
        assert(index > 0 && index <= node->lenght);
        splitted_rope_create(self);
        if (index == 0) {
            self->left_tree = NULL;
            self->right_tree = node;
        } else if (index == node->lenght) {
            self->left_tree = node;
            self->right_tree = NULL;
        } else {
            splitted_string st = *split_string_at_pos(index, node->word);
            self->left_tree = create_leaf(st.left_string);
            self->right_tree = create_leaf(st.right_string);
        }
    } else if (index == node->lenght) {
        splitted_rope_create(self);
        self->left_tree = node->left_child;
        self->right_tree = node->right_child;
    } else if (index < node->lenght) {
        splitted_rope_create(self);
        splitted_rope pair = *split(index, node->left_child);
        self->left_tree = pair.left_tree;
        self->right_tree = join(pair.right_tree, node->right_child);
    } else {
        splitted_rope_create(self);
        splitted_rope pair = *split(index - node->lenght, node->right_child);
        self->left_tree = join(node->left_child, pair.left_tree);
        self->right_tree = pair.right_tree;
    }

    return self;
}

void join(rope_node* self, rope_node* r1, rope_node* r2) {
    self->left_child = r1;
    self->right_child = r2;
    self->lenght = r1->lenght + r2->lenght;
    self->word = NULL;
}
