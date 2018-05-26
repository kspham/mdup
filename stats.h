#ifndef _STATS_H_
#define _STATS_H_

#include "attr.h"

/* stats attribute */
#define N_COVER			80
#define N_ISIZE 		1000
#define N_MLC			100

struct stats_t {
	int id;
	int64_t n_read;
	int64_t n_read1;
	int64_t n_read2;
	int64_t n_dup;			/* mapped and is duplicate */
	int64_t n_unmap;
	int64_t n_mq0;			/* mapped and mapping quality = 0 */
	int64_t sum_len1;
	int64_t sum_len2;
	int64_t q30_base1;
	int64_t q30_base2;
	int64_t n_base1;
	int64_t n_base2;
	int64_t isize[N_ISIZE + 1];	/* insert size histogram (remove duplicate) */
	int64_t cover[N_COVER + 1];	/* coverage histogram (remove duplicate) */
};

void coverage_init(int n_chr);

void coverage_init_target(int id, int target_len);

void coverage_destroy();

void coverage_get(struct stats_t *stats, int target_len);

void coverage_add(bam1_t *b, struct stats_t *stats);

void get_basic_stats(bam1_t *b, struct stats_t *stats);

void stats_init(struct stats_t *stats, int id);

void stats_merge(struct stats_t *target, struct stats_t *source);

#endif /* _STATS_H_ */