/* Attempt to read relevant ACR-NEMA groups for reading Siemens images. */
/* Copyright 1994 Timothy L. Davis */

#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#ifndef NO_MMAP
#include <sys/mman.h>
#endif /*NO_MMAP*/
#include <limits.h>
#include "read_image.h"
#ifndef NO_FLOAT_H
#include <float.h>
#endif /*NO_FLOAT_H*/
#include "assert_print.h"
#include "ds_head_constants.h"
#include "ds_date.h"
#include "ds_head_basic_types.h"
#include "ds_head_acr_groups_types.h"

/*Every element has this as its first records*/
typedef struct acr_elem_head_struct {
  short group_number;
  short element_number;
  long element_length;
} acr_elem_head_t;

/*First element in each group defines the remaining group length. Use for skipping.*/
typedef struct acr_elem0_struct {
  acr_elem_head_t head;
  long group_length;
} acr_elem0_t;

int ByteSwapping = 1;
int WordSwapping = 1;
int ReadACRLoud=0, ReadACRVeryLoud=0;

/* BYTE-SWAP if necessary after fread, word swap is later. */
size_t fread_swap(void *ptr, size_t size, size_t nitems, FILE *stream)
{
  size_t fread_return = fread(ptr, size, nitems, stream);
  char *cptr = (char *) ptr;
  int i;
  if (!fread_return) return 0;
  if (ByteSwapping) for (i = 0; i < fread_return*size-1; i+=2)
    { char tmp= cptr[i];   cptr[i]=cptr[i+1];      cptr[i+1]=tmp;  }
  return fread_return;
}

/*Word swap is on demand since we must do it field by field! (yuck!) */
void word_swap(void *ptr, size_t nitems)
{
  int i;
  short *wptr = (short *)ptr;
  if (WordSwapping) for (i=0; i< nitems-1; i+=2)
    { short tmp=wptr[i]; wptr[i]=wptr[i+1]; wptr[i+1]=tmp; }
}

/* 
 * Read the group length (disk size), allocate memory, and
 * read C-style element data for the next group in the file.
 * return pointer to collapsed data, group number, actual collapsed data 
 * length.
 */
void *read_next_group(FILE *fp, int *groupnum_ret, size_t *grouplen_ret)
{
  struct acr_elem0_struct elem0;
  struct acr_elem_head_struct elem_head;
  char *group_ptr, *gp;

  if (fread_swap(&elem0, sizeof(elem0), 1, fp) != 1)
    return 0; /* Couldn't find group number until end of file. */
  word_swap(&elem0.head.element_length,2);
  word_swap(&elem0.group_length,2);

  assert_print(group_ptr = (char *) malloc(elem0.group_length), 
	       "Couldn't malloc ACR group data."); /* Too large but okay. */
  gp = group_ptr;
  /* Read an element head */
  while(fread_swap(&elem_head, sizeof(elem_head), 1, fp), 
	(elem_head.group_number == elem0.head.group_number)){
    word_swap(&elem_head.element_length,2);
    if (gp+elem_head.element_length-group_ptr >= elem0.group_length) {
      if (ReadACRLoud) perror("Error:  Group on file is same size or larger than structure!");
      break;
    }
    /* Read the associated element head's data into position in the data */
    fread_swap((void *)gp, elem_head.element_length, 1, fp);
    gp += elem_head.element_length;
  }
  if (groupnum_ret) *groupnum_ret = elem0.head.group_number;
  if (grouplen_ret) *grouplen_ret = gp-group_ptr;
  return (void *)group_ptr;
}

/*
 * Search through all header groups until the proper number is found,
 * then read all of its data elements into a C-style structure without
 * the associated length and group number fields.
 */
