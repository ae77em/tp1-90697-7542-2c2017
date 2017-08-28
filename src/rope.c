#include "rope.h"

void rope_node_create(rope_node* self) {
    self->left_child = NULL;
    self->right_child = NULL;
    self->word = NULL;
    self->weight = 0;
}

void rope_node_create_leaf(rope_node* self, char *str) {
    rope_node_create(self);
    self->word = str;
    self->weight = strlen(str);
}

void rope_node_destroy(rope_node* node) {
    // si se hace, es recorrer todo el arbol y fritar los nodos...
}

void splitted_rope_create(splitted_rope *self) {
    rope_node left;
    rope_node right;

    rope_node_create(&left);
    rope_node_create(&right);

    self->left_tree = &left;
    self->right_tree = &right;
}

void splitted_rope_destroy(splitted_rope *sr) {
    free(sr);
}

void print(rope_node *self, char *direction) {
    printf("\nvoy a la %s", direction);

    if (self == NULL) {
        puts(" -> nodo nulo. Vuelvo.");
        return;
    }

    print(self->left_child, "izquierda");
    print(self->right_child, "derecha");

    if (self->word != NULL) {
        printf("Palabra: %s \t\t Peso: %d\n", self->word, self->weight);
    } else {
        printf("Palabra: VACIO \t\t Peso: %d\n", self->weight);
    }
}

splitted_string *split_string_at_pos(int pos, char* s) {
    splitted_string* ss = (splitted_string*) malloc(sizeof (splitted_string));
    size_t string_lenght = strlen(s);

    if (pos < string_lenght) {
        ss->first_half = malloc(sizeof (char) * (pos + 1));
        strncpy(ss->first_half, s, pos);
        ss->first_half[pos] = '\0';

        ss->second_half = malloc(sizeof (char) * (string_lenght - pos + 1));
        strncpy(ss->second_half, s + pos, string_lenght - pos + 1);
    } else {
        ss->first_half = NULL;
        ss->second_half = NULL;
    }

    return ss;
}

void split(splitted_rope *pair, int index, rope_node* node) {
    splitted_rope_create(pair);

    if (node->left_child == NULL) {
        assert(index > 0 && index <= node->weight);
        if (index == 0) {
            pair->left_tree = NULL;
            pair->right_tree = node;
        } else if (index == node->weight) {
            pair->left_tree = node;
            pair->right_tree = NULL;
        } else {
            splitted_string st = *split_string_at_pos(index, node->word);

            rope_node left;
            rope_node right;

            rope_node_create(&left);
            rope_node_create(&right);

            pair->left_tree = &left;
            pair->right_tree = &right;

            rope_node_create_leaf(pair->left_tree, st.first_half);
            rope_node_create_leaf(pair->right_tree, st.second_half);
        }
    } else if (index == node->weight) {
        pair->left_tree = node->left_child;
        pair->right_tree = node->right_child;
    } else if (index < node->weight) {
        splitted_rope tmp;
        split(&tmp, index, node->left_child);
        pair->left_tree = tmp.left_tree;
        join(pair->right_tree, (& tmp)->right_tree, node->right_child);
        //print_rope(node->right_child, "node->right_child");
    } else {
        splitted_rope tmp;
        split(&tmp, index - node->weight, node->right_child);
        join(pair->left_tree, node->left_child, tmp.left_tree);
        pair->right_tree = tmp.right_tree;
    }
}

void join(rope_node *parent, rope_node* left_child, rope_node* right_child) {
    parent->left_child = left_child;
    parent->right_child = right_child;
    parent->weight = calculate_weight(*&left_child);
    parent->word = NULL;
}

int calculate_weight(rope_node *subtree) {
    int current_weight = 0;

    if (subtree != NULL) {
        current_weight += calculate_weight(subtree->left_child);

        if (subtree->word != NULL) {
            subtree->weight = strlen(subtree->word); // TODO: sacar cuando esté
            //                                          seguro de que las hojas
            //                                          siempre se crean con
            //                                          peso.
            current_weight += subtree->weight;
        } else {
            subtree->weight = current_weight;
        }

        current_weight += calculate_weight(subtree->right_child);
    }

    return current_weight;
}
