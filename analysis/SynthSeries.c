/* <html><head><title>SynthSeries source</title></head><body> <h1>SynthSeries.c</h1>
<i>Please note last saved date:</i>
 <b><font size=+1><b>$Date: 2006/08/01 16:26:56 $</font></b><p>
<center>
<table bgcolor="black" cellpadding=5 border="1">
	<tr><td><font color="white">
SynthSeries is used to create a time series of MR images with characteristics similar
to those of actual fMRI images. Specifically, Rayleigh noise at a user specified depth
is added through Fourier transformation and pixel intensities are modulated according
to a 'paradigm file' with patterns of activation specified by an input image, typically
an activation map from a real MR study using the same data set. 
	</font></td></tr></table><p>
	&copy;2002-today  <a href="mailto:mscohen@ucla.edu">Mark S. Cohen, Ph.D.</a><br>
	<a href="http://www.brainmapping.org">UCLA Brain Mapping Division</a><br>
</center>
<h2>How to make SynthSeries</h2>
<ol>
	<li>Build ImgLib.o analyzeUtil.o ImgLibError.o VLib.o (from
	<a href= 
"http://airto.loni.ucla.edu/BMCweb/SharedCode/SharedSoftware.html#Anchor-UCLA-35829">
www.brainmapping.org</a>)
	<li>compile SynthSeries, linking the objects above. You will need to have C++ extensions and the Math Library.
	A typical command might be:<br><font face=courier><p>
	<center>
		cc ImgLib.o analyzeUtil.o ImgLibError.o VLib.o MathLib.o SynthSeries.c -o SynthSeries -lm</font>
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
<a href="http://airto.loni.ucla.edu/BMCweb/HowTo/SynthSeries.html">
Brain Mapping Center support pages</a>.
<p>
Many of these programs have Macintosh executables available at: <a href="ftp://ftp.brainmapping.org/">ftp.brainmapping.org</a></tr>
</td></tr></table></center>
<pre>
<!--
$Author: mscohen $
$Log: SynthSeries.c,v $
Revision 1.7  2006/08/01 16:26:56  mscohen
Moved gasdev and ran1 to MathLib

Revision 1.6  2003/02/24 23:23:02  mscohen
Modified links to point to airto (not porkpie)

Revision 1.5  2003/01/15 02:08:32  mscohen
Improved reporting of 'M' option

Revision 1.2  2003/01/10 23:10:43  mscohen
Fixed output byte order problem.

Revision 1.1  2002/11/03 06:15:16  mscohen
Initial revision

Revision 1.1  2002/11/03 06:11:33  mscohen
Initial revision

-->
*/

#include "getopt.h"
#include "ImgLib.h"
#include "MathLib.h"

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
	char    OutFileName[256];
	char    InFileName[256];
	char    OutBaseName[256];
	short   output_datatype;
	char    FunctionFileName[256];
	char    ParadigmFileName[256];
	float   ModulationDepth;
	float   NoiseLevel;
	IMAGE   inIm;
	IMAGE   funcIm;
	IMAGE   outIm;
	int     funcImageNum;    // which image from the functional series to use
	short   NumOutImages;
	Boolean ForceFloat;      // force output type to float for analyze images
	Boolean Verbose;
	FILE    *inFile;
	FILE    *paraFile;
	FILE    *funcFile;
	FILE    *outFile;
	FILE    *ProcFile;
} UtilGlobal;

UtilGlobal u;

/* PROTOTYPES */
Boolean InterpretConversionResult( char *ConversionString, short rules );
OSErr SynthSeries( void );
OSErr AdjustOutputIMAGE( IMAGE *outIm );
void  print_usage( char *name );
OSErr ProcessCommandLine( int argc, char *argv[] );
OSErr OutBaseNameFromOutName( char *OutFileName, char *OutBaseName );
OSErr RunMain( int argc, char **argv, char *id );
OSErr InitializeUserVariables( void );
OSErr CheckConsistency( IMAGE *inIm, IMAGE *refIm);
OSErr AddModulation( float paraVal, float *refImage, float ModulationDepth, float *noiseImage, long inVolSize );
OSErr AddRayleighNoise( float *noiseImage, float noiseLevel, IMAGE *im, long seed );
//float gasdev(long *idum);
// float ran1(long *idum);

