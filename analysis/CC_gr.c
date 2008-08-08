/****************************************************************************/
/* $Id: CC_gr.c,v 2.16 2004/02/03 20:46:52 mscohen Exp mscohen $ */
/*$Log: CC_gr.c,v $
 *Revision 2.16  2004/02/03 20:46:52  mscohen
 *Fixed an error in the calculation df. Affects probability estimates slightly.
 *
 *Revision 2.15  2002/09/10 22:22:03  mscohen
 *changed call to OpenProcFile
 *
 *Revision 2.14  2002/09/09 21:57:50  nina
 *same as 2.13; problem with RCS
 *
 *Revision 2.13  2002/09/09 21:52:31  nina
 *changed fopen mode from "r"/"w" to "rb"/"wb" as applicable
 *
 *Revision 2.12  2002/09/09 21:29:04  nina
 *no modifications, RCS consolidation
 *
 * Revision 2.11  2002/04/17  04:00:35  mscohen
 * Changed overlay call to Overlay
 *
 * Revision 2.9  2001/08/24  21:38:39  zrinka
 * adjusted for change in ck_fwrite (from 5 to 4 arguments)
 *
 * Revision 2.9  2001/08/24  21:38:39  zrinka
 * adjusted for change in ck_fwrite (from 5 to 4 arguments)
 *
 * Revision 2.7  2000/11/07  02:44:27  mscohen
 * Repaired after Wil destroyed user directories.
 *
 * Revision 2.6  2000/05/16  20:11:42  mscohen
 * Fixed a range error in probability (for VERY unlikely events).
 *
 * Revision 2.5  2000/03/23  13:08:43  mscohen
 * New version of ImgLib and ImgLib.h
 *
 * Revision 2.4  1999/09/30  01:02:08  mscohen
 * added handling of new proc file tools and ck_fwrite
 *
 * Revision 2.3  1999/09/29  17:12:48  mscohen
 * Fixed buffer size problems for multImg
 * Added ck_fwrite
 * Made current with ImgLib for HanningSmooth and IMAGE struct
 *
 * Revision 2.2  1998/04/21  20:21:53  mscohen
 * Maintenance check-in
 *
 * Revision 1.17  1998/04/21  05:57:52  mscohen
 * More error checking. Post revision number in print_usage.
 *
 * Revision 1.17  1998/04/21  05:57:52  mscohen
 * More error checking. Post revision number in print_usage.
 *
 * Revision 1.15  1998/03/28  09:12:46  mscohen
 * Upgraded to new version of ImgLib.
 * Added simple error handling
 *
 * Revision 1.14  1997/10/08  20:57:12  mscohen
 * Archive of working version
 *
 * Revision 1.13  1997/04/11  18:49:01  mcohen
 * Changed default name to magicoverlay
 *
 * Revision 1.12  1997/04/11  05:04:29  mcohen
 * changed the contents of .magicoverlay
 *
 * Revision 1.11  1997/04/11  04:56:04  mcohen
 * This is the first release version (!)
 *
 * Revision 1.10  1997/04/11  02:14:27  mcohen
 * This is a working implementation with bug fixes for vector over writes
 *
 * Revision 1.9  1997/04/08  18:47:51  mcohen
 * Maintenance save
 *
 * Revision 1.8  1997/04/08  00:05:48  mcohen
 * Fixed numerous bugs related to revision levels
 *
 * Revision 1.7  1997/04/07  23:48:26  mcohen
 *
*
* Updated version of correlation calculations that is more memory efficient.
*
* The basic idea is to make an image of the intermediate calculations so that
* successive images can be simply added. After each addition, the correlation
* map is to be updated accordingly.
*
* Intended as a prototype for later statistical tools
*
*  Modification history:
*  1.0  9/17/96 -- automask, UC_Readimage , ImgLib routines, color map
*       9/18/96 -- incorporate slice loop. Streamline the code
*  1.1  3/12/97 -- Make covl file optional. Change default file names
*       3/16/97 -- Change most calloc's to malloc's. Free several vectors.
*                  Eliminate need for .bfloat extension (defaults...)
*  1.2  3/24/97 -- First pass at real time. This version still bounded by parafile size.
*                  Radically modified the subroutine structures.
*       3/26/97 -- Arbitrary paradigm files. Conventional processing broken
*                  Need to remove some hard coded defaults (file names)
*                  Remove unused variables in main()
*       4/2/97 --  Fix Multislice bug (change slicectr loop)
*  1.3  4/2/97 --  Multi Display and interpolate
*  1.4  4/3/97 --  Convert slice logic to handle each time point as a volume.
*                  Eliminate the slice loop and slice naming problems.
*                  User option to output volumes or multi-display.
*                  Change verboseMode outputs. Added saving of Multi-slice bshorts.
*  2.0             First tests of real time!!!!
*  2.1             Modify File saving logic - All covlRT files in one place
*
*******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "getopt.h"
#include "ImgLib.h"
#include "ImgLibError.h"

#undef mactest
#ifdef mactest
#include <console.h>
#endif
#define OSErr short

/***** APPLICATION-SPECIFIC CONSTANTS ******/
#define IGNORE 1234.5
#define N_OUTVOLUMES 4	/* number of statistical images in output file. */
#define TINY 1.0e-20
#define LS_MAX -50
#define STAT_MAX 1.0
#define STAT_CUTOFF 0.4
#define SLOPE_CUTOFF 0.1
#define SLOPE_MAX 15
#define true 1
#define false 0
#define LIMIT 60

