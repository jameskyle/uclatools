/*
 *  Definition of image files and image registers
 * 
 *  Copyright (c): 1985, 1987 Context Vision AB
 *
 */

#define image_table_directory "./"

#define image_magic 0xE9ED0000

#define im_p16 1
#define im_s16 2
#define im_c16 3
#define im_p8 4
#define im_u8 5
#define im_s8 6
#define im_c8 7
#define im_x8 8
#define im_b1 9
#define im_p32 10
#define im_s32 11
#define im_c32 12

/* Header record for image files */
struct image_head
{   unsigned long id;
    unsigned long off;
    unsigned long len;
    unsigned short hgh;
    unsigned short wid;
    unsigned short typ;
};

/* Image register file descriptor */
struct ireg_file
{   int imno;
    int index;
    unsigned short hgh;
    unsigned short wid;
    unsigned short typ;
    int c_time;
    int c_user;
    int c_group;
    int a_time;
    int a_user;
    int a_group;
    int protection;
};

/* Image register descriptor */
struct ireg
{   unsigned char *ptr;
    unsigned short hgh;
    unsigned short wid;
    unsigned short typ;
    unsigned int phys_addr;
    unsigned int bf_index;
};

/* int irinit() */
/* */
/* Initialilizes or reinitializes this module. */
/* It is not required to call this routine, but it is useful if */
/* the "database" has been changed. */

int irinit();

/* irclear() */
/* Clear the internal status. */
/* Useful when the file has been changed by another process. */
/* It is recomended to use this procedure instead of irinit. */

void irclear(void);

/* struct ireg *irfind(n) */
/*   int n; */
/* Find an image register descriptor given the image register number */

struct ireg *irfind();

/* int imsize(ih) */
/*   struct image_head *ih; */
/* Gets number of bytes in an image */

int imsize();

/* int irsize(tp) */
/*   struct ireg *tp; */
/* Gets number of bytes in an image register */

int irsize();

/* char *pixstr(n) */
/*   int n; */
/* Converts pixel data type number to string */

char *pixstr();

/* int pixnum(s) */
/*   char *s; */
/* Converts pixel data type name to number */

int pixnum();

char *cvint();


#define IR_NOERR 0
#define IR_SYSERR 1
#define IR_BFOFFERR 2
#define IR_NODCUERR 3
#define IR_FOUNDERR 4
#define IR_BFACCESSERR 5
#define IR_TYPERR 6
