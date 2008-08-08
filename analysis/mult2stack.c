#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "getopt.h"

#include "VLib.h"
#include "ImgLib.h"

#define DEBUG

void print_usage( char *name )
{
	fprintf( stderr, "Usage: %s -i InFile, -o OutFile, -r rows, -c cols\n", name );
	fprintf( stderr, "\tAll arguments are required!\n" );
	fprintf( stderr, "Only the base name is needed for the output file.\n" );
	fprintf( stderr, "Outputs a processing record: outputname.%s.proc\n",name);
}

int main(int argc, char **argv)
{
	static char id[] = "$Revision: 1.5 $$Date: 2002/09/10 22:14:51 $";
    char    *invec, *outvec;   /* image vectors */
    int     timePt;
    short   rows = 0;
	short   cols = 0;
    int     slices;
    long    MultImSize;         /* size of one image */
	short   SmallX, SmallY; /* X and Y size of single image from multi-display */
    int     argp=EOF;
    char    *in_fname = NULL;
	char    *out_basename = NULL;
    char    out_fname[128], procfname[128];
    FILE    *imFile;
	FILE    *procFile;
    IMAGE   im;
	short   dSize;
	char    typeString[40];
	OSErr   error = noErr;

	while ( (argp=getopt(argc,argv,"o:i:r:c:h?")) != EOF ) {
		switch( argp )   {
			case 'o': out_basename = optarg; break;
			case 'i': in_fname = optarg; break;
			case 'c': cols = atoi( optarg ); break;
			case 'r': rows = atoi( optarg ); break;
			case 'h':
			case '?':
			default:
				print_usage( argv[0] );
				exit( -1 );
			break;
		}
	}

	if( !out_basename || !in_fname || !rows || !cols ) {
	   print_usage( argv[0] );
	   exit(-1);
	}

	error = UC_Readheader(in_fname, &im);
	if( error ) {
		ILError( error, "Reading Header" );
	}

	switch( im.file_type )
	{
		case BSHORT:
			dSize = get_datasize( T_USHORT );
			sprintf( typeString, "bshort" );
			break;
		case BUCHAR:
			dSize = get_datasize( T_UCHAR );
			sprintf( typeString, "buchar" );
			break;
		case BFLOAT:
			dSize = get_datasize( T_FLOAT );
			sprintf( typeString, "bfloat" );
			break;
		default:
			ILError( DATA_OUT_OF_RANGE, "unknown type" );
	}

/* Parse output file name and create proc file name */
	strcpy( procfname, out_basename );
	
	MultImSize= im.dim.x*im.dim.y;
	SmallX = im.dim.x / cols;
	SmallY = im.dim.y / rows;
	slices = rows * cols;

/*  Buffer allocation  */
	invec =  ( char *)ck_malloc( MultImSize * dSize,  "image vector" );
	outvec = ( char *)ck_malloc( SmallX * SmallY * slices * dSize, "output vector" );

	imFile = errfopen( in_fname, "rb" );

	error = OpenProcFile( argc, argv, procfname, id, &procFile );
	ILError( error, "Opening Proc file" );

	for( timePt = 0; timePt < im.dim.timePts; timePt++ ) {
		int theSlice = 0;

		error = UC_Readimage( imFile, &im, invec, theSlice, timePt );
		if( error ) {
			ILError( error, "Reading image" );
		}

// fool unmdisplay by treating all input as char
		error = unmdisplay( outvec, invec, SmallX * dSize, SmallY, slices, T_UCHAR );
		if( error ) {
			ILError( error, "unmdisplay" );
		}

		if( im.dim.timePts > 1 ) {
			sprintf( out_fname, "%s.%0.3d", out_basename, timePt+1 );
		} else {
			sprintf( out_fname, "%s", out_basename );
		}

		fprintf( procFile, "\tCreated %s.%s\n", out_fname, typeString );

		error = WriteMGHImage( outvec, out_fname, SmallX, SmallY, (short)slices, im.data_type );
		if( error ) {
			ILError( error, "Writing output image" );
		}
	}
	error = ck_fclose( imFile );
	if( error ) ILError( error, "input file" );

	error = ck_fclose( procFile );
	if( error ) ILError( error, "proc file" );

	free( outvec );
	free( invec );

  return 0;
}
