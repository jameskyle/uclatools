/* ss.c: Stack Statistics for a stack of images acquired before and during
 * a bolus injection of contrast.  Calculates area, fwhm, mtt, peak, t0, 
 * and gamma parameters point-by-point for points in a rectangular section
 * of an image defined as the smallest box encompassing an roi.
 * The image stack may first be converted to deltaR2 format.
 * If conversion occurs, baseline average and standard deviation reflect
 * ORIGINAL input data. Otherwise, they reflect the deltaR2 data.
 */

#include <stdio.h>
#include <string.h>
#include <math.h>
#include "read_image.h"

double NaN = 0.0 / 0.0 ;
double Inf = 1.0 / 0.0 ;

/* Anything 3 sdevs out of average is not noise */
#define NOT_NOISE 3
#define ARGS "o:i:t:v:R:E:cs:kKB:b:e:l:VfampTzwZWDg"
extern char *optarg;
extern int optind;
void print_usage(char *name)
{
  fprintf(stderr,"Usage: %s [(options)]\n\n\
Options:\n\
         Input/output file specification:\n\
         [ -o outfile] to specify output file.  Will create a file with type\n\
                       drawn from file extension.  Currently, only\n\
                       *.bfloat is supported. *.hdr written also.\n\
         [ -i infile]  input file (w/ extension) to be read by read_image().\n\
\n\
         ROI specification: (default: entire image)\n\
         [ -t otlfile] to specify a .otl file (seg).\n\
         [ -v ovlfile] to specify a .ovl text file (xds).\n\
\n\
         Imaging parameters:\n\
         [ -R TR] time between images in the stack.\n\
         [ -E TE] TE for the imaging sequence.\n\
\n\
         Data processing parameters:\n\
         [ -c ]          to convert the image stack to deltaR2 format.\n\
         [ -s skip]      to skip images in the input stream.\n\
         [ -k ]          to smooth images with a 3x3 uniform kernel.\n\
         [ -K ]          to smooth images with a 3x3 Hanning kernel.\n\
         [ -B baseline ] number of images to use as baseline (pre-bolus)\n\
         [ -b begin]     First image to be considered in stats.\n\
         [ -e end ]      Last image to be considered (default: end of file).\n\
         [ -l length ]   Number of images to be processed.\n\
\n\
         Interaction parameters:\n\
         [ -V ] for verbose mode.\n\
\n\
Output flags (image maps are written in order given on the command line):\n\
[ -f ] Full width at half max\n\
[ -a ] Area\n\
[ -m ] Mean transit time (1st moment / 0th moment)\n\
[ -p ] Peak value\n\
[ -T ] Arrival time\n\
[ -z ] Input baseline mean\n\
[ -w ] Input baseline standard deviation\n\
[ -Z ] delta R2 baseline mean\n\
[ -W ] delta R2 baseline deviation\n\
[ -D ] entire Delta R2 stack\n\
[ -g ] Gamma function fit for all maps, A*(t-t0)^alpha * exp((t-t0)/beta)\n\
\n\
Note that the output is a stack of images corresponding to the output flags\n\
given, in the same order as the flags.\n",name);
}

