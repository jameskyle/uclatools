#define NEW_HDR 0
#define OLD_HDR 1
#define LABLENGTH       30
#define MYLABLENGTH     32

#include <stdlib.h> 
#include <string.h>
#include <sys/fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <stdio.h>

void*malloc(),*realloc();

struct pt {short x, y;} ;
 
struct outl_data_old {
    struct pt seed;
    short num;
    short area;
    char label[LABLENGTH];
} ;
 
struct outl_data_new {
    short version;
    short rows;
    short cols;
    short scale;
    struct pt seed;
    short num;
    short area;
    char label[LABLENGTH];
} ;
 
union hdr {
    struct outl_data_old old;
    struct outl_data_new new;
} ;

int read_otl(char *filename, char *region, int rows, int cols, 
	 unsigned char **raster, int *factor);

int summarize_otl(char *fname, char **regions);
