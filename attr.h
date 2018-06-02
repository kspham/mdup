#ifndef _ATTR_H_
#define _ATTR_H_

#include "utils.h"
#include "htslib/sam.h"

/* Sam flag */
#define FLAG_PAIR		0x001	/* Read has its mate */
#define FLAG_PROPER 		0x002	/* Read and its mate are proper pair */
#define FLAG_UNMAP 		0x004	/* Read is unmapped */
#define FLAG_M_UNMAP		0x008	/* Read's mate is unmapped */
#define FLAG_REVERSE 		0x010	/* Read is reversed */
#define FLAG_M_REVERSE 		0x020	/* Read's mate is reversed */
#define FLAG_READ1 		0x040	/* Read is first read */
#define FLAG_READ2 		0x080	/* Read is second read */
#define FLAG_NOT_PRI		0x100	/* Alginment is not primary */
#define FLAG_DUPLICATE 		0x400	/* Read is pcr or duplicated */
#define FLAG_SUPPLEMENT		0x800	/* Alignment is supplementary */

#define MLC_CONS_THRES		50000
#define MLC_20KB		20000
#define MLC_100KB		100000
#define MLC_BIN_PLOT		4000

extern struct prog_args args;

#endif /* _ATTR_H_ */
