#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * search_in_stream - Busca el término `term` en cada línea del stream `fp`.
 *                    Imprime las líneas que contengan el término (case sensitive).
 *                    Usa getline() para soportar líneas arbitrariamente largas.
 * @fp:   Stream de archivo (puede ser stdin).
 * @term: Cadena de búsqueda.
 */
void search_in_stream(FILE *fp, const char *term) {
    char *line = NULL;
    size_t len = 0;
    ssize_t read;

    while ((read = getline(&line, &len, fp)) != -1) {
        if (strstr(line, term) != NULL) {
            printf("%s", line);
        }
    }

    free(line);
}

/**
 * main - Punto de entrada de wgrep.
 *        Uso: ./wgrep <searchterm> [file1 file2 ...]
 *        Si no se dan argumentos imprime uso y sale con código 1.
 *        Si no se da archivo, lee desde stdin.
 *        Si un archivo no puede abrirse, imprime error y sale con código 1.
 */
int main(int argc, char *argv[]) {
    /* Sin argumentos: imprimir uso */
    if (argc == 1) {
        fprintf(stderr, "wgrep: searchterm [file ...]\n");
        exit(1);
    }

    const char *term = argv[1];

    /* Solo término de búsqueda, sin archivos: leer desde stdin */
    if (argc == 2) {
        search_in_stream(stdin, term);
        exit(0);
    }

    /* Buscar en cada archivo provisto */
    for (int i = 2; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            fprintf(stderr, "wgrep: cannot open file\n");
            exit(1);
        }
        search_in_stream(fp, term);
        fclose(fp);
    }

    exit(0);
}
