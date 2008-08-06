/* <html><head><title>variance source</title></head><body> <h1>variance.c</h1>
<i>Please note last saved date:</i>
 <b><font size=+1><b>$Date: 2002/12/17 21:17:56 $</font></b><p>
<center>
<table bgcolor="black" cellpadding=5 border="1">
	<tr><td><font color="white">
SYNOPSIS_OF_PROGRAM
	</font></td></tr></table><p>
	&copy;2002-today  <a href="mailto:mscohen@ucla.edu">Mark S. Cohen, Ph.D.</a><br>
	<a href="http://www.brainmapping.org">UCLA Brain Mapping Division</a><br>
</center>
<h2>How to make variance</h2>
<ol>
	<li>Build ImgLib.o analyzeUtil.o ImgLibError.o VLib.o (from
	<a href="http://airto.loni.ucla.edu/BMCweb/SharedCode/SharedSoftware.html#Anchor-UCLA-35829">
www.brainmapping.org</a>)
	<li>compile variance, linking the objects above. You will need to have C++ extensions and the Math Library.
	A typical command might be:<br><font face=courier><p>
	<center>
		cc ImgLib.o analyzeUtil.o ImgLibError.o VLib.o MathLib.o variance.c -o variance -lm</font>
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
<a href="http://porkpie.loni.ucla.edu/HowTo/variance.html">
Brain Mapping Center support pages</a>.
<p>
Many of these programs have Macintosh executables available at: <a href="ftp://ftp.brainmapping.org/">ftp.brainmapping.org</a></tr>
</td></tr></table></center>
<pre>
<!--
$Author: mscohen $
$Log: variance.c,v $
Revision 1.4  2002/12/17 21:17:56  mscohen
redirected web addresses

Revision 1.3  2002/12/17 21:10:53  mscohen
Complete re-write

 * /
 *
Revision 1.1  2002/01/04 19:58:43  mscohen
Initial revision
-->
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
	short   output_datatype;
	short   NumOutImages;
	IMAGE   inIm;
	IMAGE   outIm;
	Boolean Verbose;
	FILE    *inFile;
	FILE    *outFile;
	FILE    *ProcFile;
} UtilGlobal;

UtilGlobal u;

/* PROTOTYPES */
OSErr Findvariance( FILE *inFile, FILE *outFile );
OSErr AdjustOutputIMAGE( IMAGE *outIm );
void  print_usage( char *name );
OSErr ProcessCommandLine( int argc, char *argv[] );
OSErr OutBaseNameFromOutName( char *OutFileName, char *OutBaseName );
OSErr RunMain( int argc, char **argv, char *id );

