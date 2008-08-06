/* <html><head><title>T2Map source</title></head><body> <h1>T2Map.c</h1>
<i>Please note last saved date:</i>
 <b><font size=+1><b>$Date: 2008/05/16 16:07:31 $</font></b><p>
<center>
<table bgcolor="black" cellpadding=5 border="1">
	<tr><td><font color="white">
Find the Ratio of a series of images compared to a single image
	</font></td></tr></table><p>
	&copy;2005-today  <a href="mailto:mscohen@ucla.edu">Mark S. Cohen, Ph.D.</a><br>
	<a href="http://www.brainmapping.org">UCLA Brain Mapping Division</a><br>
</center>
<h2>How to make T2Map</h2>
<ol>
	<li>Build ImgLib.o analyzeUtil.o ImgLibError.o VLib.o MathLib.o (from
	<a href="http://airto.loni.ucla.edu/BMCweb/SharedCode/SharedSoftware.html#Anchor-UCLA-35829">
www.brainmapping.org</a>)
	<li>compile T2Map, linking the objects above. You will need to have C++ extensions and the Math Library.
	A typical command might be:<br><font face=courier><p>
	<center>
		cc ImgLib.o analyzeUtil.o ImgLibError.o VLib.o MathLib.o T2Map.c -o T2Map -lm</font>
	</center>
</ol>
<center>
<table border=2 cellpadding=8 width="90%"><tr><td>
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
Unless otherwise stated, I do not offer technical support beyond that found on the
<a href="http://airto.loni.ucla.edu/BMCweb/HowTo/T2Map.html">
Brain Mapping Center support pages</a>.
<p>
Many of these programs have Macintosh executables available at: <a href="ftp://ftp.brainmapping.org/">ftp.brainmapping.org</a></tr>
</td></tr></table></center>
<pre>
<!--
$Author: mscohen $
$Log: T2Map.c,v $
Revision 1.6  2008/05/16 16:07:31  mscohen
Fixed myriad problems with gaussSmooth
Deprecated function fitPts in favor of easier to read use of LinFit
Smoothing is now in mm instead of pixels

Revision 1.4  2006/06/09 18:39:31  mscohen
Created anew

Revision 1.3  2006/05/20 04:14:37  mscohen
Ccomplete re-write. Generate T2 maps based on chi-squared minimization of linear fit.

*/

#include "getopt.h"
#include "ImgLib.h"

#define false 0
#define true 1
#ifndef noErr
#define noErr 0
#endif

#undef MAC
#ifdef MAC
#include <console.h>
#include <sioux.h>
#endif

/* globals */
typedef struct {
	char    OutFileName[255];
	char    InFileName[255];
	char    OutBaseName[256];
	char    TEFileName[256];
	short   output_datatype;
	short   NumOutImages;
	IMAGE   inIm;
	IMAGE   outIm;
	Boolean Verbose;
	Boolean autoThresh;
	Boolean Smooth;
	float   xSmooth;
	float   ySmooth;
	float   zSmooth;
	FILE    *inFile;
	FILE    *outFile;
	FILE	*TEFile;
	FILE    *ProcFile;
	float   threshold;
	long    nonNoise;
} UtilGlobal;

UtilGlobal u;

/* PROTOTYPES */
OSErr fitPts(float inData[], unsigned char *theMask,
             float te[], float T2[], float intercept[], float chi2[], long VolSize);
OSErr InitializeUserVariables( void );
OSErr T2Fit( FILE *inFile, FILE *outFile );
OSErr AdjustOutputIMAGE( IMAGE *outIm );
void  print_usage( char *name );
OSErr ProcessCommandLine( int argc, char *argv[] );
OSErr OutBaseNameFromOutName( char *OutFileName, char *OutBaseName );
OSErr RunMain( int argc, char **argv, char *id );

/****************************   Initialize   *************************/
OSErr InitializeUserVariables( void )
{
	OSErr error = noErr;

	error = vclr( &u, sizeof( UtilGlobal ), T_CHAR );
	RETURNONERROR;

	u.NumOutImages = 0;
	u.threshold = 0;
	u.autoThresh = true;
	u.Smooth = false;
	u.xSmooth = u.ySmooth = u.zSmooth = 3;
	
	return error;
}

