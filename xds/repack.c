static char *rcs_repack_c = "$Header: /export/homes/tldavis/src/RCS/repack.c,v 1.32 1994/12/08 19:37:33 tldavis Exp tldavis $";

/* repack.c: Repackage an image, truncating and catenating images in space and
 * time.  Written by Timothy Davis to support xds, the X Display Stack program,
 * and ss, the image stack statistics engine.
 * $Source: /export/homes/tldavis/src/RCS/repack.c,v $
 * $Log: repack.c,v $
 * Revision 1.32  1994/12/08  19:37:33  tldavis
 * Nothing...
 *
 * Revision 1.31  1994/12/08  19:30:38  tldavis
 * After adding Greg's additions (AGS)
 *
 * Revision 1.30  1994/09/19  03:32:04  tldavis
 * Made more quiet with the -q option.
 *
 * Revision 1.30  1994/09/19  03:32:04  tldavis
 * Made more quiet with the -q option.
 *
 * Revision 1.29  1994/07/04  03:26:42  tldavis
 * Fixed bug which prevented .im files from working.
 * (Added an 'else' in last read_image set).
 *
 * Revision 1.28  1994/06/28  14:49:03  tldavis
 * Incorporating Helsinki additions, chiefly reading ACR/NEMA images.
 *
 * Revision 1.27  1994/05/04  00:11:54  tldavis
 * Fixed repack reordering bug for ANMR .im files.  Now orders volume data
 * through time by default (slices together).
 *
 * Revision 1.27  1994/05/04  00:11:54  tldavis
 * Fixed repack reordering bug for ANMR .im files.  Now orders volume data
 * through time by default (slices together).
 *
 * Revision 1.26  1994/05/03  23:50:36  tldavis
 * Fixed for RS/6000 to accept NO_IEEEFP flag (since it's not there).
 *
 * Revision 1.24  1994/03/13  03:05:58  tldavis
 * Fixed 2 more bugs:
 * 1) 0-length records while not rescaling but cropping (e.g. -2)
 * 2) .im -> .bshort was checking rescale.
 *
 * Revision 1.23  1994/03/09  17:11:39  tldavis
 * Fixed numerous bugs, most notably
 * -p option was skipping the last file and not writing anything
 * It was checking min/max anyway (pass 1) due to left-out parens in loop.
 * min/max check now do absolute, don't necessarily span 0
 * A few small messages changed.
 *
 * Revision 1.22  1994/02/16  03:23:24  tldavis
 * Fixed -a averaging bug (need to set output_rows before using)
 *
 * Revision 1.22  1994/02/16  03:23:24  tldavis
 * Fixed -a averaging bug (need to set output_rows before using)
 *
 * Revision 1.21  1994/02/16  01:56:20  tldavis
 * Use CmdName everywhere instead of argv[0]
 * /
 *
 * Revision 1.20  1994/02/16  01:53:15  tldavis
 * Added new version warning.
 *
 * Revision 1.19  1994/02/16  00:33:00  tldavis
 * Remove long double reference for sgi.
 *
 * Revision 1.18  1994/02/15  23:54:10  tldavis
 * A few cosmetic bugfixes.
 *
 * Revision 1.18  1994/02/15  23:54:10  tldavis
 * A few cosmetic bugfixes.
 *
 * Revision 1.17  1994/02/15  22:49:57  tldavis
 * After major rewrites, before testing.
 * Changes:
 * 1) Skips first pass if not trying to rescale.
 * 2) Updated rescaling code... hopefully bugs fixed.
 * 3) Treats unknown data types as similar to bushort
 * 4) Might work for multiple image sizes, truncating to smallest size
 *
 * Revision 1.16  1994/02/15  18:48:38  tldavis
 * Attempt to bring up to speed on all platforms.
 *
 */
#ifdef __sgi
#  include <values.h>
#  include <nan.h>
#  define isnan(X) (IsNANorINF(X) && !IsINF(X))
#  define NO_LONG_DOUBLE
#endif /* __sgi */

#ifdef BSD
#  define NO_LONG_DOUBLE
#endif

#if (defined(SVR4) && !defined(NO_IEEEFP))
#  include <ieeefp.h> /* For int finite(double) Solaris */
#endif

#ifdef NO_FINITE
#  define finite(X) (!IsNANorINF((X)))
#endif

#ifdef NEED_GETOPT_H
#  include "getopt.h"
#endif

