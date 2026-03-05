# Laboratorio 1 - Introducción al Lenguaje C
**Laboratorio de Sistemas Operativos — Universidad de Antioquia**

---

## 1. Integrantes

| Jenny Andrea Orozco Osorio| Jennya.orozco@udea.edu.co| CC.43.918.288 |
| David Julián Penagos Arroyave | julian.penagos@udea.edu.co | CC.1.037.610.202 |

---

## 2. Estructura del repositorio

```
lab1/
├── README.md
├── wcat/
│   └── wcat.c
├── wgrep/
│   └── wgrep.c
└── wzip/
    ├── wzip.c
    └── wunzip.c
```

---

## 3. Compilación

```bash
gcc -Wall -o wcat/wcat wcat/wcat.c
gcc -Wall -o wgrep/wgrep wgrep/wgrep.c
gcc -Wall -o wzip/wzip wzip/wzip.c
gcc -Wall -o wzip/wunzip wzip/wunzip.c
```

---

## 4. Documentación de funciones

### 4.1 wcat

#### `int print_file(const char *filename)`
Abre el archivo indicado en modo lectura con `fopen()`. Lee el contenido línea a línea con `fgets()` usando un buffer de 4096 bytes y lo imprime con `printf("%s", buffer)`. Cierra el archivo con `fclose()` al terminar. Retorna `0` en éxito y `1` si el archivo no puede abrirse.

#### `int main(int argc, char *argv[])`
Punto de entrada. Si no se reciben archivos (`argc == 1`) sale con código `0`. De lo contrario llama a `print_file()` por cada archivo en orden; si alguno falla, sale con código `1` inmediatamente.

---

### 4.2 wgrep

#### `void search_in_stream(FILE *fp, const char *term)`
Recorre el stream `fp` línea por línea usando `getline()` (que soporta líneas arbitrariamente largas mediante reserva dinámica de memoria). Por cada línea, usa `strstr()` para buscar el término. Si lo encuentra, imprime la línea completa con `printf()`. Libera el buffer con `free()` al finalizar.

#### `int main(int argc, char *argv[])`
Punto de entrada. Sin argumentos imprime el uso y sale con código `1`. Con solo el término de búsqueda (sin archivos), redirige la búsqueda a `stdin`. Con archivos, los abre uno a uno y llama a `search_in_stream()`; si alguno falla, sale con código `1`.

---

### 4.3 wzip

#### `void compress_stream(FILE *fp, int *cur_char, int *cur_count)`
Implementa Run-Length Encoding (RLE) leyendo byte a byte con `fgetc()`. Mantiene el carácter actual y su conteo en variables compartidas entre llamadas (para soportar múltiples archivos como una secuencia continua). Cuando detecta un cambio de carácter, escribe con `fwrite()` la entrada de 5 bytes: 4 bytes del entero `count` en binario + 1 byte del carácter.

#### `int main(int argc, char *argv[])`
Punto de entrada. Sin archivos imprime el uso y sale con código `1`. Procesa cada archivo manteniendo el estado RLE entre ellos. Al finalizar todos los archivos, vuelca el último run pendiente.

---

### 4.4 wunzip

#### `void decompress_stream(FILE *fp)`
Lee entradas de 5 bytes desde el stream `fp` usando `fread()`: primero 4 bytes como entero `count`, luego 1 byte como carácter `c`. Imprime `c` exactamente `count` veces con `putchar()`. Detecta archivos corruptos si el carácter no puede leerse tras leer el contador.

#### `int main(int argc, char *argv[])`
Punto de entrada. Sin archivos imprime el uso y sale con código `1`. Abre cada archivo en modo binario (`"rb"`) y llama a `decompress_stream()`. Si algún archivo no puede abrirse, sale con código `1`.

---

## 5. Problemas durante el desarrollo y soluciones

| Problema | Solución |
|---|---|
| **wgrep con líneas muy largas**: `fgets()` trunca líneas más largas que el buffer. | Se reemplazó por `getline()`, que reserva memoria dinámicamente y no tiene límite fijo de longitud. |
| **wzip con múltiples archivos**: Al comprimir varios archivos, el último run de un archivo y el primero del siguiente deben fusionarse si el carácter es el mismo. | Se pasó el estado `cur_char` y `cur_count` por referencia entre llamadas a `compress_stream()`, preservando el contexto entre archivos. |
| **wunzip formato binario**: Abrir el archivo comprimido en modo texto (`"r"`) en algunos sistemas puede alterar los bytes al leer. | Se cambió a modo binario `"rb"` en `fopen()` para garantizar que los 4 bytes del entero se lean sin transformación. |
| **wcat sin argumentos**: El comportamiento esperado difiere del `cat` de UNIX que lee stdin. | El programa sale con código `0` sin hacer nada, según lo especificado en el enunciado. |

---

## 6. Pruebas realizadas

### wcat

```bash
# Sin argumentos -> exit 0
./wcat; echo $?   # 0

# Imprimir un archivo
echo "hola mundo" > test.txt
./wcat test.txt   # hola mundo

# Múltiples archivos
./wcat test.txt test2.txt   # contenido concatenado

# Archivo inexistente -> mensaje de error + exit 1
./wcat noexiste.txt 2>&1   # wcat: cannot open file
echo $?                    # 1
```

### wgrep

```bash
# Sin argumentos -> uso + exit 1
./wgrep 2>&1      # wgrep: searchterm [file ...]

# Búsqueda normal (case sensitive)
./wgrep foo bar.txt   # líneas con "foo"
./wgrep Foo bar.txt   # (vacío, "Foo" != "foo")

# Leer desde stdin
echo -e "tiene foo\nno match" | ./wgrep foo   # tiene foo

# Archivo inexistente
./wgrep foo noexiste.txt 2>&1   # wgrep: cannot open file
```

### wzip / wunzip

```bash
# Sin argumentos
./wzip 2>&1     # wzip: file1 [file2 ...]
./wunzip 2>&1   # wunzip: file1 [file2 ...]

# Ciclo compress/decompress
echo -n "aaaaaaaaaabbbb" > test.txt
./wzip test.txt > test.z
./wunzip test.z   # aaaaaaaaaabbbb

# Verificar tamaño comprimido (debería ser menor)
wc -c test.txt test.z

# Múltiples archivos como una sola secuencia
./wzip file1.txt file2.txt > combined.z
./wunzip combined.z   # contenido de ambos archivos concatenado
```

---

## 7. Manifiesto de transparencia (IA generativa)

Se utilizó IA generativa (Chatgpt) para:
- Generación inicial del código fuente de los cuatro programas.
- Identificación del problema de múltiples archivos en `wzip` y la solución con estado compartido.
- Redacción de este README.

Todo el código fue revisado, compilado y probado manualmente. Comprendimos el funcionamiento de cada función y somos responsables de los resultados.

---

## 8. Video de sustentación

> Enlace: 
