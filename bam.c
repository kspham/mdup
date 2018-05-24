#include "bam.h"
#include "argument.h"
#include "khash_bx.h"
#include "duplicate.h"
#include "molecule.h"

void bam_inf_init(struct bam_inf_t *bam_inf, const char *file_path)
{
	bam_inf->bam_path = file_path;
	bam_inf->cur_id = 0;

	/* Open bam file and the index file */
	samFile *bam_f;
	if (!(bam_f = sam_open(file_path, "rb")))
		__PERROR("Could not open BAM file");
	if (!(bam_inf->bam_i = sam_index_load(bam_f, file_path)))
		__ERROR("BAM file must be indexed!");

	/* Init the header */
	bam_inf->b_hdr = sam_hdr_read(bam_f);
	sam_close(bam_f);
}

void read_bam_unmapped(struct bam_inf_t *bam_inf, struct stats_t *stats)
{
	samFile *bam_f = sam_open(bam_inf->bam_path, "rb");
	hts_itr_t *iter = sam_itr_queryi(bam_inf->bam_i, HTS_IDX_NOCOOR, 0, 0);
	bam1_t *b = bam_init1();
	char file_path[BUFSZ];
	sprintf(file_path, "%s/unmapped.bam", args.out_dir);
	samFile *out_bam_f = sam_open(file_path, "wb");

	/* all bam record here is unmapped */
	while (sam_itr_next(bam_f, iter, b) >= 0) {
		if ((b->core.flag & (FLAG_NOT_PRI | FLAG_SUPPLEMENT)) != 0) {
			__VERBOSE("\n");
			__ERROR("Unmapped read doesn't have not primary flag or suplementary flag!");
		}
		get_basic_stats(b, stats);
		if (!args.is_remove)
			sam_write1(out_bam_f, bam_inf->b_hdr, b);
	}

	sam_close(out_bam_f);
}

void read_bam_target(struct bam_inf_t *bam_inf, int id, struct stats_t *stats)
{
	int target_len = bam_inf->b_hdr->target_len[id];
	samFile *in_bam_f = sam_open(bam_inf->bam_path, "rb");
	hts_itr_t *iter = sam_itr_queryi(bam_inf->bam_i, id, 1, target_len);
	bam1_t *b = bam_init1();
	int bx_map_cnt = 0;
	khash_t(KHASH_STR) *khash_bx = kh_init(KHASH_STR);
	char file_path[BUFSZ];
	sprintf(file_path, "%s/temp.%s.bam", args.out_dir,
		bam_inf->b_hdr->target_name[id]);
	samFile *out_bam_f = sam_open(file_path, "wb");

	while (sam_itr_next(in_bam_f, iter, b) >= 0) {
		uint8_t *tag_data;
		char *bar_s = NULL;
		int bx_id = -1;

		/* skip align is supplementary or not primary */
		if (b->core.flag & (FLAG_NOT_PRI | FLAG_SUPPLEMENT)) {
			duplicate_try_process(b->core.pos, stats,
					      out_bam_f, bam_inf->b_hdr);
			if (!args.is_remove)
				sam_write1(out_bam_f, bam_inf->b_hdr, b);
			continue;
		}

		get_basic_stats(b, stats);

		/* only get basic stats for unmapped read */
		if (b->core.n_cigar == 0) {
			duplicate_try_process(b->core.pos, stats,
					      out_bam_f, bam_inf->b_hdr);
			if (!args.is_remove)
				sam_write1(out_bam_f, bam_inf->b_hdr, b);
			continue;
		}

		tag_data = bam_aux_get(b, "BX");
		if (!tag_data)
			__ERROR("Read doesn't have BX:Z: tag!");
		bar_s = bam_aux2Z(tag_data);
		bx_id = khash_bx_get_id(khash_bx, &bx_map_cnt, bar_s);

		duplicate_insert(b, bx_id, stats, out_bam_f, bam_inf->b_hdr);
	}

	duplicate_process(stats, out_bam_f, bam_inf->b_hdr);
	cal_rest_coverage(stats);
	// mlc_get_last(stats);
	khash_bx_destroy(khash_bx);
	sam_close(in_bam_f);
	sam_close(out_bam_f);
}