/******************************    Findvariance     ***************************
*   This is where the work will be done
*************************************************************************************/
OSErr Findvariance( FILE *inFile, FILE *outFile )
{
	OSErr   error = noErr;
	long    theTimePt;
	void    *inData, *outData;
	long    VolSize, outVolSize, inVolSize;
	IMAGE   hdrIm;
	void    *Sx2, *Sx;
	float   denominator, fmax, fmin;
	short   rules;

// Force input data type to float
	u.inIm.data_type = T_FLOAT;

	VolSize    = u.inIm.dim.isoX * u.inIm.dim.isoY * u.inIm.dim.n_slices;
	inVolSize  = VolSize * get_datasize( u.inIm.data_type );
	outVolSize = VolSize * get_datasize( u.outIm.data_type );
	inData  = ck_malloc( inVolSize,  "Storage for input data" );
	outData = ck_malloc( outVolSize, "Storage for output data" );
	Sx      = ck_calloc( VolSize, sizeof( float ),  "Sum of x points" );
	Sx2     = ck_calloc( VolSize, sizeof( float ),  "Sum of squared x points" );
	
	for( theTimePt = 0; theTimePt < u.inIm.dim.timePts; theTimePt++ ) {

		error = GetSelectedVolume( inFile, &u.inIm, inData, theTimePt );
		RETURNONERROR;

// update summed x values
		error = vadd( inData, 1, Sx, 1, Sx, 1, VolSize, u.inIm.data_type );
		RETURNONERROR;
		
		error = vsq( inData, 1, VolSize, u.inIm.data_type );
		RETURNONERROR;

// update summed x2 values
		error = vadd( inData, 1, Sx2, 1, Sx2, 1, VolSize, u.inIm.data_type );
		RETURNONERROR;
				
		if( u.Verbose ) {
			printf( "\tTime point %d of %d\n", theTimePt+1, u.outIm.dim.timePts );
		}
	}
	
// Create and write the average:
	denominator = 1.0/theTimePt;
	
	error = vsmul( Sx, 1, Sx, 1, &denominator, VolSize, u.inIm.data_type );
	RETURNONERROR;
	
	error = type_convert( Sx, u.inIm.data_type, outData, u.outIm.data_type,
	                      VolSize, &fmax, &fmin, &rules );
	RETURNONERROR;

	error = ck_fwrite( outData, 1, outVolSize, outFile );
	RETURNONERROR;
	u.NumOutImages++;
	
// calculate and write the variance	
	error = vsq( Sx, 1, VolSize, u.inIm.data_type );
	RETURNONERROR;

	error = vsmul( Sx2, 1, Sx2, 1, &denominator, VolSize, u.inIm.data_type );
	RETURNONERROR;
	
	error = vsub( Sx2, 1, Sx, 1, Sx2, 1, VolSize, u.inIm.data_type );
	RETURNONERROR;
	
	error = type_convert( Sx2, u.inIm.data_type, outData, u.outIm.data_type,
	                      VolSize, &fmax, &fmin, &rules );
	RETURNONERROR;

	error = ck_fwrite( outData, 1, outVolSize, outFile );
	RETURNONERROR;
	u.NumOutImages++;

// calculate and write the standard deviation	
	error = vsqrt( Sx2, 1, VolSize, u.inIm.data_type );
	RETURNONERROR;
	
	error = type_convert( Sx2, u.inIm.data_type, outData, u.outIm.data_type,
	                      VolSize, &fmax, &fmin, &rules );
	RETURNONERROR;

	error = ck_fwrite( outData, 1, outVolSize, outFile );
	u.NumOutImages++;

	return error;
}

OSErr AdjustOutputIMAGE( IMAGE *outIm )
{
	outIm->dim.timePts = u.NumOutImages;
	return noErr;
}

/* functions */
void print_usage( char *name )
{
	printf( "USAGE: %s -i infile.ext1 -o outfile.ext2 [V|h|?]\n\n", name );
	printf( "\tCalculates the MEAN, variance and STANDARD DEVIATION of each\n" );
	printf( "\tpixel location in the time series.\n\n" );
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

	while ( (argp=getopt(argc,argv,"i:o:Vh?OPTIONS")) != EOF )
	switch( argp )   {
		case 'o':
			strcpy( u.OutFileName, optarg );
		break;

		case 'i':
			strcpy( u.InFileName, optarg );
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

	error = ProcessCommandLine( argc, argv );
	RETURNONERROR;

	error = OpenProcFile( argc, argv, u.OutFileName, id, &u.ProcFile );
	RETURNONERROR;

/* read in the input image header */
	error = UC_Readheader( u.InFileName, &u.inIm );
	RETURNONERROR;

	memcpy( &u.outIm, &u.inIm, sizeof( IMAGE ));	// copy the input header to the output
	u.NumOutImages = 0;

	error = OutBaseNameFromOutName( u.OutFileName, u.OutBaseName );
	RETURNONERROR;

	error = FindOutputType( u.OutFileName, &u.outIm.file_type, &u.outIm.data_type );
	RETURNONERROR;

	error = ck_fopen( &u.inFile, u.InFileName,  "rb" );
	RETURNONERROR;

	error = ck_fopen( &u.outFile, u.OutFileName,  "wb" );
	RETURNONERROR;
	
/* do something */
	error = Findvariance( u.inFile, u.outFile );
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
	fprintf( u.ProcFile,"Output image 1:    Mean\n" );
	fprintf( u.ProcFile,"Output image 1:    Variance\n" );
	fprintf( u.ProcFile,"Output image 1:    Standard deviation\n" );

	ck_fclose( u.ProcFile );

	return error;
}

/******************************************/

int main( int argc, char **argv)
{
	OSErr   error = noErr;
	static  char id[] = "$Revision: 1.4 $$Date: 2002/12/17 21:17:56 $";

#ifdef MAC
	SIOUXSettings.autocloseonquit = false;
	SIOUXSettings.asktosaveonclose = false;
	argc = ccommand(&argv);
#endif

	error = RunMain( argc, argv, id );
	ILError( error, "Running main program" );

}

/* </pre></body></html> */

