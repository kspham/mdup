#include "argument.h"

struct prog_args args;

void print_usage()
{
	__VERBOSE("./mdup <option> reference_file in_bam_file\n");
	__VERBOSE("\n");
	__VERBOSE("Version: %s\n", VERSION);
	__VERBOSE("\n");
	__VERBOSE("mdup take a BAM file as input, the Bam file must be sorted by coordinate and be indexed.\n");
	__VERBOSE("Recommend using BWA to align cloud-read to referenece. All alignment record must have\n");
	__VERBOSE("BX:Z: tag present for barcode.\n");
	__VERBOSE("\n");
	__VERBOSE("Option:\n");
	__VERBOSE("  -t INT                number of threads [1]\n");
	__VERBOSE("  -o DIR                output directory [\"./mdup_out/\"]\n");
	__VERBOSE("  -g FILE               reference file that generated bam file (better statictis)\n");
	__VERBOSE("  -k                    keep all record from BAM file, turn on duplicate bit flag instead.\n");
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
	args.n_thread = 1;
	args.is_remove = true;
	args.reference = NULL;

	if (argc < 3) {
		print_usage();
		exit(0);
	}

	while ((c = getopt(argc, argv, "g:kt:o:")) >= 0) {
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

	args.n_thread = __min(args.n_thread, 20);
	args.n_thread = __max(args.n_thread, 1);
	make_dir(args.out_dir);
}
