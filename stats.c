#include "stats.h"

static int **cell;

void coverage_init(int n_chr)
{
	__CALLOC(cell, n_chr);
}

void coverage_init_target(int id, int target_len)
{
	__CALLOC(cell[id], target_len);
}

void coverage_destroy()
{
	__FREE_AND_NULL(cell);
}

void coverage_add(bam1_t *b, struct stats_t *stats)
{
	int pos, i, isize;
	uint32_t *cigar;

	isize = __abs(b->core.isize) < N_ISIZE ? __abs(b->core.isize) : N_ISIZE;
	++stats->isize[isize];
	pos = b->core.pos;
	cigar = bam_get_cigar(b);

	for (i = 0; i < b->core.n_cigar; ++i) {
		int oplen = bam_cigar_oplen(cigar[i]);
		char opchr = bam_cigar_opchr(cigar[i]);
		if (opchr == 'D') {
			pos += oplen;
		} else if (opchr == 'M') {
			++cell[stats->id][pos];
			--cell[stats->id][pos + oplen];
			pos += oplen;
		}
	}
}

void coverage_get(struct stats_t *stats, int target_len)
{
	int i, cnt;
	for (i = cnt = 0; i < target_len; ++i) {
		cnt += cell[stats->id][i];
		assert(cnt >= 0);
		if (cnt < N_COVER)
			++stats->cover[cnt];
	}
	assert(cnt == 0);
	free(cell[stats->id]);
}

void get_basic_stats(bam1_t *b, struct stats_t *stats)
{
	uint8_t *qual = bam_get_qual(b);
	int i;

	++stats->n_read;

	if (b->core.n_cigar == 0)
		++stats->n_unmap;
	else if (b->core.qual == 0)
		++stats->n_mq0;
	if (b->core.flag & FLAG_READ1) {
		++stats->n_read1;
		stats->sum_len1 += b->core.l_qseq;
		for (i = 0; i < b->core.l_qseq; ++i) {
			++stats->n_base1;
			if (qual[i] >= 30)
				++stats->q30_base1;
		}
	} else {
		++stats->n_read2;
		stats->sum_len2 += b->core.l_qseq;
		for (i = 0; i < b->core.l_qseq; ++i) {
			++stats->n_base2;
			if (qual[i] >= 30)
				++stats->q30_base2;
		}
	}	
}

void stats_init(struct stats_t *stats, int id)
{
	stats->id = id;
	stats->n_read1 = 0;
	stats->n_read2 = 0;
	stats->n_read = 0;
	stats->n_dup = 0;
	stats->n_unmap = 0;
	stats->n_mq0 = 0;
	stats->sum_len1 = 0;
	stats->sum_len2 = 0;
	stats->q30_base1 = 0;
	stats->q30_base2 = 0;
	stats->n_base1 = 0;
	stats->n_base2 = 0;
	memset(stats->isize, 0, N_ISIZE * sizeof(int64_t));
	memset(stats->cover, 0, N_COVER * sizeof(int64_t));
}

void stats_merge(struct stats_t *target, struct stats_t *source)
{
	int i;
	target->n_read += source->n_read;
	target->n_read1 += source->n_read1;
	target->n_read2 += source->n_read2;
	target->n_dup += source->n_dup;
	target->n_unmap += source->n_unmap;
	target->n_mq0 += source->n_mq0;
	target->sum_len1 += source->sum_len1;
	target->sum_len2 += source->sum_len2;
	target->q30_base1 += source->q30_base1;
	target->q30_base2 += source->q30_base2;
	target->n_base1 += source->n_base1;
	target->n_base2 += source->n_base2;
	for (i = 0; i <= N_ISIZE; ++i)
		target->isize[i] += source->isize[i];
	for (i = 0; i <= N_COVER; ++i)
		target->cover[i] += source->cover[i];
}