void read_overlay(char *fname, int rows, int cols, char **overlays,int *factor)
{
  int x=0, y=0, n, end = 0;
  FILE *ovlfile = fopen(fname, "r");

  (*overlays) = (char *) calloc(rows*cols, sizeof(char));
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

main(int argc, char **argv)
{
  struct image im; /* From read_image.h */
  char *roi_fname=0, *image_fname=0, *output_fname=0, *output_hdrname=0;
  FILE *outfile=0, *outhdr=0;
  int otl_factor;
  char *otl_raster=0;
  char verboseMode=0, gammaMode=0, ovlMode=0, convertMode=0;
  double TR=1.0, TE=1.0;
  
  int startx=99999, starty=99999, endx=0, endy=0;
  int skip=0, baseline_images = 10, begin = 0, end = 99999, length = 0; 
  int ParameterTimepts=0, ParameterBegin=0, ParameterEnd=0;
  int image_size = 0;
  int argp = EOF;
  unsigned int Roipts, Timepts, roi_rows, roi_cols;
  float **vec_rows;
  float *dR2_ave=0, *dR2_sdev=0, *raw_ave=0, *raw_sdev=0, *null_image=0;
  float *fwhm_image=0, *area_image=0, *mtt_image=0, *peak_image=0, *t0_image=0;
  char  fwhm_flag=0, area_flag=0, mtt_flag=0, peak_flag=0, t0_flag=0;
  float **output_order[32];
  char *output_name[32];
  int outputs=0;

  int i, j, k;

  /* Process options, possibly including user-specified image region */
  while ((argp=getopt(argc,argv,ARGS)) != EOF)
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
    case 'R': TR = atof(optarg); break;
    case 'E': TE = atof(optarg); break;
    case 's': skip = atoi(optarg); break;
    case 'k': Kernel = Uniform; break;
    case 'K': Kernel = Hanning; break;
    case 'B': baseline_images = atoi(optarg); break;
    case 'b': begin = atoi(optarg); break;
    case 'e': end = atoi(optarg); break;
    case 'l': length = atoi(optarg); break;
    case 'V': verboseMode = 1; break;
    case 'c': convertMode = 1; break;

    case 'f': fwhm_flag = 1; 
      output_order[outputs] = &fwhm_image; 
      output_name[outputs++] = "Full width at half max (-f)"; 
      break;
    case 'a': area_flag = 1; 
      output_order[outputs] = &area_image; 
      output_name[outputs++] = "Area under dR2 peak    (-a)"; 
      break;
    case 'm': mtt_flag = 1;  
      output_order[outputs] = &mtt_image; 
      output_name[outputs++] = "Mean transit time      (-m)"; 
      break;
    case 'p': peak_flag = 1; 
      output_order[outputs] = &peak_image; 
      output_name[outputs++] = "Peak of dR2            (-p)";
	break;
    case 'T': t0_flag = 1;   
      output_order[outputs] = &  t0_image; 
      output_name[outputs++] = "Contrast arrival time  (-t)";
      break;
    case 'z': 
      output_order[outputs] = &raw_ave;  
      output_name[outputs++] = "Input baseline average (-z)";
      break;
    case 'w': 
      output_order[outputs] = &raw_sdev; 
      output_name[outputs++] = "Input baseline stdev   (-w)";
      break;
    case 'Z': 
      output_order[outputs] = &dR2_ave;  
      output_name[outputs++] = "dR2 baseline average   (-Z)";
      break;
    case 'W': 
      output_order[outputs] = &dR2_sdev; 
      output_name[outputs++] = "dR2 baseline stdev     (-W)";
      break;
    case 'D': 
      output_order[outputs] =  &null_image; /* NULL POINTER for dR2 stack */
      output_name[outputs++] = "dR2 image stack        (-D)";
      break;
    case 'g': fprintf(stderr,"Gamma function not supported yet; ask jlbox.\n");
      break;

    case 'h':
    case '?':
    default:
      print_usage(argv[0]);
      exit(-1);
    }
    
  if (!image_fname) {
    if (optind < argc)
      image_fname = argv[optind++];
    else {    
      fprintf(stderr,"Too few command-line arguments!\n");
      print_usage(argv[0]);
      exit(-1);
    }
  }
  if (optind < argc) {
    fprintf(stderr,"Too many command-line arguments!\n");
    print_usage(argv[0]);
    exit(-1);
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
	if (otl_raster[i+j*im.cols] > otl_factor) {
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
  if (baseline_images > im.images) baseline_images = im.images;
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
      double *vec = (double *) malloc(sizeof(double)
				      *(roi_rows>roi_cols?roi_rows:roi_cols));

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
    for (j = starty; j < endy; j++)
      for (i = startx; i < endx; i++)
	vec_rows[n++][k-skip] = im.row_ptr.f[j][i];
    free(im.image.c); /* These were allocated by read_image(); */
    free(im.row_ptr.c);
    if (verboseMode) fprintf(stderr, ".");
  }
  if (verboseMode) fprintf(stderr,"Done.\n");

  dR2_ave = (float *) calloc(Roipts,sizeof(float));
  dR2_sdev = (float *) calloc(Roipts,sizeof(float));
  if (convertMode) {
    raw_ave = (float *) calloc(Roipts,sizeof(float));
    raw_sdev = (float *) calloc(Roipts,sizeof(float));
  } else {
    raw_ave = dR2_ave;
    raw_sdev = dR2_sdev;
  }
  if (fwhm_flag||t0_flag) fwhm_image = (float *) malloc(Roipts*sizeof(float));
  if (area_flag||mtt_flag) 
                 area_image = (float *) malloc(Roipts*sizeof(float));
  if ( mtt_flag)  mtt_image = (float *) malloc(Roipts*sizeof(float));
  if (peak_flag||fwhm_flag||t0_flag) 
                 peak_image = (float *) malloc(Roipts*sizeof(float));
  if (  t0_flag)   t0_image = (float *) malloc(Roipts*sizeof(float));
  if (fwhm_flag&&!fwhm_image || area_flag&&!area_image || mtt_flag&&!mtt_image 
      || peak_flag&&!peak_image || t0_flag&&!t0_image || convertMode&&!raw_ave 
      || convertMode&&!raw_sdev || !dR2_ave || !dR2_sdev)
    { fprintf(stderr,"Couldn't malloc maps.\n"); exit(-1); }

  
  if (convertMode) { /* If necessary, convert the data to deltaR2 format. */
    float *pave = raw_ave, *psdev = raw_sdev, 
          **vec = vec_rows, **vec_end=vec_rows+Roipts;

    /* Calculate baseline average and standard deviation for input data */
    for (vec = vec_rows; vec < vec_end; vec++) {
      float *tp = *vec, *tp_end = tp + baseline_images, val;
      double tmp;
      while (tp < tp_end) {
	*pave  += (val = *tp++);
	*psdev += val*val;
      }
      if (baseline_images == 0) {
	*pave++ = 1.0;
	*psdev++ = 0.0;
      } else {
	tmp = (*pave++ /= baseline_images); 
	tmp = *psdev/baseline_images - tmp*tmp;
	if (tmp < 0) *psdev++ = NaN;
	else *psdev++ = sqrt((double)tmp);
      }
    }

    if (verboseMode) fprintf(stderr,"Converting to deltaR2 format...");
    for (vec = vec_rows, pave = raw_ave; vec < vec_end; vec++, pave++) {
      float *tp = *vec, *tp_end = *vec+Timepts;
      double val;
      while (tp < tp_end)
	if ((val = *tp / *pave) <=0) *tp++ = -Inf;
	else *tp++ = -log(val)/TE;
    }
    if (verboseMode) fprintf(stderr,"Done.\n");
  }

  { /* Calculate baseline average and standard deviation for delta R2's.  */
    float *pave = dR2_ave, *psdev = dR2_sdev, 
          **vec = vec_rows, **vec_end=vec_rows+Roipts;
    int NInfs = 0;

    for (vec = vec_rows; vec < vec_end; vec++) {
      float *tp = *vec, *tp_end = tp + baseline_images;
      double val, tmp;
      while (tp < tp_end) {
	*pave  += (val = *tp++);
	*psdev += val*val;
      }
      if (baseline_images == 0) {
	*pave++ = 0.0;
	*psdev++ = 0.0;
      } else {
	tmp = (*pave++ /= baseline_images); 
	if (!finite(tmp)) NInfs++;
	tmp = *psdev/baseline_images - tmp*tmp;
	if (tmp < 0) *psdev++ = NaN;
	else *psdev++ = sqrt((double)tmp);
      }
    }
    if (NInfs) 
      fprintf(stderr,"Note: %d NaN's in baseline dR2 images.\n", NInfs);
  }

#ifdef NOTDEF /* Do below in fwhm */  
  if (t0_flag) {/* Calculate arrival time, as interp between first point beyond
		   the cutoff and the cutoff point. */
    /* Image pointers to inc: t0, vec, ave, sdev */
    float *t0 = t0_image, **vec=vec_rows, **vec_end=vec_rows+Roipts;
    float *ave = dR2_ave, *sdev = dR2_sdev;
    if (verboseMode) fprintf(stderr,"Calculating t0 map...");
    for (vec = vec_rows; vec < vec_end; vec++) {
      float *tp = *vec+ParameterBegin, *tp_endminus1 = *vec+ParameterEnd-1;
      float prev_val=0, cutoff = NOT_NOISE * *sdev++, ave_val = *ave++;
      float t = ParameterBegin;
      while (tp < tp_endminus1) /* Don't let us go off the end of the data */
	if (fabs(*tp -ave_val) > cutoff) 
	     break; /* Go to next ROI point */
	else { prev_val = *tp++; t++; }
      *t0++ = (t + fabs((*tp - (cutoff+ave_val))/(*tp - prev_val)) )*TR;
    }
    if (verboseMode) fprintf(stderr,"Done.\n");
  }
#endif /*NOTDEF*/

  if (peak_flag || fwhm_flag || t0_flag) {
    float *peak = peak_image, **vec=vec_rows, **vec_end=vec_rows+Roipts;
    if (verboseMode) fprintf(stderr,"Calculating peak map...");
    for (vec = vec_rows; vec < vec_end; vec++) {
      float *tp = *vec+ParameterBegin, *tp_end = *vec+ParameterEnd;
      float high=-1E31;
      while (tp < tp_end) {
	if (*tp > high) high = *tp++;
	else tp++;
      }
      *peak++ = high;
    }
    if (verboseMode) fprintf(stderr,"Done.\n");
  }
  
  if (fwhm_flag || t0_flag) { /* Calculate full width at half max. */
    float *fwhm = fwhm_image, **vec=vec_rows, **vec_end=vec_rows+Roipts;
    float *t0 = t0_image;
    float *ave = dR2_ave, *peak = peak_image;
    if (verboseMode) fprintf(stderr,"Calculating fwhm map...");
    for (vec = vec_rows; vec < vec_end; vec++) {
      float *tp = *vec+ParameterBegin, *tp_end = *vec+ParameterEnd;
      float t_hm1, t_hm2, prev_val=*tp, cutoff = (*peak++ + *ave++)/2;
      float t=-1;
      while (t++, tp < tp_end) { 
	if (*tp > cutoff) { /* Ascending crossing the cutoff point */
	  t_hm1 = (t-1 + (cutoff-prev_val)/(*tp - prev_val))*TR;
	  prev_val = *tp++;
	  break; /* Go to second half-max time */
	} else prev_val = *tp++;
      }
      while (t++, tp < tp_end) { /* Descending crossing the cutoff point */
	if (*tp < cutoff) { 
	  t_hm2 = (t-1 + (cutoff-prev_val)/(*tp -prev_val))*TR;
	  break;
	} else prev_val = *tp++;
      }
      *fwhm++ = t_hm2 - t_hm1;
      *t0++ = t_hm1;
    }
    if (verboseMode) fprintf(stderr,"Done.\n");
  }

  if (area_flag || mtt_flag) { /* Calculate area under the peak */
    float *area = area_image, **vec=vec_rows, **vec_end=vec_rows+Roipts;
    float *ave = dR2_ave;
    if (verboseMode) fprintf(stderr,"Calculating area map...");
    for (vec = vec_rows; vec < vec_end; vec++, ave++) {
      float *tp = *vec+ParameterBegin+1, *tp_end = *vec+ParameterEnd;
      double sum = 0;
      /* Simple zero-order sample-and-hold sum*/
      while (tp < tp_end)
	sum += (double)(*tp++ - *ave);
      *area++ = (sum * (double)TR);
    }
    if (verboseMode) fprintf(stderr,"Done.\n");
  }

  if (mtt_flag) {
    float *mtt = mtt_image, **vec=vec_rows, **vec_end=vec_rows+Roipts;
    float *ave = dR2_ave, *area = area_image;
    if (verboseMode) fprintf(stderr,"Calculating mtt map...");
    for (vec = vec_rows; vec < vec_end; vec++, ave++) {
      float *tp = *vec+ParameterBegin+1, *tp_end = *vec+ParameterEnd;
      double sum = 0; 
      int ctr = 1;
      while (tp < tp_end) {
	sum += (double)((*tp++ - *ave) * (2.0*ctr - 1.0));
	ctr++;
      }
      *mtt++ = (sum*TR*TR*0.5) / *area++;
    }
    if (verboseMode) fprintf(stderr,"Done.\n");
  }

  {
    int output_images = outputs;
    if (verboseMode) fprintf(stderr, "Computing ROI averages\n");
    fprintf(stdout,"\nROI averages (in order of data flags):\n\n");
    for (k=0; k < outputs; k++) {
      float *image = *output_order[k];
      double sum = 0, sumsq=0, val;
      int n = 0;
      if (!image) { /* Signifies delta R2 stack parameter */
	fprintf(stdout, "%s: A total of %d %dx%d images.\n", output_name[k],
		Timepts, roi_cols, roi_rows);
	output_images += Timepts-1; /* Real output images added */
      } else {
	double tmp;
	for (j=starty; j < endy; j++)
	  for (i=startx; i < endx; i++)
	    if ((!otl_raster || otl_raster[i+j*im.cols] > otl_factor)
		&& finite(val= *image++)) 
	      { sum += val; sumsq += val*val; n++; }
	sum /= n; sumsq /= n;
	if ((tmp=sumsq - sum*sum) < 0.0) tmp = NaN;
	else tmp = sqrt(tmp);
	fprintf(stdout, "%s: % #8.6lg +/- % #8.6lg over %d pixels.\n", 
		output_name[k], sum, tmp, n);
      }
    }
  
    if (outfile) {
      if (verboseMode)
	fprintf(stderr, "\nWriting %s as %d parameter images, each %dx%d.\n\n",
		output_fname, output_images, roi_cols, roi_rows);
      /* Write out a vector of time data for each ROI point. */
      for (k=0; k < outputs; k++)
	if (*output_order[k])
	  fwrite(*output_order[k], sizeof(float), Roipts, outfile); 
	else {
	  float **vec=vec_rows, **vec_end=vec_rows+Roipts;
	  if (verboseMode) fprintf(stderr,"Writing delta R2 stack...");
	  for (i = 0; i < Timepts; i++) /* Write in image order */
	    for (j=0; j < Roipts; j++)
	      fwrite(&vec_rows[j][i], sizeof(float), 1, outfile);
	  if (verboseMode) fprintf(stderr,"Done.\n");
	}
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