/* alternative to CalcT2
OSErr ln( float *data, long npts )
{
	long i;
	OSErr error = noErr;
	
	for( i=0; i<npts; i++ ) {
		if( data[i] <=0 ) {
			data[i] = 0;
		} else {
			data[i] = log(data[i]);
		}
	}

	return error;
}

OSErr fitPts(float inData[], unsigned char theMask[],
             float te[], float T2[], float intercept[], float chi2[], long VolSize)
{
	long    thePt;
	int     nx = u.inIm.dim.timePts;
	float   *yvals;
	float   R2;
	int     i;
	OSErr   error = noErr;
	
	yvals = (float *)ck_malloc( nx * sizeof(float),  "Storage for yvals" );
	
	error = vclr( T2, VolSize, T_FLOAT);              RETURNONERROR;
	error = vclr( chi2, VolSize, T_FLOAT);            RETURNONERROR;
	error = ln( inData, VolSize*u.inIm.dim.timePts ); RETURNONERROR;

	for( thePt=0; thePt<VolSize; thePt++ ) {
		if( theMask[thePt] ) {
			for(i=0; i<nx; i++) {
				yvals[i] = inData[thePt + i*VolSize];
			}

			error = Linfit( te, yvals, nx, &R2, &intercept[thePt], &chi2[thePt] );
				RETURNONERROR;
			intercept[thePt] = exp( intercept[thePt] );

			if(R2 != 0) {
				T2[thePt] = -1/R2;
			} else {
				T2[thePt] = chi2[thePt] = intercept[thePt] = 0;
			}
		}
	}
	return error; 
}		
*/
/**********************************    T2Fit     ************************************
*   This is where the work will be done
*************************************************************************************/
OSErr T2Fit( FILE *inFile, FILE *outFile )
{
	OSErr   error = noErr;
	long    theTimePt;
	int     i;
	float   *inData, *outData, *T2, *intercept, *chi2, *Smoother;
	long    VolSize, outVolBytes, inVolBytes;
	float	te[ u.inIm.dim.timePts ];
	unsigned char *theMask;
	short   rules;

	u.NumOutImages   = 0;
	VolSize          = u.inIm.dim.isoX * u.inIm.dim.isoY * u.inIm.dim.n_slices;
	inVolBytes       = VolSize * get_datasize( T_FLOAT );
	outVolBytes      = VolSize * get_datasize( u.outIm.data_type );
	T2               = (float *)ck_malloc( inVolBytes,  "Storage for R2" );
	intercept        = (float *)ck_malloc( inVolBytes,  "Storage for intercept" );
	chi2             = (float *)ck_malloc( inVolBytes,  "Storage for chi2"  );
	outData          = (float *)ck_malloc( outVolBytes, "Storage for output data" );
	inData           = (float *)ck_malloc( inVolBytes * u.inIm.dim.timePts,  "Storage for input data" );
	theMask          = (unsigned char*)ck_malloc( VolSize * sizeof( unsigned char ),  "Storage for theMask" );
	if( u.Smooth ) {
		Smoother = (float *)ck_malloc( inVolBytes * 2, "Storage for smoothing" );
	}

// load up te vector
	if( u.Verbose ) {
		printf( "TE: " );
	}
	for( i=0; i<u.inIm.dim.timePts; i++ ) {
		fscanf(u.TEFile ,"%f", &te[i]);
		if( u.Verbose ) {
			printf( "%0.3f ", te[i] );
		}
	}
	
// We will do everything in float
	SetImDatatype( &u.inIm, T_FLOAT );

//	Load all of the images into a single matrix
	for ( theTimePt = 0; theTimePt < u.inIm.dim.timePts; theTimePt++ ) {
		error = GetSelectedVolume( inFile, &u.inIm, inData+theTimePt*VolSize, theTimePt );
		RETURNONERROR;
		if( u.Smooth ) {
			vmov( inData+theTimePt*VolSize, 1, Smoother, 1, VolSize, T_FLOAT );
			error = gaussSmooth( &u.inIm, Smoother, inData+theTimePt*VolSize, &u.xSmooth, &u.ySmooth, &u.zSmooth );
			RETURNONERROR;

		}
		RETURNONERROR;
	}
	

// threshold on first images
	if( u.autoThresh ) {
		error = autoMask( inData, theMask, VolSize, &u.threshold, &u.nonNoise, T_FLOAT );
		RETURNONERROR;
	} else if( u.threshold != 0 ) {
		u.nonNoise = ThreshMask( inData,  theMask, VolSize, &u.threshold, T_FLOAT );
	} else {
		u.nonNoise = VolSize;
	}

	error = CalcT2( &u.inIm, inData, theMask, te, T2, intercept, chi2 ); RETURNONERROR;

	free( inData );

	error = type_convert( T2, T_FLOAT,
						  outData, u.outIm.data_type,
						  VolSize, &u.outIm.theMinf, &u.outIm.theMaxf, &rules );
	if( error && error != CONVERSION_ERROR ) return error;
	error = ck_fwrite( outData, 1, outVolBytes, outFile );
	u.NumOutImages++;
	RETURNONERROR;

	error = type_convert( intercept, T_FLOAT,
						  outData, u.outIm.data_type,
						  VolSize, &u.outIm.theMinf, &u.outIm.theMaxf, &rules );
	if( error && error != CONVERSION_ERROR ) return error;
	error = ck_fwrite( outData, 1, outVolBytes, outFile );
	u.NumOutImages++;
	RETURNONERROR;

	if( u.Smooth ) {
		free( Smoother );
	}

// Make R2 image
	for( i=0; i<VolSize; i++ ) {
		if( T2[i] !=0 ) {
			T2[i] = 1/T2[i];
		}
	}
	error = type_convert( T2, T_FLOAT,
						  outData, u.outIm.data_type,
						  VolSize, &u.outIm.theMinf, &u.outIm.theMaxf, &rules );
	if( error && error != CONVERSION_ERROR ) return error;
	error = ck_fwrite( outData, 1, outVolBytes, outFile );
	u.NumOutImages++;
	RETURNONERROR;
	error = type_convert( chi2, T_FLOAT,
						  outData, u.outIm.data_type,
						  VolSize, &u.outIm.theMinf, &u.outIm.theMaxf, &rules );
	if( error && error != CONVERSION_ERROR ) return error;
	error = ck_fwrite( outData, 1, outVolBytes, outFile );
	u.NumOutImages++;
	RETURNONERROR;

	fprintf( u.ProcFile, "Output file contains maps of T2, M0, R2 and chi2\n" );
	fprintf( u.ProcFile, "TE's used: " );
	for( i=0; i<u.inIm.dim.timePts; i++ ) {
		fprintf( u.ProcFile,  "%0.3f ", te[i] );
	}
	fprintf( u.ProcFile, "\n" );
	fprintf( u.ProcFile, "Input data were intensity thresholded at %0.3f\n", u.threshold ); 
	fprintf( u.ProcFile, "Non-Noise Points:\t%ld\n", u.nonNoise );
	if( u.Smooth ) {
		fprintf( u.ProcFile, "Input images were smoothed with a width of %0.3f mm in x, %0.3f mm in y and %0.3f mm in z\n", 
			u.xSmooth, u.ySmooth, u.zSmooth );
	}
	fprintf( stderr, "Output file contains maps of T2, M0, R2 and chi2\n" );
	fprintf( stderr, "TE's used: " );
	for( i=0; i<u.inIm.dim.timePts; i++ ) {
		fprintf( stderr,  "%0.3f ", te[i] );
	}
	fprintf( stderr, "\n" );
	fprintf( stderr, "Input data were intensity thresholded at %0.3f\n", u.threshold ); 
	fprintf( stderr, "Non-Noise Points:\t%ld\n\n", u.nonNoise );
	if( u.Smooth ) {
		fprintf( stderr, "Input images were smoothed with a width of %0.3f mm in x, %0.3f mm in y and %0.3f mm in z\n", 
			u.xSmooth, u.ySmooth, u.zSmooth );
	}
	free( theMask );
	free( outData );
	free( chi2 );
	free( intercept );
	free( T2 );

	return error;
}

