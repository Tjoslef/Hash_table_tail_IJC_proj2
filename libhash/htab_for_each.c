#include "../htab.h"
#include "../htab_private.h"
#include <stddef.h>
/*
 * Applies a given function to each key-value pair in the hash map.
 *
 * @param t Pointer to the hash map.
 * @param f Function to apply to each key-value pair (takes htab_pair_t* as argument).
 */
void htab_for_each(const htab_t * t, void (*f)(htab_pair_t *data)){
     if (!t || !f) return;
    for(size_t i = 0; i < t->arr_size;i++){
        h_item *item = t->ptr[i];
        while(item){
            h_item *next = item->next;
            f(&item->pair);
            item = next;
        }
    }
}
