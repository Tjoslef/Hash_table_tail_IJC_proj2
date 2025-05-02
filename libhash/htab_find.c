#include "../htab.h"
#include "../htab_private.h"
/*
 * Searches for a key in the hash table.
 * @param t   Pointer to the hash table.
 * @param key Key to search for.
 * @return    Pointer to the matching key-value pair, or NULL if not found.
 */
htab_pair_t *htab_find(const htab_t * t, htab_key_t key){
    uint64_t hash = htab_hash_function(key);
    size_t index = hash % t->arr_size;
    h_item *start = t->ptr[index];
    while(start != NULL){
        if (strcmp(key,start->pair.key) == 0) {
            return &(start->pair);
        }
        start = start->next;
    }
    return NULL;
}