Boolean InterpretConversionResult( char *ConversionString, short rules )
{
	if( rules == kLossLess ) {
		return false;
	}
	
	if( rules & kResolutionLost ) {
		sprintf( ConversionString, "\tThe output type has lower precision than the input type.\n" );
	}
	if( rules & kDataRescaled ) {
		sprintf( ConversionString, "\tOutput amplitudes were rescaled.\n" );
	}
	if( rules & kRescaledNarrowRange ) {
		sprintf( ConversionString, "\tThe input range was very narrow - amplitdues were rescaled\n" );
	}
	if( rules & kUnknownConversion ) {
		sprintf( ConversionString, "\tType conversion was not possible.\n" );
	}
	
	return true;
}

/****************************   Initialize   *************************/
OSErr InitializeUserVariables( void )
{
	OSErr error = noErr;

	error = vclr( &u, sizeof( UtilGlobal ), T_CHAR );
	RETURNONERROR;

	u.funcImageNum    = 0;	// user may override
	u.NoiseLevel      = 10;
	u.ModulationDepth = 20.0;
	u.Verbose         = false;
	u.ForceFloat      = false;
	u.NumOutImages = 0;
	
	return error;
}

OSErr CheckConsistency( IMAGE *inIm, IMAGE *refIm )
{
	OSErr        error = noErr;

/* run slice range detection dialog (rBaseSliceRange) */
// sizes must be integer multiples
	if( inIm->dim.isoX != refIm->dim.isoX || inIm->dim.isoY != refIm->dim.isoY ) {
		return 1;
	}

	else if( inIm->dim.scanspacing != refIm->dim.scanspacing )
	{
		return 1;
	}

	else if( inIm->dim.n_slices != refIm->dim.n_slices )
	{
		return 1;
	}		
	
	else if( inIm->dim.dfov != refIm->dim.dfov )
	{
		return 1;
	}

	return error;
}


OSErr AddModulation( float paraVal, float *refImage, float ModulationDepth, float *noiseImage, long inVolSize )
{
	OSErr error = noErr;
	long  i;
	
	for( i=0; i<inVolSize; i++ ) {
		noiseImage[i] = refImage[i] * ModulationDepth * paraVal + noiseImage[i];
	}
	
	return error;
}

OSErr AddRayleighNoise( float *noiseImage, float noiseLevel, IMAGE *im, long seed )
{
	OSErr error = noErr;
	long  imSize;
	float *FFTBuff;
	float invImSize;	/* 1/imSize */
	float rootInvImSize;
	short theImage;
	float theNoise;
	long  i;
	int   x = im->dim.isoX;
	int   y = im->dim.isoY;
	long  inputOffset;

	imSize = x * y;
	invImSize = 1.0/imSize;
	rootInvImSize = sqrt( invImSize );
	noiseLevel = noiseLevel*rootInvImSize;	// inputs are scaled first...

	FFTBuff = (float *)malloc( 2*imSize*sizeof( float ));

// do the FFT separately for each image
	for( theImage=0; theImage < im->dim.n_slices; theImage++ ) {
		inputOffset = imSize * theImage;  // in units of 'sizeof(float)'

/* FFT effects */
	/* place in real part */
		vclr( FFTBuff, imSize*2, T_FLOAT );
		for( i=0; i<imSize; i++ ) {
			FFTBuff[i*2] = invImSize * noiseImage[i + inputOffset];
		}

		error = cfft2d( FFTBuff, x, y, FORWARD );
		RETURNONERROR;
		
// add noise in k-space
		for(i=0; i< imSize * 2; i++ ) {
			theNoise = gasdev( &seed );
			FFTBuff[i] = FFTBuff[i] + theNoise * noiseLevel;
		}
		
		exch_quad( FFTBuff, x, y );
		
		error = cfft2d( FFTBuff, x, y, REVERSE );
		RETURNONERROR;
		
		error = vmag( FFTBuff, 2, FFTBuff, 1, imSize );
		RETURNONERROR;
		
		for( i=0; i<imSize; i++ ) {
			noiseImage[i + inputOffset] = FFTBuff[i];
		}
	}

	memFree( (void **)&FFTBuff, "FFTBuff" );
/* FFT */

	return error;
}

