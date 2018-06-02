#include "utils.h"

int8_t nt4_table[256] = {
	4, 4, 4, 4,   4, 4, 4, 4,   4, 4, 4, 4,   4, 4, 4, 4, 
	4, 4, 4, 4,   4, 4, 4, 4,   4, 4, 4, 4,   4, 4, 4, 4, 
	4, 4, 4, 4,   4, 4, 4, 4,   4, 4, 4, 4,   4, 4, 4, 4,
	4, 4, 4, 4,   4, 4, 4, 4,   4, 4, 4, 4,   4, 4, 4, 4, 
	4, 0, 4, 1,   4, 4, 4, 2,   4, 4, 4, 4,   4, 4, 4, 4, 
	4, 4, 4, 4,   3, 4, 4, 4,   4, 4, 4, 4,   4, 4, 4, 4, 
	4, 0, 4, 1,   4, 4, 4, 2,   4, 4, 4, 4,   4, 4, 4, 4, 
	4, 4, 4, 4,   3, 4, 4, 4,   4, 4, 4, 4,   4, 4, 4, 4, 
	4, 4, 4, 4,   4, 4, 4, 4,   4, 4, 4, 4,   4, 4, 4, 4, 
	4, 4, 4, 4,   4, 4, 4, 4,   4, 4, 4, 4,   4, 4, 4, 4, 
	4, 4, 4, 4,   4, 4, 4, 4,   4, 4, 4, 4,   4, 4, 4, 4, 
	4, 4, 4, 4,   4, 4, 4, 4,   4, 4, 4, 4,   4, 4, 4, 4, 
	4, 4, 4, 4,   4, 4, 4, 4,   4, 4, 4, 4,   4, 4, 4, 4, 
	4, 4, 4, 4,   4, 4, 4, 4,   4, 4, 4, 4,   4, 4, 4, 4, 
	4, 4, 4, 4,   4, 4, 4, 4,   4, 4, 4, 4,   4, 4, 4, 4, 
	4, 4, 4, 4,   4, 4, 4, 4,   4, 4, 4, 4,   4, 4, 4, 4
};

char *nt4_char = "ACGTN";
char *rev_nt4_char = "TGCAN";

pthread_mutex_t lock_global = PTHREAD_MUTEX_INITIALIZER;

size_t xfread(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	size_t ret = fread(ptr, size, nmemb, stream);
	if (ret != nmemb)
		__ERROR("fread error, file is wrong format or is corrupted");
	return ret;
}

size_t xfwrite(void *ptr, size_t size, size_t nmemb, FILE *stream)
{
	size_t ret = fwrite(ptr, size, nmemb, stream);
	if (ret != nmemb)
		__ERROR("fwrite error, could not write data to file");
	return ret;
}

double realtime()
{
	struct timeval tp;
	gettimeofday(&tp, NULL);
	return tp.tv_sec + tp.tv_usec * 1e-6;
}

double cputime()
{
	struct rusage r;
	getrusage(RUSAGE_SELF, &r);
	return r.ru_utime.tv_sec + r.ru_stime.tv_sec + 1e-6 * (r.ru_utime.tv_usec + r.ru_stime.tv_usec);
}

void make_dir(const char *path)
{
	struct stat st = {0};
	if (stat(path, &st) == -1)
		if (mkdir(path, 0700))
			__PERROR("Could not make output directory");
}

char *get_rev(const char *seq, int len)
{
	if (seq == NULL)
		return NULL;

	int i, k;
	char *ret = malloc(len + 1);
	for (i = 0, k = len - 1; i < len; ++i, --k)
		ret[i] = seq[k];
	ret[len] = '\0';
	return ret;
}

char *get_rev_complement(const char *seq, int len)
{
	if (seq == NULL)
		return NULL;

	char *ret = malloc(len + 1);
	int i, k;
	for (i = 0, k = len - 1; i < len; ++i, --k) 
		ret[i] = rev_nt4_char[nt4_table[seq[k]]];
	ret[len] = '\0';
	return ret;
}

void normalize_dir(char *path)
{
	int len = strlen(path), i, j;
	for (i = 0; i < len - 1; ) {
		if (path[i] == '/' && path[i + 1] == '/') {
			for (j = i; j < len; ++j)
				path[j] = path[j + 1];
			--len;
		} else {
			++i;
		}
	}
}

char *str_concate(const char *str1, const char *str2)
{
	size_t len1 = strlen(str1);
	size_t len2 = strlen(str2);
	char *str3 = malloc(len1 + len2 + 1);
	strcpy(str3, str1);
	strcpy(str3 + len1, str2);
	str3[len1 + len2] = '\0';
	return str3;
}

void append_file(const char *dest, const char *src, int offset)
{
	FILE *fi_dest = fopen(dest, "r+");
	FILE *fi_src = fopen(src, "r");
	assert(fi_dest);
	assert(fi_src);
	fseek(fi_dest, offset, SEEK_END);
	while (1) {
		char buf[MBSZ];
		memset(buf, 0, MBSZ);
		int ret = fread(buf, 1, MBSZ, fi_src);
		xfwrite(buf, 1, ret, fi_dest);
		if (ret < MBSZ)
			break;
	}
	fclose(fi_dest);
	fclose(fi_src);
}
