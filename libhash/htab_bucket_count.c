#include "../htab.h"
#include "../htab_private.h"
/*
*@param the hash map it self
*@return size_t arr_size of hash map
*Counting how current number bucket in hash map
*/
 size_t htab_bucket_count(const htab_t * t){
     return t->arr_size;
 }
