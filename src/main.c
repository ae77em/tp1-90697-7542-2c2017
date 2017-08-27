#include <stdio.h>
#include <stdlib.h>
#include "rope.h"

int main(void) {
    char* str1 = "para esplitear";
    char* str2 = "_hola2";
    char* str3 = "_hola3";
    char* str4 = "_hola4";

    rope_node node1;
    rope_leaf_create(&node1, str1);
    rope_node node2;
    rope_leaf_create(&node2, str2);
    rope_node node3;
    rope_leaf_create(&node3, str3);
    rope_node node4;
    rope_leaf_create(&node4, str4);


    rope_node subroot12;
    join(&subroot12, &node1, &node2);
    rope_node subroot34;
    join(&subroot34, &node3, &node4);
    rope_node root;
    join(&root, &subroot12, &subroot34);

    printf("Imprimo primer arbol armado:\n");
    print_rope(&root);
    printf("\nFin primer arbol\n");

    printf("\n\nEspliteo primer arbol\n");
    splitted_rope sr0;
    splitted_rope_create(&sr0);
    split(&sr0, 2, &root);
    printf("\nArbol espliteado izquierdo\n");
    print_rope(sr0.left_tree);
    printf("\nArbol espliteado derecho\n");
    print_rope(sr0.right_tree);
    printf("\nFin primer arbol espliteado\n");

    join(&root, sr0.left_tree, sr0.right_tree);
    printf("Imprimo primer rejoineado:\n");
    print_rope(&root);
    printf("\nFin primer arbol rejoineado\n");

    rope_node_destroy(&root);
    splitted_rope_destroy(&sr0);

    rope_node_create(&node1);
    node1.word = str1;

    splitted_rope sr;
    split(&sr, 2, &node1);

    printf("\n\nImprimo arbol espliteado:\n");
    printf("Arbol izquierdo:\n");
    print_rope(sr.left_tree);
    printf("\nArbol derecho:\n");
    print_rope(sr.right_tree);
    printf("\nFin arbol espliteado\n");

    join(&root, sr.left_tree, sr.right_tree);

    printf("\n\nImprimo arbol espliteado vuelto a joinear:\n");
    print_rope(&root);
    printf("\nFin arbol joineado\n");

    rope_node_destroy(&root);

    splitted_rope_destroy(&sr);

    return EXIT_SUCCESS;
}
