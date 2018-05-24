#include "htslib/khash.h"

#include "stats.h"
#include "argument.h"
#include "bam.h"
#include "duplicate.h"

static int n_done;
static double prev_time;
static int64_t sum_nt4, sum_amb;
static struct stats_t all_stats;

static pthread_mutex_t lock_merge = PTHREAD_MUTEX_INITIALIZER;
static pthread_mutex_t lock_id = PTHREAD_MUTEX_INITIALIZER;

static int get_median_isize(struct stats_t *stats)
{
	int i;
	int64_t sum1 = 0, sum2 = 0;
	for (i = 0; i <= N_ISIZE; ++i)
		sum1 += stats->isize[i];
	for (i = 0; i <= N_ISIZE; ++i) {
		sum2 += stats->isize[i];
		if (sum2 >= (sum1 >> 1))
			return i;
	}
	return N_ISIZE;
}

// void output_mlc()
// {
// 	total_gem_detected = 0;
// 	int64_t total_dna_20kb = 0, total_dna_100kb = 0;
// 	int64_t total_mlc_len = 0, total_mlc_cnt = 0, n_mlc = 0;
// 	struct arr_pair_t mlc = (struct arr_pair_t){.val = NULL, .sz = 0};
// 	int64_t mlc_plot[N_MLC];
// 	memset(mlc_plot, 0, N_MLC * sizeof(int64_t));

// 	fprintf(fi_
// 	fprintf(fi_mlc, "#chr\tstart\tend\tbarcode\tread_count\n");

// 	for (i = 0; i < stats->n_gem; ++i) {
// 		if (!genome_st.gem[i].sz) {
// 			free(genome_st.gem[i].barcode);
// 			continue;
// 		}
// 		++total_gem_detected;
// 		n_mlc += genome_st.gem[i].sz;

// 		for (j = 0; j < genome_st.gem[i].sz; ++j) {
// 			struct mole_t *cur_mlc = genome_st.gem[i].mlc + j;
// 			int len = cur_mlc->len;
// 			int cnt = cur_mlc->cnt;
// 			int start = cur_mlc->start;
// 			int end = cur_mlc->end;
// 			int chr_id = cur_mlc->chr_id;
// 			char *barcode = genome_st.gem[i].barcode;
// 			fprintf(fi_mlc, "%s\t%d\t%d\t%s\t%d\n",
// 				bam_inf->b_hdr->target_name[chr_id],
// 				start, end, barcode, cnt);
// 			if (len >= MLC_20KB)
// 				total_dna_20kb += len;
// 			if (len >= MLC_100KB)
// 				total_dna_100kb += len;
// 			total_mlc_len += len;
// 			total_mlc_cnt += cnt;
// 			++mlc.sz;
// 			if ((mlc.sz & (mlc.sz - 1)) == 0)
// 				mlc.val = realloc(mlc.val, (mlc.sz << 1) *
// 						  sizeof(uint64_t));
// 			mlc.val[mlc.sz - 1] = (struct pair_t){.first = cnt, .second = len};

// 			int bin_id = len / MLC_BIN_PLOT;
// 			if (bin_id < N_MLC)
// 				mlc_plot[bin_id] += len;
// 		}

// 		free(genome_st.gem[i].barcode);
// 	}

// 	qsort(mlc.val, mlc.sz, sizeof(uint64_t), cmpfunc_mlc);
// 	int64_t sum = 0;
// 	int n50_read_per_mlc = -1;
// 	for (i = sum = 0; i < mlc.sz; ++i) {
// 		int cnt = mlc.val[i].first;
// 		sum += cnt;
// 		if (sum >= (total_mlc_cnt >> 1)) {
// 			n50_read_per_mlc = cnt;
// 			break;
// 		}
// 	}
// 	assert(n50_read_per_mlc != -1);

	/* output barcode summary */
	// fprintf(fi_sum, "******** Sequencing summary ********\n");
	// fprintf(fi_sum, "GEMs Detected:\t%d\n", total_gem_detected);
	// fprintf(fi_sum, "Mean DNA per GEM:\t%ld\n",
	// 	total_mlc_len / total_gem_detected);
	// fprintf(fi_sum, "DNA in Molecules >20kb:\t%.1f%%\n",
	// 	1.0 * total_dna_20kb / total_mlc_len * 100);
	// fprintf(fi_sum, "DNA in Molecules >100kb:\t%.1f%%\n",
	// 	1.0 * total_dna_100kb / total_mlc_len * 100);
	// fprintf(fi_sum, "Mean Molecule Length:\t%ld\n",
	// 	total_mlc_len / mlc.sz);
	// fprintf(fi_sum, "Mean Molecule per GEM:\t%ld\n",
	// 	n_mlc / total_gem_detected);
	// fprintf(fi_sum, "N50 Reads per Molecule (LPM):\t%d\n", n50_read_per_mlc);
