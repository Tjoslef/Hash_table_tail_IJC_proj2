#include "../htab.h"
#include "../htab_private.h"
#include <stddef.h>
#include <stdlib.h>
/*
* Destroys the hash map and all its contents.
*
* @param t Pointer to the hash map to destroy.
*/
void htab_free(htab_t * t){
    if (!t) return;
    for(size_t i = 0; i < t->arr_size;i++){
        h_item *item = t->ptr[i];
        while(item){
        h_item *next = item->next;
        free((void *)item->pair.key);
        free(item);
        item = next;
        }
    }
    free(t->ptr);
    free(t);
}
