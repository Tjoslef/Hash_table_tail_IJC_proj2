#include "htab.h"
typedef struct htab_item {
    htab_pair_t pair;
    struct htab_item *next;
} h_item;
typedef struct htab {
    h_item** ptr;
    size_t arr_size;
    size_t size;
}htab_t;
