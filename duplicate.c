#include "duplicate.h"
#include "argument.h"

static struct duplst_t *duplicate;

static inline int cmpfunc_alg(const void *a, const void *b)
{
	struct align_t *u = (struct align_t *)a;
	struct align_t *v = (struct align_t *)b;
	int ret;
	if (u->bx_id != v->bx_id)
		return u->bx_id - v->bx_id;
	if (u->b->core.mpos != v->b->core.mpos)
		return u->b->core.mpos - v->b->core.mpos;
	if (u->b->core.mtid != v->b->core.mtid)
		return u->b->core.mtid - v->b->core.mtid;
	if (u->flag != v->flag)
		return u->flag - v->flag;
	ret = strcmp(u->cigar, v->cigar);
	if (ret != 0)
		return ret;
	if (u->mcigar == NULL && v->mcigar == NULL)
		return 0;
	if (u->mcigar == NULL)
		return -1;
	if (v->mcigar == NULL)
		return 1;
	ret = strcmp(u->mcigar, v->mcigar);
	/* ret == 0 -> all are equal */
	return ret;
}

void duplicate_init(int n_target)
{
	__CALLOC(duplicate, n_target);
}

void duplicate_destroy()
{
	__FREE_AND_NULL(duplicate);
}

static int sum_base_qual(bam1_t *b)
{
	uint8_t *qual = bam_get_qual(b);
	int ret = 0, i;
	for (i = 0; i < b->core.l_qseq; ++i)
		ret += qual[i];
	return ret;
}

void duplicate_process(struct stats_t *stats, samFile *out_bam_f, bam_hdr_t *b_hdr)
{
	int max_val, pos, ret, u = 0, v = 0, i;
	struct align_t *align = duplicate[stats->id].align;
	int n_align = duplicate[stats->id].n_align;

	qsort(align, n_align, sizeof(struct align_t), cmpfunc_alg);

	while (u < n_align) {
		max_val = sum_base_qual(align[u].b);
		pos = u;
		v = u + 1;

		while (v < n_align && cmpfunc_alg(&align[u], &align[v]) == 0) {
			ret = sum_base_qual(align[v].b);
			if (ret > max_val) {
				max_val = ret;
				pos = v;
			}
			++v;
			++stats->n_dup;
		}

		for (i = u; i < v; ++i) {
			if (i == pos)
				continue;
			align[i].b->core.flag |= FLAG_DUPLICATE;
			if (!args.is_remove)
				sam_write1(out_bam_f, b_hdr, align[i].b);
		}

		sam_write1(out_bam_f, b_hdr, align[pos].b);
		get_coverage(align[pos].b, stats);
		// mlc_insert(align[u].bx_id, align[u].b, stats);
		u = v;
	}

	for (i = 0; i < n_align; ++i) {
		bam_destroy1(align[i].b);
		free(align[i].mcigar);
		free(align[i].cigar);
	}
	duplicate[stats->id].n_align = 0;
}

static char *convert_scigar(uint32_t *bcigar, int sz)
{
	assert(bcigar);
	char *ret = NULL, tmp[BUFSZ];
	int i, len = 0, tlen;

	for (i = 0; i < sz; ++i) {
		sprintf(tmp, "%d%c", bam_cigar_oplen(bcigar[i]),
			bam_cigar_opchr(bcigar[i]));
		tlen = strlen(tmp);
		__REALLOC(ret, len + tlen);
		memcpy(ret + len, tmp, tlen);
		len += tlen;
	}
	__REALLOC(ret, len + 1);
	ret[len] = '\0';

	return ret;
}

void duplicate_try_process(int pos, struct stats_t *stats,
			   samFile *out_bam_f, bam_hdr_t *b_hdr)
{
	struct align_t *align =duplicate[stats->id].align;
	int n_align = duplicate[stats->id].n_align;

	if (n_align > 0 && pos != align[0].b->core.pos)
		duplicate_process(stats, out_bam_f, b_hdr);
}

void duplicate_insert(bam1_t *b, int bx_id, struct stats_t *stats,
		      samFile *out_bam_f, bam_hdr_t *b_hdr)
{
	struct align_t **align = &(duplicate[stats->id].align);
	int *n_align = &(duplicate[stats->id].n_align);
	uint8_t *tag_data = bam_aux_get(b, "MC");

	if (*n_align > 0 && b->core.pos != (*align)[0].b->core.pos) {
		duplicate_process(stats, out_bam_f, b_hdr);
		goto insert;
	}

insert:
	++(*n_align);
	__REALLOC(*align, *n_align);
	(*align)[*n_align - 1].b = bam_dup1(b);
	(*align)[*n_align - 1].b->core.flag &= ~FLAG_DUPLICATE;
	(*align)[*n_align - 1].bx_id = bx_id;
	(*align)[*n_align - 1].cigar = convert_scigar(bam_get_cigar(b), b->core.n_cigar);
	if (tag_data)
		(*align)[*n_align - 1].mcigar = strdup(bam_aux2Z(tag_data));
	else
		(*align)[*n_align - 1].mcigar = NULL;
	(*align)[*n_align - 1].flag = b->core.flag & (FLAG_REVERSE | FLAG_M_REVERSE);
}
