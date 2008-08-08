#include <stdio.h>
#include <string.h>
#include "ImgLib.h"

int main( int argc, char **argv )
{
	IMAGE im;
	dsr   myDSR;
	short tmp;
	FILE  *fp;
	char  inName[256], outName[256];
	OSErr error = noErr;

	printf( "%s - Treat times points in an analyze header as slices\n", argv[0] );
	printf( "WARNING -- this tool will overwrite the original header\n" );

	if( argc == 1 ) {
		printf( "Enter the name of an analyze image file ( xxx.img ): " );
		scanf( "%s", inName );
	} else {
		strcpy( inName, argv[1] );
	}

	error = UC_Readheader( inName, &im );
	ILError( error, "reading input header" );

	tmp = im.dim.n_slices;
	im.dim.n_slices = im.dim.timePts;
	im.dim.timePts = tmp;

	error = dsrFromIMAGE( &im, &myDSR );
	ILError( error, "Creating analyze header" );

	strcpy( outName, inName );
	sprintf( strrchr( outName, '.' ), ".hdr" );

	printf( "Header will be overwritten to %s\n", outName );

	error = ck_fopen( &fp, outName, "wb" );
	ILError( error, "opening output header" );

	error = ck_fwrite( &myDSR, sizeof( dsr ), 1, fp );
	ILError( error, "writing analyze header" );

	error = ck_fclose( fp );
	ILError( error, "closing header file" );

	return 0;
}
