#include "argument.h"

struct prog_args args;

#define DEFAULT_N_THREAD	4
#define DEFAULT_THRES_LEN_MLC	1000
#define DEFAULT_THRES_READ_MLC	4

void print_usage()
{
	__VERBOSE("./mdup <option> in_bam_file\n");
	__VERBOSE("\n");
	__VERBOSE("Version: %s\n", VERSION);
	__VERBOSE("\n");
	__VERBOSE("mdup take a BAM file as input, the Bam file must be sorted by coordinate and be indexed.\n");
	__VERBOSE("Recommend using BWA to align cloud-read to referenece. All alignment record must have\n");
	__VERBOSE("BX:Z: tag present for barcode.\n");
	__VERBOSE("\n");
	__VERBOSE("Option:\n");
	__VERBOSE("  -t INT                number of threads (default: 4)\n");
	__VERBOSE("  -o DIR                output directory (default: \"./mdup_out/\")\n");
	__VERBOSE("  -g FILE               reference file that generated bam file (for better statictis)\n");
	__VERBOSE("  -n INT                minimum number of reads require for a molecule (default: 4)\n");
	__VERBOSE("  -l INT                minimum length require for a molecule (default: 1000)\n");
	__VERBOSE("  -k                    don't mark duplicate\n");
	__VERBOSE("\n");
	__VERBOSE("This tool will generate some file in output directory:\n");
	__VERBOSE("  output.bam            BAM file after processed\n");
	__VERBOSE("  summary.inf           stats about sequencing and GEM performance\n");
	__VERBOSE("  plot.html             plot for some metrics of stats\n");
	__VERBOSE("  molecule.tsv          all molecule detected info\n");
}

void get_args(int argc, char *argv[])
{
	int c;
	args.out_dir = "mdup_out";
	args.n_thread = DEFAULT_N_THREAD;
	args.is_remove = true;
	args.reference = NULL;
	args.thres_read_mlc = DEFAULT_THRES_READ_MLC;
	args.thres_len_mlc = DEFAULT_THRES_LEN_MLC;

	if (argc < 2) {
		print_usage();
		exit(0);
	}

	while ((c = getopt(argc, argv, "l:n:g:kt:o:")) >= 0) {
		switch (c) {
		case 't':
			args.n_thread = atoi(optarg);
			break;
		case 'o':
			args.out_dir = optarg;
			break;
		case 'g':
			args.reference = optarg;
			break;
		case 'k':
			args.is_remove = false;
			break;
		case 'l':
			args.thres_len_mlc = atoi(optarg);
			break;
		case 'n':
			args.thres_read_mlc = atoi(optarg);
			break;
		case 'h':
			print_usage();
			exit(0);
		default:
			__VERBOSE("ERROR: Unknow option -%c!\n\n", c);
			print_usage();
			exit(1);
		}
	}

	if (optind + 1 == argc) {
		args.in_bam = argv[optind];
	} else {
		__VERBOSE("ERROR: Wrong argument format!\n\n");
		print_usage();
		exit(1);
	}

	/* limit max threads because bottle neck from I/O */
	args.n_thread = __min(args.n_thread, 10);
	args.n_thread = __max(args.n_thread, 1);
	make_dir(args.out_dir);
}