#undef DEBUG

/* Prototypes */
void print_usage( char *name );

float FindGlobalRatio( float *volOneSum, unsigned short int *theVolume,
                         unsigned char *mask,long volSize, int npts );

void  MakeCCandSMap( float *Sy, float *Sy2, float *Sxy, float Sx2,
                     float Sx, float SSxx,
                     unsigned char *mask, float *CMap, float *SMap,
                     int npts, long volSize );

void  InterpCropMap( float *theMap, float *theDest, int bilinear, int xs, int ys, int Slices, int Ratio );

void  WriteStatOverlay( float *baseMult, float *StatVol, unsigned char *ColMap,
                        float *buf1, float *buf2, int xs, int ys, int Slices, 
                        float stat_cutoff, int interp, int bilinear, FILE *OVLFile );
//                        float stat_cutoff, int interp, int bilinear, char *name );

void  MakeProbabilityMap( float *CMap, float *PMap, float df, long volSize );

//-----------------------------------------------------

void print_usage( char *name )
{
  static char id[] = "$Revision: 2.16 $$Date: 2004/02/03 20:46:52 $";

  fprintf(stderr,"\nCalculate correlation between a reference vector (paradigm file)\n" );
  fprintf(stderr,"and the time history of each pixel\n" );
  fprintf(stderr,"\n%s\n", id );
  fprintf(stderr,"\nUsage: %s -i input_file -o output_file (basename) -p paradigm_file\n",name);
  fprintf(stderr,"   [ -G global normalization -R normalization_ratio_file\n");
  fprintf(stderr,"     -F Change to Light Speed (real-time mode enabled)\n");
  fprintf(stderr,"     -V verbose\n     -K Smoothing\n     -N noise_threshold\n");
  fprintf(stderr,"     -Q create quicklook (.covl and .sovl) files\n");
//  fprintf(stderr,"     -B specify a file to use as the base image\n");
  fprintf(stderr,"     -L t cutoff for slope and quicklook map\n");
  fprintf(stderr,"     -U upper limit of t displayed in quicklook maps\n");
  fprintf(stderr,"     -b bshort multi-display file (for time series)\n" );
  fprintf(stderr,"     -G global normalization\n     -R normalization_ratio_file\n");
  fprintf(stderr,"     -v output as volume format (may be useful for MedX) ]\n\n");
  fprintf(stderr,"The first three arguments are required.\n");
  fprintf(stderr,"Auto thresholding is ON by default, use -N 0 to override\n");
  fprintf(stderr,"Outputs a processing record: outputname.%s.proc\n",name);
  fprintf(stderr,"\n%s -h prints an extensive (out of date) help file\n", name);
}

//-----------------------------------------------------

float FindGlobalRatio( float *volOneSum, unsigned short int *theVolume,
                         unsigned char *mask, long volSize, int npts )
{
/* Returns the calculated ratio of the current image to the first image
   Normalization is done later. Finds the First Image Sum if needed */
   int i;
   float currVolSum, Ratio;

   for( i=1, currVolSum=0; i< volSize; i++ )  /* i is the image location */
   {
      currVolSum += theVolume[i] * (int)mask[i];
   }
   if( npts == 1 )
   {
      *volOneSum = currVolSum;
   }
   Ratio = *volOneSum / currVolSum;
   return( Ratio );
}  
   
//-----------------------------------------------------

void MakeCCandSMap( float *Sy, float *Sy2, float *Sxy, float Sx2,
                    float Sx, float SSxx,
                    unsigned char *mask, float *CMap, float *SMap,
                    int npts, long volSize )
/* make image maps of the correlation coefficient and the slope fit */
{
   int     den = ( npts > 0 ? npts : 1 );
   int     i;
   float   ssxy, ssyy;
   float   tmp;
   
   vclr(CMap, volSize, T_FLOAT);
   vclr(SMap, volSize, T_FLOAT);
//printf("  i  Sx2   Sx   SSxx   Sy[i]   Sy2[i]   Sxy[i]   den\n");
   for( i=0; i<volSize; i++ )
   {	/* step through pixels */
      if( (int)mask[i] )   
      {
/* by standard terminology, SSxy is the sum of the products of the deviation scores */
          ssxy = Sxy[i] - ( Sx * Sy[i] )/den;
          ssyy = Sy2[i] - ( Sy[i] * Sy[i] )/den;
          tmp = sqrt( SSxx * ssyy);
          if(tmp > TINY)
          {
             CMap[i] =(float)(ssxy/tmp);
          }
          else
             CMap[i] = TINY;
          if( SSxx > 0 )
          {
             SMap[i] = ssxy/SSxx;
          }
/*  A useful debug statement */
/*
if( i%80 == 0 )
printf( "%0.4d %0.4f %0.4f %0.4f %0.4f %0.4f %0.4f %0.4d\n",
        i, Sx2, Sx, SSxx, Sy[i], Sy2[i], Sxy[i], den);
*/
/*
   Normalize about mean for pixel, convert to percent.
   The slope is in pixel units/unit behavior, so divide by the average pixel
   intensity at this location during all used time points. Underestimates change
   magnitude for large signal changes...
*/
          SMap[i] = SMap[i] * 100 * den / Sy[i];
      }
   }
}