void *read_group(FILE *fp, int group_number, int group_size)
{
  struct acr_elem0_struct elem0;
  struct acr_elem_head_struct elem_head;

  char *group_ptr, *gp;

  rewind(fp); /* This way ordering of calls is irrelevant. */
  if (fread_swap(&elem0, sizeof(elem0), 1, fp) != 1)
    return 0; /* Couldn't find group number until end of file. */
  word_swap(&elem0.head.element_length,2);
  word_swap(&elem0.group_length,2);
  if (ReadACRVeryLoud) fprintf(stderr,"group 0x%x, ", elem0.head.group_number);

  /* Search forward through groups until group number is found in elem0. */
  while (elem0.head.group_number != group_number) {
    fseek(fp, elem0.group_length, SEEK_CUR);
    if (fread_swap(&elem0, sizeof(elem0), 1, fp) != 1)
      return 0; /* Couldn't find group number until end of file. */
    word_swap(&elem0.head.element_length,2);
    word_swap(&elem0.group_length,2);
    if (ReadACRVeryLoud) fprintf(stderr,"group 0x%x, ", 
				 elem0.head.group_number);
  }
  assert_print(group_ptr = (char *) malloc(group_size), 
	       "Couldn't malloc ACR group data.");
  gp = group_ptr;
  while(fread_swap(&elem_head, sizeof(elem_head), 1, fp), 
	(elem_head.group_number == group_number)){
    word_swap(&elem_head.element_length,2);
    if (gp+elem_head.element_length-group_ptr >= group_size) {
      if (ReadACRLoud) perror("Error:  Group on file is same size or larger than structure!");
      break;
    }
    fread_swap((void *)gp, elem_head.element_length, 1, fp);
    gp += elem_head.element_length;
  }
  if (ReadACRLoud) if (gp < group_ptr + group_size)
    fprintf(stderr,"Warning: group 0x%x shorter than expected.\n", group_number);
  
  return (void *)group_ptr;
}

void ACR_read_image(char *fname, struct image *im, unsigned int *NumRet,
		    void (*report)(void *private_data));
void report_header(void *private_data);


#ifdef TESTING
main(int argc, char **argv)
{
  struct image im;
  FILE *infile = fopen(argv[1], "rb");
  void *grp;
  int grpnum;
  size_t grpsize;

#ifdef NOTDEF
  rewind(infile);
  fprintf(stdout,"Groups: ");
  while ((grp=read_next_group(infile, &grpnum, &grpsize)))
    fprintf (stdout, "%x, ", grpnum);
  fprintf(stdout,".\n\n");
#endif /*NOTDEF*/

  ReadACRLoud=1;
  ReadACRVeryLoud=1;
  ACR_read_image(argv[1], &im, 0, 0);

  fprintf(stdout, "min = %f, max = %f.  ", im.min, im.max);
  fprintf(stdout,"Done!\n");

  exit(0);
}
#endif /*TESTING*/

void ACR_read_image(char *fname, struct image *im, unsigned int *NumRet,
		    void (*report)(void *private_data))
{
  unsigned short rows, cols;
  unsigned int i, N;
  float max, min;
  double scale, shift;

  FILE *acr_file = fopen(fname, "rb");

  acr_relationship_t *grp20   = read_group(acr_file, 0x20, sizeof(*grp20) +8192);
  acr_presentation_t *grp28   = read_group(acr_file, 0x28, sizeof(*grp28) +8192);
  short              *grp7FE0 = read_group(acr_file, 0x7FE0, grp28->Rows*grp28->Columns*sizeof(short) +8192);

  assert_print(grp28, "Group 28 (Presentation) error reading.");
  assert_print(grp7FE0, "Group 7FE0 (Image Data) error reading.");
  
  if (ReadACRLoud) fprintf(stderr,"ImageDimension %d, Rows %d, Columns %d.\n",
			   grp28->ImageDimension, grp28->Rows, grp28->Columns);

  scale = 1;
  shift = 0;
  im->images = 1;
  im->rows = rows = grp28->Rows;
  im->cols = cols = grp28->Columns;
  N = rows*(int)cols;
  im->image.f = (float *) malloc(N*sizeof(float));
  im->row_ptr.f = (float **) malloc(rows*sizeof(float *));
  for (i=0; i<rows; i++) im->row_ptr.f[i] = im->image.f + i*cols;
#ifndef NO_FLOAT_H
  max = - (min = FLT_MAX);
#else
  max = - (min = MAXFLOAT);
#endif
  for (i=0; i<N; i++) {
    float scaled = (((float)grp7FE0[i])-shift)/scale;
    if (scaled > max) max = scaled;
    if (scaled < min) min = scaled;
    im->image.f[i] = scaled;
  }
  im->min = min; im->max = max;
  if (NumRet) *NumRet = grp20 ? /*Need to parse text fields!|||*/0 : 0 ;

  if (report) (*report)(grp28);

  fclose(acr_file);
  return;
}
