#include "rope.h"

static char *CMD_INSERT = "insert";
static char *CMD_DELETE = "delete";
static char *CMD_SPACE = "space";
static char *CMD_NEWLINE = "newline";
static char *CMD_PRINT = "print";
static char *CMD_FMT_INSERT = "\tinsert\t<pos>\t<text>";
static char *CMD_FMT_DELETE = "\tdelete\t<pos>\t<from>";
static char *CMD_FMT_SPACE = "\tspace\t<pos>";
static char *CMD_FMT_NEWLINE = "\tnewline\t<pos>";
static char *CMD_FMT_PRINT = "\tprint";
static char *SPACE = " ";
static char *NEWLINE = "\n";

void print_invalid_cmd_msg() {
    puts("El comando ingresado es inválido.");
    puts("Los comandos válidos, con sus parámetros, son:");
    puts(CMD_FMT_INSERT);
    puts(CMD_FMT_DELETE);
    puts(CMD_FMT_SPACE);
    puts(CMD_FMT_NEWLINE);
    puts(CMD_FMT_PRINT);
}

void print_invalid_param_msg(char *cmd_name, char *cmd_format) {
    puts("Cantidad de parámetros incorrecta.");
    printf("Formato de comando '%s':\n", cmd_name);
    puts(cmd_format);
}

void test_rope() {
    rope_t *tree = (rope_t *) malloc(sizeof (rope_t));
    rope_create(tree);

    append(tree, "Hola ");
    print(tree);
    insert(tree, 0, "*");
    print(tree);
    append(tree, "que tal");
    print(tree);
    insert(tree, 5, "Pepo, ");
    print(tree);
    append(tree, "?");
    print(tree);
    insert(tree, 3, "caca");
    print(tree);
    // insert(tree, 0, "¿"); // <- ocupa 2 bytes !!!
    insert(tree, 1, "-"); // <- ocupa 2 bytes !!!
    print(tree);
    insert(tree, 0, "ooloo");
    print(tree);
    append(tree, " Bien, pero estresado :(");
    print(tree);

    delete(tree, 2, 3);
    print(tree);

    delete(tree, 5, 10);
    print(tree);

    delete(tree, 14, 16);
    print(tree);

    insert(tree, 46, "|me voy a dormir...|");
    print(tree);

    rope_destroy(tree);
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
    rope_t *tree = (rope_t *) malloc(sizeof (rope_t));
    rope_create(tree);

    char cmd[16] = {0};
    int fst_param;
    char scnd_param[480] = {0};
    char line[512] = {0};
    FILE* input;

    if (argc > 1) {
        input = fopen(argv[1], "r");
    } else {
        input = stdin;
    }

    int readcounter = 0;

    while (fgets(line, sizeof (line) - 1, input) != NULL) {
        *cmd = '\0';
        fst_param = 0;
        *scnd_param = '\0';

        readcounter = sscanf(line,
                "%s %i %s",
                cmd,
                &fst_param,
                scnd_param);

        if (readcounter != EOF) { // contemplo el comando vacío...
            if (strcmp(cmd, CMD_INSERT) == 0) {
                if (readcounter == 3) {
                    insert(tree, fst_param, scnd_param);
                } else {
                    print_invalid_param_msg(CMD_INSERT, CMD_FMT_INSERT);
                }
            } else if (strcmp(cmd, CMD_DELETE) == 0) {
                if (readcounter == 3) {
                    delete(tree, fst_param, atoi(scnd_param));
                } else {
                    print_invalid_param_msg(CMD_DELETE, CMD_FMT_DELETE);
                }
            } else if (strcmp(cmd, CMD_SPACE) == 0) {
                if (readcounter == 2) {
                    insert(tree, fst_param, SPACE);
                } else {
                    print_invalid_param_msg(CMD_SPACE, CMD_FMT_SPACE);
                }
            } else if (strcmp(cmd, CMD_NEWLINE) == 0) {
                if (readcounter == 2) {
                    insert(tree, fst_param, NEWLINE);
                } else {
                    print_invalid_param_msg(CMD_NEWLINE, CMD_FMT_NEWLINE);
                }
            } else if (strcmp(cmd, CMD_PRINT) == 0) {
                if (readcounter == 1) {
                    //print(tree);
                } else {
                    print_invalid_param_msg(CMD_PRINT, CMD_FMT_PRINT);
                }
            } else {
                print_invalid_cmd_msg();
            }
        }
    }

    rope_destroy(tree);

    if (input != stdin) {
        fclose(input);
    }

    return EXIT_SUCCESS;
}
