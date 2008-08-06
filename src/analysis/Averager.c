/* <html><head><title>Averager source</title></head><body> <h1>Averager.c</h1>
<i>Please note last saved date:</i>
 <b><font size=+1><b>$Date: 2003/01/11 06:21:11 $</font></b><p>
<center>
<table bgcolor="black" cellpadding=5 border="1">
	<tr><td><font color="white">
Averages selected images to create a synthetic time series. Especially useful for repeated event-related trial designs.
	</font></td></tr></table><p>
	&copy;2002-today  <a href="mailto:mscohen@ucla.edu">Mark S. Cohen, Ph.D.</a><br>
	<a href="http://www.brainmapping.org">UCLA Brain Mapping Division</a><br>
</center>
<h2>How to make Averager</h2>
<ol>
	<li>Build ImgLib.o analyzeUtil.o ImgLibError.o VLib.o MathLib.o (from
	<a href="http://airto.loni.ucla.edu/BMCweb/SharedCode/SharedSoftware.html#Anchor-UCLA-35829">
www.brainmapping.org</a>)
	<li>compile Averager, linking the objects above. You will need to have C++ extensions and the Math Library.
	A typical command might be:<br><font face=courier><p>
	<center>
		cc ImgLib.o analyzeUtil.o ImgLibError.o VLib.o MathLib.o Averager.c -o Averager -lm</font>
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
<a href="http://airto.loni.ucla.edu/BMCweb/HowTo/Averager.html">
Brain Mapping Center support pages</a>.
<p>
Many of these programs have Macintosh executables available at: <a href="ftp://ftp.brainmapping.org/">ftp.brainmapping.org</a></tr>
</td></tr></table></center>
<pre>
<!--
*  $Log: Averager.c,v $
*  Revision 1.12  2003/01/11 06:21:11  mscohen
*  Upgraded to latest UtilityTemplate
*  Fixed bug in -R option
*
*  Revision 1.10  2002/09/10 21:57:14  mscohen
*  modified call to OpenProcFile
*
*  Revision 1.9  2002/09/09 21:18:28  nina
*  changed "r"/"w" to "rb"/"wb" in fopen call, where appropriate
*
*  Revision 1.8  2002/04/16 19:44:43  mscohen
*  modified html (again)
*
 * Revision 1.7  2002/04/16  19:06:56  mscohen
 * modified html
 *
 * Revision 1.6  2002/04/12  23:20:15  mscohen
 * More testing. Seems stable. Fix minor labeling issues. Complete MakeSampleImages function.
 *
 * Revision 1.5  2002/04/12  06:13:14  mscohen
 * Small bug fixes. Still not fully tested.
 *
 * Revision 1.4  2002/04/12  05:54:04  mscohen
 * Fixed many errors in counting
 * Added normalization to percent
 *
 * Revision 1.3  2002/04/10  23:23:53  mscohen
 * Fixed counting error in denominator for global average
 *
 * Revision 1.2  2002/03/28  11:49:16  mscohen
 * Added facility to average image range
 * fixed bug in data range for output images
 * captured potential div by zero error
 *
 * Revision 1.1  2002/01/04  17:03:09  mscohen
 * Initial revision
-->
*/

/* This program accepts input from a file containing a white space delimited list of
image time points and averages these together */

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
	char    avgFileName[256];
	short   output_datatype;
	short   NumOutImages;
	IMAGE   inIm;
	IMAGE   outIm;
	Boolean ShowPercent;	// express output files as percent
	Boolean ForceInteger;	// Analyze output files as integer
	Boolean Verbose;
	FILE    *inFile;
	FILE    *outFile;
	FILE    *avgFile;	/* file containing list of images to be averaged */
	FILE    *ProcFile;
} UtilGlobal;

UtilGlobal u;

/* PROTOTYPES */
OSErr InitializeUserVariables( void );
OSErr FormAverage( FILE *inFile );
OSErr FindFirstAndLastTimePts( FILE *avgFile, short *FirstTimePt,  short *LastTimePt );
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

	u.ShowPercent = true;
	u.ForceInteger = false;
	u.NumOutImages = 0;
	
	return error;
}

OSErr FindFirstAndLastTimePts( FILE *avgFile, short *FirstTimePt,  short *LastTimePt )
{
// input file is assumed to be opened
	OSErr error = noErr;
	short theTimePt;
	int   ret;
	
	ret = fscanf( avgFile, "%hd", &theTimePt );
	if( ret != 1 ) {
		return READ_ERROR;
	}

	*FirstTimePt = *LastTimePt = theTimePt;  

	while( ret==1 ) {
		*FirstTimePt = min( *FirstTimePt, theTimePt );
		*LastTimePt  = max( *LastTimePt,  theTimePt );
		ret = fscanf( avgFile, "%hd", &theTimePt );
	}

	rewind( avgFile );

	if( *FirstTimePt <= 0 || *LastTimePt > u.inIm.dim.timePts ) {
		return DATA_OUT_OF_RANGE;
	} else {
		return noErr;
	}
}

