#include "common_socket.h"

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <unistd.h>

#define ERR_SOCKET_CREATE               2
#define ERR_SOCKET_DESTROY              3
#define ERR_SOCKET_BIND                 4
#define ERR_SOCKET_LISTEN               5
#define ERR_SOCKET_GETHOST              6
#define ERR_SOCKET_CONNECT              7
#define ERR_SOCKET_ACCEPT               8
#define ERR_SOCKET_CLOSED_SEND         -2
#define ERR_SOCKET_SEND                -3
#define ERR_SOCKET_CLOSED_RECEIVE      -2
#define ERR_SOCKET_RECEIVE             -3

/*
 * Crea el socket obteniendo un filedescriptor del sistema operativo. Recibe las
 * características de configuración básicas.
 */
int socket_create(socket_t *self) {
    int status = EXIT_SUCCESS;

    self->socket = socket(AF_INET, SOCK_STREAM, 0);

    if (self->socket == -1) {
        status = ERR_SOCKET_CREATE;
    }

    return status;
}

int socket_destroy(socket_t *self) {
    int status = EXIT_SUCCESS;

    status = close(self->socket);
    free(self);

    return status;
}

/*
 * bind: Enlaza el socket identificado por el filedescriptor con una dirección y
 * puerto locales.
 *
 * listen: configura el socket para recibir conexiones en la dirección y puerto
 * previamente determinada mediante bind().
 */
int socket_bind_and_listen(socket_t *self, unsigned short port) {
    int status = EXIT_SUCCESS;
    int bind_result = 0;
    struct sockaddr_in addr;

    memset((char *) &addr, 0, sizeof (addr));
    addr.sin_family = AF_INET;
    addr.sin_port = htons(port);
    addr.sin_addr.s_addr = INADDR_ANY;

    bind_result = bind(self->socket,
            (struct sockaddr *) &addr,
            (socklen_t)sizeof (struct sockaddr));

    if (bind_result < 0) {
        status = ERR_SOCKET_BIND;
    } else {
        int listen_status = listen(self->socket, 1);

        if (listen_status < 0) {
            status = ERR_SOCKET_LISTEN;
        }
    }

    return status;
}

/*
 * Conecta el socket a la dirección y puerto destino.
 * Determina dirección y puertos locales si no se utilizó bind() previamente.
 */
int socket_connect(socket_t *self, const char* host_name, unsigned short port) {
    int status = EXIT_SUCCESS;
    struct hostent *he;

    if ((he = gethostbyname(host_name)) == NULL) {
        status = ERR_SOCKET_GETHOST;
    } else {
        struct sockaddr_in their_addr;
        int conn_status;

        their_addr.sin_family = AF_INET; // orden de bytes de la máquina
        their_addr.sin_port = htons(port); // short, orden de bytes de la red
        their_addr.sin_addr = *((struct in_addr *) he->h_addr_list[0]);
        memset(&(their_addr.sin_zero), 8, sizeof (int)); // poner a 0 el resto

        conn_status = connect(
                self->socket,
                (struct sockaddr *) &their_addr,
                sizeof (struct sockaddr));

        if (conn_status == -1) {
            status = ERR_SOCKET_CONNECT;
        }
    }

    return status;
}

/*
 * Espera una conexión en el socket previamente configurado con listen().
 */
int socket_accept(socket_t *self, socket_t* accepted_socket) {
    int status = EXIT_SUCCESS;

    unsigned int newsockfd;

    newsockfd = accept(self->socket, NULL, NULL);

    if (newsockfd < 0) {
        status = ERR_SOCKET_ACCEPT;
    } else {
        accepted_socket->socket = newsockfd;
    }

    return status;
}

/*
 * Envía datos a través del socket
 */
int socket_send(socket_t *self, const char* buffer, size_t length) {
    int sent = 0;
    int s = 0;
    bool is_valid_socket = true;

    while (sent < length && is_valid_socket) {
        s = send(self->socket, &buffer[sent], length - sent, MSG_NOSIGNAL);

        if (s == 0) {
            is_valid_socket = false;
            sent = ERR_SOCKET_CLOSED_SEND;
        } else if (s < 0) {
            is_valid_socket = false;
            sent = ERR_SOCKET_SEND;
        } else {
            sent += s;
        }
    }

    return sent;
}

/*
 * Recibe datos a través del socket.
 */
int socket_receive(socket_t *self, char* buffer, size_t length) {
    int received = 0;
    int r = 0;
    bool is_valid_socket = true;
    bool has_ended = false;

    while (received < length && is_valid_socket && !has_ended) {

        r = recv(self->socket, &buffer[received], length - received, MSG_NOSIGNAL);

        if (r == 0) {
            has_ended = true;
        } else if (r < 0) {
            is_valid_socket = false;
            received = ERR_SOCKET_RECEIVE;
        } else {
            received += r;
        }
    }

    return received;
}

/*
 * Se utiliza para cerrar el envío y la recepción de datos en forma ordenada.
 */
void socket_shutdown(socket_t *self) {
    shutdown(self->socket, SHUT_RDWR);
}

void socket_shutdown_send(socket_t *self) {
    shutdown(self->socket, SHUT_WR);
}
