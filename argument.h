#ifndef _ARGUMENT_H_
#define _ARGUMENT_H_

#include "attr.h"

#define VERSION			"1.4"

struct prog_args {
	int n_thread;
	bool is_remove;
	char *in_bam;
	char *reference;
	char *out_dir;
	int thres_read_mlc;
	int thres_len_mlc;
};

void print_usage();

void get_args(int argc, char *argv[]);

#endif /* _ARGUMENT_H_ */
