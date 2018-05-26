#ifndef _MOLECULE_H_
#define _MOLECULE_H_

#include "attr.h"
#include "stats.h"

struct mlc_t {
	int *pos;
	int last_len;
	int sz;
	char *bar_s;
};

void mlc_init(int n_target, char **target_name);

void mlc_destroy(int n_target, char **target_name);

void mlc_insert(int bx_id, bam1_t *b, struct stats_t *stats);

void mlc_get_last(struct stats_t *stats);

void mlc_fetch(struct stats_t *stats, int pos);

#endif /* _MOLECULE_H_ */