OSErr SynthSeries( void )
{
	OSErr   error = noErr;
	float   *refImage;         // image volume used for anatomical basis
	float   *noiseImage;       // image with noise added
	float   *funcImage;
	float   paraVal;
	void    *outData;
	long    VolSize, outVolSize, inVolSize;
	float   outMax, outMin;
	short   rules=0;           // used, and ignored, in type conversion
	short   ValsRead;
	char    ConversionString[256];  // text message on file conversion

	VolSize    = u.inIm.dim.isoX * u.inIm.dim.isoY * u.inIm.dim.n_slices;
	inVolSize  = VolSize * get_datasize( T_FLOAT );
	outVolSize = VolSize * get_datasize( u.outIm.data_type );

	refImage   = (float *)ck_malloc( inVolSize,  "Storage for reference image" );
	funcImage  = (float *)ck_malloc( inVolSize,  "Storage for functional map" );
	noiseImage = (float *)ck_malloc( inVolSize,  "Storage for synthetic image" );
	outData    = ck_malloc( outVolSize, "Storage for output data" );

	outMax = u.inIm.theMaxf;
	outMin = u.inIm.theMinf;

// Force the data type of the inData to float
	u.inIm.data_type = T_FLOAT;
	error = GetSelectedVolume( u.inFile, &u.inIm, refImage, 0 );	// reads only the first volume!
	RETURNONERROR;

	u.funcIm.data_type = T_FLOAT;
	error = GetSelectedVolume( u.funcFile, &u.funcIm, funcImage, u.funcImageNum );
	RETURNONERROR;

	while( !feof( u.paraFile )) {
		ValsRead = fscanf( u.paraFile, "%f", &paraVal );
		if( ValsRead != 1 ) {
			break;
		}
		memcpy( noiseImage, refImage, inVolSize);

		error = AddModulation( paraVal, funcImage, u.ModulationDepth, noiseImage, VolSize );
		RETURNONERROR;
		
		error = AddRayleighNoise( noiseImage, u.NoiseLevel, &u.inIm, (long)clock() );
		RETURNONERROR;

/* convert the contents of outdata to the transformed version of indata */
		type_convert( (void *)noiseImage, T_FLOAT, (void *)outData, u.outIm.data_type,
		                      VolSize, &u.outIm.theMaxf, &u.outIm.theMinf, &rules );		
		printf( "Image %d\n", u.NumOutImages+1 );
		if( u.Verbose ) {
			if( InterpretConversionResult( ConversionString, rules ) ) {
				printf( ConversionString );
			}
		}

		u.outIm.theMaxf = u.outIm.theMaxf > outMax ? u.outIm.theMaxf : outMax;
		u.outIm.theMinf = u.outIm.theMinf < outMax ? u.outIm.theMinf : outMin;
		
		error = ck_fwrite( outData, 1, outVolSize, u.outFile );
		u.NumOutImages++;
	}

	error = ck_fclose( u.paraFile );

	memFree( (void **)&outData, "outData" );
	memFree( (void **)&noiseImage, "outData" );
	memFree( (void **)&funcImage, "outData" );
	memFree( (void **)&refImage, "inData" );
	
	fprintf( u.ProcFile,"TIME POINTS:       %d\n", u.NumOutImages );
	fprintf( u.ProcFile,"NOISE LEVEL:       %f\n", u.NoiseLevel );
	fprintf( u.ProcFile,"MODULATION DEPTH:  %f\n", u.ModulationDepth );

	return error;
}

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
	printf( "USAGE: %s -i infile.ext -o outfile.ext -f functionFile.ext\n-p paradigmFile [-N | -M | -F]\n\n", name );
	printf( "Using infile as an anatomical reference, create a time series of images\n" );
	printf( "whose pixel intensity is modulated according to the contents of the paradigm file\n" );
	printf( "with to a spatial distribution specified by the functionFile\n" );
	printf( "with depth specified by -M (multiples of the function file and paradigm file)\n" );
	printf( "and with Rayleigh-distributed noise whose magnitude\n" );
	printf( "(in pixel intensity units) is controlled by -N\n\n" );
	printf( "More information can be found on the Brain Mapping lab HowTo pages\n" );
}

