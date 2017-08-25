#include <stdio.h>
#include <stdlib.h>
#include "rope.h"

int main(void) {

    char hola1[] = "hola1";
    char hola2[] = "hola2";
    char hola3[] = "hola3";
    char hola4[] = "hola4";

    rope_node *node1 = create_rope();
    rope_node *node2 = create_rope();
    rope_node *node3 = create_rope();
    rope_node *node4 = create_rope();

    node1->word = hola1;
    node2->word = hola2;
    node3->word = hola3;
    node4->word = hola4;

    rope_node *subroot12 = join(node1, node2);
    rope_node *subroot34 = join(node3, node4);
    rope_node *root = join(subroot12, subroot34);

    print_rope(root);
    destroy_rope(root);

    return EXIT_SUCCESS;
}
