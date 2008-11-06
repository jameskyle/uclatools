/* Read the ANMR image file type.  Test routine returns human-readable header
*/

#ifdef titan
#define NO_MMAP
#define NO_FLOAT_H
#endif

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

#include "pd.h"
#define IMAGE_HEADER_OFFSET 12
#define IMAGE_DATA_OFFSET 80
#define MAX_ANMR_SIZE (512*512)

void ANMR_read_image(char *fname, struct image *im, unsigned int *NumRet,
		     void (*report)(struct displayHdr *anmr_header));
void report_header(struct displayHdr *h);

void report_header(struct displayHdr *h)
{
  fprintf(stderr, "ANMR Header info:\n\
style=%hd, geSave=%d, anmrSave=%d, rotation=%d,reflectionX=%d, reflectionY=%d,\n\
scale=%f, shift=%f,\n\
displayX=%hd, displayY=%hd, slice=%hd, image=%hd, interImage=%d,\n\
echo=%hd, phase=%hd, numInterImages=%d,\n\
expand1=%lx, expand2=%lx, expand3=%lx, expand4=%lx\n",
	  h->style, h->geSave, h->anmrSave, h->rotation, 
	  h->reflectionX, h->reflectionY,
	  h->scale, h->shift,
	  h->displayX, h->displayY, h->slice, h->image, h->interImage,
	  h->echo, h->phase, h->numInterImages, 
	  h->expand1, h->expand2, h->expand3, h->expand4);
}

#ifdef TESTING
main(int argc, char *argv[])
{ struct image im_struct;
  ANMR_read_image(argv[1], &im_struct, 0, report_header);  exit(0);
}
#endif /*TESTING*/

void ANMR_read_image(char *fname, struct image *im, unsigned int *NumRet,
		     void (*report)(struct displayHdr *anmr_header))
{
  unsigned short rows, cols;
  unsigned int i, N;
  float max, min;
  char *file_map;
  struct displayHdr *anmr_header;
  unsigned short *anmr_data;
  double scale, shift;

  int anmr_filedes = open(fname, O_RDONLY);

#ifndef NO_MMAP
  file_map = (char *) 
    mmap(/*addr=*/ 0, IMAGE_HEADER_OFFSET+sizeof(struct displayHdr)
	 +MAX_ANMR_SIZE*sizeof(short), 
	 PROT_READ,MAP_PRIVATE/*|MAP_NORESERVE*/, anmr_filedes, /*offset=*/0);
  anmr_header = (struct displayHdr *)  (file_map+IMAGE_HEADER_OFFSET);
  anmr_data = (unsigned short *)  (file_map+IMAGE_DATA_OFFSET);
#else /* can't use mmap(), so use fread instead. */
  {
    FILE *infile = fdopen(anmr_filedes, "rb");
    file_map = (char *) malloc(IMAGE_HEADER_OFFSET 
			       + sizeof(struct displayHdr));
    fread(file_map, 1, IMAGE_HEADER_OFFSET+sizeof(struct displayHdr), infile);
    anmr_header = (struct displayHdr *) (file_map+IMAGE_HEADER_OFFSET);
    anmr_data = (unsigned short *) 
             malloc(sizeof(short)*anmr_header->displayY*anmr_header->displayX);
    fread(anmr_data,sizeof(short),anmr_header->displayY*anmr_header->displayX,
	  infile);
  }
#endif
  scale = anmr_header->scale;
  shift = anmr_header->shift;
  im->images = 1;
  im->rows = rows = anmr_header->displayY; 
  im->cols = cols = anmr_header->displayX;
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
    float scaled = (((float)anmr_data[i])-shift)/scale;
    if (scaled > max) max = scaled;
    if (scaled < min) min = scaled;
    im->image.f[i] = scaled;
  }
  im->min = min; im->max = max;
  if (NumRet) *NumRet = ((unsigned int)anmr_header->image)*(1<<16) + anmr_header->slice;
  if (report) (*report)(anmr_header);
#ifndef NO_MMAP
  munmap((caddr_t) file_map, IMAGE_HEADER_OFFSET+sizeof(struct displayHdr)
	 +MAX_ANMR_SIZE*sizeof(short));
#else
    /* Free the malloc()'ed memory */
    free(file_map);
    free(anmr_data);
#endif
  close(anmr_filedes);
}
