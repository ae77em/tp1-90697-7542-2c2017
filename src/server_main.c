#include "server_main.h"
#include "server_rope.h"

/*
 * Los pasos para establecer un socket del lado del servidor son:
 * 1. Crear un socket con la función socket()
 * 2. Enlazar el socket a una dirección utilizando la función bind().
 * Para un socket en internet, la dirección consiste en un numero de puerto
 * en la maquina donde es ejecutado.
 * 3. Escuchar por conexiones con la función listen()
 * 4. Aceptar conexiones con la función accept()
 * 5. Enviar y recibir datos con send() y recv()
 * 6. Finalizar la transmisión en el socket que escucha conexiones con
 * shutdown()
 * 7. Cerrar todos los sockets debidamente con close() al finalizar.
 */

int server_main(int argc, char *argv[]) {

    if (argc != 3) {
        puts("La cantidad de parámetros es incorrecta.");
        puts("El formato de ejecución del server es:");
        puts("    ./tp server <port>");
        return EXIT_FAILURE;
    }

    unsigned short port;

    if (atoi(argv[2]) == 0) {
        puts("El puerto indicado no es correcto.");
        return EXIT_FAILURE;
    } else {
        port = atoi(argv[2]);
    }

    rope_t *rope = (rope_t*) malloc(sizeof (rope_t));
    rope_create(rope);

    socket_t * server_socket = (socket_t*) malloc(sizeof (socket_t));
    socket_t * accepted_socket = (socket_t*) malloc(sizeof (socket_t));

    socket_create(server_socket);
    socket_bind_and_listen(server_socket, port);
    socket_accept(server_socket, accepted_socket);

    char *buffer = (char*) malloc(sizeof (char));
    int received;

    received = socket_receive(accepted_socket, buffer, MAX_RECV_BUFFER);

    puts("----------");
    for (int x = 0; x < received; x++) {
        printf("%04x\n", buffer[x]);
    }
    puts("");
    if (received > 0) {
        int ptr_shift = 0;
        int opcode;
        int opcode_size = sizeof (int);
        int pos;
        int pos_size = sizeof (int);

        while (buffer[ptr_shift] != 4) { // EOT

            opcode = 7; // lo seteo con una op invalida
            pos = -9999;

            memcpy((void*) &opcode, (void*) (buffer + ptr_shift), opcode_size);

            switch (opcode) {
                case OPCODE_INSERT:
                    ptr_shift += opcode_size;

                    memcpy((void*) &pos, (void*) (buffer + ptr_shift), pos_size);

                    ptr_shift += pos_size;
                    int size;
                    int size_size = sizeof (unsigned short);
                    memcpy((void*) &size, (void*) (buffer + ptr_shift), size_size);

                    ptr_shift += size_size;
                    char *text = malloc(sizeof (char));
                    strncpy(text, buffer + ptr_shift, size);
                    ptr_shift += size;

                    insert(rope, pos, text);

                    free(text);
                    break;
                case OPCODE_DELETE:
                    ptr_shift += opcode_size;
                    memcpy((void*) &pos, (void*) (buffer + ptr_shift), pos_size);
                    ptr_shift += pos_size;
                    int to;
                    int to_size = sizeof (unsigned int);
                    memcpy((void*) &to, (void*) (buffer + ptr_shift), to_size);
                    ptr_shift += to_size;
                    delete(rope, pos, to);
                    break;

                case OPCODE_SPACE:
                    ptr_shift += opcode_size;
                    memcpy((void*) &pos, (void*) (buffer + ptr_shift), pos_size);
                    ptr_shift += pos_size;
                    space(rope, pos);
                    break;

                case OPCODE_NEWLINE:
                    ptr_shift += opcode_size;
                    memcpy((void*) &pos, (void*) (buffer + ptr_shift), pos_size);
                    ptr_shift += pos_size;
                    newline(rope, pos);
                    break;

                case OPCODE_PRINT:
                    ptr_shift += opcode_size;
                    print(rope);
                    break;
                default:
                    puts("Operación inexistente.");
            }
        }
    }

    socket_shutdown(server_socket);

    socket_destroy(server_socket);
    rope_destroy(rope);

    free(buffer);
    free(accepted_socket);
    free(server_socket);

    return EXIT_SUCCESS;
}
