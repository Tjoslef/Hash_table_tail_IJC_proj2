#include "../htab.h"
#include "../htab_private.h"
/*
 * Creates and initializes a hash table with a specified bucket count.
 * @param n Number of buckets to allocate in the hash table.
 * @return Pointer to the newly created hash table, or NULL on allocation failure.
 * - Allocates memory for the hash table structure and bucket array.
 * - Initializes all buckets to NULL using calloc().
 * - Sets both size (total items) and arr_size (bucket count) to n.
 */
htab_t *htab_init(size_t n){
    htab_t* htable = malloc(sizeof(struct htab));
       if (htable == NULL) {
           return NULL;
       }
       htable->size = 0;
       htable->arr_size = n;
       htable->ptr = calloc(htable->arr_size, sizeof(h_item*));
       if(htable->ptr == NULL){
           free(htable);
           return 0;
       }
       return htable;
}