// }

static void output_result(struct stats_t *stats, char **target_name)
{
	stats->cover[0] -= sum_amb;
	char file_path[BUFSZ];
	FILE *fi_sum;

	sprintf(file_path, "%s/summary.inf", args.out_dir);
	if (!(fi_sum = fopen(file_path, "w")))
		__PERROR("Could not open summary.inf");

	/* output sequencing summary */
	fprintf(fi_sum, "******** Sequencing summary ********\n");
	if (stats->n_base1 != 0)
		fprintf(fi_sum, "Mode: Paired-end\n");
	else
		fprintf(fi_sum, "Mode: Single-end\n");
	fprintf(fi_sum, "Number of Reads:\t%lu\n", stats->n_read);
	if (stats->n_base1 != 0) {
		fprintf(fi_sum, "Average Read 1 Length:\t%lu\n",
			stats->sum_len1 / stats->n_read1);
		fprintf(fi_sum, "Average Read 2 Length:\t%lu\n",
			stats->sum_len2 / stats->n_read2);
	} else {
		fprintf(fi_sum, "Average Read Length:\t%lu\n",
			stats->sum_len2 / stats->n_read);
	}
	fprintf(fi_sum, "PCR Duplication:\t%.2f%%\t\n",
		1.0 * stats->n_dup / stats->n_read * 100);
	fprintf(fi_sum, "Mapped Reads:\t%.2f%%\t\n",
		1.0 * (stats->n_read - stats->n_unmap) / stats->n_read * 100);
	fprintf(fi_sum, "MQ0 Reads:\t%.2f%%\t\n",
		1.0 * stats->n_mq0 / stats->n_read * 100);

	if (stats->n_base1 != 0) {
		fprintf(fi_sum, "Q30 bases, Read 1:\t%.2f%%\n",
			1.0 * stats->q30_base1 / stats->n_base1 * 100);
		fprintf(fi_sum, "Q30 bases, Read 2:\t%.2f%%\n",
			1.0 * stats->q30_base2 / stats->n_base2 * 100);
	} else {
		fprintf(fi_sum, "Q30 bases:\t%.2f%%\n",
			1.0 * stats->q30_base2 / stats->n_base2 * 100);
	}
	fprintf(fi_sum, "Zero Coverage:\t%.2f%%\n",
		1.0 * stats->cover[0] / sum_nt4 * 100);
	if (stats->n_base1 != 0)
		fprintf(fi_sum, "Median Insert Size:\t%d\n", get_median_isize(stats));
	fprintf(fi_sum, "Mean Depth:\t%.1fX\n",
		1.0 * (stats->sum_len1 + stats->sum_len2) / (sum_nt4 + sum_amb));
	fprintf(fi_sum, "\n");

	fclose(fi_sum);
}

static void *process_pool(void *data)
{
	struct bam_inf_t *bam_inf = (struct bam_inf_t *)data;
	struct stats_t stats;
	int id;

	do {
		pthread_mutex_lock(&lock_id);
		if (bam_inf->cur_id == bam_inf->b_hdr->n_targets) {
			pthread_mutex_unlock(&lock_id);
			pthread_exit(NULL);
			return NULL;
		} else {
			id = bam_inf->cur_id++;
		}
		pthread_mutex_unlock(&lock_id);

		memset(&stats, 0, sizeof(struct stats_t));
		stats.id = id;
		read_bam_target(bam_inf, id, &stats);

		pthread_mutex_lock(&lock_id);
		__VERBOSE("\rDone %d targets", ++n_done);
		stats_merge(&all_stats, &stats);
		pthread_mutex_unlock(&lock_id);
	} while (1);
}

