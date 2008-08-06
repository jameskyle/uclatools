/* <html><head><title>EQ source</title></head><body><h1>EQ.c</h1>
<i>Please note last saved date:</i>
 <b><font size=+1>$Date: 2003/05/13 05:29:39 $</font></b><p>
<center>
<table bgcolor="black" cellpadding="5" border="1">
    <tr><td><font color="white">
EQ creates an aggressively smoothed 3D (or 2D) data set, then normalizes the raw input images by this volume.
The data are then rescaled to have the same mean value as the input images. The efficacy of this tool stems
from the handling of the "background" signal:  the background is filled with the mean value before smoothing
to avoid edge artifacts.
    </font></td></tr></table><p>
    &copy;1999-today  <a href="mailto:mscohen@ucla.edu">Mark S. Cohen, Ph.D.</a><br>
    <a href="http://www.brainmapping.org">UCLA Brain Mapping Division</a><br>
</center>
<h2>How to make EQ</h2>
<ol>
    <li>Build <font face="courier">ImgLib.o analyzeUtil.o ImgLibError.o VLib.o MathLib.o</font> (from
    <a href=
"http://airto.loni.ucla.edu/BMCweb/SharedCode/SharedSoftware.html#Anchor-UCLA-35829">
www.brainmapping.org</a>)
    <ul><li>I.e.,: <font face="courier">cc -c ImgLib.c analyzeUtil.c ImgLibError.c Vlib.c MathLib.c</font></ul>
    <li>If you got this from the web, download the html source and change the file name to <font face="courier"><b>EQ.c
</b></font>
    <li>Compile <font face="courier"><b>EQ</b></font>, linking the objects above. You will need to have C++ extensions
and the Math Library.
    A typical command might be:<br><p>
    <center>
        <font face="courier">
        cc ImgLib.o analyzeUtil.o ImgLibError.o VLib.o MathLib.o EQ.c -o EQ -lm</font>
    </center>
</ol>
<center>
<table border="2" cellpadding="8" width="90%"><tr><td>
This software is made available <b>AS IS</b> and no warranty is made as to its accuracy
or performance.<p>
Unlimited academic use of this software is granted, except that the any
publications that derive from its use, or any new software that incorporates
its underlying sources or algorithms must specifically acknowledge the author.
Modification of the program to meet local needs is encouraged, but all such
changes are to be communicated to the author to support the continued growth
of these tools.
<p>
Commercial, for-profit, use of the sources or algorithms is expressly prohibited
without permission of the
<a href="mailto:mscohen@ucla.edu">author</a>.
<p>
Unlesss otherwise stated, I do not offer technical support beyond that found on the
<a href="http://airto.loni.ucla.edu/BMCweb/HowTo/eq.html">
Brain Mapping Center support pages</a>.
</td></tr></table></center>
<pre>
<!--
$Id: equalize.c,v 1.28 2003/05/13 05:29:39 zrinka Exp $
$Log: equalize.c,v $
Revision 1.28  2003/05/13 05:29:39  zrinka
fixed im.data_type; in the beginning this is set to T_FLOAT
but must be changed back to T_USHORT before writing header
file for output image. zrinka

Revision 1.27  2003/05/13 00:34:50  zrinka
modified call for creating ANALYZE header because it was creating
headers with wrong x and y voxel sizes. New call is to function CreateHeader
instead of a local writeHeader function. zrinka.

Revision 1.26  2002/11/19 20:50:03  zrinka
fixed some html

Revision 1.25  2002/11/04 23:26:40  zrinka
small changes to support new version of ImgLib
changed fopen mode from 'r"/"w" to "rb"/"wb" where appropriate

Revision 1.24  2002/05/01 03:18:07  mscohen
Primarily html changes

 * Revision 1.23  2002/02/09  10:18:54  mscohen
 * Propagate file min and max to analyze output header
 * Display (in verbose mode) input file max and min.
 *
 * Revision 1.22  2001/11/21 09:11:19  mscohen
 * Fixed nonsense display of fill value, and made (slightly) more chatty.
 *
 * Revision 1.21  2001/10/05  22:21:02  mscohen
 * Modified disclaimer notice
 *
 * Revision 1.20  2001/10/01  19:00:05  mscohen
 * added html tags for publishing on the web
 *
 * Revision 1.19  2001/08/24  20:53:47  zrinka
 * moved padVolume function to MathLib.c.
 * removed 'crop' flag and adjusted call
 * to GetSelectedVolume.
 * Still needs to be tested.
 *
 * Revision 1.18  2001/08/16  18:42:14  zrinka
 * added two functions: PadVolume (pad volume data with zeros; real values)
 * and UnPadVolumeComplex (remove padding of the data; complex values).
 * Also, adjusted the code to handle irregular images (slice
 * dimensions not a power of 2).
 *
 * Revision 1.17  2001/07/03  21:59:06  mscohen
 * Revised calls to ck_fwrite and ck_fclose to be consistent with current Vlib
 * Tested (crudely) with the eSlice <->gDataZS modification.
 *
 * Revision 1.16  2001/07/03  20:46:53  zrinka
 * in CalcDimensions: eSlice removed - GDataZS used instead
 *
 * Revision 1.15  2001/03/17  05:24:38  mscohen
 * Now processes images whose dimensions are not a power of two
 *
 * Revision 1.13  2000/04/19  18:24:06  mscohen
 * re thought handling of under range inputs.
 *
 * Revision 1.13  2000/04/19  18:24:06  mscohen
 * re thought handling of under range inputs.
 *
 * Revision 1.12  2000/04/14  00:02:04  mscohen
 * Fixed truncation errors for inputs with narrow data range.
 *
 * Revision 1.11  2000/03/23  13:12:45  mscohen
 * New version of ImgLib and ImgLib.hNew version of ImgLib and ImgLib.h
 *
 * Revision 1.10  2000/03/14  20:54:00  mscohen
 * added more help
 *
 * Revision 1.9  2000/03/08  06:22:17  mscohen
 * more typing errors
 *
 * Revision 1.8  2000/03/08  06:20:35  mscohen
 * minor typing errors
 *
 * Revision 1.7  2000/03/08  06:16:32  mscohen
 * Added output of bias field. Fixed possible bug in command line processing
 *
 * Revision 1.6  2000/01/05  00:10:30  mscohen
 * Post the output file type options with mini-help screen.
 *
 * Revision 1.5  1999/10/01  17:40:16  mscohen
 * More identification of revision number
 *
 * Revision 1.4  1999/10/01  16:33:38  mscohen
-->
*/
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include "ImgLib.h"
#include "ImgLibError.h"
#include "MathLib.h"
#include "getopt.h"

