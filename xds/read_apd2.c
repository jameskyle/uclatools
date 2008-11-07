/* Read the ANMR APD-2 image file type.  
*  Test routine returns human-readable header
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
#include <string.h>
#include <math.h>
#include "read_image.h"

#ifndef NO_FLOAT_H
#include <float.h>
#endif /*NO_FLOAT_H*/

#define MAX_ANMR_SIZE (512*512)
#define TRAILER_SIZE (16)
void APD2_read_image(char *fname, struct image *im, int image_num)
{
  unsigned int xsize=0, ysize=0, num_slices=0, images_per_slice=0;
  int image_rows, image_cols;
  unsigned int read_fov=0, phase_fov=0;
  unsigned int i, N;
  float max, min;
  unsigned short *file_map, *anmr_data;
  double scale, shift;
  int anmr_filedes;
  char *sym_fname = strdup(fname);
  char *sym_fname2 = strdup(fname);

  strcpy(strrchr(sym_fname,'_'),".irp"); /* APD2 filename skips last field */
  strcpy(strrchr(sym_fname2,'.')+1,"irp"); /* Same basename as parent instead*/
  if (symCreate("", sym_fname) < 0 && symCreate("", sym_fname2) < 0 ) {
    /* Support old .img file type: 256x256 shorts */
    xsize = 256;
    ysize = 256;
    num_slices = 1;
    images_per_slice = 1;
    read_fov = phase_fov = 200;
  } else {
    symSearch("xsize", "%d", &xsize);
    symSearch("ysize", "%d", &ysize);
    symSearch("num_slices", "%d", &num_slices);
    symSearch("images_per_slice", "%d", &images_per_slice);
    symSearch("read_fov", "%d", &read_fov);
    symSearch("phase_fov", "%d", &phase_fov);
    symDestroy();
  }

  if (!xsize || !ysize || !num_slices || !images_per_slice 
      || !read_fov || !phase_fov) {
    fprintf(stderr,"Error: some necessary items missing from symbol table.\n");
    exit(-1);
  }

  if (read_fov * ysize == phase_fov * xsize) {
    /* Already have square pixels. */
  } else {
    if (read_fov/xsize > phase_fov/ysize) 
      /* Tall samples were widened in file */ 
      xsize = rint(ysize*(read_fov/(double)phase_fov));
    else /* Wide samples were stretched in file */
      ysize = rint(xsize*(phase_fov/(double)read_fov));
  }

  im->images = num_slices*images_per_slice;
  im->rows = ysize; 
  im->cols = xsize; 
  N = ysize*(int)xsize;
  im->image.f = (float *) malloc(N*sizeof(float));
  im->row_ptr.f = (float **) malloc(ysize*sizeof(float *));
  for (i=0; i<ysize; i++) im->row_ptr.f[i] = im->image.f + i*xsize;
#ifndef NO_FLOAT_H
  max = - (min = FLT_MAX);
#else
  max = - (min = MAXFLOAT);
#endif

  /* Load image data from file. */
  anmr_filedes = open(fname, O_RDONLY);
#ifndef NO_MMAP
  file_map = (unsigned short *) 
    mmap(/*addr=*/ 0, 
	 num_slices*images_per_slice*(N*sizeof(short) + TRAILER_SIZE), 
	 PROT_READ,MAP_PRIVATE/*|MAP_NORESERVE*/, anmr_filedes, /*offset=*/0);
  anmr_data = file_map + image_num*(N + TRAILER_SIZE/sizeof(short));
#else /* can't use mmap(), so use fread instead. */
  {
    FILE *infile = fdopen(anmr_filedes, "rb");
    anmr_data = (unsigned short *) malloc(N*sizeof(short));
    fseek(file_map, image_num(N*sizeof(short) + TRAILER_SIZE), 0);
    fread(anmr_data,sizeof(short),N,infile);
  }
#endif

  for (i=0; i<N; i++) {
    float scaled = anmr_data[i]; /* No scale or shift any more! */
    if (scaled > max) max = scaled;
    if (scaled < min) min = scaled;
    im->image.f[i] = scaled;
  }
  im->min = min; im->max = max;

#ifndef NO_MMAP
  munmap(file_map, num_slices*images_per_slice*(N*sizeof(short) + TRAILER_SIZE));
#endif
  free(anmr_data);
  close(anmr_filedes);
}
