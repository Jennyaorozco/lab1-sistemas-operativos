#include <stdio.h>
#include <stdlib.h>

/**
 * print_file - Abre un archivo y imprime su contenido línea por línea.
 * @filename: Ruta del archivo a leer.
 * Retorna 0 en éxito, 1 en error de apertura.
 */
int print_file(const char *filename) {
    FILE *fp = fopen(filename, "r");
    if (fp == NULL) {
        fprintf(stderr, "wcat: cannot open file\n");
        return 1;
    }

    char buffer[4096];
    while (fgets(buffer, sizeof(buffer), fp) != NULL) {
        printf("%s", buffer);
    }

    fclose(fp);
    return 0;
}

/**
 * main - Punto de entrada. Itera sobre los archivos dados en argv e imprime
 *        su contenido. Si no se pasan archivos, retorna 0 inmediatamente.
 *        Si algún archivo no puede abrirse, imprime error y sale con código 1.
 */
int main(int argc, char *argv[]) {
    /* Sin archivos: salir con éxito (comportamiento diferente al cat de UNIX) */
    if (argc == 1) {
        exit(0);
    }

    for (int i = 1; i < argc; i++) {
        if (print_file(argv[i]) != 0) {
            exit(1);
        }
    }

    exit(0);
}
