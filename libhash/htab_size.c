#include "../htab.h"
#include "../htab_private.h"
/*
*Function returning size of hash_table
*@param t Pointer to hash_table
*@return size of hash table
*/
size_t htab_size(const htab_t * t){
    return t->size;
}
