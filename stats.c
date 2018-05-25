#include "stats.h"

static int **cell;
static int *offset;
static int *cur_cover;
static int *target_len;

void coverage_init(int n_target, uint32_t *_target_len)
{
	int i;
	__CALLOC(cell, n_target);
	__CALLOC(offset, n_target);
	__CALLOC(cur_cover, n_target);
	__CALLOC(target_len, n_target);
	memcpy(target_len, _target_len, n_target * sizeof(int));
	for (i = 0; i < n_target; ++i)
		__CALLOC(cell[i], MBSZ * 2);
}

void coverage_destroy(int n_target)
{
	int i;
	for (i = 0; i < n_target; ++i)
		free(cell[i]);
	__FREE_AND_NULL(cell);
}

void cal_rest_coverage(struct stats_t *stats)
{
	int i;
	for (i = 0; ; ++i) {
		if (i < 2 * MBSZ)
			cur_cover[stats->id] += cell[stats->id][i];
		else
			assert(cur_cover[stats->id] == 0);
		if (i + offset[stats->id] == target_len[stats->id])
			break;
		assert(cur_cover[stats->id] >= 0);
		if (cur_cover[stats->id] < N_COVER)
			++stats->cover[cur_cover[stats->id]];
		else
			++stats->cover[N_COVER];
	}
}

void collect_coverage(struct stats_t *stats, int len)
{
	int i;
	for (i = 0; i < len; ++i) {
		cur_cover[stats->id] += cell[stats->id][i];
		assert(i + offset[stats->id] < target_len[stats->id]);
		assert(cur_cover[stats->id] >= 0);
		if (cur_cover[stats->id] < N_COVER)
			++stats->cover[cur_cover[stats->id]];
		else
			++stats->cover[N_COVER];
	}
}

void get_coverage(bam1_t *b, struct stats_t *stats)
{
	int pos, i, isize, sum;
	uint32_t *cigar;

	isize = __abs(b->core.isize) < N_ISIZE ? __abs(b->core.isize) : N_ISIZE;
	++stats->isize[isize];
	pos = b->core.pos - offset[stats->id];
	cigar = bam_get_cigar(b);

	if (pos >= 2 * MBSZ) {
		collect_coverage(stats, 2 * MBSZ);
		memset(cell[stats->id], 0, 2 * MBSZ * sizeof(int));
		pos -= 2 * MBSZ;
		offset[stats->id] += 2 * MBSZ;
		while (pos >= MBSZ) {
			pos -= MBSZ;
			offset[stats->id] += MBSZ;
			stats->cover[0] += MBSZ;
		}
	}

	for (i = sum = 0; i < b->core.n_cigar; ++i) {
		int oplen = bam_cigar_oplen(cigar[i]);
		char opchr = bam_cigar_opchr(cigar[i]);
		sum += oplen;
		if (sum >= MBSZ)
			__ERROR("Total cigar length over 1 milion!");
		if (pos + oplen >= 2 * MBSZ) {
			collect_coverage(stats, MBSZ);
			memmove(cell[stats->id], cell[stats->id] + MBSZ, MBSZ * sizeof(int));
			memset(cell[stats->id] + MBSZ, 0, MBSZ * sizeof(int));
			offset[stats->id] += MBSZ;
			pos -= MBSZ;
		}

		if (opchr == 'D') {
			pos += oplen;
		} else if (opchr == 'M') {
			++cell[stats->id][pos];
			--cell[stats->id][pos + oplen];
			pos += oplen;
		}
	}
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
