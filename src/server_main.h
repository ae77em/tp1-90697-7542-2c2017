#ifndef SERVER_MAIN_H
#define SERVER_MAIN_H

#include "common_socket.h"
#include "common_structs.h"

#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <arpa/inet.h>
#include <stdbool.h>

int server_main(int argc, char *argv[]);

#endif /* SERVER_MAIN_H */