static void read_bam(struct bam_inf_t *bam_inf)
{
	__VERBOSE("Mark duplicate ... \n");
	prev_time = realtime();

	int i;
	char file_path[BUFSZ], result_path[BUFSZ];
	samFile *out_bam_f;
	sprintf(result_path, "%s/output.bam", args.out_dir);
	out_bam_f = sam_open(result_path, "wb");
	sam_hdr_write(out_bam_f, bam_inf->b_hdr);
	sam_close(out_bam_f);

	/* Get unmapped read, tid == -1 */
	read_bam_unmapped(bam_inf, &all_stats);

	/* get bam stats by parallel */
	pthread_t *pthr = calloc(args.n_thread, sizeof(pthread_t));;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setstacksize(&attr, THREAD_STACK_SIZE);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
	for (i = 0; i < args.n_thread; ++i)
		pthread_create(&pthr[i], &attr, process_pool, bam_inf);
	for (i = 0; i < args.n_thread; ++i)
		pthread_join(pthr[i], NULL);

	for (i = 0; i < bam_inf->b_hdr->n_targets; ++i) {
		sprintf(file_path, "%s/temp.%s.bam", args.out_dir,
			bam_inf->b_hdr->target_name[i]);
		append_file(result_path, file_path);
		remove(file_path);
	}
	sprintf(file_path, "%s/unmapped.bam", args.out_dir);
	append_file(result_path, file_path);
	remove(file_path);

	free(pthr);
	pthread_attr_destroy(&attr);
	__VERBOSE("\nTotal time: %.2fs\n", realtime() - prev_time);
}

static int load_reference(char *file_path)
{
	__VERBOSE("Loading reference ... ");

	prev_time = realtime();
	int i, id;
	size_t len = 0;
	char *s = NULL, c;
	FILE *fi = fopen(file_path, "r");

	if (!fi) {
		__VERBOSE("\n");
		__PERROR("Could not open reference file");
	}

	c = getc(fi);
	if (c != '>') {
		__VERBOSE("\n");
		__ERROR("Genome file is not fasta format!");
	}
	ungetc(c, fi);

	id = -1;
	while (xgetline(&s, &len, fi) != EOF) {
		if (s[0] == '>') {
			++id;
			continue;
		}
		for (i = 0; s[i]; ++i)
			if (nt4_table[s[i]] == 4)
				++sum_amb;
	}

	fclose(fi);
	__VERBOSE("%.2fs\n", realtime() - prev_time);
	return id + 1;
}

int main(int argc, char *argv[])
{
	int i;

	get_args(argc, argv);
	struct bam_inf_t bam_inf;
	bam_inf_init(&bam_inf, args.in_bam);

	for (i = 0; i < bam_inf.b_hdr->n_targets; ++i)
		sum_nt4 += bam_inf.b_hdr->target_len[i];
	
	if (args.reference) {
		if (bam_inf.b_hdr->n_targets != load_reference(args.reference))
			__ERROR("Reference file is not correct!");
		sum_nt4 -= sum_amb;
		assert(sum_nt4 > 0);
	}

	duplicate_init(bam_inf.b_hdr->n_targets);
	coverage_init(bam_inf.b_hdr->n_targets, bam_inf.b_hdr->target_len);
	// mlc_init(bam_inf.b_hdr->n_targets, args.distance_thres);

	read_bam(&bam_inf);

	__VERBOSE("Output result ... \n");
	output_result(&all_stats, bam_inf.b_hdr->target_name);

	coverage_destroy(bam_inf.b_hdr->n_targets);
	duplicate_destroy();
	pthread_mutex_destroy(&lock_id);
	pthread_mutex_destroy(&lock_merge);

	return 0;
}