//-----------------------------------------------------

void  InterpCropMap( float *theMap, float *theDest, int bilinear, int xs, int ys, int Slices, int Ratio )
{
   float *buf1;
   OSErr error = noErr;

   buf1 = (float *)ck_malloc( xs*ys*sizeof( float ), "interpolation buffer" );

   xcrop( theMap, xs, ys*Slices, buf1, T_FLOAT );

   if( bilinear )
   {
      error = xinterp( buf1, xs/2, ys*Slices, Ratio, theDest, T_FLOAT );
      if( error ) {
         ILError( error, "xinterp" );
      }
   }
   else
   {
      error = pixrepX( buf1, xs/2, ys*Slices, Ratio, theDest, T_FLOAT );
      if( error ) {
         ILError( error, "pxrepX" );
      }
   }
   free( buf1 );
}

//-----------------------------------------------------

void  WriteStatOverlay( float *baseMult, float *StatVol, unsigned char *ColMap,
                        float *buf1, float *buf2, int xs, int ys, int Slices, 
                        float stat_cutoff, int interp, int bilinear, FILE *OVLFile )
{
/* Takes two images (BaseVol and StatVol) and creates and saves an overlay file called, "name"
*  the BaseVol is assumed to be prepared (cropped and interpolated and mdisplay) already		*/

   short   Rows, Cols;
   float *statMult;
   int   Ratio = 2;
   OSErr error = noErr;
   
   if( interp )
   {
   /* Always use pixel replication for the statistical maps */
      xcrop( StatVol, xs, ys*Slices, buf1, T_FLOAT );
      if( bilinear )
      {
         error = xinterp( buf1, xs/2, ys*Slices, Ratio, buf2, T_FLOAT );
         if( error ) {
            ILError( error, "xinterp in WriteStatOverlay" );
         }
         error = mdisplay( statMult, buf2, xs, ys, Slices, T_FLOAT );
         if( error ) {
            ILError( error, "mdisplay in WriteStatOverlay" );
         }
      }
      else
      {
         error = pixrepX( buf1, xs/2, ys*Slices, Ratio, buf2, T_FLOAT );
         if( error ) {
            ILError( error, "pixrepX in WriteStatOverlay" );
         }
         error = mdisplay( statMult, buf2, xs, ys, Slices, T_FLOAT );
         if( error ) {
            ILError( error, "mdisplay in WriteStatOverlay" );
         }
      }
   }
   else
   {
      error = mdisplay( statMult, StatVol, xs, ys, Slices, T_FLOAT );
      if( error ) {
         ILError( error, "mdisplay in WriteStatOverlay" );
      }
   }

   error = Overlay( baseMult, statMult, ColMap, xs*Cols*ys*Rows, STAT_MAX, stat_cutoff, kDual );
   free( statMult );
   fwrite( ColMap, sizeof(unsigned char), xs*Cols*ys*Rows, OVLFile );
}

//-----------------------------------------------------

void MakeProbabilityMap( float *CMap, float *PMap, float df, long volSize )
/* calculate probability map according to Numerical Recipes formula */
{
	int    i;
	float prob;
	double too_big;
	float  t;
	double ln_10 = log((double)(10) );

	too_big = exp(LS_MAX);

	for( i=0; i< volSize; i++ )
	{
		if( (1.0 - CMap[i] + TINY) * (1.0 + CMap[i] + TINY) == 0 ) {
			ILError( DIVIDE_BY_ZERO_ERR, "MakeProbabilityMap" );
		}

		t = CMap[i] * sqrt(df/((1.0 - CMap[i] + TINY) * (1.0 + CMap[i] + TINY)));
		if( df+t*t == 0 ){
			ILError( DIVIDE_BY_ZERO_ERR, "MakeProbabilityMap" );
		}

		prob = betai(0.5*df, 0.5, df/(df+t*t));
		if( prob > too_big ) {
			prob = too_big;
		}

		if( prob <= 10E-9 ) {
			prob = 10e-9;
		}
		PMap[i] = ( CMap[i]>0 ) ? (float)(-log(prob)/(float)ln_10) : (float)(log(prob)/(float)ln_10);
	}
}

//-----------------------------------------------------