#undef mactest
#ifdef mactest
#include <console.h>
#endif

#define OSErr short
#define noErr 0

FILE	*MessageLog;

// Function definitions
void   InitGlobals( void );
OSErr  ProcessCommandLine( int argc, char *argv[] );
void   print_usage( char *name );
void   print_help( char *name );
OSErr  CalcDimensions( IMAGE *im, float *zscale );
OSErr  PadImageSize( IMAGE *im );
float  CalcAverage( float *gThresh, float *data, long pts, long *validPts );
OSErr  FirstFT( float *imData );
OSErr  kSmooth( float *imData, float *gaussKernel );
OSErr  ExtractImage( float *imData, float *RawData, short extract_img, IMAGE *im );
void   NormalizeAndPad( float *SmoothData, float *RawData, float averageInt );
OSErr  writeVolume( FILE *f, float *data, long gVolSize, int fileType );
OSErr  writeHeader( IMAGE *im, int fileType, char *baseName );

// Globals
	Boolean    gVerbose = false;              // Set up chatty screen activity
	Boolean    gVolumeMode = false;
	Boolean    gAutoMask = true;              // Program detects thresholds
	Boolean    gSaveBias = false;
	float      gScale;                        // rescale the input values, to minimize digitization error
	float      gxSmooth = -1;                 // width of the smoothing in x, in pixels
	float      gySmooth = -1;                 // width of the smoothing in y, in pixels
	float      gzSmooth = -1;                 // smoothing width along z (pixels) 
	float      gCorrFactor = 1.0;             // magnitude of correction applied
	float      gThresh;                       // noise threshold
	float      gThreshAndFill = -1;           // Fill all pixels below this with this value
	char       *gOutput_fname=NULL;
	char       *gImage_fname=NULL;
	short      gExtract_img = -1;             // show processing steps for this image
	int        gOutfileType = ANALYZE;
	long       gImSize, gVolSize, gDataVolSize;
	short      gXS, gYS, gZS;
	short      gDataXS, gDataYS, gDataZS;     // size of data as processed - powers of 2
	FILE       *gProcFile;

/******************************    ProcessCommandLine     ***************************
*	capture the user input, mostly globals
*************************************************************************************/
OSErr ProcessCommandLine( int argc, char *argv[] )
{
	int argp = EOF;
	short argsProcessed = 0;
	
// capture user options
	while ( (argp=getopt(argc,argv,"o:i:W:Z:N:T:f:x:F:Vbh?")) != EOF ) {
	   argsProcessed++;
	   switch( argp )   {
	      case 'o': gOutput_fname = optarg; break;
	      case 'i':
	          gImage_fname = optarg;
	          break;
	      case 'W': gxSmooth = gySmooth = atof( optarg ); break;
	      case 'Z': gzSmooth = atof( optarg ); break;
	      case 'N': gThresh = atof( optarg );
	                gAutoMask = 0;
	                break;
	      case 'T': gOutfileType = atoi( optarg ); break;
	      case 'f': gCorrFactor = atof( optarg ); break;
	      case 'V': gVerbose = true; break;
	      case 'x': gExtract_img = atoi( optarg ); break;
	      case 'F': gThreshAndFill = atof( optarg );
	                gThresh = gThreshAndFill;
	                gAutoMask = false;
	                break;
          case 'b': gSaveBias = true; break;
	      case 'h':
	      case '?':
	          print_help( argv[0] );
	          printf("\n\n");
	          exit(-1);
	      default:
	          print_usage( argv[0] );
	          exit( -1 );
	   }
	}

	if( gCorrFactor > 1.0 || gCorrFactor < 0.0 ) {
		printf( "Correction factor (f) must range between 0 and 1\n" );
		return -1;
	}

	if( !gImage_fname || !gOutput_fname ) {
		print_usage( argv[0] );
		return -1;
	}
	return noErr;
}

/******************************      print_usage         *****************************
*	terse help message
*************************************************************************************/
void print_usage( char *name )
{
  printf( "\nUsage: %s -i input_file -o output_file [ -W smoothing width\n", name );
  printf( "-Z smoothing width (Z) -N Noise Threshold\n" );
  printf( "-T output file type (default '5' = analyze, '1' = bshort, '2' = bfloat)\n" );
  printf( "-F Fill value - prior to smoothing, pad all pixels below this with this value\n" );
  printf( "-f correction factor (0 - 1.0) strength of correction to apply ]\n" );
  printf( "-b output a 'bias field' image\n" );
  printf( "Only the base name of the output file is needed (always outputs bfloat)\n");
  printf( "The first two arguments are required.\n");
  printf( "\n%s -h shows a longer help screen\n", name );
}

