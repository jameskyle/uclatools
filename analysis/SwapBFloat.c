/* $Author: mscohen $
*  $Date: 2002/09/10 22:06:02 $
*  $Log: SwapBFloat.c,v $
*  Revision 1.2  2002/09/10 22:06:02  mscohen
*  changed calls to ck_fopen, ck_fclose and ck_fwrite
*
*  Revision 1.1  2002/09/10 00:12:32  nina
*  Initial revision
*
*/

#include <string.h>
#include "getopt.h"
#include "VLib.h"

#define false 0
#define true 1
#ifndef noErr
#define noErr 0
#endif

#define NAME_SIZE 64

typedef struct {
	int	ys;
	int	xs;
	int	zs;
	int ByteOrder;
} imageDims;

/* globals */
typedef struct {
	char    OutFileName[255];
	char    InFileName[255];
	char    OutBaseName[256];
	short   output_datatype;
	imageDims	id;
	Boolean Verbose;
	FILE    *inFile;
	FILE    *outFile;
	FILE    *ProcFile;
} UtilGlobal;

UtilGlobal u;

/* PROTOTYPES */
OSErr DoSwap( FILE *inFile, FILE *outFile );

void  print_usage( char *name );
OSErr ProcessCommandLine( int argc, char *argv[] );

int main( int argc, char **argv)
{
/*  sample command lines
-i bdv.bshort -o junk.bshort -V -# 16
*/
	OSErr   error = noErr;
	static  char id[] = "$Revision: 1.2 $$Date: 2002/09/10 22:06:02 $";
	char    headerName[ NAME_SIZE ];
	char    *extension;
	char    ofName[ NAME_SIZE ];
	int     OutOrder;

	error = ProcessCommandLine( argc, argv );
	ILError( error, "error in command line" );

	extension = strrchr( u.InFileName, '.' );
	if( !strlen(extension) ) {
		printf( "Input file name error\n" );
		return 0;
	}
	if( !equalString( extension, ".bfloat" )) {
		printf( "This utility works only on bfloat images!\n" );
		return 0;
	}

//	strcpy( headerName, u.InFileName, NAME_SIZE );
	strcpy( headerName, u.InFileName ); //zrinka 09/10/02
	
	extension = strrchr( headerName, '.' );
	if( !strlen(extension) ) {
		printf( "Input file name error\n" );
		return 0;
	}
	sprintf( extension, ".hdr" );
	u.inFile = errfopen( headerName, "r" );
	
	fscanf( u.inFile, "%d%d%d%d", &u.id.ys, &u.id.xs, &u.id.zs, &u.id.ByteOrder );
//	error = ck_fclose( u.inFile, "input header" );
	error = ck_fclose( u.inFile ); //zrinka 09/10/02
	
	u.inFile  =  errfopen( u.InFileName,  "rb" );

	strcpy( ofName, u.OutFileName );
	strcat( ofName, ".bfloat" );
	u.outFile =  errfopen( ofName, "wb" );

	error = DoSwap( u.inFile, u.outFile );
	ILError( error, argv[0] );

	error = ck_fclose( u.inFile );
	ILError( error, "infile" );

	error = ck_fclose( u.outFile );
	ILError( error, "infile" );

	strcpy( ofName, u.OutFileName );
	strcat( ofName, ".hdr" );
	u.outFile = errfopen( ofName, "w" );
	if( u.id.ByteOrder == 1 ) {	// pc-dec bytOrder
		OutOrder = 0;
	} else {
		OutOrder = 1;
	}

	fprintf( u.outFile, "%d %d %d %d", u.id.ys, u.id.xs, u.id.zs, OutOrder );
	error = ck_fclose( u.outFile );
	ILError( error, "main" );
}

/* functions */
void print_usage( char *name )
{
	printf( "USAGE: %s -i infile.bfloat -o outfileBaseName\n\n", name );
	printf( "Changes the byte order of bfloat files. (Works only on bfloat)\n" );
	printf( "Make sure that the base name of the input and output files differ!\n" );
	printf( "More information can be found on the Brain Mapping lab HowTo pages\n" );
}

/******************************    DoSwap     ***************************
*   This is where the work will be done
*************************************************************************************/
OSErr DoSwap( FILE *inFile, FILE *outFile )
{
	OSErr   error = noErr;
	long    theTimePt;
	void    *inData, *outData;
	long    bufSize;
	int     elemSize;
	void    *buf;
	int     count;

	elemSize = sizeof( float );
	bufSize = u.id.xs * u.id.ys * elemSize;
	buf = ck_malloc( bufSize, "swap buffer" );

	for( count=0; count<u.id.zs; count++ ) {
		if( u.Verbose ) {
			printf( "." );
		}
		error = ck_fread( buf, sizeof( char ), bufSize, inFile );
		ILError( error, "DoSwap" );

		error = vbyteswap( buf, u.id.xs * u.id.ys, T_FLOAT );
		ILError( error, "DoSwap" );

		error = ck_fwrite( buf, sizeof( char ), bufSize, outFile );
		ILError( error, "DoSwap" );
	}

	return error;
}

/******************************    ProcessCommandLine     ***************************
*   capture the user input, mostly globals
*************************************************************************************/
OSErr ProcessCommandLine( int argc, char *argv[] )
{
	OSErr error = noErr;

	int argp = EOF;
	short argsProcessed = 0;

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
