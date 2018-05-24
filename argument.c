#include "argument.h"

struct prog_args args;

void print_usage()
{
	__VERBOSE("./mdup <option> reference_file in_bam_file\n");
	__VERBOSE("\n");
	__VERBOSE("Version: %s\n", VERSION);
	__VERBOSE("\n");
	__VERBOSE("Input bam_file must be sorted by coordinate and be indexed. Only process for primary reads.\n");
	__VERBOSE("\n");
	__VERBOSE("Option:\n");
	__VERBOSE("  -t INT                number of threads [1]\n");
	__VERBOSE("  -o DIR                output directory [\"./mdup_out/\"]\n");
	__VERBOSE("  -g FILE               reference file that generated bam file (better statictis)\n");
	__VERBOSE("  -r                    remove unneeded read (read unmapped, supplementary alignment,\n");
	__VERBOSE("                        not primary aligment, read is mark duplicate, read fail quality checks)\n");
	__VERBOSE("  -b                    barcode mode (reads must has BX:Z: tag present for barcode)\n");
	__VERBOSE("  -n                    don't write new bam (get statistic purpose)\n");
	__VERBOSE("\n");
	__VERBOSE("This tool will generate some file:\n");
	__VERBOSE("  output.bam            bam file after processed\n");
	__VERBOSE("  summary.inf           summary of bam file\n");
	__VERBOSE("  plot.html             plot for some metrics\n");
	__VERBOSE("  molecule.tsv          all molecule info (barcode mode)\n");
}

void get_args(int argc, char *argv[])
{
	int c;
	args.out_dir = "mdup_out";
	args.n_thread = 1;
	args.is_barcode = false;
	args.is_remove = false;
	args.is_write = true;
	args.reference = NULL;

	if (argc < 3) {
		print_usage();
		exit(0);
	}

	while ((c = getopt(argc, argv, "g:rnbht:o:")) >= 0) {
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
		case 'r':
			args.is_remove = true;
			break;
		case 'b':
			args.is_barcode = true;
			break;
		case 'n':
			args.is_write = false;
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
