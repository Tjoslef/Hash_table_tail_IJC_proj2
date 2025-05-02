#include "../htab.h"
#include "../htab_private.h"
/*
 * Computes a hash value for a given string key.
 * @param str Pointer to the key (null-terminated string).
 * @return    Hash value as size_t.
 * Uses the 65599 hash algorithm as specified in the assignment.
 */
size_t htab_hash_function(htab_key_t str) {
    uint32_t h = 0;
    const unsigned char *p;
    for (p = (const unsigned char*)str; *p != '\0'; p++)
        h = 65599 * h + *p;
    return h;
}