main(int argc, char **argv)
{
static char id[] = "$Revision: 2.16 $$Date: 2004/02/03 20:46:52 $";
    char    versionStr[31];
/*  variables specific to this statistical test */
    float	Sx;	        /* paradigm sums */
    float	Sx2;		/* paradigm sum of squares */
    float	SSxx;		/* Sum of paradigm squared deviations */
    float	*Sy;		/* Image-sized sum of pixels values */
    float	*Sy2;		/* image-sized vector of current sum of squares and squared deviations */
    float	*Sxy;		/* Image-sized product of image data and paradigm */
    float	*CMap;		/* the output is a map of the correlation coefficient */
    float	*SMap;		/* the output is a map of the slope of the fit */
    float	*TSMap;		/* thresholded (at cc>stat_cutoff) map of the slope */
    float	*PMap;		/* a map of the -log (base 10) of the probabilities */
    
/* Real Time Stuff */
    char	ProcListfname[] = "APDProcList";
    char    procIDnumber[255];       /* the number of the xds display process id */
    int     limit;
    int     interp = false;      /* until I can handle this more smoothly */
    int     procDone = false;    
    int     RT = false;
    FILE    *RTFile;

/* General stat processing */
/* Flags */
    OSErr   error = noErr;
    int    	globalNormalization = false;	/* flag to indicate this option -- off by default */
    int     FirstPass = true;
    int     FirstPassComplete = false;
    int     makeMDbshort = false;
    int    	quicklook = false;
    int    	AutoMask = true;
    int    	makeRatio = false;
    int    	Kernel = false;
    int     verboseMode = false;
    int    	crop = true;	         /* Auto detect of cropping */
    int     volumeDisplay = false;   /* outputs volumes instead of multi image display */
    int     bilinear = false;
    int     newBase = false;   /* indicates that user has selected their own base image */

    time_t	now;
    struct	tm *date;
    FILE	*ParaFile, *imFile, *RatioFile=NULL, *ProcRecfp, *ProcListfp, *bshortfp, *fp;
    IMAGE	im;
    float   Xi;             /* current value of paradigm file */
    float	globalRatio;	/* Ratio of first image to current image  */
    float	theImgPt;       /* stores the corrected value */
    float   *theBase;       /* pointer reassigned as needed */
    float  	volOneSum = 0;  /* For Global Normalization */
    float	stat_cutoff=STAT_CUTOFF;	/* Nominal cutoff for displaying stats in output */
    float   stat_max = STAT_MAX;
    float   *statMult, *multImg, *baseImg, *buff1, *buff2;
    long    volSize;
    long    imSize;
    long	numNotNoise;    /* number of suprathreshold pixels */
    int     Slices;
    int     imageYS;        /* Y size of an image */
    short   Rows, Cols;
    int	    i, volCtr;
    int    	slicectr;       /* slice location */
    int     sl_offset;      /* offset into image file for current image */
    int    	npts;           /* the number of points used in stat processing */
    int     vol_y;          /* ysize of volumes */
    int    	noiseThreshold = 0;
    int     numIgnored = 0; /* number of points ignored in calcs */
    int    	argp=EOF;
    char	*output_fname, *paradigm_fname, *ratio_fname, *image_fname, *in_name, *baseIm_name;
    char    buffer[127];      /* Used for general string handling */
unsigned char       *mask;      /* vector of valid pixels */
unsigned short int 	*theVolume;	/* vector of current image data - y in the correlation */
unsigned char       *ColMap;

#ifdef mactest
long 	mem;
argc = ccommand( &argv );
MaxMem( &mem );
#endif

while ( (argp=getopt(argc,argv,"o:i:p:N:R:L:U:bIFGKVQvhs")) != EOF )
   switch( argp )   {
      case 'o': output_fname = optarg; break;
      case 'i': in_name = optarg;
                image_fname = in_name;
                error = UC_Readheader(in_name,&im);
                if( error != noErr ) {
                	ILError( IMG_FILE_ERROR, "opening header for input image" );
		}
                break;
      case 'p': paradigm_fname = optarg;
                break;
      case 'N': noiseThreshold = atoi( optarg );
                AutoMask = false;
                break;
      case 'R': globalNormalization = true;
                ratio_fname = optarg; 
                makeRatio = 1;
                break;
      case 'L': stat_cutoff = atof(optarg);
                break;
      case 'U': stat_max = atof( optarg );
                break;
      case 'B': baseIm_name = optarg;
                newBase = true;
                break;
      case 'b': makeMDbshort = true;
                break;
      case 'v': volumeDisplay = true;
                break;
      case 'I': interp = true; /* interp AND crop */
                break;
      case 'F': ProcListfp = waitfopen( ProcListfname, "r", LIMIT ); /* ProcListfname = optarg; */
                RT = true;        /* enabling the processing list converts to Real Time */
                break;
      case 'G': globalNormalization = true; break;
      case 'K': Kernel = true; break;
      case 's': bilinear = true; break;
      case 'V': verboseMode = true; break;
      case 'Q': quicklook = true; break;
      case 'h':
      case '?':
          print_usage( argv[0] );
          fprintf(stderr, "\n\n");
          sprintf( buffer,"cat/usr/users/dox/CC_help.h");
          system( buffer );
          exit(-1);
      default:
          print_usage( argv[0] );
          exit( -1 );
   }

/* Input range checks */
if (( argc < 3 ) && !RT )  {
   print_usage( argv[0] );
   exit(-1);
   }

if( !RT ) {
   if ( !image_fname || !output_fname || !paradigm_fname )
   {
      fprintf(stderr,"\nYou must enter names for the input, output and paradigm files\n");
      print_usage( argv[0] );
      exit( -1 );
   }

} else {
   if ( !output_fname || !paradigm_fname )
   {
      fprintf(stderr,"\nYou must enter names for the output and paradigm files in real time mode\n");
      print_usage( argv[0] );
      exit( -1 );
   }
}

if( (stat_cutoff >= stat_max) || (stat_cutoff<0) )
{
   printf(  "stat_cutoff must be between 0 and stat_max ( %0.4f ). Will be set to %0.4f\n",
            stat_max, STAT_CUTOFF );
   stat_cutoff = STAT_CUTOFF;
}

if( stat_max < stat_cutoff )
{
   printf(  "stat_max must be greater than stat_cutoff ( %0.4f ). Will be set to %0.4f\n",
            stat_cutoff, STAT_MAX );
   stat_max = STAT_MAX;
}

if( noiseThreshold )
   {
      printf(  "Auto threshold disabled, using Threshold of: %d\n",noiseThreshold );
      AutoMask = false;
   }

if( RT )
{
   image_fname = (char *)ck_malloc( 255, "image_fname" );
   WaitNextItemFromList( image_fname, ProcListfp, ProcListfname, LIMIT );

# ifdef DEBUG
   printf(  "\nFirst Image file name is: %s\n", image_fname );
# endif

  error = UC_Readheader((char *)image_fname,&im);
  if( error ) {
     ILError( error, "Opening header for real-time file" );
  }
}

imageYS = im.dim.y;       /* use imageYS from here on */

/* toggle between real time and normal modes */
if( !RT )
   {
      Slices = im.dim.n_slices;
      limit = im.dim.timePts;

      sl_offset = im.dim.timePts;      /* index into multi slice file */
      npts = get_para_file_size( paradigm_fname );

      if( im.dim.timePts != npts )
      {
         fprintf(stderr,
            "\n%s - WARNING: The paradigm has %d points, but there are %d images.\n\n",
            argv[0],npts, im.dim.timePts);
      }
   }
else     /* real time mode */
   {
      Slices = im.dim.timePts;
      im.dim.y = im.dim.y * Slices;  /* this is how UC_ReadImage treats data as volumes */
      limit = 50000;      /* in real time mode, the processing ends when image_fname is "Done" */
   }
   
imSize = im.dim.x * imageYS;
vol_y = imageYS * Slices;
volSize = im.dim.x * imageYS * Slices;

if( verboseMode )
{
	printf( "Doing verbose\n" );
   printf("im.dim.x:%d, imageYS:%d, vol_y:%d, im.dim.timePts:%d\n",im.dim.x, imageYS, vol_y, im.dim.timePts);
   printf("im.dim.n_slices:%d\tSlices:%d\tvolSize:%d\n", im.dim.n_slices, Slices, volSize );
//   printf("Need x crop: %s\tneed y crop:%s\tmacByteOrder:%s\n",
//   	YorN( (im.flags & kNeedXCrop) ), YorN( (im.flags & kNeedYCrop) ), YorN( (im.flags & kmacByteOrder) ) ); 
}

/*******  ALLOCATE ALL NEEDED BUFFERS **********/
getRowsCols( Slices, &Rows, &Cols );
ColMap   = (unsigned char *)ck_malloc( im.dim.x*imageYS*Rows*Cols * sizeof( unsigned char ), "ColMap");

# ifdef DEBUG
printf("Rows:%d\tCols:%d\tColMapSize:%d\n", Rows, Cols ,im.dim.x*imageYS*Rows*Cols );
# endif

theVolume= (unsigned short int *)ck_malloc( volSize*sizeof( unsigned short int ), "theVolume");
mask     = (unsigned char *)ck_calloc( volSize, sizeof( unsigned char ), "mask");
Sy       = (float *)ck_malloc( volSize * sizeof( float ), "Sy");
Sxy      = (float *)ck_malloc( volSize * sizeof( float ), "Sxy");
Sy2      = (float *)ck_malloc( volSize * sizeof( float ), "Sy2");
SMap     = (float *)ck_malloc( volSize * sizeof( float ), "SMap");
PMap     = (float *)ck_malloc( volSize * sizeof( float ), "PMap");
TSMap    = (float *)ck_malloc( volSize * sizeof( float ), "TSMap");
CMap     = (float *)ck_malloc( volSize * sizeof( float ), "CMap");
/* general purpose buffers */
buff1    = (float *)ck_malloc( volSize * sizeof( float ), "buff1");
buff2    = (float *)ck_malloc( volSize * sizeof( float ), "buff2");
multImg  = (float *)ck_malloc( imSize * Rows * Cols * sizeof( float ), "multImg");

/*
sprintf( buffer, "ps -eaf | grep xds > theXDSprocess" );
system( buffer );
fp = fopen( "theXDSprocess", "r" );
fscanf( fp, "%s%s", buffer, procIDnumber );
printf(  "The process number: %s\n", procIDnumber );
fclose( fp );
*/

ParaFile = waitfopen( paradigm_fname, "r", LIMIT );
if( !RT )
{
   imFile = errfopen( image_fname, "rb" );
}

/*  Prepare the output files */
if( makeMDbshort )
{  /* file is opened first to zero it, and reopened for append */
   sprintf( buffer, "%s.Multi.bshort", output_fname );
   bshortfp = errfopen( buffer, "wb" );
   fclose( bshortfp );
   bshortfp = errfopen( buffer, "a" );
}
if( makeRatio )
{
   sprintf(buffer,"%s", ratio_fname);
   RatioFile=errfopen(buffer,"w");
   fprintf(RatioFile,"Image\tCorrection Ratio\n");
}

/* clear the processing vectors */
vclr(mask,volSize, T_UCHAR );
   
/********** INSERT TEST-SPECIFIC BUFFERS HERE ********/
vclr(Sy, volSize, T_FLOAT);
vclr(Sxy, volSize, T_FLOAT);   vclr(Sy2, volSize, T_FLOAT);
vclr(PMap, volSize, T_FLOAT);  vclr(TSMap, volSize, T_FLOAT);

/***************** Loop on time series *****************/

numIgnored = 0;

volCtr = 0;
volOneSum = 0;
FirstPass = true;
FirstPassComplete = false;
npts = 0;
Sx = 0;
Sx2 = 0;
procDone = false;
if( RT )
{
   sprintf( buffer, "%s.covlRT.buchar", output_fname );
   RTFile = errfopen( buffer, "wb" );
}

if( verboseMode )
{
   if( Slices == 1 )
      printf(  "\nImage \tWeight" );
   else
      printf(  "Volume\tWeight" );
   if( globalNormalization )
      printf(  "\tNormalization Ratio" );
   printf(  "\n" );
}

im.data_type = T_USHORT;

while( !procDone )  /* Loop through time series */
{
   WaitNextItemFromList ( buffer, ParaFile, paradigm_fname, LIMIT );
   Xi = atof( buffer );

   if( Xi == IGNORE )       /* no more processing on ignored images */
   {
      numIgnored++;
      if( verboseMode )
         printf(  "%3.0d\t+++ IGNORED +++\n", volCtr+1 );
   }
   else
   {
      npts++;
      if( verboseMode )
         printf(  " %3.0d   \t%0.5f\t", volCtr+1, Xi );
/* Read the next volume */
	error = GetSelectedVolume( imFile, &im, theVolume, volCtr );
	if( error ) {
	  ILError( error, "GetSelectedVolume" );
	}
 
      if( makeMDbshort ) /* Creating multi display sets */
      {
         error = mdisplay( multImg, theVolume, im.dim.x, imageYS, Slices, T_USHORT );
         if( error ) {
            ILError( error, "mdisplay in output of multi-bshort file" );
         }
         fwrite( multImg, sizeof(unsigned short int), im.dim.x*Cols*imageYS*Rows, bshortfp );
      }
/* Special processing of first image */
      if( FirstPass )  
      {
         if( AutoMask )
            error = autoMask( theVolume, mask, volSize, &noiseThreshold, &numNotNoise,  T_USHORT );
            if( error ) {
               ILError( error, "Call to AutoMask" );
            }
         else
            numNotNoise = ThreshMask( theVolume, mask, volSize, &noiseThreshold, T_USHORT );
         FirstPass = false;    /* set this flag */
      }

/* Smooth the image */
      if( Kernel )
//       HanningSmooth9( theVolume, (unsigned short *)buff2, im.dim.x, vol_y, T_USHORT );
         HanningSmooth( kSmooth9, theVolume, (unsigned short *)buff2, im.dim.x, vol_y, T_USHORT );

/* Normalize if needed */
      if( globalNormalization )
      {
         globalRatio = FindGlobalRatio( &volOneSum, theVolume, mask, volSize, npts );
         if( RatioFile != NULL )
            fprintf(RatioFile, "%d\t%f\n", volCtr+1, globalRatio);
         if(verboseMode) printf( "   %0.4f\n", globalRatio );
      }
      else if(verboseMode) printf( "\n" );

/***** START PER IMAGE STATISTICAL PROCESSING ******/
/* Check out the paradigm:  X is the reference vector, y is the image */
      Sx   +=  Xi;
      Sx2  +=  Xi * Xi;
      SSxx = Sx2 - (Sx * Sx)/npts;
# ifdef DEBUG
   printf(  "Done with X values\n" );
# endif
      /* process each image pixel */
      for( i=0; i<volSize; i++ )
      {
         if( (int)mask[i] )
         {
            theImgPt = theVolume[i];
            if(globalNormalization)
               theImgPt = theImgPt * globalRatio;
            Sy[i]  += theImgPt;
            Sy2[i] += theImgPt * theImgPt;
            Sxy[i] += theImgPt * Xi;
         }   /* process each pixel */
      }
/******* END IMAGE STATISTICAL PROCESSING *********/
      if( ( FirstPassComplete ) && ( RT ) )  /* enable real time display */
      {
/* Continuously output revised color maps */
         MakeCCandSMap( Sy, Sy2, Sxy, Sx2, Sx, SSxx, mask, CMap, SMap, npts, volSize );
/* the real time images are not presently cropped and interp'd. */
         if( interp )
         {
            xcrop( Sy, im.dim.x, vol_y, buff1, T_FLOAT );
            error = xinterp( buff1, im.dim.x/2, vol_y, 2, buff2, T_FLOAT );
            if( error ) {
               ILError( error, "xinterp" );
            }
            error = mdisplay( multImg, buff2, im.dim.x, imageYS, Slices, T_FLOAT );
            if( error ) {
               ILError( error, "mdisplay of overlay map" );
            }
         }
         else
         {
            error = mdisplay( multImg, Sy, im.dim.x, imageYS, Slices, T_FLOAT );
            if( error ) {
               ILError( error, "mdisplay of overlay map" );
            }
         }

         WriteStatOverlay( multImg, CMap, ColMap, buff1, buff2,
                           im.dim.x, imageYS, Slices, stat_cutoff, interp, bilinear, RTFile );
                           
     }    /* continuous color maps */
     if( !FirstPassComplete && RT )
     {
        sprintf( buffer, "%s.covlRT.buchar", output_fname );
        fp = errfopen( "/pd/images/magicoverlay", "wb" );
        fprintf( fp, "%s", buffer );
        fclose( fp );
     }
  }    /* usePt */
     
/* read the image list. If the images are all processed, wait. Otherwise, read the next one. */
# ifdef DEBUG
   printf(  "Waiting for the next image name (current: %s)\n", image_fname );
# endif
  if( !FirstPass )
  {
     FirstPassComplete = true;
  }
  if( RT )
  {
     WaitNextItemFromList( image_fname, ProcListfp, ProcListfname, LIMIT );
  }
# ifdef DEBUG
   printf(  "Have the next image name (current: %s)\n", image_fname );
# endif
/* quit when the image list says done, or all volumes have been processed */
  fprintf(stderr, ".");
  if( ( !strcmp( image_fname, "Done" ) ) || ( ++volCtr >= limit ) )
  {
     procDone = true;
  }
}    /* while not procDone, which is the image time loop */

if(verboseMode) fprintf(stderr,"\nNon-noise points: %d\tNoise Threshold:%d\n",
                                         numNotNoise, noiseThreshold );
fclose( ParaFile );
if( !RT )
{
   fclose( imFile );
}

if( RT )
{
   fclose ( RTFile );
   sprintf( buffer, "%s.covlRT.hdr", output_fname );
   RTFile = errfopen( buffer, "w" );
   fprintf( RTFile, "%d %d %d %d", imageYS*Rows, im.dim.x*Cols, volCtr, macByteOrder() );
   fclose ( RTFile );
}

if( makeRatio )
{
   fclose( RatioFile );  
}

if( makeMDbshort )
{
   fclose( bshortfp );
   sprintf( buffer, "%s.Multi.hdr", output_fname );
   bshortfp = errfopen( buffer, "w" );
   fprintf( bshortfp, "%d %d %d %d", imageYS*Rows, im.dim.x*Cols, npts, macByteOrder() );
   fclose( bshortfp );
}

/******** BUILD THE STATISTICAL MAPS ********/
if(verboseMode) fprintf(stderr,"Making the correlation maps\n");

MakeCCandSMap( Sy, Sy2, Sxy, Sx2, Sx, SSxx, mask, CMap, SMap, npts, volSize );

MakeProbabilityMap( CMap, PMap, npts-2, volSize );

for( i=0; i< volSize; i++ )
{
  TSMap[i] = (( CMap[i] > stat_cutoff ) || (CMap[i]< -stat_cutoff)) ? SMap[i] : 0.0;
}


if( quicklook )
{     
if(verboseMode) fprintf(stderr,"Making the quicklook maps\n");
   if( interp )
   {
      xcrop( Sy, im.dim.x, vol_y, buff1, T_FLOAT );
      error = xinterp( buff1, im.dim.x/2, vol_y, 2, buff2, T_FLOAT );
      if( error ) {
         ILError( error, "xcrop" );
      }
      error = mdisplay( multImg, buff2, im.dim.x, imageYS, Slices, T_FLOAT );  
      if( error ) {
         ILError( error, "mdisplay of Quicklook" );
      }
   }
   else
   {
      error = mdisplay( multImg, Sy, im.dim.x, imageYS, Slices, T_FLOAT );  
      if( error ) {
         ILError( error, "mdisplay of Quicklook" );
      }
   }    
   sprintf( buffer, "%s.covl.buchar", output_fname );
   fp = errfopen (buffer, "wb" );
   WriteStatOverlay( multImg, CMap, ColMap, buff1, buff2,
                     im.dim.x, imageYS, Slices, stat_cutoff, interp, bilinear, fp );
   fclose( fp );
   printf(  "Wrote Correlation Map overlay file: %s.covl.buchar\n", output_fname );
   sprintf( buffer, "%s.covl.hdr", output_fname );
   fp = errfopen( buffer, "w" );
   fprintf( fp, "%d %d %d %d", imageYS*Rows, im.dim.x*Cols, 1, macByteOrder() );
   fclose( fp );

   
   sprintf( buffer,"%s.sovl.buchar", output_fname );
   fp = errfopen (buffer, "wb" );
   WriteStatOverlay( multImg, TSMap, ColMap, buff1, buff2,
                     im.dim.x, imageYS, Slices, stat_cutoff, interp, bilinear, fp );
   fclose( fp );
   printf(  "Wrote Slope Map overlay file: %s.buchar\n", output_fname );
   sprintf( buffer, "%s.sovl.hdr", output_fname );
   fp = errfopen( buffer, "w" );
   fprintf( fp, "%d %d %d %d", imageYS*Rows, im.dim.x*Cols, 1, macByteOrder() );
   fclose( fp );

}  /* quicklook */

/******** DONE BUILDING THE STATISTICAL MAPS ********/

/* Write out the final maps */

if( interp )
{
   InterpCropMap( CMap, CMap, bilinear, im.dim.x, imageYS, Slices, 2 );
   InterpCropMap( SMap, SMap, bilinear, im.dim.x, imageYS, Slices, 2 );
   InterpCropMap( TSMap, TSMap, bilinear, im.dim.x, imageYS, Slices, 2 );
   InterpCropMap( PMap, PMap, bilinear, im.dim.x, imageYS, Slices, 2 );
}

sprintf( buffer,"%s.bfloat",output_fname );
fp = errfopen (buffer, "wb" );

sprintf( buffer,"%s.hdr",output_fname );

if( volumeDisplay )
{
if(verboseMode) fprintf(stderr,"Writing as volumes\n");
   ck_fwrite( CMap, sizeof(float), volSize, fp );
   ck_fwrite( SMap, sizeof(float), volSize, fp );
   ck_fwrite( TSMap,sizeof(float), volSize, fp );
   ck_fwrite( PMap, sizeof(float), volSize, fp );
   fclose( fp );
   fp = errfopen( buffer, "w" );
   fprintf( fp, "%d %d %d %d",vol_y, im.dim.x, N_OUTVOLUMES, macByteOrder() );
   fclose( fp );
}
else    /* Defaults to multi image display */
{
   long Size;
   Size = im.dim.x*Cols*imageYS*Rows;

   error = mdisplay( multImg, CMap, im.dim.x, imageYS, Slices, T_FLOAT );
      if( error ) { ILError( error, "mdisplay in final output" ); }
   ck_fwrite( multImg, sizeof(float), Size, fp );

   error = mdisplay( multImg, SMap, im.dim.x, imageYS, Slices, T_FLOAT );
      if( error ) { ILError( error, "mdisplay in final output" ); }
   ck_fwrite( multImg, sizeof(float), Size, fp );

   error = mdisplay( multImg, TSMap, im.dim.x, imageYS, Slices, T_FLOAT );
      if( error ) { ILError( error, "mdisplay in final output" ); }
   ck_fwrite( multImg, sizeof(float), Size, fp );

   error = mdisplay( multImg, PMap, im.dim.x, imageYS, Slices, T_FLOAT );
      if( error ) { ILError( error, "mdisplay in final output" ); }
   ck_fwrite( multImg, sizeof(float), Size, fp );

   fclose( fp );
   fp = errfopen( buffer, "w" );
   fprintf( fp, "%d %d %d %d",imageYS*Rows, im.dim.x*Cols, N_OUTVOLUMES, macByteOrder() );   
   fclose( fp );
}

printf(  "\n%s -- Processing completed\n", argv[0] );
fflush( stdout );

/* Prepare the processing record file */  
date = localtime( &now );
error = OpenProcFile( argc, argv, output_fname, id, &ProcRecfp );
ILError( error, "Opening Proc file" );

fprintf( ProcRecfp,"Image 0: cc map;\tImage 1: slope fit;\n");
fprintf( ProcRecfp,"Image 2: cc . slope;\tImage 3: -log(10)(p)\n");
fprintf( ProcRecfp,"FIRST INPUT FILE:   %s\n", image_fname );
fprintf( ProcRecfp,"SLICES:             %d\n", Slices );
fprintf( ProcRecfp,"PARADIGM FILE:      %s\n", paradigm_fname );
fprintf( ProcRecfp,"NUM IMAGES:         %d\n", npts );
fprintf( ProcRecfp,"AUTO-THRESHOLD:     %s\n", YorN( AutoMask) );
fprintf( ProcRecfp,"SMOOTHING:          %s\n", YorN( Kernel ) );
fprintf( ProcRecfp,"NORMALIZATION:      %s\n", YorN( globalNormalization ) );
if( makeRatio )
{
   fprintf( ProcRecfp,"RATIO FILE:         %s\n", ratio_fname);
}
if( makeMDbshort )
{
   sprintf( buffer, "%s.Multi.bshort", output_fname );
   fprintf( ProcRecfp,"IMG CONVERSION TO:  %s\n", buffer );
}
fprintf( ProcRecfp,"CC CUTOFF:          %0.3f\n", stat_cutoff);
if( RT || quicklook )
{
   fprintf( ProcRecfp,"CC MAX (display):   %0.4f\n", STAT_MAX );
   fprintf( ProcRecfp,"SLOPE RANGE:        %0.3f - %0.3f\n", SLOPE_CUTOFF, SLOPE_MAX );
}
fprintf( ProcRecfp,"REAL TIME MODE:     %s\n", YorN( RT ));
if( RT ) fprintf( ProcRecfp,"PROCESS LIST:       %s\n", ProcListfname);
fprintf( ProcRecfp,"NON-NOISE PTS:      %d\n", numNotNoise );
fprintf( ProcRecfp,"NOISE THRESHOLD:    %d\n", noiseThreshold );
fclose( ProcRecfp );

sprintf( buffer,"%s.bfloat",output_fname );
printf(  "Wrote output file: %s\n", buffer );
printf(  "\nIn this version of %s the first image is a map of the Correlation Coefficient\n", argv[0] );
printf(  "The second is a map of the SLOPE fitting the images to the paradigm.\n" );
printf(  "The third maps of the slope of pixels above the nominal cc cutoff [%0.3f]\n", stat_cutoff );
printf(  "The fourth image is -log(base 10) of the probabilities\n" );
   
/* Tidy up before going home */
free( theVolume );    free( mask );         free( ColMap );		
free( Sy );           free( Sxy);           free( Sy2 );
free( SMap );         free( TSMap );        free( PMap );
free( CMap );         free( buff1 );        free( buff2 );
free( multImg );

#undef DEBUG
#undef OSErr
#undef TINY

}
/* </PRE> </CODE> */
