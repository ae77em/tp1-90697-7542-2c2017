#include <stdio.h>
#include <stdlib.h>

int main(void) {
	printf("Hola mundo \n");
	printf("Tamanio de un entero: %zu \n", sizeof(int));

	FILE* input;

	input = fopen("saraza", "r");


	if (! input) {
		printf("archivo no abierto (no existe ;) ) \n");
		return EXIT_FAILURE;
	}


	return EXIT_SUCCESS;
}