/******************************      print_help         ******************************
*	Long help message
*************************************************************************************/
void print_help( char *name )
{

	char	response[25];

	printf( 
"%s is designed as a simple and rapid tool for correcting slowly varying\n\
image intensity deviations in medical (esp. Magnetic Resonance) images.\n\n\
%s works by calculating a highly smoothed version of the input data,\n\
and then normalizing the input image by this smoothed data set. To prevent\n\
a variety of serious artifacts, including edge blooming and noise amplification,\n\
the program first calculates which locations are within the volume of interest.\n\
Before smoothing, all out of volume (i.e. noise) pixels, are first set equal to\n\
the average intensity within the data volume. After smoothing and normalization,\n\
these locations are given the arbitrary intensity of zero.\n\n\
Parameters:\n\
\tUser MUST enter input (-i) and output (-o) file names.\n\
\t-W in-plane smoothing (in pixels)\n\
\t\tdefaults to three-eighths of the image width.\n\
\t-Z through plane smoothing (in slices)\n\
\t\tdefaults to three-eighths the number of slices.\n\
\t-N noise threshold\n\
\t\tamplitude below which pixels will be classified as non-image\n\
\t\tBy default, this is determined by the program.\n\
\t-T output file type:\n\
\t\t1 = bshort\n\
\t\t2 = bfloat\n\
\t\t5 = analyze (default)\n\
\t-f correction factor (0 - 1.0)\n\
\t\tstrength of intensity correction. Defaults to 1.\n\
\t-b output a 'bias field' image\n\
\t-G input and output will be Signa genesis format. Output image\n\
\t   name will have 'c' added -> E1041S2I001.MR becomes Ec1041S2I001.MR\n\
\t   The image file name base must follow -G. ( -G E1041S2* )\n\
\t--------- THIS MUST BE THE LAST COMMAND LINE ARGUMENT!!!---------\n\
\t-V verbose mode\n\
\t-h print this note.\n", name, name );

printf( "More (y/n): " );
scanf( "%s", response );

if( response[0] == 'y' || response[0] == 'Y' ) {
	printf( "\nThis is part of a set of utilities to pre-process signa files for intensity correction,\n" );
	printf( "and to post-process the resulting files to convert them to a valid Signa format.\n" );
	printf( "==============\n" );
	printf( "The tool Genesis2Analyze takes a series of ExxxxSxxxIxxx.MR files and makes them into a single\n" );
	printf( "4-D analyze file. It also saves a record of the original names, and the complete header for each\n\n" );
	printf( "Typing Genesis2Analyze (my way of providing simple help) gives:\n\n" );
	printf( "Usage: Genesis2Analyze basename*.MR\n" );
	printf( "Renumber all files ending in '*.MR' with 3 digit numbers\n" );
	printf( "Create an analyze 4D file with these contents\n\n\n" );
	printf( "Note that you must keep a copy of the NameFile (which ends in .NAME_FILE)\n\n" );
	printf( "        and the headers file, which ends in: .GenesisHeaders, in order to\n\n" );
	printf( "        successfully reconstruct the genesis headers using Analyze2Genesis\n" );
	printf( "===============\n" );
	printf( "Analyze2Genesis does the reverse operation, extracting each slice in an analyze file, and giving\n" );
	printf( "it a name and header from the output derived from Genesis2Analyze.\n\n" );
	printf( "Typing Analyze2Genesis gives:\n\n" );

	printf( "Usage:\n" );
	printf( "Analyze2Genesis -i AnalyzeInputFile -n NameFile\n\n" );
	printf( "        Analyze2Genesis will convert a (4D) analyze file to a series of genesis\n" );
	printf( "        files having names corresponding to the contents of the NameFile\n\n\n" );

	printf( "Note that you must keep a copy of the NameFile (which ends in .NAME_FILE)\n\n" );
	printf( "         and the headers file, which ends in: .GenesisHeaders, in order to\n\n" );
	printf( "        successfully reconstruct the genesis headers using Analyze2Genesis\n" );
	printf( "        The user is responsible for ensuring that the name file has a separate\n" );
	printf( "        entry for each slice location in the analyze file\n\n" );
	printf( "================\n\n" );
	printf( "A complete session transcript follows for RF correction. Note that the warning messages for mv\n" );
	printf( "and differing byte order are expected.\n\n" );
	printf( " Genesis2Analyze E*.MR\n" );
	printf( "Renaming files\n" );
	printf( "mv: E2123S2I124.MR and E2123S2I124.MR are identical.\n\n" );
	printf( "... etc ...\n\n" );
	printf( "mv: E2123S2I100.MR and E2123S2I100.MR are identical.\n\n" );
	printf( "WARNING: Computer and data differ in byte order\n\n" );
	printf( "        Patience for a moment\n\n\n" );
	printf( "Created Analyze (4D) file: E2123S2.img\n" );
	printf( "        Save these files!: E2123S2.NAME_FILE and E2123S2.GenesisHdrs\n\n" );
	printf( "To create corresponding genesis images from an analyze file named XXX.img, use:\n" );
	printf( "        Analyze2Genesis -n E2123S2.NAME_FILE -i XXX.img\n" );
	printf( "All done\n\n" );
	printf( "--------------------\n\n" );
	printf( "> EQ -i E2123S2.img -o eqTst\nEQ\n" );
	printf( "        $Revision: 1.28 $$Date: 2003/05/13 05:29:39 $\n" );
	printf( "Working\n\n" );
	printf( "Done. Thank you for using EQ.\n" );
	printf( "mscohen@ucla.edu -- http://www.brainmapping.org\n" );
	printf( "\n--------------------\n\n" );
	printf( " Analyze2Genesis -n E2123S2.NAME_FILE -i eqTst.img\n" );
	printf( "Using E2123S2.GenesisHdrs for genesis headers\n\n" );
	printf( "......................................................................\n\n" );
	printf( "> ls Ec*\n" );
	printf( "Ec2123S2I001.MR   Ec2123S2I026.MR   Ec2123S2I051.MR   Ec2123S2I076.MR Ec2123S2I101.MR\n" );
	printf( " ... etc ...\n" );
	printf( "Ec2123S2I025.MR   Ec2123S2I050.MR   Ec2123S2I075.MR   Ec2123S2I100.MR\n\n" );
	printf( "Mark S. Cohen, Ph.D.\n" );
	printf( "UCLA Brain Mapping Division\n" );
	printf( "660 Charles Young Dr. S.\n" );
	printf( "Los Angeles, CA 90095\n\n" );
	printf( "office: 310/897-1690\n" );
	printf( "lab:    310/825-9142\n" );
	printf( "http://www.brainmapping.org/\n" );
}
}

