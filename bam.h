#ifndef _BAM_H_
#define _BAM_H_

#include "attr.h"
#include "stats.h"

struct bam_inf_t {
	const char *bam_path;
	hts_idx_t *bam_i;
	bam_hdr_t *b_hdr;
	int cur_id;
};

void bam_inf_init(struct bam_inf_t *bam_inf, const char *file_path);

void read_bam_unmapped(struct bam_inf_t *bam_inf, struct stats_t *stats);

void read_bam_target(struct bam_inf_t *bam_inf, int id, struct stats_t *stats);

#endif /* _BAM_H_ */