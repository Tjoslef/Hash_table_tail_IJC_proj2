#include "../htab.h"
#include "../htab_private.h"
/*
 * Looks up a key in the hash table, adding it if not found.
 *
 * @param t   Pointer to the hash table (modified if adding new key).
 * @param key Key to search/add.
 * @return    Pointer to existing or new key-value pair, NULL on failure.
 *
 * - Searches the appropriate bucket using hash/index.
 * - If key exists: returns existing pair.
 * - If key missing: allocates new entry, copies key, initializes value to 0.
 * - On memory failure: frees entire hash table and returns NULL.
 * - Caller should not free the returned key pointer (managed by hash table).
 */
htab_pair_t * htab_lookup_add(htab_t * t, htab_key_t key){
     if (key == NULL) return NULL;
    uint64_t hash = htab_hash_function(key);
    size_t index = hash % t->arr_size;
    h_item *start = t->ptr[index];
    while(start != NULL){
        if (strcmp(key,start->pair.key) == 0) {
                        return &(start->pair);
        }
        start = start->next;
    }
    h_item *tmp = malloc(sizeof(h_item));
    if(tmp == NULL ){
        htab_free(t);
      return NULL;
    }
    if(!(tmp->pair.key = strdup(key))){
        free(tmp);
        return NULL;
    }
    tmp->pair.value = 0;
    tmp->next = t->ptr[index];
    t->ptr[index] = tmp;
    t->size++;
    return &tmp->pair;
}