/******************************    ProcessCommandLine     ***************************
*   capture the user input, mostly globals
*************************************************************************************/
OSErr ProcessCommandLine( int argc, char *argv[] )
{
	OSErr error = noErr;
	int argp = EOF;
	short argsProcessed = 0;

	u.Verbose = false;

	while ( (argp=getopt(argc,argv,"i:o:f:p:N:M:#:FVh?OPTIONS")) != EOF )
	switch( argp )   {
		case 'o':
			strcpy( u.OutFileName, optarg );
		break;

		case 'i':
			strcpy( u.InFileName, optarg );
		break;

		case 'f':
			strcpy( u.FunctionFileName, optarg );
		break;

		case 'p':
			strcpy( u.ParadigmFileName, optarg );
		break;

		case 'N':
			u.NoiseLevel = atof( optarg );
		break;
		
		case 'M':
			u.ModulationDepth = atof( optarg );
		break;

		case '#':
			u.funcImageNum = atoi( optarg ) - 1;
		break;
		
		case 'F':
			u.ForceFloat = true;
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
	if( !strlen( u.InFileName )
	 || !strlen( u.OutFileName )
	 || !strlen( u.FunctionFileName )
	 || !strlen( u.ParadigmFileName ) ) {
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

	error = UC_Readheader( u.FunctionFileName, &u.funcIm );
	RETURNONERROR;
	
	error = CheckConsistency( &u.funcIm, &u.inIm );
	if( error ) {
		printf( "The data sizes of the functional image and base image are inconsistent\n" );
		printf( "Aborting\n" );
		return noErr; // we already posted it!
	}

	memcpy( &u.outIm, &u.inIm, sizeof( IMAGE ));	// copy the input header to the output header, or?

	error = OutBaseNameFromOutName( u.OutFileName, u.OutBaseName );
	RETURNONERROR;

	error = FindOutputType( u.OutFileName, &u.outIm.file_type, &u.outIm.data_type );
	RETURNONERROR;
	
	if( u.outIm.file_type == ANALYZE) {
		if( u.ForceFloat ) {
			u.outIm.data_type = T_FLOAT;
		} else {
			u.outIm.data_type = T_SHORT;
		}
	}

	error = ck_fopen( &u.inFile, u.InFileName,  "rb" );
	RETURNONERROR;

	error = ck_fopen( &u.outFile, u.OutFileName,  "wb" );
	RETURNONERROR;

	error = ck_fopen( &u.funcFile, u.FunctionFileName,  "rb" );
	RETURNONERROR;

	error = ck_fopen( &u.paraFile, u.ParadigmFileName,  "r" );
	RETURNONERROR;
	
/* do it */
	error = SynthSeries();
	RETURNONERROR;

	error = ck_fclose( u.funcFile );
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

	ck_fclose( u.ProcFile );

	return error;
}

/******************************************/

int main( int argc, char **argv)
{
	OSErr   error = noErr;
	static  char id[] = "$Revision: 1.7 $$Date: 2006/08/01 16:26:56 $";

#ifdef MAC
	SIOUXSettings.autocloseonquit = false;
	SIOUXSettings.asktosaveonclose = false;
	argc = ccommand(&argv);
#endif

	error = RunMain( argc, argv, id );
	ILError( error, "Running main program" );

}

/* </pre></body></html> */

