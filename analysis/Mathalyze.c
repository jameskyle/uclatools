/*****************************************************************************
		Mathalyze

	A simple math processor for images as a general utility.

	c2000 - 2003 Mark S. Cohen, Ph.D.
	UCLA School of Medicine
---------------------------------
$Log: Mathalyze.c,v $
Revision 1.13  2008/05/16 19:07:31  mscohen
Added gaussian smoothing function.

Revision 1.12  2008//03/21 20:50:20  mscohen
(Hopefully) repaired errors in revision tree.

Revision 1.9  2006/03/26 06:23:17  mscohen
*** empty log message ***

Revision 1.8  2006/03/26 06:02:55  mscohen
Completed transition to handling with complex
Added upgrade path to save as real-image or mag-phase image pairs

Revision 1.8  2006/03/25 08:42:54  mscohen
All data are now handled as complex. The allows read and write of complex FT data

Revision 1.7  2006/03/24 23:21:26  mscohen
added Rayleigh function
fixed numerous errors in FFT
fixed problem in handling of MGH files that could have caused file corruptions

Revision 1.7  2006/02/21 19:38:24  mscohen
Added log and thresholding functions.

Revision 1.6  2003/03/30 00:51:04  mscohen
revision control

Revision 1.5  2003/03/30 00:48:17  mscohen
Modified to include Maximum absolute value and to allow output of Analyze image types. Upgraded source code for clarity and stability.

Revision 1.3  2002/09/09 23:04:57  nina
changed fopen mode from "r"/"w" to "rb"/"wb" where applicable

Revision 1.2  2000/05/05 21:51:52  mscohen
Fixed proc file problems

 * Revision 1.1  2000/04/24  16:16:39  mscohen
 * Initial revision
 *
*****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <math.h>
#include "ImgLib.h"
#include "MathLib.h"
#include "getopt.h"

OSErr  ProcessCommandLine( int argc, char *argv[] );
OSErr  CompareFileDimensions( IMAGE *AIm, IMAGE *BIm );
OSErr  OutBaseNameFromOutName( char *OutFileName, char *OutBaseName );
OSErr  SelectOperation( char *FirstName, char *SecondName, int *Op );
OSErr  DivideImages( float *num, float *den, float *out, long npts );
OSErr  FFT( float *in, float *out, IMAGE *im, int Direction );
OSErr  DoInverse( float *in, float *out, long volSize );
OSErr  ln( float *in, float *out, long volSize );
OSErr  MaxIP( float *Adata, float *Bdata, float *out, long volSize );
OSErr  MinIP( float *Adata, float *Bdata, float *out, long volSize );
OSErr  MaxAbs( float *Adata, float *Bdata, float *out, long volSize );
OSErr  Rayleigh( float *noiseImage, float percentNoise, IMAGE *im, long seed );
void   PrintUsage( char *name );
OSErr  Mathalyze(int argc, char **argv);

// GLOBALS
enum
{
	ADD = 1,
	SUBTRACT,
	MULTIPLY,
	DIVIDE,
	kFFT,
	SQUAREROOT,
	SQUARE,
	SCALAR,
	INVERSE,
	MAXIP,
	MINIP,
	MAXABS,
	LOG,
	GSMOOTH,	// Gaussian (k) smoothing
	THRESH,
	NOISE
};
	
	static char id[] = "$Revision: 1.13 $$Date: 2008/05/16 19:07:31 $";
	char	*gOutFileName = NULL;
	char	*gFileAName   = NULL;
	char	*gFileBName   = NULL;
	Boolean gVerbose      = false;
	FILE	*gProcFile    = NULL;
	FILE    *MessageLog;
	short   gNImages;
	short   gXS, gYS, gZS, gTimePts;
	float   gScalar;
	float   gThresh;
	float	gxsmooth = 0, gysmooth = 0, gzsmooth = 0;
	double  gSum;
	long    gImSize, gVolSize, gDataVolSize;
	float   gNoise = 0;
	int     Direction = FORWARD;

/************************    ProcessCommandLine     ***************************
*	capture the user input, mostly globals
*******************************************************************************/
OSErr ProcessCommandLine( int argc, char *argv[] )
{
	int argp = EOF;
	short argsProcessed = 0;
	
// capture user options
	while ( (argp=getopt(argc,argv,"o:A:B:Vh?")) != EOF ) {
		argsProcessed++;

		switch( argp )   {
			case 'o':
				gOutFileName = optarg;
			break;

			case 'A':
				gFileAName = optarg;
			break;

			case 'B':
				gFileBName = optarg;
			break;

			case 'V':
				gVerbose = true;
			break;

			case 'h':
			case '?':
			default:
			  PrintUsage( argv[0] );
			  exit( -1 );
		}
	}

	if( !gFileAName || !gOutFileName ) {
		PrintUsage( argv[0] );
	}

	if( gFileBName ) {
		gNImages = 2;
	} else {
		gNImages = 1;
	}

	return noErr;
}