#ifndef __DATE__
#  define __DATE__ "$Date: 1994/12/08 19:37:33 $"
#endif

#define NeedFunctionProtoypes 1
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <limits.h>
#include "read_image.h"
#include "read_otl.h"
#include "complex.h" /* Uses macros, but not code from complex.o NR */
#include "repack_help.h"
#include "read_im.h"
#include "assert_print.h"
#include <float.h>
#define MAXIMAGEFILES (8192)

/*FORWARD*/ void read_overlay(char *fname,int rows,int cols, 
			      unsigned char **overlays,int *factor);

int Quiet=0;

void print_usage(char *name) { if (!Quiet) fprintf(stderr,Usage, name); }

typedef enum new_filetype_tag {NotNew_t=0, ANMR_t, APD2_t, ACR_t } new_filetype_t;

struct file_info_struct {
  char *fname; int skip, num, end; float min, max, rep_min, rep_max;
  new_filetype_t NewFiletype;
  unsigned int ImageNum; 
} file_info[MAXIMAGEFILES] = {0, 0, 0, 0, 0, 0, 0, 0, 0};

enum {t_simple=0, t_magnitude=1, t_phase=2, t_real=3, t_imaginary=4} 
      DisplayType = t_simple;

char *Intro_Usage = 
"%s, Repackage image data $Revision: 1.32 $, compiled %s.\n\
Copyright 1994 Timothy Davis <tldavis@fMRI.MIT.edu>.\n\
If you have problems, try repack.old and e-mail me.  Thanks!\n";

