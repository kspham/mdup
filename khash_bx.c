#include "khash_bx.h"

#include <stdio.h>
#include <assert.h>

void khash_bx_destroy(khash_t(KHASH_STR) *khash_bx)
{
	khint_t i;
	for (i = kh_begin(bx_kh); i != kh_end(khash_bx); ++i)
		if (kh_exist(khash_bx, i))
			free((char*)kh_key(khash_bx, i));
	kh_destroy(KHASH_STR, khash_bx);
}

int khash_bx_get_id(khash_t(KHASH_STR) *khash_bx, int *bx_map_cnt, char *bx)
{
	int is_new;
	khiter_t k = kh_put(KHASH_STR, khash_bx, bx, &is_new);
	assert(is_new == 0 || is_new == 1);

	if (is_new) {
		kh_value(khash_bx, k) = *bx_map_cnt;
		*bx_map_cnt += 1;
		kh_key(khash_bx, k) = strdup(bx);
	}

	return kh_value(khash_bx, k);
}