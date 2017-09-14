#include "client_main.h"

char *CMD_INSERT = "insert";
char *CMD_DELETE = "delete";
char *CMD_SPACE = "space";
char *CMD_NEWLINE = "newline";
char *CMD_PRINT = "print";
char *CMD_FMT_INSERT = "\tinsert\t<pos>\t<text>";
char *CMD_FMT_DELETE = "\tdelete\t<pos>\t<from>";
char *CMD_FMT_SPACE = "\tspace\t<pos>";
char *CMD_FMT_NEWLINE = "\tnewline\t<pos>";
char *CMD_FMT_PRINT = "\tprint";

static void do_client_proccessing(char *host, unsigned short port, FILE* file);
static void send_insert(socket_t *sock, int fst_param, char *scnd_param);
static void send_delete(socket_t *sock, int fst_param, int scnd_param);
static void send_space(socket_t *sock, int fst_param);
static void send_newline(socket_t *sock, int fst_param);
static void send_print(socket_t *sock);
static void print_invalid_cmd_msg();
static void print_invalid_param_msg(char *cmd_name, char *cmd_format);

/*
 * posibles parámetros:
 *
 * insert​ ​ <pos>​ ​ <text>
 * delete​ ​ <to>​ ​ <from>
 * space​ ​ <pos>
 * newline​ ​ <pos>
 * print
 *------------------------------------------
 * Los pasos para establecer un socket del lado del cliente son:
 * 1. Crear un socket con la función socket()
 * 2. Enlazar el socket a la dirección del servidor utilizando connect()
 * 3. Enviar y recibir datos con send() y recv()
 * 4. Cerrar todos los sockets debidamente con close() al finalizar.
 */
int client_main(int argc, char *argv[]) {
    FILE* input;
    unsigned short port;

    if (argc < 4 || argc > 5) {
        puts("La cantidad de parámetros es incorrecta.");
        puts("El formato de ejecución del client es:");
        puts("    ./tp client <host> <port> [<input>]");
        return EXIT_FAILURE;
    }

    if (atoi(argv[3]) == 0) {
        puts("El puerto indicado no es correcto.");
        return EXIT_FAILURE;
    } else {
        port = atoi(argv[3]);
    }

    if (argc == 5) {
        input = fopen(argv[4], "r");
    } else {
        input = stdin;
    }

    do_client_proccessing(argv[2], port, input);

    if (input != stdin) {
        fclose(input);
    }

    return EXIT_SUCCESS;
}

static void do_client_proccessing(char *host, unsigned short port, FILE* file) {
    char cmd[16] = {0};
    int fst_param;
    char scnd_param[480] = {0};
    char line[512] = {0};

    int readcounter = 0;

    socket_t *client_socket = (socket_t*) malloc(sizeof (socket_t));

    socket_create(client_socket);

    if (socket_connect(client_socket, host, port) != EXIT_SUCCESS) {
        socket_destroy(client_socket);
        free(client_socket);
        return;
    }

    while (fgets(line, sizeof (line) - 1, file) != NULL) {
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
                    send_insert(client_socket, fst_param, scnd_param);
                } else {
                    print_invalid_param_msg(CMD_INSERT, CMD_FMT_INSERT);
                }
            } else if (strcmp(cmd, CMD_DELETE) == 0) {
                if (readcounter == 3) {
                    send_delete(client_socket, fst_param, atoi(scnd_param));
                } else {
                    print_invalid_param_msg(CMD_DELETE, CMD_FMT_DELETE);
                }
            } else if (strcmp(cmd, CMD_SPACE) == 0) {
                if (readcounter == 2) {
                    send_space(client_socket, fst_param);
                } else {
                    print_invalid_param_msg(CMD_SPACE, CMD_FMT_SPACE);
                }
            } else if (strcmp(cmd, CMD_NEWLINE) == 0) {
                if (readcounter == 2) {
                    send_newline(client_socket, fst_param);
                } else {
                    print_invalid_param_msg(CMD_NEWLINE, CMD_FMT_NEWLINE);
                }
            } else if (strcmp(cmd, CMD_PRINT) == 0) {
                if (readcounter == 1) {
                    send_print(client_socket);
                } else {
                    print_invalid_param_msg(CMD_PRINT, CMD_FMT_PRINT);
                }
            } else {
                print_invalid_cmd_msg();
            }
        }
    }

    socket_shutdown_send(client_socket);

    unsigned short msg_length = 0;
    int recv = 0;
    char response_buffer[MAX_DATA_BUFFER];
    bool the_end_is_here = false;

    while (!the_end_is_here
            && (recv = socket_receive(client_socket,
            (char*) &msg_length,
            _SIZE_OF_SHORT_)) > 0) {
        msg_length = ntohs(msg_length);

        recv = socket_receive(client_socket, response_buffer, msg_length);
        the_end_is_here = (recv <= 0);

        if (!the_end_is_here) {
            response_buffer[msg_length] = '\0';
            printf("%s", response_buffer);
        }
    }

    socket_shutdown(client_socket);
    socket_destroy(client_socket);
    free(client_socket);
}