/******************************    CalcDimensions      ******************************
*
*	Given the IMAGE struct, define the dimensions used for local processing.
*	Mostly used to shorten variable names.
*************************************************************************************/
OSErr CalcDimensions( IMAGE *im, float *zscale )
{
	
	gXS = im->dim.x;
	gYS = im->dim.y;
	gZS = im->dim.n_slices;

	gDataXS = 2;	// gDataXS is the size of the data volume, which may be larger than gXS
	while( gDataXS < gXS ) {	// the FFT needs a power of 2 foreach dimension
		gDataXS *= 2;
	}
	
	gDataYS = 2;	// gDataYS is the size of the data volume, which may be larger than gYS
	while( gDataYS < gYS ) {	// the FFT needs a power of 2 foreach dimension
		gDataYS *= 2;
	}
	
	if( gZS > 1 ) {
		gDataZS = 2;	// gDataZS is the size of the data volume, which may be larger than gZS
		gVolumeMode = true;
		while( gDataZS < gZS ) {	// the FFT needs a power of 2 foreach dimension
			gDataZS *= 2;
		}
		
		if( gVerbose ) {
			printf( "The data are being processed as %d slices\n", gDataZS );
		}
	} else {
		gVolumeMode = false;
		gDataZS = 1;
	}
	
	gImSize  = gXS * gYS;
	gVolSize = gImSize * gZS;
	gDataVolSize = gDataXS * gDataYS * gDataZS;
	if( !gDataVolSize || !gVolSize ) {
		printf( "Invalid header data\n" );
		return -1;
	}
	
	if( gVerbose ) {
		printf( "The input file %s has dimensions\n\t%d(x) x %d(y) x %d(z) x %d(time)\n", 
		         gImage_fname, gXS, gYS, im->dim.n_slices, im->dim.timePts );
		if( gVolumeMode ) {
			printf( "Processing in volume mode\n\n" );
		}
	}
// Determine smoothing dimensions
	if( gxSmooth < 0.0 ) {          // userWidth was initialized to -1
		gxSmooth = 3*gXS/8;
	}
	if( gySmooth < 0.0 ) {
		gySmooth = 3*gYS/8;
	}	
	if( gzSmooth < 0.0 ) {
		gzSmooth = 3*im->dim.n_slices/8;
	}
	*zscale = gzSmooth*gzSmooth / (gZS*gZS);
	
	if( gVerbose ) {
		printf( "The smoothing width is %0.3f pixels in x\n%0.3f in y,\nand %0.3f pixels in z\n",
		        gxSmooth, gySmooth, gzSmooth );
	}
	fprintf( gProcFile, "Image Dimensions ...... %hd (X) x %hd (Y) x %hd (Z ) x %hd (time )\n",
	         gXS, gYS, gZS, im->dim.timePts );
	fprintf( gProcFile, "Smoothing width ....... %0.3f pixels in x\n", gxSmooth );
	fprintf( gProcFile, "                        %0.3f pixels in y\n", gySmooth );
	fprintf( gProcFile, "                        %0.3f pixels in z\n", gzSmooth );

	return noErr;
}

/*******************************   Calc Average   ************************
*
*	Determine the average intensity and number of locations of pixels
*	whose intensity exceeds 'gThresh'
**************************************************************************/     
float CalcAverage( float *Thresh, float *data, long pts, long *validPts )
{
	long   i;
	float  average;
	char   response[3];
	
	average = 0.0;
	*validPts = 0L;

	for( i=0; i<pts; i++ ) {
		if( data[i] > *Thresh ) {
			average += data[i];
			(*validPts)++;
		}
	}
	
	average = (float)(average / *validPts);

	if( *validPts > 3*pts/4 ) {	// still pretty liberal
		printf( "\nCurrent average: %0.3f\n", average );
		printf( "Your threshold is probably too low.\n" );
		printf( "The threshold is currently %0.2f\n", *Thresh );
		printf( "Suggestion: increase threshold to %0.2f\n", *Thresh*2 );

		printf( "\nSet new threshold? [y]/n: " );
		gets( response );
		if( !strcmp(response, "N") || !strcmp( response, "n" )) {
			;
		} else {
			printf( "New threshold level [%0.2f]: ", *Thresh*2 );
			gets( response );
			if( strlen( response )) {
				*Thresh = atof( response );
			} else {
				*Thresh = *Thresh*2;
			}	
			printf( "recalculating with threshold of %0.2f\n", *Thresh );
			if( gThreshAndFill > 0 ) {
				gThreshAndFill = *Thresh;
			}
			average = CalcAverage( Thresh, data, pts, validPts );
		}
	}
	
	fprintf( gProcFile, "Valid points........... %ld\n", *validPts );
	fprintf( gProcFile, "Average image signal .. %0.3f\n", average );
	fprintf( gProcFile, "Threshold used ........ %0.3f\n", *Thresh );
	if( gThreshAndFill > 0 ) {
		fprintf( gProcFile, "Fill used ............. %f\n", gThreshAndFill );
	} else {
		fprintf( gProcFile, "Fill used ............. %f\n", average );
	}	
	return average;
}

