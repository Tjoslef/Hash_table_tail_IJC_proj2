#include "htab.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "io.h"
#include <time.h>
#define INIT_SIZE 256
static size_t big_value = 0;
/*
 * Updates the global variable big_value if the item's value is greater.
 * @param item Pointer to a key-value pair in the hash table.
 */
void biggest_value(htab_pair_t *item) {
    if(item->value > big_value){
        big_value = item->value;
    }
}

/*
 * Prints the key and value for items whose value matches the global maximum (big_value).
 * @param item Pointer to a key-value pair in the hash table.
 */
void big_print(htab_pair_t *item) {
    if(item->value == big_value){
        printf("%s\t%lu\n", item->key, big_value);
    }
}
/*
 * Main function: Reads words from stdin, counts their frequency,
 * and prints the most frequent word(s).
 *
 * Uses a hash table for counting and finds the maximum frequency.
 *
 * @return 0 on success, 1 on error.
 */
int main(){
   char *buffer = malloc(INIT_SIZE);
   if(buffer == NULL){
       perror("Failed to malloc memory");
       return 1;
   }
   FILE *file = fopen("/dev/stdin","r");
   if (file == NULL) {
          perror("Failed to open stdin");
          free(buffer);
          return 1;
      }

   int len;
   htab_t *htable = htab_init(617);
   /*
   * the reason for this number is that it is prime number and in hash function there is modulo
   * for that it eliminated collision because is not divisible and 617 because i was testing it on
   * quite large data
   */
   if (htable == NULL) {
          fprintf(stderr, "Failed to initialize hash table\n");
          free(buffer);
          fclose(file);
          return 1;
      }
   while ((len = read_word(INIT_SIZE, buffer, file)) != EOF) {
       char *tmp = strdup(buffer);
       if(tmp == NULL){
           perror("Failed to strdup memory");
           htab_free(htable);
           free(buffer);
           return 1;
       }
       htab_pair_t *p = htab_lookup_add(htable, tmp);
       free(tmp);
       if (p != NULL) {
           p->value += 1;
       }else {
           fprintf(stderr,"something goes wrong in htab_lookup_add \n");
           return 1;
       }
   }
   htab_for_each(htable, biggest_value);
   htab_for_each(htable, big_print);
   free(buffer);
   htab_free(htable);
   fclose(file);
   return 0;
}
