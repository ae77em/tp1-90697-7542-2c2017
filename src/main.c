#include "rope.h"

static char *cmd_fmt_insert = "\tinsert\t<pos>\t<text>";
static char *cmd_fmt_delete = "\tdelete\t<pos>\t<from>";
static char *cmd_fmt_space = "\tspace\t<pos>";
static char *cmd_fmt_newline = "\tnewline\t<pos>";
static char *cmd_fmt_print = "\tprint";

void print_invalid_command_message() {
    puts("El comando ingresado es inválido.");
    puts("Los comandos válidos, con sus parámetros, son:");
    puts(cmd_fmt_insert);
    puts(cmd_fmt_delete);
    puts(cmd_fmt_space);
    puts(cmd_fmt_newline);
    puts(cmd_fmt_print);

}

void print_invalid_parameters_message(char *cmd_name, char *cmd_format) {
    puts("Cantidad de parámetros incorrecta.");
    printf("Formato de comando '%s':\n", cmd_name);
    puts(cmd_format);
}

/*
 * posibles parámetros:
 *
 * insert​ ​ <pos>​ ​ <text>
 * delete​ ​ <to>​ ​ <from>
 * space​ ​ <pos>
 * newline​ ​ <pos>
 * print
 */
int main(int argc, char *argv[]) {

    int status = EXIT_FAILURE;

    char arguments[argc][255];

    for (int i = 1; i < argc; i++) {
        sscanf(argv[i], "%s", arguments[i - 1]);
        puts(arguments[i - 1]);
    }

    if (strcmp(arguments[0], "insert") == 0) {
        if (argc == 4) {
            puts("comando insert");
            status = EXIT_SUCCESS;
        } else {
            print_invalid_parameters_message("insert", cmd_fmt_insert);
        }
    } else if (strcmp(arguments[0], "delete") == 0) {
        if (argc == 4) {
            puts("comando delete");
            status = EXIT_SUCCESS;
        } else {
            print_invalid_parameters_message("delete", cmd_fmt_delete);
        }
    } else if (strcmp(arguments[0], "space") == 0) {
        if (argc == 3) {
            puts("comando space");
            status = EXIT_SUCCESS;
        } else {
            print_invalid_parameters_message("insert", cmd_fmt_delete);
        }
    } else if (strcmp(arguments[0], "newline") == 0) {
        if (argc == 3) {
            puts("comando newline");
            status = EXIT_SUCCESS;
        } else {
            print_invalid_parameters_message("insert", cmd_fmt_newline);
        }
    } else if (strcmp(arguments[0], "print") == 0) {
        if (argc == 2) {
            puts("comando print");
            status = EXIT_SUCCESS;
        } else {
            print_invalid_parameters_message("insert", cmd_fmt_print);
        }
    } else {
        print_invalid_command_message();
    }

    char str1[] = "Hola ";
    char str2[] = "que tal";
    char str3[] = "Pepo, ";
    char str4[] = "?";
    char str5[] = "¿";

    rope *tree = malloc(sizeof (rope));
    rope_create(tree);

    append(tree, str1);
    print(tree);
    append(tree, str2);
    print(tree);
    insert(tree, 5, str3);
    print(tree);
    append(tree, str4);
    print(tree);
    insert(tree, 0, str5);
    print(tree);
    insert(tree, 7, "\n***\n");
    print(tree);

    rope_destroy(tree);

    return status;
}