/************************************************************************************/
OSErr AdjustOutputIMAGE( IMAGE *outIm )
{
	outIm->dim.timePts = u.NumOutImages;	// make sure that the output file is dimensioned.

	if( macByteOrder() ) {
		SET( outIm->dim.flags, kmacByteOrder );
	} else {
		CLEAR( outIm->dim.flags, kmacByteOrder );
	}

	return noErr;
}

/* functions */
void print_usage( char *name )
{
	printf( "USAGE: %s -i TESeriesImages.ext1 -f Text_file_of_te_value -o outfile.ext3 [t | V | h | ?]\n\n", name );
	printf( "Create maps of T2, M0, chi2 and R2. Non-fit data points are replaced by NaN\n" );
	printf( "\t-t      -- threshold value for images (automatic otherwise)\n" );
	printf( "\t-k      -- Gaussian smoothing width, in mm.\n" );
	printf( "\t-x, -y, -z  -- independent control of x, y and z smoothing\n" );
	printf( "\tV      -- verbose mode\n" );
	printf( "\th or ? -- print mini-help\n\n" );
	printf( "More information can be found on the Brain Mapping lab HowTo pages\n" );
}

/******************************    ProcessCommandLine     ***************************
*   capture the user input, mostly globals
*************************************************************************************/
OSErr ProcessCommandLine( int argc, char *argv[] )
{
	OSErr error = noErr;
	int argp = EOF;

	u.Verbose = false;

	while ( (argp=getopt(argc,argv,"i:o:f:x:y:z:k:t:Vh?")) != EOF )
	switch( argp )   {
		case 'o':
			strcpy( u.OutFileName, optarg );
		break;

		case 'i':
			strcpy( u.InFileName, optarg );
		break;

		case 'f':
			strcpy( u.TEFileName, optarg );
		break;
		
		case 'k':
			u.Smooth = true;
			u.xSmooth = u.ySmooth = u.zSmooth = strtof( optarg, 0 );
		break;
		
		case 'x':
			u.Smooth = true;
			u.xSmooth = strtof( optarg, 0 );
		break;
		
		case 'y':
			u.Smooth = true;
			u.ySmooth = strtof( optarg, 0 );
		break;
		
		case 'z':
			u.Smooth = true;
			u.zSmooth = strtof( optarg, 0 );
		break;
		
		case 't':
			u.autoThresh = false;
			u.threshold = strtof( optarg, 0 );
		break;

		case 'V':
			u.Verbose = true;
		break;

		case 'h':
		case '?':
		default:
			print_usage( argv[0] );
			exit( -1 );
	}
	
/* check for required arguments */
	if( !strlen( u.InFileName ) || !strlen( u.OutFileName )) {
		print_usage( argv[0] );
		exit( -1 );
	}

	return error;
}

