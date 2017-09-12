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
    socket_t * server_socket = (socket_t*) malloc(sizeof (socket_t));
    socket_t * client_socket = (socket_t*) malloc(sizeof (socket_t));

    rope_create(rope);
    socket_create(server_socket);
    socket_create(client_socket);

    if (socket_bind_and_listen(server_socket, port) != EXIT_SUCCESS) {
        rope_destroy(rope);
        socket_destroy(server_socket);
        socket_destroy(client_socket);
        return EXIT_FAILURE;
    }

    if (socket_accept(server_socket, client_socket) != EXIT_SUCCESS) {
        rope_destroy(rope);
        socket_destroy(server_socket);
        socket_destroy(client_socket);
        return EXIT_FAILURE;
    }

    char *send_buffer = (char*) malloc(sizeof (char)*MAX_DATA_BUFFER);
    memset(send_buffer, 0, MAX_DATA_BUFFER);
    char *text = (char*) malloc(sizeof (char)*MAX_DATA_BUFFER);
    memset(text, 0, MAX_DATA_BUFFER);
    char *sprint_result = (char*) malloc(sizeof (char)*MAX_DATA_BUFFER);
    memset(sprint_result, 0, MAX_DATA_BUFFER);
    char *recv_buffer = (char*) malloc(sizeof (char)*MAX_DATA_BUFFER);
    memset(recv_buffer, 0, MAX_DATA_BUFFER);

    char *aux_buffer = (char*) malloc(sizeof (char)*MAX_DATA_BUFFER);
    int received;
    int lines_printed = 0;

    memset(aux_buffer, 0, MAX_DATA_BUFFER);

    received = socket_receive(client_socket, aux_buffer, MAX_DATA_BUFFER);

    if (received > 0) {

        /********/
        int print_counter = 0;
        /********/
        int ptr_shift = 0;
        int opcode;
        int opcode_size = sizeof (int);
        int pos;
        int pos_size = sizeof (int);
        unsigned short size;
        unsigned short size_size = sizeof (unsigned short);
        int to;
        int to_size = sizeof (int);
        int sprint_size;

        memcpy(recv_buffer, aux_buffer, received);

        while (ptr_shift < received) { // EOT
            opcode = 0; // lo seteo con una op invalida
            pos = 0;

            memcpy(
                    (void*) &opcode,
                    (void*) (recv_buffer + ptr_shift),
                    opcode_size);

            opcode = ntohl(opcode);

            switch (opcode) {
                case OPCODE_INSERT:
                    ptr_shift += opcode_size;

                    memcpy(
                            (void*) &pos,
                            (void*) (recv_buffer + ptr_shift),
                            pos_size);

                    pos = ntohl(pos);

                    ptr_shift += pos_size;

                    memcpy(
                            (void*) &size,
                            (void*) (recv_buffer + ptr_shift),
                            size_size);

                    size = ntohs(size);

                    ptr_shift += size_size;

                    strncpy(text, recv_buffer + ptr_shift, size);
                    text[size] = '\0';
                    ptr_shift += size;

                    insert(rope, pos, text);

                    break;
                case OPCODE_DELETE:
                    ptr_shift += opcode_size;

                    memcpy(
                            (void*) &pos,
                            (void*) (recv_buffer + ptr_shift),
                            pos_size);

                    pos = ntohl(pos);

                    ptr_shift += pos_size;

                    memcpy(
                            (void*) &to,
                            (void*) (recv_buffer + ptr_shift),
                            to_size);

                    to = ntohl(to);

                    ptr_shift += to_size;
                    delete(rope, pos, to);
                    break;

                case OPCODE_SPACE:
                    ptr_shift += opcode_size;
                    memcpy(
                            (void*) &pos,
                            (void*) (recv_buffer + ptr_shift),
                            pos_size);

                    pos = ntohl(pos);

                    ptr_shift += pos_size;
                    space(rope, pos);
                    break;

                case OPCODE_NEWLINE:
                    ptr_shift += opcode_size;
                    memcpy(
                            (void*) &pos,
                            (void*) (recv_buffer + ptr_shift),
                            pos_size);

                    pos = ntohl(pos);

                    ptr_shift += pos_size;
                    newline(rope, pos);
                    break;

                case OPCODE_PRINT:
                    print_counter++;
                    ptr_shift += opcode_size;
                    memset(sprint_result, 0, MAX_DATA_BUFFER);
                    sprint(rope, sprint_result);
                    lines_printed++;
                    sprint_size = strlen(sprint_result);
                    strncat(
                            send_buffer,
                            sprint_result,
                            sprint_size);

                    break;

                default:
                    printf("%d", print_counter);
                    puts("Operación inexistente.");
                    ptr_shift = received;
            }
        }

        /*********** MANDO LA RTA TODA JUNTA ****************/
        int size_struct = sizeof (short);
        int size_string = strlen(send_buffer);
        char buffer[MAX_DATA_BUFFER];

        struct response_command_t sc = {
            .size = htons(size_string)
        };

        memcpy((void*) buffer, (void*) &sc, size_struct);
        memcpy(
                (void*) (buffer + size_struct),
                (void*) send_buffer,
                size_string);

        socket_send(client_socket, buffer, size_struct + size_string);
        /*****************************************************/

        //free(text);
        //free(send_buffer);
    }

    free(send_buffer);
    free(text);
    free(sprint_result);
    free(recv_buffer);

    free(aux_buffer);

    rope_destroy(rope);
    socket_shutdown(server_socket);
    socket_destroy(server_socket);
    socket_shutdown(client_socket);
    socket_destroy(client_socket);

    if (received >= 0) {
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}
