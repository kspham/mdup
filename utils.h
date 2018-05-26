#ifndef _UTILS_H_
#define _UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>
#include <stdint.h>
#include <stdarg.h>
#include <math.h>
#include <errno.h>
#include <ctype.h>
#include <pthread.h>
#include <semaphore.h>
#include <getopt.h>
#include <unistd.h>

#include <sys/stat.h>
#include <sys/time.h>

/* color terminal */
#define RED			"\x1B[31m"
#define GREEN			"\x1B[32m"
#define YELLOW			"\x1B[33m"
#define BLUE			"\x1B[34m"
#define MAGENTA			"\x1B[35m"
#define CYAN			"\x1B[36m"
#define WHITE			"\x1B[37m"
#define RESET			"\x1B[0m"

#define MAX_INT32		2147483647
#define MIN_INT32		-2147483648

#define MASK32			4294967295ULL

#define BUFSZ			4096
#define	MBSZ			1048576

#define THREAD_STACK_SIZE	16777216

#define FORWARD			0
#define REVERSE			1
#define LEFT			0
#define RIGHT			1

/*
 * Built in macros
 */

#define __abs(x) 		((x) < 0 ? -(x) : (x))

#define __min(a, b) 		((a) < (b) ? (a) : (b))

#define __max(a, b) 		((a) > (b) ? (a) : (b))

#define __min3(a, b, c)		__min(__min((a), (b)), (c))

#define __max3(a, b, c)		__max(__max((a), (b)), (c))

#define __round_up_32(x) 	(--(x), (x) |= (x) >> 1,		       \
				 (x) |= (x) >> 2, (x) |= (x) >> 4,	       \
				 (x) |= (x) >> 8, (x) |= (x) >> 16, ++(x))

/*
 * Built-in macros function
 */

#define __CALLOC(ptr, sz)	(ptr) = calloc(sz, sizeof(*(ptr)))

#define __MALLOC(ptr, sz)	(ptr) = malloc(sizeof(*(ptr)) * (sz))

#define __REALLOC(ptr, sz)	(ptr) = realloc((ptr), sizeof(*(ptr)) * (sz))

#define __FREE_AND_NULL(ptr) do {					       \
	free(ptr);							       \
	(ptr) = NULL;							       \
} while (0)

#define __VERBOSE(fmt, args...) fprintf(stderr, fmt, ##args)

#define __WARNING(fmt, args...) do {					       \
	fprintf(stderr, "WARNING: " fmt, ##args);			       \
} while(0)

#define __ERROR(fmt, args...) do {					       \
	pthread_mutex_lock(&lock_global);				       \
	fprintf(stderr, "ERROR: " fmt "\n", ##args);			       \
	exit(EXIT_FAILURE);						       \
} while(0)

#define __PERROR(fmt) do {						       \
	pthread_mutex_lock(&lock_global);				       \
	fprintf(stderr, "ERROR: ");					       \
	perror(fmt);							       \
	exit(EXIT_FAILURE);						       \
} while(0)

#define __SWAP(x, y) do {						       \
	assert(sizeof(x) == sizeof(y));					       \
	int8_t __swap_temp[sizeof(x)];					       \
	memcpy(__swap_temp, &(y), sizeof(x));				       \
	memcpy(&(y), &(x), sizeof(x));					       \
	memcpy(&(x), __swap_temp, sizeof(x));				       \
} while (0)

/*
 * Built-in function
 */

/* check fread function read enough nmemb */
size_t xfread(void *ptr, size_t size, size_t nmemb, FILE *stream);

/* check fwrite function write enough nmemb */
size_t xfwrite(void *ptr, size_t size, size_t nmemb, FILE *stream);

/* get time */
double realtime();

/* make directory if is not exist */
void make_dir(const char *path);

/* reverse compelemnt */
char *get_rev_complement(const char *seq, int len);

/* reverse string */
char *get_rev(const char *seq, int len);

/* return new char* concate s1 and s2 */
char *str_concate(const char *s1, const char *s2);

/* remove redundant / character */
void normalize_dir(char *path);

void append_file(const char *dest, const char *src);

/*
 * Global variable
 */

extern int8_t nt4_table[256];
extern char *nt4_char, *rev_nt4_char;
extern pthread_mutex_t lock_global;

#endif /* _UTILS_H_ */