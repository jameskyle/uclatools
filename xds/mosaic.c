#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "assert_print.h"
#include "read_image.h"


/*	
  mosaic num_cols infile outfile
  */

main(int argc, char **argv)
{
  int numImages, numCols, Nin, Min, Nout;
  float *outimage;
  struct image im;
  char *infile, *outfile;
  FILE *outfileptr;
  int i,j,k=0,l,m,col=0,imagerows=0;
  
  assert_print(argc==4,"usage: mosaic num_cols infile outfile.bfloat");
  numCols = atoi(argv[1]);
  infile = argv[2];
  outfile = argv[3];

  read_image(infile, T_FLOAT, &im, 0, 1);
  Nin = im.cols; Min = im.rows; 
  numImages = im.images;
  Nout = Nin*numCols;

  assert_print(outfileptr=fopen(outfile,"w"),
	       "Couldn't open output file for writing.");
  assert_print(outimage = (float *) malloc(Nout*Min*sizeof(float)),
	       "Couldn't malloc outimage.");
  
  while (k < im.images) {
    if (read_image(infile, T_FLOAT, &im, k++, 1) < 0) break;
    
    for (m=0; m < Min; m++)
      for (l = 0; l < Nin; l++)
	outimage[col*Nin + Nout*m + l] = im.row_ptr.f[m][l];
    if (k % numCols == 0) {
      imagerows++;
      fwrite(outimage, sizeof(float), Nout*Min, outfileptr);
      col=0;
    } else col++;
  }

  if (col) {
    for (m=0; m < Min; m++) /* Zero fill remainging columns */
      for (l=Nin*col; l<Nout; l++) 
	outimage[Nout*m + l] = 0.0;
    fwrite(outimage, sizeof(float), Nout*Min, outfileptr); /* Last images */
    imagerows++;
  }
  
  fclose(outfileptr);

  strcpy(strrchr(outfile,'.')+1,"hdr");
  assert_print(outfileptr = fopen(outfile,"w"),"Couldn't open .hdr file.");
  fprintf(outfileptr,"%d %d %d %d\n",imagerows*Min,Nout,1,0);
}