int main(int argc, char **argv)
{
  struct image im; /* From read_image.h */
  float *ave_image=0, *ave_ptr=0;
  char *otl_fname = 0;
  char *CmdName=0;
  char *output_fname = 0, *output_hdrname = 0;
  FILE *outfile = stdout, *outhdr = 0;
  int otl_factor;
  unsigned char *otl_raster;
  int startx=10000, starty=10000, endx=0, endy=0;
  int x=0,y=0,input_rows=0,input_cols=0, output_rows=0, output_cols=0;
  int numImages = 0;
  int output_type = BLOCK_FLOAT, argp = EOF;
  int i, j, k, l, m;
  int numimagefiles=0;
  int RescaleData=0, DontRescaleData=0, RescaleIndividually=0, RescaleCheck=0;
  int FloatIn=0, FloatOut=0;
  int UseOverlay = 0;
  int DeOversample = 0;
  int SortFiles = 0;
  int DontSort = 0;
  int NumPartitions = 0, partition=-1;
  int BlockAverage = 0, BlockAverageCutoff=0, averaging=0;
  int Fillet=0;
  int NanFlag=0;
  /*const*/ double Inf = DBL_MAX / DBL_MIN; /* if no infinity() */
  double output_min=Inf, output_max=-output_min;
  double input_min=Inf, input_max=-input_min;
  double trunc_min=-Inf, trunc_max=-trunc_min;
  double input_rep_min=Inf, input_rep_max=-input_rep_min;
  double usermin = Inf, usermax = -usermin, newnanval = 0.0;

  if (!(CmdName=strrchr(argv[0],'/'))) CmdName=argv[0]; else CmdName++;
  
  /* Process options, possibly including user-specified image region */
  while (optind < argc) { /* WARNING: optind MOD HACK */
    struct file_info_struct *info = &file_info[numimagefiles];
    if ((argp=getopt(argc,argv,
		     "i:s:e:#:x:y:r:c:2v:o:hRIMPl:m:SNDqOp:a:C:f:F:"))==EOF)
      /* Process any non-option arguments as additional filenames. */
      { argp = 'i'; optarg = argv[optind++];  /*|||optind mod hack !!!*/ } 
    switch(argp) {
    case 'i': {
      char *type = strrchr(optarg,'.')+1;
      info->NewFiletype = NotNew_t;
      info->ImageNum = 0;
      info->fname = optarg;
      info->max = -(info->min=Inf);
      
      if (strcmp(type,"bfloat") == 0 || strcmp(type,"flt") == 0
	  || strcmp(type,"ome")==0 || strcmp(type,"image")==0) 
	{ info->rep_min = -(info->rep_max = Inf); FloatIn=1; }
      else if (strcmp(type,"anmr") == 0 || strcmp(type,"bushort")==0
	       || strcmp(type,"ged") == 0 || strcmp(type,"gedno")==0)
	{ info->rep_min = 0; info->rep_max = (1U<<16)-1; }
      else if (strcmp(type,"ima") == 0)
	{ info->rep_min = 0; info->rep_max = (1U<<15)-1; /* Really, smaller */
	  info->NewFiletype = ACR_t; /*||| SortFiles++; */ }
      else if (strcmp(type,"im") == 0)
	{ info->rep_min = 0; info->rep_max = (1U<<15)-1; /* Really, smaller */
	  info->NewFiletype = ANMR_t; SortFiles++; }
      else if (strcmp(type,"img") == 0)
	{ info->rep_min = 0; info->rep_max = (1U<<15)-1;
	  info->NewFiletype = APD2_t; }
      else if (strcmp(type,"bchar")==0)
	{ info->rep_max = (1<<7)-1; info->rep_min = -(1<<7)+1; }
      else if (strcmp(type,"buchar") == 0) 
	{ info->rep_max = (1L<<8)-1; info->rep_min = 0; }
      else if (strcmp(type,"bshort") == 0 || strcmp(type,"img")==0
	       || strcmp(type,"idf")==0 || strcmp(type,"gen")==0)
	{ info->rep_max = (1UL<<15)-1; info->rep_min = -(1L<<15)+1; }
      else if (strcmp(type,"bushort") == 0)
	{ info->rep_max = (1UL<<16)-1; info->rep_min = 0; }
      else if (strcmp(type,"bulong") == 0)
	{ info->rep_max = (1UL<<31)+((1UL<<31)-1); info->rep_min = 0; }
      else if (strcmp(type,"blong") == 0) {
	info->rep_max = (1UL<<30)+((1UL<<30)-1); 
	info->rep_min = (-(1L<<30)+1) - (1L<<30); 
      } else { /*Assume other formats have bushort-like characteristics */
        /* NEEDS TO BE FIXED!!! "sis",... */
	info->rep_min = 0; info->rep_max = 32767; FloatIn=0;
      }
      
      if (info->rep_min < input_rep_min) input_rep_min = info->rep_min;
      if (info->rep_max > input_rep_max) input_rep_max = info->rep_max;
      
      /* Fill in defaults for the next image */
      file_info[++numimagefiles].skip = info->skip;
      file_info[numimagefiles].end = info->end;
      file_info[numimagefiles].num = info->num; 
    } break; 
    case 'a': BlockAverage = atoi(optarg); 
      if (!BlockAverageCutoff) BlockAverageCutoff=BlockAverage;
      break;
    case 'C': BlockAverageCutoff = atoi(optarg); break;
    case 's': info->skip = atoi(optarg); break;
    case 'e': info->end =  atoi(optarg);  info->num = 0; break;
    case '#': info->end = 0; info->num = atoi(optarg); break;
    case 'x':    x = atoi(optarg); break;
    case 'y':    y = atoi(optarg); break;
    case 'r': output_rows = atoi(optarg); break;
    case 'c': output_cols = atoi(optarg); break;
    case '2': DeOversample = 1; break;
    case 'v': otl_fname=optarg; 
      if (strstr(otl_fname,".otl")) UseOverlay=0; else UseOverlay=1;
      break;
    case 'l': usermin=atof(optarg); break;
    case 'm': usermax=atof(optarg); break;
    case 'F': NanFlag=1; newnanval=atof(optarg); break; /*AGS*/
    case 'R': DisplayType = t_real;	break;
    case 'I': DisplayType = t_imaginary; break;
    case 'M': DisplayType = t_magnitude; break;
    case 'P': DisplayType = t_phase; break;
    case 'S': RescaleData=1; break;
    case 'N': DontRescaleData=1; break;
    case 'D': RescaleIndividually=1; RescaleData=1; break;
    case 'q': Quiet=1; break;
    case 'O': DontSort=1; break;
    case 'p': NumPartitions= atoi(optarg); break;
    case 'f': Fillet=atoi(optarg); break;
    case 'o': /* Output file name.  If a block data type, prepare .hdr */
      { char *type = strrchr(output_fname=optarg,'.')+1;
	if (strcmp(type,"bfloat") == 0) {
	  output_type=BLOCK_FLOAT;output_min= -(output_max=Inf); FloatOut=1; }
	else if (strcmp(type,"flt") == 0) {
	  output_type = FLT; output_min = -(output_max = Inf);  FloatOut=1; }
	else if (strcmp(type,"anmr") == 0) {
	  output_type = ANMR;  /* same as bushort */
	  output_max = (1UL<<16)-1; output_min = 0; }
	else if (strcmp(type,"bchar") == 0) {
	  output_type = BLOCK_CHAR;
	  output_max = (1<<7)-1; output_min = -(1<<7)+1; }
	else if (strcmp(type,"buchar") == 0) {
	  output_type = BLOCK_UCHAR;
	  output_max = (1<<8)-1; output_min = 0; }
	else if (strcmp(type,"bshort") == 0) {
	  output_type = BLOCK_SHORT; 
	  output_max = (1UL<<15)-1; output_min = -(1L<<15)+1; }
	else if (strcmp(type,"bushort") == 0) {
	  output_type = BLOCK_USHORT; 
	  output_max = (1UL<<16)-1; output_min = 0; }
	else if (strcmp(type,"blong") == 0) {
	  output_type = BLOCK_LONG; 
	  output_max = (1UL<<30)+((1UL<<30)-1); 
	  output_min = (-(1L<<30)+1) - (1L<<30); } 
	else if (strcmp(type,"bulong") == 0) {
	  output_type = BLOCK_ULONG;
	  output_max = (1UL<<31)+((1UL<<31)-1); output_min = 0; }
	else output_type = NONE;
	if (!(outfile=fopen(output_fname, "w")))
	  {fprintf(stderr,"Couldn't open output %s!\n",optarg);exit(-1);}
	if (type && (output_type==ANMR || output_type>=BLOCK_CHAR)) {
	  output_hdrname = strdup(output_fname);
	  strcpy(strrchr(output_hdrname,'.')+1,"hdr");
	  if (!(outhdr=fopen(output_hdrname, "w"))) {
	    fprintf(stderr,"Couldn't open output header file %s!\n",
		    output_hdrname);  exit(-1);
	  }
	}
      }
      break;
    case 'h':  case '?':  Quiet = 0; /* Drop through to print_usage loud */
    default:   print_usage(CmdName); exit(-1);
    }
  }
  
  if (!Quiet)
    fprintf(stderr,Intro_Usage, CmdName, __DATE__);/* Send short help */
  
  /* Figure out whether we know we aren't going to rescale */
  if (!DontRescaleData) {
    if (output_max < input_rep_max || output_min > input_rep_min ||RescaleData)
      RescaleCheck=1; /* May need to rescale, yet to be determined;
		      * Collect max/min statistics anyway.
		      */
  } else RescaleCheck=RescaleData=RescaleIndividually=0;
         /* Override: don't rescale if -D flag is set! */
  if (SortFiles>1 && !DontSort) /*Need first pass to get file order*/
    RescaleCheck=1;

  if (numimagefiles >= MAXIMAGEFILES)
    fprintf(stderr, "Too many image files! Max is %d\n",MAXIMAGEFILES);
  
  if (numimagefiles == 0) {
    fprintf(stderr, "No image file(s) specified!\n"); print_usage(CmdName);
    exit(-1);
  }
  
  /******** FIRST PASS: Get min/max if rescaling data. *******/
  /* Purpose: Fill in file_info[l].min, .max, for each input file. */
  /* May be necessary to read entire file if min, max, are not known and
   * rescaling may take place.
   */
  if (!RescaleCheck && !Quiet)
    fprintf(stderr,"Skipping first pass: not rescaling.\n");
  if (RescaleCheck) {
    if (!Quiet) 
      fprintf(stderr,"Checking min/max of input for possible rescale.\n");
    for (l=0; l < numimagefiles; l++) {
      int end_k = file_info[l].skip + 1;
      for (k = file_info[l].skip; k < end_k; k++) {
	if (NumPartitions && (partition= ((partition+1) % NumPartitions)) != 0)
	  continue; /* Skip all but the first of each */
	if (file_info[l].NewFiletype == ANMR_t) 
	  ANMR_read_image(file_info[l].fname, &im, &file_info[l].ImageNum, 0);
	else if (file_info[l].NewFiletype == APD2_t) 
	  APD2_read_image(file_info[l].fname, &im, k);
	else if (file_info[l].NewFiletype == ACR_t) 
	  ACR_read_image(file_info[l].fname, &im, &file_info[l].ImageNum, 0);
	else {
	  int ans = read_image(file_info[l].fname, T_FLOAT, &im, k, 1);
	  assert_print(im.version == READ_IMAGE_VERSION,
		       "read_image header incompatible with object code!\n");
	  if (ans < 0)
	    switch (ans) {
	    case NO_SUCH_FILE: 
	      fprintf(stderr, "read_image(): No such file %s\n",
		      file_info[l].fname); 
	      exit(-1);
	    case BAD_FILE:
	      fprintf(stderr,"read_image(): Bad file %s\n",file_info[l].fname);
	      exit(-1);
	    case BAD_SCALE:
	      fprintf(stderr,"read_image():Bad scale %s\n",file_info[l].fname);
	      exit(-1);
	    default: fprintf(stderr,"Unknown err in read_image(): %s\n",
			     file_info[l].fname);
	      exit(-1);
	    }
	}
	
	if (file_info[l].end && file_info[l].end < im.images) 
	  im.images = file_info[l].end;
	if(file_info[l].num==0 || file_info[l].num>im.images-file_info[l].skip)
	  file_info[l].num = im.images-file_info[l].skip;
	end_k = file_info[l].num + file_info[l].skip; /* SET END OF LOOP YUCK*/

	if (DisplayType != t_simple) im.cols /= 2;
	if (DisplayType != t_simple || !finite(im.min) || !finite(im.max)) {
	  register int i;  register double data;    
	  fcomplex *cimage = (fcomplex *)im.image.f;
	  int NM = im.cols*im.rows;
	  im.min = Inf; im.max = -Inf;
	  switch (DisplayType) { /* Complex  min/max */
	  case t_simple: 
	    for (i = 0; i < NM; i++) if (finite(data = im.image.f[i]))
	      {if (im.min>data) im.min = data; if (im.max<data) im.max = data;}
	    break;
	  case t_real: 
	    for (i = 0; i < NM; i++) if (finite(data = cimage[i].r))
	      {if (im.min>data) im.min = data; if (im.max<data) im.max = data;}
	    break;
	  case t_imaginary: 
	    for(i = 0; i < NM; i++) if (finite(data = cimage[i].i))
	      {if (im.min>data) im.min = data; if (im.max<data) im.max = data;}
	    break;
	  case t_magnitude:	
	    for(i = 0; i < NM; i++) if (finite(data = MCabs(cimage[i])))
	      {if (im.min>data) im.min = data; if (im.max<data) im.max = data;}
	    break;
	  case t_phase:
	    for (i = 0; i < NM; i++) if (finite(data = MCarg(cimage[i])))
	      {if (im.min>data) im.min = data; if (im.max<data) im.max = data;}
	    break;
	  }
	} /* else use im.min and im.max as given by read_image(). */
	if (file_info[l].min > im.min) file_info[l].min = im.min; 
	if (file_info[l].max < im.max) file_info[l].max = im.max;
	free(im.image.c); /* Only using the info, not the data */
	free(im.row_ptr.c);
      }
      if (input_min > file_info[l].min) input_min = file_info[l].min;
      if (input_max < file_info[l].max) input_max = file_info[l].max;
      if (input_rows == 0 || input_rows > im.rows) input_rows = im.rows;
      if (input_cols == 0 || input_cols > im.cols) input_cols = im.cols;
    } 
  } /*** END OF FIRST PASS file_info[] and total min,max determination ***/

  /* If not doing first pass, at least need default image dimensions. */
  if (!input_rows || !input_cols) { /* Need image dimensions: read one file. */
    if (file_info[0].NewFiletype == ANMR_t) ANMR_read_image(file_info[0].fname, &im,0,0);
    else if (file_info[0].NewFiletype == ACR_t) ACR_read_image(file_info[0].fname, &im,0,0);
    else if (file_info[0].NewFiletype == APD2_t)APD2_read_image(file_info[0].fname, &im, 0);
    else {
      int ans = read_image(file_info[0].fname, T_FLOAT, &im, 0, 1);
      assert_print(im.version == READ_IMAGE_VERSION,
		   "read_image header incompatible with object code!\n");
      if (ans < 0) switch (ans) {
      case NO_SUCH_FILE: 
	fprintf(stderr, "read_image(): No such file %s\n",file_info[0].fname); 
	exit(-1);
      case BAD_FILE:
	fprintf(stderr,"read_image(): Bad file %s\n",file_info[0].fname);
	exit(-1);
      case BAD_SCALE:
	fprintf(stderr,"read_image():Bad scale %s\n",file_info[0].fname);
	exit(-1);
      default: fprintf(stderr,"Errorin read_image(): %s\n",file_info[0].fname);
	exit(-1);
      }
    }
    if (DisplayType != t_simple) im.cols /= 2;
    input_rows = im.rows;
    input_cols = im.cols;
  } /* End of need to determine input_rows */
  
  /***** TRUNCATION IN SPACE: Use given outline/overlay to crop image *****/
  if (otl_fname) { 
    int i, j;
    
    if (UseOverlay) 
      read_overlay(otl_fname, input_rows, input_cols, &otl_raster, &otl_factor);
    else read_otl(otl_fname, "", input_rows, input_cols, &otl_raster, &otl_factor);
    otl_factor /= 2; /* Use points which are halfway into ROI. */
    
    /* 
     * Now we have the image dimensions and an otl for it. Find the smallest
     * box enclosing the otl.
     */
    for (j = 0; j < input_rows; j++)
      for (i = 0; i < input_cols; i++)
	if (otl_raster[i+j*input_cols] > (unsigned int)otl_factor) {
	  if (i < startx) startx = i;
	  if (j < starty) starty = j;
	  if (i+1 > endx) endx = i+1;
	  if (j+1 > endy) endy = j+1;
	}
    if (!output_cols) output_cols = endx-startx;
    if (!output_rows) output_rows = endy-starty;
    if (x) startx = x;
    if (y) starty = y;
    endx = startx + output_cols;
    endy = starty + output_rows;
  } else {
    startx = x;
    starty = y; 
    if (output_cols) endx = startx + output_cols; else endx = input_cols;
    if (output_rows) endy = starty + output_rows; else endy = input_rows;
    if (endx > input_cols) endx = input_cols;
    if (endy > input_rows) endy = input_rows;
  }
  if (DeOversample) {
    startx = input_cols/4;
    endx = startx*3;
  }
  output_cols = endx-startx;
  output_rows = endy-starty;
  if (!Quiet && (output_cols != input_cols || output_rows != input_rows))
    fprintf(stderr,
	    "Truncating input: %dx%d, output:%dx%d, orig upper left (%d,%d)\n",
	    input_cols, input_rows, output_cols, output_rows, startx, starty);
  /*** End of truncation in space ***/

  /***** REORDERING OF INPUT TO OUTPUT FILES *****/
  if (SortFiles>1 && !DontSort) {
    if (!Quiet) fprintf(stderr,
         "Sorting files by image number rather than command-line ordering.\n");
    for (l=0; l < numimagefiles; l++)
      for (m=l+1; m < numimagefiles; m++) /* Simple bubble sort ||| USE ANSI*/
	if (file_info[l].ImageNum > file_info[m].ImageNum) {
	  struct file_info_struct temp = file_info[l];
	  file_info[l] = file_info[m];
	  file_info[m] = temp;
	  /* This is slow becase it's copying the entire file_info_struct. */
	}
    if (!Quiet) {
      fprintf(stderr, "Resorting.  New input ordering: ");
      for (l=0; l < numimagefiles-1; l++)
	fprintf(stderr, "%s, ", file_info[l].fname);
      fprintf(stderr, "%s.\n", file_info[numimagefiles-1].fname);
    }
  }

  if (BlockAverage)
    ave_image = (float *) calloc(output_cols*output_rows, sizeof(float));

  /****** Planning for truncation and rescaling.  *******/
  if (finite(usermin)) input_min = trunc_min = usermin;/*Prepare to truncate*/
  if (finite(usermax)) input_max = trunc_max = usermax;/*Prepare to truncate*/

  /* If input data won't fit in the output, rescale it. */
  if (RescaleCheck && !DontRescaleData && 
      ((FloatIn && !FloatOut)|| output_min>input_min  || output_max<input_max))
    RescaleData=1;
  
  if (!Quiet) {
    if (!RescaleIndividually) {
      if (RescaleData) 
	fprintf(stderr, "Rescaling data from [%g..%g] to [%g..%g]\n",
		input_min, input_max, output_min, output_max);
      else if (RescaleCheck)
	fprintf(stderr,"Data [%g..%g] not rescaled to output range [%g..%g]\n",
		input_min, input_max, output_min, output_max);
      else 
	fprintf(stderr,"Data not rescaled to output range [%g..%g]\n",
		output_min, output_max);
    } else fprintf(stderr,"Rescaling each input file individually:\n");
  }
  /******* End of planning for truncation and rescaling *******/

  if (output_type == FLT) {/* WRITE A .flt HEADER COMPATIBLE WITH SEG */
    long Nlong= (long)output_cols, Mlong = (long)output_rows, zerolong = 0;
    fwrite(&Mlong, sizeof(long), 1, outfile); 
    for (i=1; i< 32; i++) fwrite(&zerolong, sizeof(long),1,outfile);
    fwrite(&Nlong, sizeof(long), 1, outfile); 
    for (i=33;i<128; i++) fwrite(&zerolong,sizeof(long),1,outfile);
  }
  
  /************* FINAL PASS:  READ, TRUNCATE, RESCALE, WRITE **************/
  /* Read all images of interest and print out the otl-containing portion */
  for (l=0; l < numimagefiles; l++) {
    double datamin, datamax, datascale;
    int end_k_guess = 99999; /* Used as a magic number below */
    int end_k = end_k_guess;
    
    if (RescaleIndividually) {
      datamin=input_min=file_info[l].min; 
      datamax=input_max=file_info[l].max;
    }
    
    /* Only scale if the shifted data won't fit or the input is floating */
    if (RescaleData) { datamin = input_min; datamax = input_max; }
    else             { datamin = output_min; datamax = output_max; }
    
    if (!finite(datamin) || !finite(output_min))
      datamin = output_min = 0;  /* Disable shifts to/from infinity */
    if (RescaleData && (FloatIn || datamax-datamin > output_max-output_min))
      datascale = (output_max-output_min)/(datamax-datamin);
    else datascale = 1.0;
    if (!finite(datascale)) datascale = 1.0; /* Disable infinite scaling */
    
    if (!Quiet && RescaleIndividually) 
      fprintf(stderr,"Rescaling %s data from [%g..%g] to [%g..%g], scl %g.\n",
	      file_info[l].fname,
	      input_min, input_max, 
	      (input_min-datamin)*datascale+output_min,
	      (input_max-datamax)*datascale+output_max, datascale);
    
    for (k = file_info[l].skip; k < end_k; k++) {
      int kk;
      if (Fillet) {
	int start = (k / Fillet) * Fillet;
	int brk = Fillet / 2 + (Fillet % 2);
	if (k%2) kk = start + brk + (k-start-1)/2;
	else     kk = start + (k-start)/2;
      } else kk = k;
      if (NumPartitions && (partition= ((partition+1) % NumPartitions)) != 0
	  && end_k != end_k_guess)
	continue; /* Skip all but the first of each */
      if (BlockAverage) averaging = (averaging+1) % BlockAverage;
      if (file_info[l].NewFiletype == ANMR_t) ANMR_read_image(file_info[l].fname, &im, 0,0);
      else if (file_info[l].NewFiletype == ACR_t) ACR_read_image(file_info[l].fname, &im, 0,0);
      else if (file_info[l].NewFiletype == APD2_t) APD2_read_image(file_info[l].fname, &im, kk);
      else if (read_image(file_info[l].fname, T_FLOAT, &im, kk, 1) < 0)
	{ fprintf(stderr,"Error in read_image()\n");exit(-1);}
      if (file_info[l].end && file_info[l].end < im.images) 
	im.images = file_info[l].end;
      if (file_info[l].num==0 || file_info[l].num>im.images-file_info[l].skip)
	file_info[l].num = im.images-file_info[l].skip;
      end_k = file_info[l].num + file_info[l].skip; /* RESET END OF LOOP YUCK*/

      ave_ptr = ave_image;
      for (j = starty; j < endy; j++) {
	float *image_row = im.row_ptr.f[j];
	fcomplex *cimage_row = (fcomplex *)im.row_ptr.f[j];
	for (i = startx; i < endx; i++) {
	  int n;
	  double data;
	  
	  switch (DisplayType) {
	  case t_simple:    data = image_row[i]; break;
	  case t_real:      data = cimage_row[i].r; break;
	  case t_imaginary: data = cimage_row[i].i; break;
	  case t_magnitude: data = MCabs(cimage_row[i]); break;
	  case t_phase:     data = MCarg(cimage_row[i]); break;
	  default: data=0; break;
	  }
	  if (!finite(data) && NanFlag) data = newnanval;
	  if (data > trunc_max) data = trunc_max;
	  else if (data < trunc_min) data = trunc_min;

	  if (BlockAverage) {
	    if (averaging) { 
	      if (averaging < BlockAverageCutoff) *ave_ptr++ += data;
	      else ave_ptr++; 
	      continue; /* Skip to next image, skip write step. */
	    } else { 
	      if (BlockAverage == BlockAverageCutoff) *ave_ptr += data;
	      data = *ave_ptr/BlockAverageCutoff; 
	      *ave_ptr++ = 0; 
	    }
	  }

	  switch(output_type) {
	  case FLT: /* drop through and write data as if bfloat */
	  case BLOCK_FLOAT: 
	    { float float_data = (float)((data-datamin)*datascale+output_min);
	      n=fwrite((char *)(&float_data), sizeof(float), 1, outfile); 
	    } break;
	  case BLOCK_SHORT:
	    { signed short short_data = 
		(signed short)((data-datamin)*datascale+output_min);
	      n=fwrite((char *)(&short_data), sizeof(short), 1, outfile);
	    } break;
	  case ANMR:  /* same as bushort, drop through */
	  case BLOCK_USHORT:
	    { unsigned short short_data;
	      short_data=(unsigned short)((data-datamin)*datascale+output_min);
	      n=fwrite((char *)(&short_data), sizeof(short), 1, outfile);
	    } break;
	  case BLOCK_CHAR:
	    { signed char char_data = 
		(signed char)((data-datamin)*datascale+output_min);
	      n=fwrite((char *)(&char_data), sizeof(char), 1, outfile);
	    } break;
	  case BLOCK_UCHAR:
	    { unsigned char char_data
		=(unsigned char)((data-datamin)*datascale+output_min);
	      n=fwrite((char *)(&char_data), sizeof(char), 1, outfile);
	    } break;
	  case BLOCK_LONG:
	    { signed long long_data = 
		(signed long)((data-datamin)*datascale+output_min);
	      n=fwrite((char *)(&long_data), sizeof(long), 1, outfile);
	    } break;
	  case BLOCK_ULONG:
	    { unsigned long long_data
		=(unsigned long)((data-datamin)*datascale+output_min);
	      n=fwrite((char *)(&long_data), sizeof(long), 1, outfile);
	    } break;
	  default:
	    fprintf(stderr, "Can't write that data type!  Use one of the supported file extension names.\n");
	    print_usage(CmdName); exit(-1);
	  }
	  if (n != 1) {fprintf(stderr,"Bad data write!\n"); exit(-1); }
	}
      }
      free(im.image.c); /* These were allocated by read_image(); */
      free(im.row_ptr.c);
    } /* END for(k), loop through images in file */
    numImages += file_info[l].num;
  } /* END for(l), loop through input files */

  /* Write header file outputs */
  if (NumPartitions) numImages = ceil((double)numImages/NumPartitions);
  if (BlockAverage)  numImages = floor((double)numImages/BlockAverage);
  if (output_type == FLT && numImages > 1)
    fprintf(stderr,"Warning: Don't write multiple images to a .flt file!\n");
  if (outhdr)   /* Write out .hdr information */
    if (fprintf(outhdr,"%d %d %d 0\n",output_rows,output_cols,numImages)==EOF)
      { fprintf(stderr,"Bad write to hdr file!\n"); exit(-1); }

  exit(0); /* Success!!! */
} /* END main() */

