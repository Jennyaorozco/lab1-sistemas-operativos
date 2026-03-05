#include <stdio.h>
#include <stdlib.h>

/**
 * compress_stream - Lee byte a byte el stream `fp` y aplica Run-Length Encoding (RLE).
 *                   Escribe en stdout entradas de 5 bytes: [int32 count][char c].
 *                   Mantiene estado entre llamadas para comprimir correctamente
 *                   múltiples archivos como una sola secuencia.
 * @fp:         Stream de lectura.
 * @cur_char:   Puntero al carácter actual en curso (estado compartido entre archivos).
 * @cur_count:  Puntero al contador actual (estado compartido entre archivos).
 */
void compress_stream(FILE *fp, int *cur_char, int *cur_count) {
    int c;
    while ((c = fgetc(fp)) != EOF) {
        if (*cur_char == -1) {
            /* Primer carácter leído en todo el proceso */
            *cur_char = c;
            *cur_count = 1;
        } else if (c == *cur_char) {
            (*cur_count)++;
        } else {
            /* Carácter diferente: volcar entrada anterior */
            fwrite(cur_count, sizeof(int), 1, stdout);
            fwrite(cur_char, sizeof(char), 1, stdout);
            *cur_char = c;
            *cur_count = 1;
        }
    }
}

/**
 * main - Punto de entrada de wzip.
 *        Uso: ./wzip file1 [file2 ...]
 *        Comprime uno o más archivos usando RLE y escribe en stdout.
 *        El formato de salida es: [4 bytes entero] [1 byte carácter] por cada run.
 */
int main(int argc, char *argv[]) {
    if (argc == 1) {
        fprintf(stderr, "wzip: file1 [file2 ...]\n");
        exit(1);
    }

    int cur_char = -1;  /* -1 indica que aún no hay carácter inicial */
    int cur_count = 0;

    for (int i = 1; i < argc; i++) {
        FILE *fp = fopen(argv[i], "r");
        if (fp == NULL) {
            fprintf(stderr, "wzip: cannot open file\n");
            exit(1);
        }
        compress_stream(fp, &cur_char, &cur_count);
        fclose(fp);
    }

    /* Volcar el último run pendiente */
    if (cur_char != -1) {
        fwrite(&cur_count, sizeof(int), 1, stdout);
        fwrite(&cur_char, sizeof(char), 1, stdout);
    }

    exit(0);
}
