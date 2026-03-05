#include <stdio.h>
#include <stdlib.h>

/**
 * decompress_stream - Lee entradas de 5 bytes [int32 count][char c] del stream fp
 *                     y escribe en stdout el caracter c repetido count veces.
 * @fp: Stream del archivo comprimido.
 */
void decompress_stream(FILE *fp) {
    int count;
    char c;

    while (fread(&count, sizeof(int), 1, fp) == 1) {
        if (fread(&c, sizeof(char), 1, fp) != 1) {
            fprintf(stderr, "wunzip: archivo comprimido corrupto\n");
            exit(1);
        }
        for (int i = 0; i < count; i++) {
            putchar(c);
        }
    }
}

/**
 * main - Punto de entrada de wunzip.
 *        Uso: ./wunzip file1 [file2 ...]
 *        Descomprime uno o mas archivos RLE y escribe el contenido en stdout.
 */
int main(int argc, char *argv[]) {
    if (argc == 1) {
        fprintf(stderr, "wunzip: file1 [file2 ...]\n");
        exit(1);
    }

    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "rb");
        if (fp == NULL) {
            fprintf(stderr, "wunzip: cannot open file\n");
            exit(1);
        }
        decompress_stream(fp);
        fclose(fp);
    }

    exit(0);
}
