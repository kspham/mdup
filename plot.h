#ifndef _PLOT_H_
#define _PLOT_H_

#include "attr.h"
#include "stats.h"

void plot_init(const char *file_path);

void plot_coverage(const int64_t *cover);

void plot_mlc_len(const int64_t *mlc_len);

void plot_destroy();

#endif /* _PLOT_H_ */