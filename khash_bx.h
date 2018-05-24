#ifndef _KHASH_BX_H_
#define _KHASH_BX_H_

#include "htslib/khash.h"

KHASH_MAP_INIT_STR(KHASH_STR, int)

void khash_bx_destroy(khash_t(KHASH_STR) *khash_bx);

int khash_bx_get_id(khash_t(KHASH_STR) *khash_bx, int *bx_map_cnt, char *bx);

#endif /* _KHASH_BX_H_ */