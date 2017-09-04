#include "client_main.h"
#include "server_main.h"

int main(int argc, char *argv[]) {
    int status = EXIT_SUCCESS;

    if (argc > 1) {
        if (strcmp(argv[1], "server") != 0 && strcmp(argv[1], "client") != 0) {
            printf("No se conoce el parámetro '%s'.\n", argv[1]);
            status = EXIT_FAILURE;
        } else {
            if (strcmp(argv[1], "server") == 0) {
                status = server_main(argc, argv);
            } else if (strcmp(argv[1], "client") == 0) {
                status = client_main(argc, argv);
            } else {
                status = EXIT_FAILURE;
            }
        }
    }

    return status;
}