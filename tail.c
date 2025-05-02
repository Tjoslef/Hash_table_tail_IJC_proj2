#include <limits.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#define _POSIX_C_SOURCE 200809L
#define _GNU_SOURCE
#include <string.h>
#include <unistd.h>
#define CHUNK_SIZE 4096 //for better manipulation
/*
 * Circular buffer struct for storing strings
 * - head: Index of the most recently added element
 * - tail: Index of the oldest element
 * - max_len: Maximum capacity of the buffer
 * - buffer: Array of string pointers for storage
 */
typedef struct {
    int head;
    int tail;
    int max_len;
    char **buffer;
} circular_b;
/*
 * struct for parameters of file/input handling
 * - n: Number of lines to maintain in buffer
 * - stdin_input: Flag indicating stdin input (1) vs file input (0)
 * - filename: Name of input file (NULL if using stdin)
 * - file: File pointer to input source
 */
typedef struct {
    int n;
    int stdin_input;
    char *filename;
    FILE *file;
} Config;
/*
 * Initializes a circular buffer
 * @param n: Maximum capacity of the buffer
 * @return: Pointer to allocated buffer, NULL on failure
 * - Allocates memory for buffer structure and string pointers
 * - Initializes all string pointers to NULL
 * - Sets initial head/tail to index 0
 */
circular_b *cbuf_create(int n){
    circular_b *cb = malloc(sizeof(circular_b));
    if(cb == NULL){
        fprintf(stderr,"malloc failed");
        return NULL;
    }
    cb->buffer = malloc(n * sizeof(char *));
    if (!cb->buffer) {
            free(cb);
            return NULL;
        }
    for (int i = 0; i < n; i++) {
           cb->buffer[i] = NULL;
    }
    cb->head = 0;
    cb->tail = 0;
    cb->max_len = n;
    return cb;
}
/*
 * Adds a string to the buffer and overwrites oldest if full
 * @param cb: Circular buffer pointer
 * @param line: String to add
 * @return: 0 on success, -1 on allocation error, 1 for oversize
 * - Uses modulo arithmetic for circular indexing
 * - Frees oldest entry if buffer is full before adding new
 * - Duplicates input string using strdup()
 */
int cbuf_put(circular_b *cb,char *line){
    if(strlen(line) > CHUNK_SIZE){
       fprintf(stderr,"to long lines");
       return 1;
    }
    char *tmp_line = strdup(line);
    if(tmp_line == NULL){
        fprintf(stderr, "error in malloc");
        return -1;
    }
    int next = (cb->head + 1) % cb->max_len;
    if(cb->tail == next){
       free(cb->buffer[next]);
       cb->tail = (cb->tail + 1) % cb->max_len;
    }
    cb->buffer[next] = tmp_line;

    cb->head = next;
    return 0;
}
/*
 * Retrieves and removes the oldest string from the buffer
 * @param cb: Circular buffer pointer
 * @return: Pointer to retrieved string
 * - Advances tail index using modulo arithmetic
 * - Does not free the string - ownership transfers to caller
 */
char *cbuf_get(circular_b *cb){
    char *line = cb->buffer[cb->tail];
    cb->tail = (cb->tail + 1) % cb->max_len;
    return line;
}
/*
 * Destroyer circular buffer and releases all memory
 * @param cb: Circular buffer pointer to free
 * - Frees all stored strings and pointer array
 * - Handles NULL pointer input gracefully
 */
void cbuf_free(circular_b *cb){
    if (cb == NULL){
        return;
    }
    for (int i = 0; i < cb->max_len; i++) {
           free(cb->buffer[i]);
       }
       free(cb->buffer);
       free(cb);
}
/*
 * Parses command line arguments to create configuration
 * @param input from user
 * @return: Configured settings struct of file
 * - Defaults: n=10, read from stdin
 * - Handles -n option with validation:
 *   - Integer conversion with error checking
 *   - Range validation (ERANGE)
 * - Opens specified file or stdin with error checking
 * - Exits program on validation errors
 */
Config parserCon(int argc, char *argv[]){
   Config set_up = { .n = 10, .stdin_input = 0, .filename = NULL, .file = NULL };
   int opt;
   while ((opt = getopt(argc, argv, "n:")) != -1) {
       switch (opt) {
           case 'n': {
                char *endptr;
                errno = 0;
                set_up.n = strtol(optarg, &endptr, 10);
                if (*optarg == '\0' || *endptr != '\0') {
                    fprintf(stderr, "Error: Invalid integer argument for -n: %s\n", optarg);
                    exit(1);
                } else if (errno == ERANGE) {
                    fprintf(stderr, "Error: Integer argument for -n out of range: %s\n", optarg);
                    _exit(1);
                }
                if(set_up.n == 0){
                    exit(0);
                }
           }
           break;
           case '?':
           fprintf(stderr, "please Usage: %s [-n <count>] [filename]\n", argv[0]);
           exit(1);
           default:
           fprintf(stderr,"unknow input");
           fprintf(stderr, "Usage: %s [-n <count>] [filename]\n", argv[0]);
           exit(1);
       }
   }
   if (optind < argc){
       set_up.filename = argv[optind];
       set_up.file = fopen(set_up.filename, "r");
        if (!set_up.file) {
            perror("Error opening file");
            exit(1);
        }
   }else {
       set_up.stdin_input = 1;
       set_up.file = fopen("/dev/stdin", "rb");
       if (!set_up.file) {
           perror("Error opening stdin");
           exit(1);
        }
   }
   return set_up;
}
/*
 * Main function: prints the last N lines from a file or stdin.
 *
 * - Parses command-line arguments for line count and input source (parserCon).
 * - Initializes a circular buffer to store up to N lines.
 * - Reads input line by line, handling overly long lines with a warning.
 * - After reading, prints the buffered lines in order.
 * - Frees all allocated resources before exiting.
 *
 * @param argc Number of command-line arguments.
 * @param argv Array of command-line argument strings.
 * @return 0 on success, 1 on error.
 */
int main(int argc, char *argv[]){
    Config config = parserCon(argc, argv);
    circular_b *cir_buf;
    cir_buf = cbuf_create(config.n);
    char buffer[CHUNK_SIZE];
    int warnings = 1;
    int first = fgetc(config.file);
    if (first == EOF) {
        return 0;
    }
    ungetc(first, config.file);
    while (fgets(buffer, sizeof(buffer), config.file) != NULL) {
        size_t len = strlen(buffer);
        if (len == sizeof(buffer) - 1 && buffer[len - 1] != '\n') {
                if (warnings-- > 0){
                    fprintf(stderr, "Warning: Line is too long\n");
                }
                int ch;
                while ((ch = fgetc(config.file)) != '\n' && ch != EOF){}
            }
        int check = cbuf_put(cir_buf,buffer);
        if(check == 1){
            fprintf(stderr,"line is too long \n");
            cbuf_free(cir_buf);
            fclose(config.file);
            return 1;
        }else if (check == -1) {
            fprintf(stderr,"malloc go brrrrr \n");
            cbuf_free(cir_buf);
            fclose(config.file);
            return 1;
        }
    }

    for(int i = 0;i < config.n;i++){
        char *line = cbuf_get(cir_buf);
        if (line != NULL) {
                    printf("%s", line);
        }else{
            continue;
        }
    }

    cbuf_free(cir_buf);
    fclose(config.file);
    return 0;
}
