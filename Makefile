CC_FLAG = 		-Wno-unused-result -Wno-char-subscripts -Wfatal-errors -Wall -pthread -O2

INCLUDE_LIB =		-I htslib/include

HTSLIB =		-L htslib/lib

OBJ_MAIN =		molecule.o		\
			duplicate.o		\
			stats.o			\
			khash_bx.o		\
			utils.o			\
			bam.o			\
			argument.o

all:
	gcc $(CC_FLAG) $(INCLUDE_LIB) $(HTSLIB) -c -o utils.o utils.c
	gcc $(CC_FLAG) $(INCLUDE_LIB) $(HTSLIB) -c -o molecule.o molecule.c
	gcc $(CC_FLAG) $(INCLUDE_LIB) $(HTSLIB) -c -o khash_bx.o khash_bx.c
	gcc $(CC_FLAG) $(INCLUDE_LIB) $(HTSLIB) -c -o duplicate.o duplicate.c
	gcc $(CC_FLAG) $(INCLUDE_LIB) $(HTSLIB) -c -o stats.o stats.c
	gcc $(CC_FLAG) $(INCLUDE_LIB) $(HTSLIB) -c -o argument.o argument.c
	gcc $(CC_FLAG) $(INCLUDE_LIB) $(HTSLIB) -c -o bam.o bam.c
	gcc $(CC_FLAG) $(INCLUDE_LIB) $(HTSLIB) $(OBJ_MAIN) markdup.c htslib/lib/libhts.a -o mdup -lz
	rm -rf *.o
