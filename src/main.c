#include "rope.h"

int main(void) {
    char str1[] = "Hello ";
    char str2[] = "my ";
    char str3[] = "na";
    char str4[] = "me i";
    char str5[] = "s";
    char str6[] = " Simon";

    rope_node leaf1;
    rope_node leaf2;
    rope_node leaf3;
    rope_node leaf4;
    rope_node leaf5;
    rope_node leaf6;
    rope_node root;

    rope_node_create_leaf(&leaf1, str1);
    rope_node_create_leaf(&leaf2, str2);
    rope_node_create_leaf(&leaf3, str3);
    rope_node_create_leaf(&leaf4, str4);
    rope_node_create_leaf(&leaf5, str5);
    rope_node_create_leaf(&leaf6, str6);
    rope_node_create(&root);

    puts("\n\nINICIA PRUEBA DE JOINEO");
    rope_node subroot12;
    join(&subroot12, &leaf1, &leaf2);
    rope_node subroot34;
    join(&subroot34, &leaf3, &leaf4);
    rope_node subroot56;
    join(&subroot56, &leaf5, &leaf6);
    rope_node subroot3456;
    join(&subroot3456, &subroot34, &subroot56);
    rope_node left_root;
    join(&left_root, &subroot12, &subroot3456);
    root.left_child = &left_root;
    calculate_weight(&root);

    print(&root, "raíz");

    puts("\n\nFINALIZA PRUEBA DE JOINEO");
    puts("=========================");
    puts("\n\nINICIA PRUEBA DE SPLIT");

    splitted_rope sr;
    splitted_rope_create(&sr);

    split(&sr, 3, &root);

    print(sr.left_tree, "root de arbol espliteado izquierdo");
    print(sr.right_tree, "root de arbol espliteado derecho");

    puts("\n\nFINALIZA PRUEBA DE SPLIT");
    puts("\n=========================");

    rope_node_destroy(&left_root);

    return EXIT_SUCCESS;
}
