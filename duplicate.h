#ifndef _DUPLICATE_H_
#define _DUPLICATE_H_

#include "stats.h"
#include "attr.h"

struct align_t {
	bam1_t *b;
	int bx_id;
	char *cigar;
	char *mcigar;
	int flag;
};

struct duplst_t {
	struct align_t *align;
	int n_align;
};

void duplicate_init(int n_target);

void duplicate_destroy();

void duplicate_process(struct stats_t *stats, samFile *out_bam_f, bam_hdr_t *b_hdr);

void duplicate_try_process(int pos, struct stats_t *stats,
			   samFile *out_bam_f, bam_hdr_t *b_hdr);

void duplicate_insert(bam1_t *b, int bx_id, struct stats_t *stats,
		      samFile *out_bam_f, bam_hdr_t *b_hdr);

#endif /* _DUPLICATE_H_ */