/*******************************   FirstFT   *******************************
*
*	The first Fourier Transform is slightly more complicated, as it
*	involves a quadrant exchange and intensity scaling
**************************************************************************/     
OSErr FirstFT( float *imData )
{
	float FTscale;
	OSErr error = noErr;
				
	FTscale = 1.0/gVolSize;
	if( gVerbose ) {
		printf( "gVolSize: %ld\n", gVolSize );
		printf( "FTscale: %f\n", FTscale );
	}

	error = vsmul( imData, 2, imData, 2, (void *)&FTscale, gDataVolSize, T_FLOAT );	// Adjust intensity for FT effects
	ILError( error, "FirstFT - vsmul" );
	
	if( !gVolumeMode ) {
		error = cfft2d( imData, gDataXS, gDataYS, FORWARD );
		ILError( error, "cfft2d-FirstFT" );
	} else {
		error = cfft3d( imData, gDataXS, gDataYS, gDataZS, FORWARD );
		ILError( error, "cfft3d-FirstFT" );
	}
	
	if( !gVolumeMode ) {
		exch_quad( imData, gDataXS, gDataYS );
	} else {
		error = exch_quad3d( imData, gDataXS, gDataYS, gDataZS );
		ILError( error, "Quadrant exchange" );
	}
	return error;
}			

/*******************************   kSmooth   *******************************
*
*	Smooth the 3D image by convolution with an already calculated gaussian
***************************************************************************/     
OSErr kSmooth( float *imData, float *gaussKernel )
{
	float Tiny;
	long  i;
	float kLoc;
	float EZ;
	float zscale;
	
	long  padImSize = gDataXS * gDataYS;	// padded image size
	
	OSErr error = noErr;

// Calculate a scaling level below which the data might as well be zero.
	Tiny = 0.01/(gXS*gYS*gZS);
	zscale = gzSmooth*gzSmooth / (gZS*gZS);
	
	for( i=0; i < gDataZS; i++ ) {
		
// Here we do one dimension of the k-smoothing by multiplying each transformed plane
		kLoc = i - gDataZS/2.0 + 0.5;
		EZ = exp( -zscale * kLoc * kLoc );
		
		if( EZ > Tiny ) {
			error = vsmul( imData + i*padImSize*2, 1, imData + i*padImSize*2, 1, (void *)&EZ, padImSize*2, T_FLOAT );
			if( error ) return error;
// In-plane smoothing			
			error = vmul( &imData[i*padImSize*2], 2, gaussKernel, 1, &imData[i*padImSize*2], 2, padImSize, T_FLOAT );
			if( error ) return error;
			error = vmul( &imData[i*padImSize*2 + 1], 2, gaussKernel, 1, &imData[i*padImSize*2 + 1], 2, padImSize, T_FLOAT );
			if( error ) return error;
		} else {
// There is no point in a trivial multiplication
			vclr( &imData[i*padImSize*2], padImSize*2, T_FLOAT );
		}
		if( gVerbose ) printf( "." );
	}
	return error;
}

/****************************   ExtractImage   *****************************
*
*	Utility function to save a selected slice plane and its intermediate
*	processing steps.
***************************************************************************/     
OSErr ExtractImage( float *imData, float *RawData, short extract_img, IMAGE *im )
{
	dsr    theDSR;
	FILE   *f;
	char   buffer[256];
	OSErr  error = noErr;
	
	if( extract_img > im->dim.n_slices -1 ) {
		ILError( SILENT_ERR, "Extract image is out of range" );
	}
	
// Raw image
// header
	sprintf( buffer, "%s.Raw.hdr", gOutput_fname );
	EmptyAnaHdr( &theDSR, "Raw Image", 0, gXS, gYS, 1, 1, axial );
	theDSR.dime.glmin = (int)im->theMinf;
	theDSR.dime.glmax = (int)im->theMaxf;

	f = fopen( buffer, "wb" );
	ck_fwrite( &theDSR, sizeof( dsr ), 1, f );
	fclose( f );
// image			
	sprintf( buffer, "%s.Raw.img", gOutput_fname );
	f = fopen( buffer, "wb" );
	error = writeVolume( f, RawData + extract_img*gImSize, gImSize, BSHORT );
	ILError( error, "Saving smoothed file" );
	fclose( f );

// Smoothed image
//  header
	sprintf( buffer, "%s.Smooth.hdr", gOutput_fname );
	EmptyAnaHdr( &theDSR, "Smoothed Image", 0, gXS, gYS, 1, 1, axial );
	theDSR.dime.glmin = (int)im->theMinf;
	theDSR.dime.glmax = (int)im->theMaxf;

	f = fopen( buffer, "wb" );
	ck_fwrite( &theDSR, sizeof( dsr ), 1, f );
	fclose( f );
// image
	sprintf( buffer, "%s.Smooth.img", gOutput_fname );
	f = fopen( buffer, "wb" );
	error = writeVolume( f, imData + extract_img*gImSize, gImSize, BSHORT );
	ILError( error, "Saving smoothed file" );
	fclose( f );
	
// Mask
// header
	sprintf( buffer, "%s.mask.hdr", gOutput_fname );
	theDSR.dime.glmin = 0;
	theDSR.dime.glmax = 1;

	f = fopen( buffer, "wb" );
	fprintf( f, "%d %d %d %d", gYS, gXS, gZS, macByteOrder() );
	fclose( f );
	
	fprintf( gProcFile, "Image %hd was extracted\n", extract_img );
	
	if( gVerbose ) {
		printf( "\nImage %hd was extracted as %s.Raw.img, %s.Smooth.img and %s.mask.buchar\n",
		         extract_img, gOutput_fname, gOutput_fname, gOutput_fname );
	}

	return error;
}

/**************************   NormalizeAndPad   *****************************
*
*	Normalize the raw image by the smoothed data, rescale to nominal
*	average intensity, and fill noise areas with original data
****************************************************************************/
void NormalizeAndPad( float *SmoothData, float *RawData, float averageInt )
{
	float  corrTimesAvg;          // correction factor multiplied by average (for loop efficiency)
	float  OneMinusCorrFactor;    // For normalization (loop invariant)
	long   i;


	OneMinusCorrFactor = 1.0 - gCorrFactor;
	corrTimesAvg = gCorrFactor * averageInt;

	if( gThreshAndFill > 0.0 ) {
		for(i=0; i<gVolSize; i++) {			
			if( RawData[i] > gThreshAndFill ) {
				SmoothData[i] = corrTimesAvg * (RawData[i]/SmoothData[i])
				            + OneMinusCorrFactor * RawData[i];
			} else {
				SmoothData[i] = RawData[i];
			}
		}
	} else {	
		for(i=0; i<gVolSize; i++) {			
			if( RawData[i] > gThresh ) {
				SmoothData[i] = corrTimesAvg * (RawData[i]/SmoothData[i])
				            + OneMinusCorrFactor * RawData[i];
			} else {
				SmoothData[i] = RawData[i];
			}
		}
	}
}

