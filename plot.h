#ifndef _PLOT_H_
#define _PLOT_H_

#include "attr.h"
#include "stats.h"

void plot_render(const char *file_path);

void plot_data_init(const char *file_path);

void plot_data_destroy();

void plot_read_cover(const int64_t *cover);

void plot_mlc_len(const int64_t *mlc_len);

void plot_mlc_cover(const int64_t *mlc_cover);

#endif /* _PLOT_H_ */