/******************************    FormAverage     ***************************
*   This is where the work will be done
*************************************************************************************/
OSErr FormAverage( FILE *inFile)
{
	OSErr   error = noErr;
	short   theTimePt;
	short   LastTimePt = 0;
	short   FirstTimePt = SHRT_MAX;
	float   MaxPixel = 0.0;
	float   MinPixel = 0.0;	/* Data range in output file */
	void    *inData, *avgData, *globalAvg;
	long    VolSize, avVolSize, inVolSize;
	int     numToAverage = 0;	/* number of images averaged at this time point */
	float   Normalizer; /* 1/numToAverage  */
	char    c;
	int     ret;

	VolSize = u.inIm.dim.isoX * u.inIm.dim.isoY * u.inIm.dim.n_slices;
	inVolSize = VolSize * get_datasize( u.outIm.data_type );
	avVolSize = VolSize * get_datasize( u.outIm.data_type );

	inData    = ck_malloc( inVolSize,  "Storage for input data" );
	avgData   = ck_calloc( avVolSize, get_datasize( u.outIm.data_type ), "Storage for output data" );
	globalAvg = ck_calloc( avVolSize, get_datasize( u.outIm.data_type ), "Storage for global average" );
	u.inIm.data_type = u.outIm.data_type;	/* read the data in the desired output format */

	error = ck_fopen( &u.avgFile, u.avgFileName, "rb" );
	RETURNONERROR;

	error = FindFirstAndLastTimePts( u.avgFile, &FirstTimePt, &LastTimePt );
	if( error ) return error;

/* Form global average from the first to the last time point */
	vclr( globalAvg, VolSize, u.outIm.data_type );
	numToAverage = 0;

	for( theTimePt=FirstTimePt; theTimePt<LastTimePt+1; theTimePt++ ) {
		error = GetSelectedVolume( inFile, &u.inIm, inData, theTimePt-1 );
		RETURNONERROR;

		MaxPixel += u.inIm.theMaxf;
		MinPixel += u.inIm.theMinf;

/* accumulate the images to be averaged */
		error = vadd( inData, 1, globalAvg, 1, globalAvg, 1, VolSize, u.outIm.data_type );
		RETURNONERROR;

		numToAverage++;
	}

	if( numToAverage == 0 ) {
		return DIVIDE_BY_ZERO_ERR;
	}

// normalize the global average
	Normalizer = 1.0/numToAverage;
	error = vfsmul( globalAvg, 1, globalAvg, 1, Normalizer, VolSize, u.outIm.data_type );	
	RETURNONERROR;

	u.outIm.theMaxf = MaxPixel * Normalizer;
	u.outIm.theMinf = MinPixel * Normalizer;

// form the average of all time points in each line
	error = vclr( avgData, VolSize, u.outIm.data_type );
	if( error ) return error;

	numToAverage = 0;

	do {
		ret = fscanf( u.avgFile, "%hd", &theTimePt );
		if( ret != 1 ) break;

		if( theTimePt > u.inIm.dim.timePts ) {
			return BEYOND_EOF;
		}
		
		printf( "%hd ", theTimePt );
		fprintf( u.ProcFile, "%hd ", theTimePt );
		fflush( stdout );

		error = GetSelectedVolume( inFile, &u.inIm, inData, theTimePt-1 );
		RETURNONERROR;

/* accumulate the images to be averaged */
		error = vadd( inData, 1, avgData, 1, avgData, 1, VolSize, u.outIm.data_type );
		RETURNONERROR;

		numToAverage++;

// skip white space
		c = fgetc( u.avgFile );
		while( c == ' ' || c == '\t' ) {
			c = fgetc( u.avgFile );
		}		
		if( c == '\n' || c == EOF || c == '\r' ) {	/* line is finished */
/* divide by the number of time points to be averaged */
			if( numToAverage == 0 ) {
				return DIVIDE_BY_ZERO_ERR;
			}

			Normalizer = 1.0/numToAverage;
			error = vfsmul( avgData, 1, avgData, 1, Normalizer, VolSize, u.outIm.data_type );
			RETURNONERROR;

			if( u.ShowPercent ) {
				error = ConvertToPercent( avgData, globalAvg, avgData, VolSize, u.outIm.data_type );
				if( error) return error;
			}

			error = ck_fwrite( avgData, 1, avVolSize, u.outFile );
			u.NumOutImages++;
			RETURNONERROR;

			printf( "--> averaged into output image %d\n", u.NumOutImages );
			fprintf( u.ProcFile, "--> averaged into output image %d\n",
                     u.NumOutImages );

			numToAverage = 0;
			vclr( avgData, VolSize, u.outIm.data_type );
		} else {
			ungetc( c, u.avgFile );
		}
	} while( !feof( u.avgFile ));

// if ShowPercent is not set, write out the global average image, as well
	if( !u.ShowPercent ) {	// only save average if not normalizing.
		error = ck_fwrite( globalAvg, 1, avVolSize, u.outFile );
		RETURNONERROR;
		u.NumOutImages++;
	
// tell the user what we did
		printf( "Images %hd through %hd averaged into output image %hd.\n",
			FirstTimePt, LastTimePt, u.NumOutImages );
		fprintf( u.ProcFile, "Image %hd is the average of images %hd through %hd.\n",
			u.NumOutImages, FirstTimePt, LastTimePt );
	}

	error = ck_fclose( u.avgFile );
	RETURNONERROR;
	
	free( globalAvg );
	free( avgData );
	free( inData );

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
	printf( "USAGE: %s -i infile.ext1 -o outfile.ext2 -a ControlFile [-R | -I]\n\n", name );
	printf( "Each line of the ControlFile should contain the time point numbers\n" );
	printf( "of images to be averaged together\n\n" );
	printf( "For example, if the control file reads:\n" );
	printf( "\t1 6 11 16\n\t2 7 12 17\n\t3 8 13 18\n\t4 9 14 19\n\t5 10 15 20\n\n" );
	printf( "the output file  will contain 5 images. The first will be the average of\n" );
	printf( "time points 1, 6, 11 and 16, the second will be the average of time points\n" );
	printf( "2, 7, 12 and 17, etc...\n\n" );
	printf( "If -R is used, the final output image will be the average of the images\n" );
	printf( "in the range from the first to the last image in the ControlFile.\n" );
	printf( "Otherwise, the output images will be expressed as a percent change from\n" );
	printf( "the mean of the first through last images listed in the control file\n\n" );
	printf( "-I Force output data type to integer, if using Analyze (.img) as output\n\n" );
	printf( "More information might be found on the Brain Mapping lab HowTo pages\n" );
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

	while ( (argp=getopt(argc,argv,"i:o:a:RIVh?")) != EOF )
	switch( argp )   {
		case 'o':
			strcpy( u.OutFileName, optarg );
		break;

		case 'i':
			strcpy( u.InFileName, optarg );
		break;

		case 'a':
			strcpy( u.avgFileName, optarg );
		break;
		
		case 'R':
			u.ShowPercent = false;
		break;

		case 'I':
			u.ForceInteger = true;
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
	if( !strlen( u.InFileName ) || !strlen( u.OutFileName )
	 || !strlen( u.avgFileName ) ) {
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

	memcpy( &u.outIm, &u.inIm, sizeof( IMAGE ));	// copy the input header to the output header, or?

	error = OutBaseNameFromOutName( u.OutFileName, u.OutBaseName );
	RETURNONERROR;

	error = FindOutputType( u.OutFileName, &u.outIm.file_type, &u.outIm.data_type );
	RETURNONERROR;

	if( u.outIm.file_type == ANALYZE ) {
		if( u.ForceInteger ) {
			u.outIm.data_type = T_USHORT;
		} else {
			u.outIm.data_type = T_FLOAT;
		}
	}

	error = ck_fopen( &u.inFile, u.InFileName,  "rb" );
	RETURNONERROR;

	error = ck_fopen( &u.outFile, u.OutFileName,  "wb" );
	RETURNONERROR;
	
/* do something */
	error = FormAverage( u.inFile );
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
	fprintf( u.ProcFile,"TIME POINTS:       %d\n", u.outIm.dim.timePts );
	fprintf( u.ProcFile,"NUM SLICES:        %d\n", u.outIm.dim.n_slices );

	ck_fclose( u.ProcFile );

	return error;
}

/******************************************/

int main( int argc, char **argv)
{
	OSErr   error = noErr;
	static  char id[] = "$Revision: 1.12 $$Date: 2003/01/11 06:21:11 $";

#ifdef MAC
	SIOUXSettings.autocloseonquit = false;
	SIOUXSettings.asktosaveonclose = false;
	argc = ccommand(&argv);
#endif

	error = RunMain( argc, argv, id );
	ILError( error, "Running main program" );

}

/* </pre></body></html> */