/***************************************************************************
*	writeVolume
*
*	inputs: floating point data
*	        open file for writing
*           number of points in volume
*   outputs: error condition
*   changes: outfile
*
*	Converts output data to the desired format, checking for range errors
****************************************************************************/
OSErr writeVolume( FILE *f, float *data, long gVolSize, int fileType )
{
	long  count;
	char  Range_error = false;
	OSErr error = noErr;
	unsigned short *sdata;

	switch( fileType )
	{
		case BFLOAT:
			ck_fwrite( data, sizeof( float ), gVolSize, f );
			break;
		case BSHORT:
		case ANALYZE:
			sdata = (unsigned short *)malloc( gVolSize * sizeof( short ));
			for( count=0; count<gVolSize; count++ ) {
				if( data[count] > USHRT_MAX ) {
					sdata[count] = USHRT_MAX;
					Range_error = true;
				} else if( data[count]<0 ) {
					sdata[count] = 0;


					Range_error = true;
				} else {
					sdata[count] = (unsigned short)(data[count]+0.5);
				}
			}
			ck_fwrite( sdata, sizeof( short ), gVolSize, f );
			break;
		default:
			return UNKNOWNFILETYPE;
	}

// handle serious errors first
	if( error ) {
		fclose( f );
		return error;
	}
	
	if( Range_error ) {
		return DATATYPE_RANGE_ERROR;
	}
	
	return error;
}

/***************************************************************
*	writeHeader
*
*	Write a header of the prescribed type
***************************************************************/
OSErr writeHeader( IMAGE *im, int fileType, char *basename )
{
	FILE  *f;
	char  buff[256];
	dsr   theDSR;
	OSErr error = noErr;
	
	sprintf( buff, "%s.hdr", basename );

	switch( fileType ) {
		case ANALYZE:
			f = fopen( buff, "wb" );
				if( !f ) {
				return WRITE_ERROR;
			}
			EmptyAnaHdr( &theDSR, "Equalized image", im->dim.scanspacing, im->dim.x,
			             im->dim.y, im->dim.n_slices, im->dim.timePts, axial );
			theDSR.dime.glmin = (int)im->theMinf;	// make sure that this is in the header
			theDSR.dime.glmax = (int)im->theMaxf;
			ck_fwrite( &theDSR, sizeof( dsr ), 1, f );
			break;
		case BSHORT:
		case BFLOAT:
			f = fopen( buff, "w" );
				if( !f ) {
				return WRITE_ERROR;
			}
			fprintf( f, "%d %d %d %d", im->dim.y, im->dim.x, im->dim.n_slices, macByteOrder() );
			break;
		default:
			error = UNKNOWNFILETYPE;
	}
	fclose( f );
	return error;
}

