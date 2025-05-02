#include "../htab.h"
#include "../htab_private.h"
#include <stddef.h>
#include <stdlib.h>
/*
 * Removes all items from the hash map and frees their memory.
 * @param t Pointer to the hash map to clear.
 * Frees all key-value pairs in every bucket and resets the hash map size to zero.
 */
void htab_clear(htab_t * t){
    for(size_t i = 0; i < t->arr_size;i++){
        h_item *item = t->ptr[i];
        while(item){
            h_item *next = item->next;
            free((void *)item->pair.key);
            free(item);
            item = next;
        }
        t->ptr[i] = NULL;
    }
    t->size = 0;
}