OSErr  CompareFileDimensions( IMAGE *AIm, IMAGE *BIm )
{
	OSErr error = noErr;

	gXS      = AIm->dim.x;
	gYS      = AIm->dim.y;
	gZS      = AIm->dim.n_slices;
	
	if( gNImages > 1 ) {
		gTimePts = min( AIm->dim.timePts, BIm->dim.timePts);
/*
		printf( "gXS: %hd\tgYS: %hd\tgZS: %hd\n", gXS, gYS, gZS );
		printf( "bXS: %hd\tbYS: %hd\tbZS: %hd\n", BIm->dim.x, BIm->dim.y, BIm->dim.n_slices );
*/
		if( gXS != BIm->dim.x ||
		    gYS != BIm->dim.y ||
		    gZS != BIm->dim.n_slices ) {
			printf( "The images do not have the same dimensions.\n" );
			return SILENT_ERR;	
		}
		
		if( BIm->dim.timePts != AIm->dim.timePts ) {
			printf( "Operations will be performed on the first %d time points\n",
				BIm->dim.timePts < AIm->dim.timePts ? BIm->dim.timePts : AIm->dim.timePts );
		}
	} else {
		gTimePts = AIm->dim.timePts;
	}
	gImSize  = gXS * gYS;
	gVolSize = gImSize * gZS;
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

OSErr  SelectOperation( char *FirstName, char *SecondName, int *Op )
{
	OSErr error = noErr;
	char  response[2];
	
	printf( "\n\nSelect (by symbol) an operation to be performed:\n\n" );
	if( gNImages > 1 ) {
		printf( "--- Operations on two images ---\n" );
		printf( "\t            \tSYMBOL\tEXPLANATION\n" );
		printf( "\t(A+B)       \t+\tAdd images A and B\n" );
		printf( "\t(A-B)       \t-\tSubtract image A from B\n" );
		printf( "\t(AB)        \tx\tMultiply images A and B\n" );
		printf( "\t(A/B)       \t/\tDivide image A by image B\n" );
		printf( "\tmax(A,B)    \t>\tMaximum Intensity Projection\n" );
		printf( "\tmin(A,B)    \t<\tMinimum Intensity Projection\n" );
		printf( "\tmax(|A|,|B|)\tA\tPixels with maximum absolute value\n" );
	}
	printf( "\n--- Operations on single image (A) ---\n" );
	printf( "\tFFT(A)   \tF\tForward 2D Fast Fourier transform\n" );
	printf( "\trevFFT(A)\tf\tReverse 2D Fast Fourier transform\n" );
	printf( "\tsqrt(A)  \tr\tSquare root of intensity\n" );
	printf( "\tA^2      \ts\tIntensity squared\n" );
	printf( "\tmA       \tm\tScalar multiply\n" );
	printf( "\t1/A      \ti\tInverse\n" );
	printf( "\tln(A)    \tl\tNatural log\n" );
	printf( "\tsmooth(A)    \tg\tGaussian smoothing\n" );
	printf( "\tThresh(A)\tt\tThreshold\n" );
	printf( "\tNoise(A) \tN\tAdd Rayleigh noise to A\n" );
	
	printf( "Select: " );	
	scanf( "%1s", response );
	
	fprintf( gProcFile, "\nOperation performed is " );

	switch( (int)response[0] ) 
	{
		case '+': *Op = ADD;        fprintf( gProcFile, "A+B\n" ); break;
		case '-': *Op = SUBTRACT;   fprintf( gProcFile, "A-B\n" ); break;	
		case 'x': *Op = MULTIPLY;   fprintf( gProcFile, "AB\n" ); break;
		case '/': *Op = DIVIDE;     fprintf( gProcFile, "A/B" ); break;
		case 'F': *Op = kFFT;       fprintf( gProcFile, "FFT(A)\n" ); Direction = FORWARD; break;
		case 'f': *Op = kFFT;       fprintf( gProcFile, "FFT(A)\n" ); Direction = REVERSE; break;
		case 'r': *Op = SQUAREROOT; fprintf( gProcFile, "sqrt(A)\n" ); break;
		case 's': *Op = SQUARE;     fprintf( gProcFile, "A^2\n" ); break;
		case 'm':
			*Op = SCALAR;
			fprintf( gProcFile, "mA\n" );
			printf( "\tEnter a scalar multiplier: " );
			scanf( "%f", &gScalar );
		break;

		case 'i': *Op = INVERSE;    fprintf( gProcFile, "1/A\n" ); break;
		case 'l': *Op = LOG;        fprintf( gProcFile, "ln(A)\n" ); break;
		case 'g': *Op = GSMOOTH;    fprintf( gProcFile, "Gaussian smoothing (A)\n" ); break;
		case 't': *Op = THRESH;
			fprintf( gProcFile, "thresh(A)\n" );
			printf( "\tEnter a threshold: " );
			scanf( "%f", &gThresh );
			break;
		case '>': *Op = MAXIP;      fprintf( gProcFile, "max(A, B)\n" ); break;
		case '<': *Op = MINIP;      fprintf( gProcFile, "min(A, B)\n" ); break;
		case 'A': *Op = MAXABS;     fprintf( gProcFile, "max |(A,B)|\n" ); break;
		case 'N': *Op = NOISE;      fprintf( gProcFile, "Rayleigh noise\n" );
			printf( "Enter noise level (percent): " );
			scanf( "%f", &gNoise );
		break;

		default : printf( "\n--- Un-recognized option ---\n" );
		          error = SelectOperation( FirstName, SecondName, Op );
		          break;
	}
	return error;
}

OSErr DivideImages( float *num, float *den, float *out, long npts )
{
	float hicutoff;
	float noiseLevel;
	long  nonNoise;
	char  response[24];
	OSErr error = noErr;
	long  i;
	
	error = imageDataRange( num, npts, &hicutoff, &noiseLevel, &nonNoise, T_COMPLEX );
	if( error ) {
		ILError( error, "Image data range" );
	}
	printf( "Set noise threshold for numerator to %0.3f (y/n)? ", noiseLevel );
	scanf( "%23s", response );
	
	if( !strcmp(response, "n") || !strcmp(response, "N" )) {
		printf( "Set new noise threshold: " );
		scanf( "%23s", response );
		noiseLevel = atof( response );
	}
	for( i=0; i<npts; i++ ) {
		num[i] = num[i] > noiseLevel ? num[i] : 0.0;				
	}

	error = vdiv( num, 1, den, 1, out, 1, npts, T_COMPLEX );
	return error;
}

OSErr FFT( float *in, float *out, IMAGE *im, int Direction )
{
	OSErr   error = noErr;
	float   FTscale = 1.0/sqrt((float)gImSize);
	short   sliceCtr;
	long    sliceOffset;
	
	if( gVerbose ) {
		printf( " FFT\n" );
	}
	
// FFT is done on a per slice basis
	error = vsmul( in, 1, out, 1, &FTscale, gVolSize, T_COMPLEX );
	RETURNONERROR;
	
	for( sliceCtr=0; sliceCtr < im->dim.n_slices; sliceCtr++ ) {
		sliceOffset = gImSize * sliceCtr * 2;

		error = cfft2d( out+sliceOffset, im->dim.x, im->dim.y, Direction );
			RETURNONERROR;

		if( Direction == FORWARD ) {
			exch_quad( out+sliceOffset, im->dim.x, im->dim.y );
			}
		}
	return error;
}

OSErr  DoInverse( float *in, float *out, long volSize )
{
	OSErr error = noErr;
	long  i;
	long  nZeroes = 0l;

	for( i=0; i<volSize; i++ ) {
		if( in[i] !=0 ) {
			out[i] = 1/in[i];
		} else {
			out[i] = 0;
			nZeroes++;
		}
	}

	fprintf( gProcFile, "%ld points were zeroes.\n", nZeroes );
	return error;
}

OSErr  ln( float *in, float *out, long volSize )
{
	OSErr error = noErr;
	long i;
	long nZeroes = 0l;

	for( i=0; i<volSize; i++ ) {
		if( in[i] >0 ) {
			out[i] = log(in[i]);
		} else {
			out[i] = 0;
			nZeroes++;
		}
	}

	fprintf( gProcFile, "%ld points were zeroes.\n", nZeroes );
	return error;
}

OSErr  GSmooth( IMAGE *im, float *in, float *out, long volSize )
{
	OSErr	error = noErr;
	
	error = gaussSmooth( im, in, out, &gxsmooth, &gysmooth, &gzsmooth );
	RETURNONERROR;

	fprintf( gProcFile, "Gaussian smoothing with kernel of %0.3f X %0.3f X %0.3f mm in x, y and z\n", gxsmooth, gysmooth, gzsmooth );
	return error;
}

OSErr  Thresh( float *in, float *out, long volSize )
{
	OSErr error = noErr;
	long i;
	long nZeroes = 0l;

	for( i=0; i<volSize; i++ ) {
		if( in[i] > gThresh ) {
			out[i] = in[i];
		} else {
			out[i] = 0;
			nZeroes++;
		}
	}

	fprintf( gProcFile, "%ld points were zeroes.\n", nZeroes );
	return error;
}

OSErr  MaxIP( float *Adata, float *Bdata, float *out, long volSize )
{
	OSErr error = noErr;
	long  i;

	for( i=0; i<volSize; i++ ) {
		out[i] = Adata[i] > Bdata[i] ? Adata[i] : Bdata[i];
	}

	return error;
}

OSErr  MinIP( float *Adata, float *Bdata, float *out, long volSize )
{
	OSErr error = noErr;
	long  i;

	for( i=0; i<volSize; i++ ) {
		out[i] = Adata[i] < Bdata[i] ? Adata[i] : Bdata[i];
	}

	return error;
}

OSErr  MaxAbs( float *Adata, float *Bdata, float *out, long volSize )
// create an image containing the maximum absolute value of images A and B
{
	OSErr error = noErr;
	long  i;

	for( i=0; i<volSize; i++ ) {
		out[i] = fabs( Adata[i] ) > fabs( Bdata[i] ) ? Adata[i] : Bdata[i];
	}

	return error;
}

OSErr Rayleigh( float *noiseImage, float percentNoise, IMAGE *im, long seed )
{
	OSErr error = noErr;
	float invImSize;	/* 1/imSize */
	short theImage;
	float theNoise;
	long  i;
	long  inputOffset;
	float *currentImage;
	double ImageMean;
	float  noiseLevel;
	
// inputs are scaled first...
	invImSize = 1.0/gImSize;
	error = vfsmul( noiseImage, 1, noiseImage, 1, invImSize, gImSize * 2, T_COMPLEX );
	RETURNONERROR;

// do the FFT separately for each image
	for( theImage=0; theImage < im->dim.n_slices; theImage++ ) {
		inputOffset = gImSize * theImage * sizeof(float) * 2;  // in units of 'sizeof(float)'
		currentImage = (float *)(noiseImage + inputOffset);
		ImageMean = vsum( currentImage, 1, gImSize*2, T_COMPLEX ) / gImSize;
		noiseLevel = percentNoise * ImageMean / (100.0 * sqrt( invImSize ));

/* FFT effects */

		error = cfft2d( currentImage, im->dim.x, im->dim.y, FORWARD );
		RETURNONERROR;
		
// add noise in k-space
		for(i=0; i< gImSize * 2; i++ ) {
			theNoise = gasdev( &seed );
			currentImage[i] = currentImage[i] + theNoise * noiseLevel;
		}
		
		exch_quad( currentImage, im->dim.x, im->dim.y );
		
		error = cfft2d( currentImage, im->dim.x, im->dim.y, REVERSE );
		RETURNONERROR;
	}

	fprintf( gProcFile, "Added Rayleigh noise of magnitude: %f.\n", noiseLevel );

	return error;
}

void PrintUsage( char *name )
{
	printf( "\nUsage: %s -A Image1 -B Image2 -o OutputFileName\n", name );
	printf( "\tImage1 and Image2 may be any standard type.\n" );
	printf( "\tPerforms simple math operations on one or two images\n" );
	printf( "\tData are always returned as float\n" );
	exit( -1 );
}

OSErr Mathalyze(int argc, char **argv)
{
	OSErr    error = noErr;
	FILE     *FileA;
	FILE     *FileB;
	FILE     *OutFile;
	int      Operation;
	char     outFormat[2];
	float    *Adata=NULL, *Bdata=NULL, *OutData=NULL;
	IMAGE    AIm;
	IMAGE    BIm;
	IMAGE    OutIm;
	char     OutBaseName[64];
	long     timePt;
	void     *OutBuff;
	short    rules = 0;
	float    fmin, fmax;

	error = ProcessCommandLine( argc, argv );
		if( error ) return error;
	
	error = OpenProcFile( argc, argv, gOutFileName, id, &gProcFile);
	ILError( error, "Opening Proc file" );

// determine the file dimensions for the image or images
	error = UC_Readheader( gFileAName, &AIm );
	RETURNONERROR;

// The output image will have the same dimensions as input image A
	memcpy( &OutIm, &AIm, sizeof( IMAGE ));
	OutIm.dim.isoX = OutIm.dim.x;
	OutIm.dim.isoY = OutIm.dim.y;

	error = ck_fopen( &FileA, gFileAName, "rb" );
	RETURNONERROR;
	
	if( gNImages > 1 ) {
		error = UC_Readheader( gFileBName, &BIm );
		RETURNONERROR;

		error = ck_fopen( &FileB, gFileBName, "rb" );
		RETURNONERROR;		
	}

	error = CompareFileDimensions( &AIm, &BIm );
	RETURNONERROR;	

// Let the user choose the mathematical operation
	SelectOperation( gFileAName, gFileBName, &Operation );

// Allocate storage for input and output images
	Adata = (float *)ck_malloc( gVolSize * get_datasize( T_COMPLEX ), "Adata" );
	if( gNImages > 1 ) {
		Bdata = (float *)ck_malloc( gVolSize * get_datasize( T_COMPLEX ), "Bdata" );
	}

	OutData = (float *)ck_malloc( gVolSize * get_datasize( T_COMPLEX ), "OutData" );

// To prepare the output file, determine its name and type.

	error = OutBaseNameFromOutName( gOutFileName, OutBaseName );
	RETURNONERROR;

	error = FindOutputType( gOutFileName, &OutIm.file_type, &OutIm.data_type );
	RETURNONERROR;
	
	if( OutIm.file_type == ANALYZE ) {
		printf( "\nSave data as\n\tshort\ts\n\tfloat\tf\n\tcomplex\tc\nSelect:" );
		scanf( "%1s", outFormat );
		if( *outFormat=='c' ) {
			SetImDatatype( &OutIm, T_COMPLEX );
		} else if ( *outFormat=='s' ) {
			SetImDatatype( &OutIm, T_SHORT );
		} else {
			SetImDatatype( &OutIm, T_FLOAT );
		}
	}

/* This step ensures that the data are read in as complex */
	SetImDatatype( &AIm, T_COMPLEX );
	SetImDatatype( &BIm, T_COMPLEX );

	error = ck_fopen( &OutFile, gOutFileName,  "wb" );
	RETURNONERROR;

	OutBuff = ck_malloc( gVolSize * get_datasize( OutIm.data_type ), "Output buffer" );

	if( Operation == GSMOOTH ) {
		printf( "Enter desired smoothing FWHM (mm) in x, y and z, separated by white space: " );
		scanf( "%f%f%f", &gxsmooth, &gysmooth, &gzsmooth );
	}

	for( timePt = 0; timePt < gTimePts; timePt++ ) {
		error = GetSelectedVolume( FileA, &AIm, Adata, timePt );
			RETURNONERROR;

		if( gNImages > 1 ) {
			error = GetSelectedVolume( FileB, &BIm, Bdata, timePt );
				if( error ) return error;

		}
		switch( Operation ) {
			case ADD:
				error = vadd( Adata, 1, Bdata, 1, OutData, 1, gVolSize, T_COMPLEX );
			break;

			case SUBTRACT:
				error = vsub( Adata, 1, Bdata, 1, OutData, 1, gVolSize , T_COMPLEX );
			break;

			case MULTIPLY:
				error = vmul( Adata, 1, Bdata, 1, OutData, 1, gVolSize, T_COMPLEX );
			break;

			case DIVIDE:
				error = DivideImages( Adata, Bdata, OutData, gVolSize );
			break;

			case kFFT:
				error = FFT( Adata, OutData, &AIm, Direction );
			break;

			case SQUARE:
				error = vsq( Adata, 1, gVolSize, T_COMPLEX );
					if( error ) return error;
				error = vmov( Adata, 1, OutData, 1, gVolSize, T_COMPLEX );
			break;

			case SQUAREROOT:
				error = vsqrt( Adata, 1, gVolSize, T_COMPLEX );
					if( error ) return error;

				error = vmov( Adata, 1, OutData, 1, gVolSize, T_COMPLEX );
			break;

			case SCALAR:
				error = vsmul( Adata, 1, OutData, 1, &gScalar, gVolSize, T_COMPLEX );
			break;

			case INVERSE:
				error = DoInverse( Adata, OutData, gVolSize * 2 );
			break;

			case LOG:
				error = ln( Adata, OutData, gVolSize * 2 );
			break;

			case GSMOOTH:
				error = GSmooth( &AIm, Adata, OutData, gVolSize * 2 );
			break;

			case THRESH:
				error = Thresh( Adata, OutData, gVolSize * 2 );
			break;
			
			case MAXIP:
				error = MaxIP( Adata, Bdata, OutData, gVolSize * 2 );
			break;

			case MINIP:
				error = MinIP( Adata, Bdata, OutData, gVolSize * 2 );
			break;

			case MAXABS:
				error = MaxAbs( Adata, Bdata, OutData, gVolSize * 2 );
			break;
			
			case NOISE:
				error = vmov( Adata, 1, OutData, 1, gVolSize, T_COMPLEX );
				RETURNONERROR;
				error = Rayleigh( OutData, gNoise, &AIm, (long)clock() );
			break;
		}
		RETURNONERROR;

/* Future: save data in pairs
		if( *outFormat == 'p' ) {
			error = vmov( OutData, 2, OutBuff, 1, gVolSize, T_FLOAT );
			RETURNONERROR;
			offset =  gVolSize * sizeof( float );
			error = vmov( OutData + sizeof(float), 2, OutBuff + offset, 1, gVolSize, T_FLOAT );
			RETURNONERROR;
		} else if ( *outFormat == 1 ) {
			error = vmag( OutData, 2, OutBuff, 1, gVolSize );
			RETURNONERROR;
			error = vphase( OutData, 2, OutBuff + offset, 1, gVolSize );
			RETURNONERROR;
			error = vdegree( OutBuff + offset, 1, OutBuff + offset, 1, gVolSize );
			RETURNONERROR;
		}
*/
		type_convert( (void *)OutData, T_COMPLEX, (void *)OutBuff, OutIm.data_type, gVolSize, &fmax, &fmin, &rules );

		error = ck_fwrite( OutBuff, get_datasize( OutIm.data_type ), gVolSize, OutFile );
		RETURNONERROR;

		if( timePt > 0 ) {
			printf( "." );
			fflush( stdout );
		}
	}

	free( OutBuff );
	ck_fclose( OutFile );
	
	fprintf( gProcFile, "\nProcessed %hd time points and wrote %s as output\n\n", gTimePts, gOutFileName );
	printf( "\nWrote %s as output.\n", gOutFileName );
	
	if( macByteOrder() ) {
		SET( OutIm.dim.flags, kmacByteOrder );
	} else {
		CLEAR( OutIm.dim.flags, kmacByteOrder );
	}

	error = CreateHeader( OutIm.file_type, &OutIm, OutBaseName );
	RETURNONERROR;

	free( Adata );

	if( Bdata != NULL ) {
		free( Bdata );
	}
	free( OutData );

	ck_fclose( OutFile );
	
	return error;
}

int main(int argc, char **argv)
{
	OSErr error = noErr;
	printf( "%s\n\t%s\n", argv[0], id );    // tell 'em who we are

#ifdef mactest
	argc = ccommand(&argv);
	SetupSIOUX();
#endif

	error = Mathalyze( argc, argv );
	if( error ) {
		ILError( error, "Mathalyze" );
	}

	error = ck_fclose( gProcFile );
	ILError( error, "closing" );

#ifdef mactest
	printf( "Enter any key to quit" );
	scanf( "%3s", input );
#endif

	return 0;	
}
