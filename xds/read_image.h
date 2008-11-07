/*
 *  READ_IMAGE_VERSION should be checked on return from read_image; it will be
 *      updated whenever the definition of a |struct image| is changed.  See
 *      the read_image listing/manual for details.
 *
 *  The first change which required updating VERSION_NUMBER was the addition
 *      of the field `type' to the struct image definition.  It is the type
 *      of file which was read; it should be one of the constants IDF, GED, 
 *      etc
 *  The second change which required updating VERSION_NUMBER was the addition
 *      of the field `union_type' to the struct image definition.  It is the
 *      type pointed to by the union pointers in the struct.  It should be one
 *      of T_CHAR, T_SHORT, etc.  For the most part the T_XXX names should be
 *      self explanatory, but one (T_NATIVE) isn't; it means ``I want the
 *      same type the imager produces''.  (In this case, the type can
 *      be determined by examining the type field in the struct image.) 
 */
#define READ_IMAGE_VERSION	3	

#define	T_CHAR		 1
#define T_SHORT		 2
#define T_LONG		 3
#define T_FLOAT		 4
#define T_DOUBLE	 5
#define T_COMPLEX	 6
#define T_UCHAR		 7
#define T_USHORT	 8
#define T_ULONG		 9
#define T_NATIVE	10


/*
 *  Any time these constants are updated, the definitions for 
 *      image_types_short and image_types should also be updated.
 */
#define NONE		0
#define IDF		1
#define GED		2
#define OMEGA		3
#define ANMR		4
#define FLT		5
#define STR		6
#define BRUK		7
#define GEDNO		8
#define BLOCK_CHAR	9
#define BLOCK_UCHAR	10
#define BLOCK_SHORT	11
#define BLOCK_USHORT	12
#define BLOCK_LONG	13
#define BLOCK_ULONG	14
#define BLOCK_FLOAT	15
#define IMG		16
#define GENESIS		17
#define SISCO		18

#define NO_SUCH_FILE	-1
#define BAD_FILE	-2
#define BAD_SCALE	-3

#include <stdlib.h>
#include <stdio.h> 
#include <ctype.h> 
#include <string.h> 
#include <sys/fcntl.h> 
#include <sys/types.h> 
#include <sys/file.h> 
#include <sys/param.h> 
#include <unistd.h> 


struct complex {
    float r, i;
};

union image_union { 
    char		*c;
    short		*s;
    long		*l;
    float		*f;
    double		*d;
    struct complex  	*co;
    unsigned char	*uc;
    unsigned short	*us;
    unsigned long	*ul; 
} ; 

union row_ptr_union { 
    char		**c;
    short		**s;
    long		**l;
    float		**f;
    double		**d;
    struct complx	**co;
    unsigned char	**uc;
    unsigned short	**us;
    unsigned long	**ul; 
} ; 

struct image { 
    int version;
    int type;
    int union_type;
    int rows;
    int cols;
    int images;
    float min, max;
    unsigned char *hdr; 
    union image_union image;
    union row_ptr_union row_ptr; 
    float dim_x, dim_y, dim_z;
    float scale;
} ; 
