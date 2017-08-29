#include "rope.h"

int main(void) {
    char *str1 = "Hello ";
    char *str2 = "my ";
    char *str3 = "na";
    char *str4 = "me i";
    char *str5 = "s";
    char *str6 = " Simon";

    rope_node *leaf1 = rope_node_create_leaf(str1);
    rope_node *leaf2 = rope_node_create_leaf(str2);
    rope_node *leaf3 = rope_node_create_leaf(str3);
    rope_node *leaf4 = rope_node_create_leaf(str4);
    rope_node *leaf5 = rope_node_create_leaf(str5);
    rope_node *leaf6 = rope_node_create_leaf(str6);
    rope_node *root = rope_node_create();

    puts("\n\nINICIA PRUEBA DE JOINEO");
    rope_node *subroot12 = rope_node_create();
    join(subroot12, leaf1, leaf2);

    rope_node *subroot34 = rope_node_create();
    join(subroot34, leaf3, leaf4);

    rope_node *subroot56 = rope_node_create();
    join(subroot56, leaf5, leaf6);

    rope_node *subroot3456 = rope_node_create();
    join(subroot3456, subroot34, subroot56);

    rope_node *left_root = rope_node_create();
    join(left_root, subroot12, subroot3456);

    root->left_child = left_root;
    calculate_weight(root);

    puts("imprimo arbol desde raíz");
    print(root);

    puts("\n\nFINALIZA PRUEBA DE JOINEO");
    puts("=========================");
    puts("\n\nINICIA PRUEBA DE SPLIT");

    splitted_rope *sr = split(3, root);

    puts("imprimo arbol espliteado izquierdo");
    print(sr->left_tree);
    puts("\nimprimo arbol espliteado derecho");
    print(sr->right_tree);

    puts("\n\nFINALIZA PRUEBA DE SPLIT");
    puts("\n=========================");

    puts("\n\nPRUEBA REJOINEADO");

    join(root, sr->left_tree, sr->right_tree);

    puts("imprimo arbol rejoineado");
    print(root);

    puts("\nFINALIZA PRUEBA REJOINEADO");
    puts("\n=========================");

    splitted_rope_destroy(sr);
    rope_node_destroy(root);

    free(leaf1);
    leaf1 = NULL;
    free(leaf2);
    leaf2 = NULL;
    free(leaf3);
    leaf3 = NULL;
    free(leaf4);
    leaf4 = NULL;
    free(leaf5);
    leaf5 = NULL;
    free(leaf6);
    leaf6 = NULL;
    free(subroot12);
    subroot12 = NULL;
    free(subroot34);
    subroot34 = NULL;
    free(subroot56);
    subroot56 = NULL;
    free(subroot3456);
    subroot3456 = NULL;
    free(left_root);
    left_root = NULL;
    free(root);
    root = NULL;
    //free(sr->left_tree);
    //free(sr->right_tree);
    free(sr);

    return EXIT_SUCCESS;
}