static void send_insert(socket_t *sock, int fst_param, char *scnd_param) {
    int size_struct = sizeof (struct insert_command_t);
    size_struct -= sizeof (char*);
    int size_string = strlen(scnd_param);
    char buffer[MAX_DATA_BUFFER];

    int msg_lenght = size_struct + size_string;

    struct insert_command_t sc = {
        .msg_lenght = htonl(msg_lenght - _SIZE_OF_UINT32_),
        .opcode = htonl(OPCODE_INSERT),
        .pos = htonl(fst_param),
        .size = htons(size_string)
    };

    memcpy((void*) buffer, (void*) &sc, size_struct);
    memcpy((void*) (buffer + size_struct), (void*) scnd_param, size_string);

    socket_send(sock, buffer, msg_lenght);
}

static void send_delete(socket_t *sock, int fst_param, int scnd_param) {
    int size = sizeof (struct delete_command_t);
    char buffer[MAX_DATA_BUFFER];

    struct delete_command_t sc = {
        .msg_lenght = htonl(size - _SIZE_OF_UINT32_),
        .opcode = htonl(OPCODE_DELETE),
        .from = htonl(fst_param),
        .to = htonl(scnd_param)
    };

    memcpy((void*) buffer, (void*) &sc, size);
    socket_send(sock, buffer, size);
}

static void send_space(socket_t *sock, int fst_param) {
    int size = sizeof (struct space_command_t);
    char buffer[MAX_DATA_BUFFER];

    struct space_command_t sc = {
        .msg_lenght = htonl(size - _SIZE_OF_UINT32_),
        .opcode = htonl(OPCODE_SPACE),
        .pos = htonl(fst_param)
    };

    memcpy((void*) buffer, (void*) &sc, size);
    socket_send(sock, buffer, size);
}

static void send_newline(socket_t *sock, int fst_param) {
    int size = sizeof (struct newline_command_t);
    char buffer[MAX_DATA_BUFFER];

    struct newline_command_t nc = {
        .msg_lenght = htonl(size - _SIZE_OF_UINT32_),
        .opcode = htonl(OPCODE_NEWLINE),
        .pos = htonl(fst_param)
    };

    memcpy((void*) buffer, (void*) &nc, size);
    socket_send(sock, buffer, size);
}

static void send_print(socket_t * sock) {
    int size = sizeof (struct print_command_t);
    char buffer[MAX_DATA_BUFFER];

    struct print_command_t pc = {
        .msg_lenght = htonl(size - _SIZE_OF_UINT32_),
        .opcode = htonl(OPCODE_PRINT)
    };

    memcpy((void*) buffer, (void*) &pc, size);
    socket_send(sock, buffer, size);
}

static void print_invalid_cmd_msg() {
    puts("El comando ingresado es inválido.");
    puts("Los comandos válidos, con sus parámetros, son:");
    puts(CMD_FMT_INSERT);
    puts(CMD_FMT_DELETE);
    puts(CMD_FMT_SPACE);
    puts(CMD_FMT_NEWLINE);
    puts(CMD_FMT_PRINT);
}

static void print_invalid_param_msg(char *cmd_name, char *cmd_format) {
    puts("Cantidad de parámetros incorrecta.");
    printf("Formato de comando '%s':\n", cmd_name);
    puts(cmd_format);
}