int main(int argc, char **argv)
{
	static char id[] = "$Revision: 1.28 $$Date: 2003/05/13 05:29:39 $";
// dimensions
	long       validpts;
	
// counters
	long       i;
	long       tp;	               // image time point

// intensity correction
	float      zscale;
	float      averageInt;         // average intensity for non-noise pixels
	float      fill;
	
// offset calculation variables
	long	   imSize;
	long	   sliceSize;
	long	   z_offset;

// processing structures & buffers
	char       buffer[256];
	char       basename[256];      // image file name without extensions
	char       ext[9];             // output filename extension
	IMAGE      im;
	float      *imData, *gaussKernel, *RawData;
	FILE       *outFile, *inFile;

	Boolean    rescale = false;    // perform a rescaling if the data range is very small
	OSErr      error = noErr;
	
	int		   real = 1;
	int		   complex = 0;
	int		   pad = 1;
	int		   unpad = 0;
	
	printf( "%s\n\t%s\n", argv[0], id );	// tell 'em who we are
#ifdef mactest
	argc = ccommand(&argv);
#endif
	
	error = ProcessCommandLine( argc, argv );
	if( error ) return error;
	
	error = OpenProcFile( argc, argv, gOutput_fname, id, &gProcFile );
	ILError( error, "Opening Proc file" );
	
// determine name extension
	switch( gOutfileType ) {
		case ANALYZE:
			strcpy( ext, "img" ); break;
		case BFLOAT:
			strcpy( ext, "bfloat" ); break;
		case BSHORT:
			strcpy( ext, "bshort" ); break;
		default:
			printf( "Unrecognized output file type: %d\n\n", gOutfileType );
			print_help( argv[0] );
			break;
	}

	if( gVerbose ) {
		char type[8];
		
		switch( gOutfileType ) {
			case BSHORT:  sprintf( type, "BSHORT" ); break;
			case BFLOAT:  sprintf( type, "BFLOAT" ); break;
			case ANALYZE: sprintf( type, "ANALYZE" ); break;
		}
		printf( "Saving data as type: %s\n", type );
	}
	
	error = UC_Readheader( gImage_fname, &im );
	if( error ) {
		ILError( INVALID_HDR, "Reading input file" );
		return 0;
	}

// Determine image dimensions
	error = CalcDimensions( &im, &zscale);
	if( error ) return error;

// Memory allocations
printf( "gDataVolSize: %ld\n", gDataVolSize );
	imData = (float *)ck_calloc( 2 * gDataVolSize, sizeof(float), "imData" );
	fill = (float)LONG_MIN;
	error = vfill( imData, (void *)&fill, 2*gDataVolSize, T_FLOAT );

	ILError( error, "padding data vector with little numbers" );
	gaussKernel = (float *)ck_malloc( gDataXS * gDataYS * sizeof(float), "gaussKernel" );

// open input file
	inFile = errfopen( gImage_fname, "rb" );
	if( inFile == NULL ) {
		printf( "Unable to open %s\n. Aborting", gImage_fname );
		return -1;
	}

// pre-calculate the smoothing kernel in 2D or 3D
	printf( "Working\n" );
	if( gVerbose ) printf( "Creating smoothing kernel\n" );

	error = TwoDGauss( gaussKernel, gxSmooth, gySmooth, gDataXS, gDataYS );
	ILError( error, "Couldn't create gaussian kernel" );

	if( gOutfileType == ANALYZE ) {
		sprintf( buffer, "%s.%s", gOutput_fname, ext );		
		outFile = errfopen( buffer, "wb" );
	}
	
// Loop on timePts
	for( tp=0; tp<im.dim.timePts; tp++ ) {  // once per time point
		float glmax=LONG_MIN;
		float glmin=LONG_MAX;
		float tpmax=LONG_MIN;
		float tpmin=LONG_MAX;	// these are used to hold data min and max.

// Setup output file
		if( gOutfileType == BSHORT || gOutfileType == BFLOAT ) {
			if( im.dim.timePts > 1 ) {
				sprintf( basename, "%s.%0.3d", gOutput_fname, tp+1 );
			} else {
				sprintf( basename, "%s", gOutput_fname );
			}	
			sprintf( buffer, "%s.%s", basename, ext );
			outFile = errfopen( buffer, "wb" );
			if( outFile == NULL ) {
				printf( "Unable to open %s\n. Aborting", buffer );
			}
		}
		
		if( im.dim.timePts>1 ) { 
			printf( "time point:%d\n", tp+1 );
		}
		
		vclr( imData, gDataVolSize, T_FLOAT );
			
		if( gVerbose ) printf( "\tReading...\n" );
		im.data_type = T_FLOAT;
				
		error = GetSelectedVolume( inFile, &im, imData, tp );
		ILError( error, "Reading image volume" );
		if( gVerbose ) printf( "\tThe input pixel intensities range from %f to %f\n",
		                         im.theMinf, im.theMaxf );

		if( im.theMaxf - im.theMinf < 256 ) {
			if( gOutfileType != BFLOAT ) {
				char response[3];
				gScale = (float)USHRT_MAX/(100*(im.theMaxf - im.theMinf));
				gScale = 10 * (int)(gScale/10);
				if( gScale >= 1 ) {
					for( i=0; i<60; i++ ) {
						printf( "*" );
					}
					printf( "\n*\tThe input data range is small.\n*\tIs it okay to multiply the values by %0.0f? ",
						gScale );
					gets( response );
					for( i=0; i<60; i++ ) {
						printf( "*" );
					}
					if( !strcmp( response, "Y" ) || !strcmp( response, "y" )) {
						rescale = true;
						error = vsmul( imData, 1, imData, 1, &gScale, gVolSize, T_FLOAT );
						ILError( error, "Intensity scaling" );
						im.theMaxf = im.theMaxf * gScale;
						im.theMinf = im.theMinf * gScale;
					}
				printf( "\n" );
				}
			}
		}

// calculate the nominal threshold and intensity scaling levels
		if( gAutoMask ) {  
			float hicut;
			error = imageDataRange( imData, gVolSize, &hicut, &gThresh, &validpts, T_FLOAT );
			ILError( error, "ImageDataRange" );
		}
		
// We must determine the average image intensity of the volume to make sure it doesn't change
		printf( "\tForming average...\n" );	
		averageInt = CalcAverage( &gThresh, imData, gVolSize, &validpts );

		if( gVerbose ) {
			printf( "\t\tAverage intensity is: %f\n", averageInt );
		}
		if( gVerbose ) {
			printf( "\t\tThreshold -- %0.4f  ", gThresh );
			printf( "  %d valid pts\n", validpts );
		}

// pad the data in all three dimensions
		PadVolume( imData, &im, gDataXS, gDataYS, gDataZS, real, pad );

// To prevent blooming of the edges, place the average intensity in the background
// Note that we only populate the real part: the imaginary part is zero...
		printf( "\tPopulating background before smoothing...\n" );
		if( gThreshAndFill > 0 ) {
			for( i=0; i < gDataVolSize; i++ ) {
				if( imData[i] < gThreshAndFill ) {
					imData[i] = gThreshAndFill;
				}
			}
		} else {
			fill = averageInt;
			for( i=0; i < gDataVolSize; i++ ) {
				if( imData[i] <= gThresh ) {
					imData[i] = fill;
				}
			}
		}

		if( gExtract_img > -1 ) {	
			dsr    theDSR;
			FILE   *f;
			char   buffer[256];
			OSErr  error = noErr;
			
			if( gExtract_img > im.dim.n_slices ) {
				ILError( SILENT_ERR, "Extract image is out of range" );
			}
			
		// Raw image
		// header
			sprintf( buffer, "%s.Fill.hdr", gOutput_fname );
			EmptyAnaHdr( &theDSR, "Fill Image", 0, gXS, gYS, 1, 1, axial );
			f = fopen( buffer, "wb" );
			ck_fwrite( &theDSR, sizeof( dsr ), 1, f );
			fclose( f );
		// image			
			sprintf( buffer, "%s.Fill.img", gOutput_fname );
			f = fopen( buffer, "wb" );
			error = writeVolume( f, imData + gExtract_img*gImSize, gImSize, BSHORT );
			ILError( error, "Saving smoothed file" );
			fclose( f );
		}
		
// Expand data from real to complex
		RealToComplex( imData, gDataVolSize );

// This is the meat of the algorithm: Fourier transform the image
// Multiply the transformed image by a gaussian
// Back transform. This convolves the image with the desired gaussian
		printf( "\tFirst FFT...\n" );	

		error = FirstFT( imData );
		ILError( error, "FirstFT");
		
		printf( "\tk-space smoothing...\n\t" );
		
		error = kSmooth( imData, gaussKernel );
		ILError( error, "k-space smoothing");

		printf( "\n\tSecond FFT...\n" );	

		if( !gVolumeMode ) {
			error = cfft2d( imData, gDataXS, gDataYS, REVERSE );
			ILError( error, "Reverse 2D Fourier transform" );
		} else {
			error = cfft3d( imData, gDataXS, gDataYS, gDataZS, REVERSE );
			ILError( error, "Reverse 3D Fourier transform" );
		}

// unpad the data in all three dimensions (data stil complex)
		PadVolume( imData, &im, gDataXS, gDataYS, gDataZS, complex, unpad );;
		
// Magnitude calculation 
//vmag is used in-place. This is is not really standard
		error = vmag( imData, 2, imData, 1, gVolSize );
		ILError( error, "Magnitude calculation" );
	
// Load an unsmoothed version of the data on the end of the already
// allocated memory space.
		printf( "\tReading (again)\n" );
		
		RawData = imData + gDataVolSize;	// point to second half of buffer

		im.data_type = T_FLOAT;
		error = GetSelectedVolume( inFile, &im, RawData, tp );
		ILError( error, "Reading image volume - second time" );

		if( rescale ) {
			error = vsmul( RawData, 1, RawData, 1, &gScale, gVolSize, T_FLOAT );
			ILError( error, "Intensity scaling" );
		}

// Undocumented feature - sample an image and its intermediate steps
		if( gExtract_img > -1 ) {
			error = ExtractImage( imData, RawData, gExtract_img, &im );
			ILError( error, "ExtractImage");
		}

// add ability to save a 'bias field'
		if( gSaveBias ) {
			dsr   theDSR;
			FILE  *f;
 			char  fname[256];
			char  basename[256];

			printf( "Saving a bias field image.\n" );
			printf( "Please enter a name (no extension): " );
			scanf ( "%s", basename );

			sprintf( fname, "%s.hdr", basename );

			error = EmptyAnaHdr( &theDSR, "Corrected image", im.dim.scanspacing,
			                     gXS, gYS, gZS, 1, axial );
			ILError( error, fname );
			theDSR.dime.datatype = DT_FLOAT;

			f = fopen( fname, "wb" );
			error = ck_fwrite( &theDSR, 1, sizeof( dsr ), f );
			ILError( error, "Writing bias field header" );
			ck_fclose( f );

			sprintf( fname, "%s.img", basename );
			f = fopen( fname, "wb" );
			error = ck_fwrite( imData, gVolSize, sizeof( float ), f );
			ILError( error, fname );
			ck_fclose( f );
		}
			
// normalize the image, for all pixels above threshold
		NormalizeAndPad( imData, RawData, averageInt );
		
		if( gExtract_img > -1 ) {
			dsr   theDSR;
			FILE  *f;
			long  p;

	// mask image
			sprintf( buffer, "%s.mask.buchar", gOutput_fname );
			f = fopen( buffer, "wb" );
			for( p=0; p<gImSize; p++ ) {
				unsigned char one = 1; unsigned char zero = 0;

				if( imData[gExtract_img*gImSize + p] > gThresh ) {
					ck_fwrite( &one,  sizeof(char), 1, f );
				} else {
					ck_fwrite( &zero, sizeof(char), 1, f );				
				}
			}

// Corrected image			
			fclose( f );
			EmptyAnaHdr( &theDSR, "Corrected image", 0, gXS, gYS, 1, 1, axial );
			sprintf( buffer, "%s.Corrected.hdr", gOutput_fname );
			theDSR.dime.glmin = (int)im.theMinf;
			theDSR.dime.glmax = (int)im.theMaxf;

			f = fopen( buffer, "wb" );
			ck_fwrite( &theDSR, sizeof( dsr ), 1, f );
			fclose( f );
	// header		
			sprintf( buffer, "%s.Corrected.img", gOutput_fname );
			f = fopen( buffer, "wb" );
			error = writeVolume( f, imData+gExtract_img * gImSize, gImSize, BSHORT );
			ILError( error, "Saving corrected file" );
			fclose( f );
		}
				
// write out the volume at this time point
		printf( "\twriting...\n" );

	// find the min and max in the output file
		error = vminmax( imData, gVolSize, &tpmax, &tpmin, T_FLOAT );
		ILError( error, "Finding data range for output file" );
	// See if this is the maximum for the time series
		im.theMaxf = tpmax > glmax ? tpmax : glmax;
		im.theMinf = tpmin < glmin ? tpmin : glmin;

		error = writeVolume( outFile, imData, gVolSize, gOutfileType );
		if( error ) {
			fclose( outFile );
			ILError( error, gOutput_fname );
		}
		if( gOutfileType == BSHORT || gOutfileType == BFLOAT ) {
			
			error = writeHeader( &im, gOutfileType, basename );
			fclose( outFile );	// separate file for each time point
			ILError( error, buffer );
		}
		
	}	// timePts
	fclose( gProcFile );
	fclose( inFile );

	free( gaussKernel );
	free( imData );

// analyze is a 4D format - single file for all volumes.
	if( gOutfileType == ANALYZE ) {
		fclose( outFile );
// zrinka 05/12/03: writeHeader was writing incorrect analyze header files (x and y voxel size was
//	always 3.125x3.125. Changed to a call to CreateHeader. Also, need to change im.dim->data_type
//	bach to ushort (float set earlier for fft processing)
//		error = writeHeader( &im, gOutfileType, gOutput_fname );
		
		im.data_type = T_USHORT;
		error = CreateHeader( gOutfileType, &im, gOutput_fname );
		ILError( error, buffer );
	}
	printf( "\nDone. Thank you for using %s.\nmscohen@ucla.edu -- http://www.brainmapping.org\n", argv[0] );

	return 0;
}

/* </pre></body></html> */
