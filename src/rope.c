#include "rope.h"

void rope_node_create(rope_node* self) {
    self->left_child = NULL;
    self->right_child = NULL;
    self->word = NULL;
    self->lenght = 0;
}

void create_leaf(rope_node* self, char *str) {
    rope_node_create(self);
    self->word = str;
    self->lenght = strlen(str);
}

void rope_node_destroy(rope_node* node) {
    //    if (node == NULL) {
    //        return;
    //    }
    //
    //    rope_node_destroy(node->left_child);
    //    rope_node_destroy(node->right_child);
    //
    //    free(node);
}

void splitted_rope_create(splitted_rope *self) {
    rope_node_create(self->left_tree);
    rope_node_create(self->right_tree);
}

void splitted_rope_destroy(splitted_rope *sr) {
    free(sr);
}

void print_rope(rope_node *self, char *direction) {
    printf("\nvoy a la %s", direction);

    if (self == NULL) {
        puts(" -> nodo nulo. Vuelvo.");
        return;
    }

    print_rope(self->left_child, "izquierda");
    print_rope(self->right_child, "derecha");

    if (self->word != NULL) {
        printf("%s\n", self->word);
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

splitted_rope *split(int index, rope_node* node) {
    splitted_rope* s = NULL;

    if (node->left_child == NULL) {
        assert(index > 0 && index <= node->lenght);
        splitted_rope_create(s);
        if (index == 0) {
            s->left_tree = NULL;
            s->right_tree = node;
        } else if (index == node->lenght) {
            s->left_tree = node;
            s->right_tree = NULL;
        } else {
            splitted_string st = *split_string_at_pos(index, node->word);
            create_leaf(s->left_tree, st.first_half);
            create_leaf(s->right_tree, st.second_half);
        }
    } else if (index == node->lenght) {
        splitted_rope_create(s);
        s->left_tree = node->left_child;
        s->right_tree = node->right_child;
    } else if (index < node->lenght) {
        splitted_rope_create(s);
        splitted_rope pair = *split(index, node->left_child);
        s->left_tree = pair.left_tree;
        *s->right_tree = join(pair.right_tree, node->right_child);
    } else {
        splitted_rope_create(s);
        splitted_rope pair = *split(index - node->lenght, node->right_child);
        *s->left_tree = join(node->left_child, pair.left_tree);
        s->right_tree = pair.right_tree;
    }

    return s;
}

rope_node join(rope_node* r1, rope_node* r2) {
    rope_node joined;
    rope_node_create(&joined);

    joined.left_child = r1;
    joined.right_child = r2;
    joined.lenght = r1->lenght + r2->lenght;
    joined.word = NULL;

    return joined;
}
