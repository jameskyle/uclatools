/* grinder.c, a drastically-reduced subset of ss.c for simplifying time-series
 * analyses.  tldavis 12/4/92  MUST COMPILE USING ANSI C (e.g. acc for SPARC)
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "mathext.h"
#include "read_image.h"
#include "read_otl.h"
#include "getopt.h"

void print_usage(char *name)
{
  fprintf(stderr,"Usage: %s [(options)]\n",name);
}

void read_overlay(char *fname, int rows, int cols, 
		  unsigned char **overlays,int *factor)
{
  int x=0, y=0, n, end = 0;
  FILE *ovlfile = fopen(fname, "r");

  (*overlays) = (unsigned char *) calloc(rows*cols, sizeof(char));
  *factor = 1;
  
  if (!ovlfile) {fprintf(stderr,"Can't find file '%s'.\n",fname); exit(-1); }
  while (!end) {
    if ((n=fscanf(ovlfile,"%d %d", &x, &y)) == EOF) {
      fprintf(stderr,"Done loading ovlfile from file '%s'.\n", fname);
      end = 1;
    } else {
      if (n != 2) {fprintf(stderr,"Problem reading ovlfile.\n");end = 1; }
      if (x >= cols || y >= rows) {
	fprintf(stderr,"WARNING! Bad ROI data input.\n");
      } else (*overlays)[x+y*cols] = 1;
    }
  }
  fclose(ovlfile);
}

float *Kernel = 0, Uniform[] = {1, 1, 1}, Hanning[] = {1, 2, 1};
int ovlMode, verboseMode=0;

main(int argc, char **argv)
{
  struct image im; /* From read_image.h */
  char *roi_fname=0, *image_fname=0, *output_fname=0, *output_hdrname=0;
  FILE *outfile=0, *outhdr=0;
  int otl_factor;
  unsigned char *otl_raster=0;
  
  int startx=99999, starty=99999, endx=0, endy=0;
  int skip=0, baseline_images = 10, begin = 0, end = 99999, length = 0; 
  int ParameterTimepts=0, ParameterBegin=0, ParameterEnd=0;
  int image_size = 0;
  int argp = EOF;
  unsigned int Roipts, Timepts, roi_rows, roi_cols;
  float **vec_rows;

  int i, j, k;

  /* Process options, possibly including user-specified image region */
  while ((argp=getopt(argc,argv,"o:i:t:v:s:e:l:kKhV")) != EOF)
    switch(argp) {
    case 'o': /* Output file name.  If a block data type, prepare .hdr */
      {	char *type = strrchr(output_fname=optarg,'.')+1;
	if (strcmp(type,"bfloat") != 0)
	  {fprintf(stderr,"Can't opnen file type %s!\n",type); exit(-1);}
	if (!(outfile=fopen(output_fname, "w")))
	  {fprintf(stderr,"Couldn't open output file %s!\n",optarg);exit(-1);}
	output_hdrname = strdup(output_fname);
	strcpy(strrchr(output_hdrname,'.')+1,"hdr");
	if (!(outhdr=fopen(output_hdrname, "w"))) {
	  fprintf(stderr,"Couldn't open output header file %s!\n",
		  output_hdrname);  exit(-1);
	}
      }
      break;
    case 'i': image_fname = optarg; break;
    case 't': roi_fname = optarg; ovlMode = 0; break;
    case 'v': roi_fname = optarg; ovlMode = 1; break;
    case 's': skip = atoi(optarg); break;
    case 'e': end = atoi(optarg); break;
    case 'l': length = atoi(optarg); break;
    case 'k': Kernel = Uniform; break;
    case 'K': Kernel = Hanning; break;
    case 'V': verboseMode = 1; break;

    case 'h':
    case '?':
    default:
      print_usage(argv[0]);
      exit(-1);
    }
    
  if (!image_fname) {
    if (optind < argc) image_fname = argv[optind++];
    else {    
      fprintf(stderr,"Too few command-line arguments!\n");
      print_usage(argv[0]); exit(-1);
    }
  }
  if (optind < argc) {
    fprintf(stderr,"Too many command-line arguments!\n");
    print_usage(argv[0]); exit(-1);
  }

  /* Get rows, columns, and number of images from read_image() */
  { int ans;
    im.version = READ_IMAGE_VERSION;
    ans=read_image(image_fname, T_FLOAT, &im, 0, 1);
    if (ans < 0)
      if (ans == NO_SUCH_FILE) 
	{fprintf(stderr,"read_image:No such file %s\n",image_fname);exit(-1); }
      else if (ans == BAD_FILE) 
	{fprintf(stderr,"read_image(): Bad file.\n"); exit(-1); } 
      else if (ans == BAD_SCALE) 
	{fprintf(stderr,"read_image(): Bad scale.\n"); exit(-1);} 
      else {fprintf(stderr,"Unknown error from read_image()\n"); exit(-1);}
    free(im.image.c); /* These were allocated by read_image(); */
    free(im.row_ptr.c);
  }

  if (roi_fname) { /*Use only the portion of the image surrounding an overlay*/
    int i, j;

    if (verboseMode) fprintf(stderr, "Reading overlay ... ");
    if (ovlMode) 
      read_overlay(roi_fname, im.rows, im.cols, &otl_raster, &otl_factor);
    else read_otl(roi_fname, "", im.rows, im.cols, &otl_raster, &otl_factor);
    otl_factor /= 2; /* Use points which are halfway into ROI. */
    if (verboseMode) fprintf(stderr, "Done.\n");
    /* 
     * Now we have the image dimensions and an otl for it. Find the smallest
     * box enclosing the otl.
     */
    if (verboseMode) fprintf(stderr, "Determining sub-region ... ");
    for (j = 0; j < im.rows; j++)
      for (i = 0; i < im.cols; i++)
	if (otl_raster[i+j*im.cols] > (unsigned) otl_factor) {
	  if (i < startx) startx = i;
	  if (j < starty) starty = j;
	  if (i > endx) endx = i+1;
	  if (j > endy) endy = j+1;
	}
    if (verboseMode) fprintf(stderr, "Done.\n");
  } else {
    startx = 0;
    starty = 0;
    endx = im.cols;
    endy = im.rows;
  }

  if (begin < skip) begin = skip;
  if (end > im.images) end = im.images; else end++; /* END POINTS AFTER LAST */
  if (length != 0) end = begin + length;
  roi_rows = endy-starty;
  roi_cols = endx-startx;
  Roipts = roi_rows*roi_cols;
  Timepts          = im.images-skip;
  ParameterTimepts = end - begin;
  ParameterBegin   = begin - skip;
  ParameterEnd     = end   - skip;
  
  if (verboseMode) fprintf(stderr, "startx=%d, starty=%d, endx=%d, endy=%d.\n",
			   startx, starty, endx-1, endy-1);

  vec_rows = (float **) malloc(Roipts*sizeof(float *));
  for (k = 0; k < Roipts; k++) 
    vec_rows[k] = (float *) malloc(Timepts*sizeof(float));

  /* Read all images of interest vectorize the otl-containing portion */
  if (verboseMode) 
    if (Kernel) fprintf(stderr, 
		   "Smoothing and converting to time-sequence vectors:\n");
    else
      fprintf(stderr, 
	           "Converting to time-sequence vectors:\n");
  for (k = skip; k < end; k++) {
    int n = 0;
    if (read_image(image_fname, T_FLOAT, &im, k, 1) < 0)
      { fprintf(stderr,"Error in read_image()\n"); exit(-1);}
    if (Kernel) { /* CONVOLVE WITH 3x3 SEPARABLE KERNEL */
      int i, j, k; /* NOTE LOCAL k */
      int filter_length = 3, offset = (filter_length-1)/2,
          f_start = -offset, f_pastend = offset + 1;
      double norm=0.0;
      double *vec = 
	(double *)malloc(sizeof(double)*(roi_rows>roi_cols?roi_rows:roi_cols));
      for (i = 0; i < filter_length; i++) norm += Kernel[i];
      for (j=0; j < roi_rows; j++) {  /* Filter each row */
	for (i = 0; i < roi_cols; i++) {
	  vec[i] = 0;
	  for (k=f_start; k<f_pastend; k++)
	    if (i+k < 0)
	      vec[i] += Kernel[k+offset]*im.row_ptr.f[j][i+k+roi_cols];
	    else if (i+k >= roi_cols)
	      vec[i] += Kernel[k+offset]*im.row_ptr.f[j][i+k-roi_cols];
	    else
	      vec[i] += Kernel[k+offset]*im.row_ptr.f[j][i+k];
	}
	for (i=0; i < roi_cols; i++) im.row_ptr.f[j][i] = vec[i]/norm;
      }
 
      for (i=0; i < roi_cols; i++) {  /* Filter each column */
	for (j=0; j < roi_rows; j++) {
	  vec[j] = 0;
	  for (k=f_start; k<f_pastend; k++)
	    if (j+k < 0)
	      vec[j] += Kernel[k+offset]*im.row_ptr.f[j+k+roi_rows][i];
	    else if (j+k >= roi_rows)
	      vec[j] += Kernel[k+offset]*im.row_ptr.f[j+k-roi_rows][i];
	    else
	      vec[j] += Kernel[k+offset]*im.row_ptr.f[j+k][i];
	}
	for (j=0; j < roi_rows; j++) im.row_ptr.f[j][i] = vec[j]/norm;
      }
      free(vec);
    }

    /* Re-arrange into vectors */
    for (j = starty; j < endy; j++)
      for (i = startx; i < endx; i++)
	vec_rows[n++][k-skip] = im.row_ptr.f[j][i];
    free(im.image.c); /* These were allocated by read_image(); */
    free(im.row_ptr.c);
    if (verboseMode) fprintf(stderr, ".");
  }
  if (verboseMode) fprintf(stderr,"Done.\n");

  { /*** THIS IS WHERE PER-TIME PROCESSING TAKES PLACE ***/
    float **vec = vec_rows, **vec_end=vec_rows+Roipts;
    /*
     * vec_rows[Roipts][Timepts] is 2-d array of data arranged by time
     * vec[time] is an individual time-series for a given voxel
     * tp runs through all time points in vec, so *tp is a signal at a time.
     * Timepts is the length of vec 
     * Roipts  is the number of positions
     */
    if (verboseMode) fprintf(stderr,"Taking -log(data/data0)...");
    for (vec = vec_rows; vec < vec_end; vec++) {
      float *tp = *vec, *tp_end = *vec+Timepts;
      double val;
      while (tp < tp_end)
			if ((val = *tp / (*vec)[0]) <=0) 
				*tp++ = -infinity();
			else 
				*tp++ = -log(val);
    }
    if (verboseMode) fprintf(stderr,"Done.\n");
  }

  {
    double sum = 0, sumsq=0, val;
    int n = 0;
    double tmp;
    float *image; /* = .... */
    if (verboseMode) fprintf(stderr, "Computing ROI averages\n");
    for (j=starty; j < endy; j++)
      for (i=startx; i < endx; i++)
	if ((!otl_raster || otl_raster[i+j*im.cols] > (unsigned)otl_factor)
	    && finite(val= *image++)) 
	  { sum += val; sumsq += val*val; n++; }
    sum /= n; sumsq /= n;
    if ((tmp=sumsq - sum*sum) < 0.0) tmp = quiet_nan(0);
    else tmp = sqrt(tmp);
    fprintf(stdout, "% #8.6lg +/- % #8.6lg over %d pixels.\n", 
	    sum, tmp, n);
  
    if (outfile) {
      int output_images=1;
      fwrite(image, sizeof(float), Roipts, outfile); 
      if (outhdr) {
	if (fprintf(outhdr,"%d %d %d 0\n", roi_rows, roi_cols, output_images) 
	    == EOF)
	  { fprintf(stderr,"Bad write to hdr file!\n"); exit(-1); }
      }
      if (verboseMode) fprintf(stderr, "Done.\n\n");
    }
  }
  exit(0); 
}
