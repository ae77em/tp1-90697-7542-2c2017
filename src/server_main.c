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
        free(rope);
        socket_destroy(server_socket);
        free(server_socket);
        socket_destroy(client_socket);
        free(client_socket);
        return EXIT_FAILURE;
    }

    if (socket_accept(server_socket, client_socket) != EXIT_SUCCESS) {
        rope_destroy(rope);
        free(rope);
        socket_destroy(server_socket);
        free(server_socket);
        socket_destroy(client_socket);
        free(client_socket);
        return EXIT_FAILURE;
    }

    char *send_buffer = (char*) malloc(sizeof (char)*MAX_DATA_BUFFER);
    memset(send_buffer, 0, MAX_DATA_BUFFER);
    char text[MAX_DATA_BUFFER];
    memset(text, 0, MAX_DATA_BUFFER);
    char sprint_result[MAX_DATA_BUFFER];
    memset(sprint_result, 0, MAX_DATA_BUFFER);
    char recv_buffer[MAX_DATA_BUFFER];
    memset(recv_buffer, 0, MAX_DATA_BUFFER);

    int msg_length = 0;
    int received;
    bool the_end_is_here = false;
    int ptr_shift = 0;
    int opcode = -1;
    int pos;
    unsigned short size;
    int to;
    unsigned short sprint_size;
    unsigned short response_size;

    /* opero recibiendo de a una operación por vez, para evitar los overflows */
    while (!the_end_is_here
            && (
            received = socket_receive(client_socket,
            (char*) &msg_length,
            _SIZE_OF_UINT32_)) > 0) {
        msg_length = ntohl(msg_length);

        received = socket_receive(client_socket, recv_buffer, msg_length);

        the_end_is_here = (received <= 0);
        if (!the_end_is_here) {
            ptr_shift = 0;
            opcode = 0;
            pos = 0;
            size = 0;
            to = 0;
            sprint_size = 0;

            while (ptr_shift < received) {
                memcpy(
                        (void*) &opcode,
                        (void*) (recv_buffer + ptr_shift),
                        _SIZE_OF_UINT32_);

                opcode = ntohl(opcode);

                switch (opcode) {
                    case OPCODE_INSERT:
                        ptr_shift += _SIZE_OF_UINT32_;

                        memcpy(
                                (void*) &pos,
                                (void*) (recv_buffer + ptr_shift),
                                _SIZE_OF_UINT32_);

                        pos = ntohl(pos);

                        ptr_shift += _SIZE_OF_UINT32_;

                        memcpy(
                                (void*) &size,
                                (void*) (recv_buffer + ptr_shift),
                                _SIZE_OF_SHORT_);

                        size = ntohs(size);

                        ptr_shift += _SIZE_OF_SHORT_;

                        strncpy(text, recv_buffer + ptr_shift, size);
                        text[size] = '\0';
                        ptr_shift += size;

                        rope_insert(rope, pos, text);

                        break;
                    case OPCODE_DELETE:
                        ptr_shift += _SIZE_OF_UINT32_;

                        memcpy(
                                (void*) &pos,
                                (void*) (recv_buffer + ptr_shift),
                                _SIZE_OF_UINT32_);

                        pos = ntohl(pos);

                        ptr_shift += _SIZE_OF_UINT32_;

                        memcpy(
                                (void*) &to,
                                (void*) (recv_buffer + ptr_shift),
                                _SIZE_OF_UINT32_);

                        to = ntohl(to);

                        ptr_shift += _SIZE_OF_UINT32_;
                        rope_delete(rope, pos, to);
                        break;

                    case OPCODE_SPACE:
                        ptr_shift += _SIZE_OF_UINT32_;
                        memcpy(
                                (void*) &pos,
                                (void*) (recv_buffer + ptr_shift),
                                _SIZE_OF_UINT32_);

                        pos = ntohl(pos);

                        ptr_shift += _SIZE_OF_UINT32_;
                        rope_space(rope, pos);
                        break;

                    case OPCODE_NEWLINE:
                        ptr_shift += _SIZE_OF_UINT32_;
                        memcpy(
                                (void*) &pos,
                                (void*) (recv_buffer + ptr_shift),
                                _SIZE_OF_UINT32_);

                        pos = ntohl(pos);

                        ptr_shift += _SIZE_OF_UINT32_;
                        rope_newline(rope, pos);
                        break;

                    case OPCODE_PRINT:
                        ptr_shift += _SIZE_OF_UINT32_;
                        memset(sprint_result, 0, MAX_DATA_BUFFER);
                        rope_sprint(rope, sprint_result);
                        sprint_size = strlen(sprint_result);
                        sprint_result[sprint_size] = '\0';

                        strncat(
                                send_buffer,
                                sprint_result,
                                sprint_size);

                        response_size = htons(sprint_size);
                        socket_send(client_socket,
                                (char*) &response_size,
                                _SIZE_OF_SHORT_);
                        socket_send(client_socket, send_buffer, sprint_size);

                        /* Limpio el buffer para que el próximo envío no re-
                          envíe el buffer anterior. */
                        memset(send_buffer, 0, MAX_DATA_BUFFER);

                        break;

                    default:
                        puts("Operación inexistente.");
                        ptr_shift = received;
                }
            }
        }
    }

    free(send_buffer);
    rope_destroy(rope);
    free(rope);

    socket_shutdown(server_socket);
    socket_destroy(server_socket);
    free(server_socket);
    socket_shutdown(client_socket);
    socket_destroy(client_socket);
    free(client_socket);

    if (received >= 0) {
        return EXIT_SUCCESS;
    } else {
        return EXIT_FAILURE;
    }
}