void read_overlay(char *fname, int rows, int cols, 
		  unsigned char **overlays,int *factor)
{
  int x=0, y=0, n;
  int end = 0;
  FILE *ovlfile = fopen(fname, "r");

  (*overlays) = (unsigned char *) calloc(rows*cols, sizeof(char));
  *factor = 1;
  
  if (!ovlfile) {
    fprintf(stderr,"Couldn't find overlay file '%s'.\n",fname);
    exit(-1);
  }
  while (!end) {
    if ((n=fscanf(ovlfile,"%d %d", &x, &y)) == EOF) {
      if (!Quiet) fprintf(stderr,"Done loading ovlfile from file '%s'.\n",
			  fname);
      end = 1;
    } else {
      if (n != 2) {
	fprintf(stderr,"Problem reading ovlfile.\n");
	end = 1;
      }
      if (x >= cols || y >= rows) {
	fprintf(stderr,"WARNING! Bad ROI data input.\n");
      } else {
	(*overlays)[x+y*cols] = 1;
      }
    }
  }
  fclose(ovlfile);
}

#ifdef NO_READ_OTL
int read_otl(char *filename, char *region, int rows, int cols, 
	     unsigned char **raster, int *factor)
{ fprintf(stderr,"Sorry, no read_otl() on this machine.\n");  exit(-1); }
#endif

  