OSErr OutBaseNameFromOutName( char *OutFileName, char *OutBaseName )
{
	OSErr error = noErr;

	strcpy( OutBaseName, OutFileName );
	*strrchr( OutBaseName, '.' ) = '\0';    /* terminate before the extension */
	
	if( !strlen( OutBaseName )) {
		return UNKNOWNTYPE;
	}

	return error;
}

/******************************************/

OSErr RunMain( int argc, char **argv, char *id )
{
	OSErr error = noErr;

	error = InitializeUserVariables();
	RETURNONERROR;

	error = ProcessCommandLine( argc, argv );
	RETURNONERROR;

	error = OpenProcFile( argc, argv, u.OutFileName, id, &u.ProcFile );
	RETURNONERROR;

/* read in the input image header */
	error = UC_Readheader( u.InFileName, &u.inIm );
	RETURNONERROR;

	memcpy( &u.outIm, &u.inIm, sizeof( IMAGE ));	      // copy the input header to the output header
	u.inIm.data_type = T_FLOAT;         // all operations on float

	error = OutBaseNameFromOutName( u.OutFileName, u.OutBaseName );
	RETURNONERROR;

	error = ck_fopen( &u.inFile, u.InFileName,  "rb" );
	RETURNONERROR;

	error = ck_fopen( &u.TEFile, u.TEFileName,  "r" );
	RETURNONERROR;

	error = FindOutputType( u.OutFileName, &u.outIm.file_type, &u.outIm.data_type );
	RETURNONERROR;

// Only floating point output types can be supported at this time.
	if( u.outIm.file_type == ANALYZE ) {
		u.outIm.data_type = T_FLOAT;
	}

	if( u.outIm.data_type != T_FLOAT ) {
		printf( "Only floating point output types are accepted\n" );
		return 0;
	}
	
	error = ck_fopen( &u.outFile, u.OutFileName,  "wb" );
	RETURNONERROR;

/* do something */
	error = T2Fit( u.inFile, u.outFile );
	RETURNONERROR;

	error = ck_fclose( u.TEFile );
	RETURNONERROR;

	error = ck_fclose( u.inFile );
	RETURNONERROR;

	error = ck_fclose( u.outFile );
	RETURNONERROR;

/* Change elements of output IMAGE (e.g., number of images/slices/timePts ) */
	error = AdjustOutputIMAGE( &u.outIm );
	RETURNONERROR;
	
	error = CreateHeader( u.outIm.file_type, &u.outIm, u.OutBaseName );
	RETURNONERROR;

/* dump processing file contents */
	ck_fclose( u.ProcFile );

	return error;
}

/******************************************/

int main( int argc, char **argv)
{
	OSErr   error = noErr;
	static  char id[] = "$Revision: 1.6 $$Date: 2008/05/16 16:07:31 $";

#ifdef MAC
	SIOUXSettings.autocloseonquit = false;
	SIOUXSettings.asktosaveonclose = false;
	argc = ccommand(&argv);
#endif

	error = RunMain( argc, argv, id );
	ILError( error, "Running main program" );

	return error;
}

/* </pre></body></html> */

