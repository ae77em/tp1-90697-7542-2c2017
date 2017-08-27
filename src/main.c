#include "rope.h"

int main(void) {
    char hola1[] = "hola1";
    char hola2[] = "hola2";
    char hola3[] = "hola3";
    char hola4[] = "hola4";

    rope_node node1;
    rope_node node2;
    rope_node node3;
    rope_node node4;

    rope_node_create(&node1);
    rope_node_create(&node2);
    rope_node_create(&node3);
    rope_node_create(&node4);

    node1.word = hola1;
    node2.word = hola2;
    node3.word = hola3;
    node4.word = hola4;

    printf("\n\nINICIA PRUEBA DE JOINEO");
    rope_node subroot12 = join(&node1, &node2);
    rope_node subroot34 = join(&node3, &node4);
    rope_node root = join(&subroot12, &subroot34);

    print_rope(&root, "raíz");
    printf("\n\nFINALIZA PRUEBA DE JOINEO");
    printf("\n=========================");
    printf("\n\nINICIA PRUEBA DE SPLIT");
    printf("\n\n NO HAGO NADA (POR AHORA)");
    printf("\n\nFINALIZA PRUEBA DE SPLIT");
    printf("\n=========================");

    rope_node_destroy(&root);

    return EXIT_SUCCESS;
}
