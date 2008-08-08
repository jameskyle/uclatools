/* <html><head><title>Analyze2Genesis source</title></head><body>
<h2>How to make Generic2bshort</h2>
<ol>
<li>Build ImgLib.o analyzeUtil.o ImgLibError.o VLib.o (from
<a
href="http://porkpie.loni.ucla.edu/BMD_HTML/SharedCode/SharedSoftware.html#Anchor-UCLA-35829
">www.brainmapping.org</a>)
<li>compile Generic2bshort, linking the objects above. You will need to have C++ extensions
and the Math Library.
A typical command might be:<br><font face=courier>
cc ImgLib.o analyzeUtil.o ImgLibError.o VLib.o Generic2bshort.c -lm -o
Generic2bshort</font>
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
<a href="http://porkpie.loni.ucla.edu/3tmri-how2.html">
Brain Mapping Center support pages</a>. </td></tr></table></center><pre>  */

/* $Id: Generic2bshort.c,v 1.2 2002/09/09 22:36:13 nina Exp $ *
$Log: Generic2bshort.c,v $
Revision 1.2  2002/09/09 22:36:13  nina
changed fopen mode from "r"/"w" to "rb"/"wb" where appropriate

Revision 1.1  2001/10/06 11:22:59  mscohen
Initial revision
 */
/* convert Generic short int files to bshort files. */
/* This very short program seems to demonstrate the power of ImgLib */
/* Somewhat more generic than gen2bshort and friends, this architecture
	detects the header size, which is useful, because the header length is variable */
/* assumes 2 bytes/pixel */

#include <stdio.h>
#include "ImgLib.h"

main( int argc, char *argv[] )
{
	FILE  *fp, *oFile;
	int   i;
	short *buff;	// storage for image data
	char  myString[64];
	char  outBase[64], outName[64];
	Boolean LittleEndianFile = false;
	OSErr error = noErr;
	int   ySize, xSize;
	long  imSize;
	long  fileStart, fileEnd, fileSize;
	long  SkipBytes;
	Boolean SingleFile;

	if( argc < 2 ) {
		printf( "You must enter the names of the input files\n" );
		exit( -1 );
	}
	printf( "Are the input files from a pc or a DEC alpha (Little endian)?: " );
	scanf( "%s", myString );
	if( myString[0] == 'Y' || myString[0] == 'y' ) {
		LittleEndianFile = true;
	}

	printf( "How many pixels in the x dimension?: " );
	scanf( "%d", &xSize );
	printf( "How many pixels in the y dimension?: " );
	scanf( "%d", &ySize );
	imSize = xSize * ySize;

	printf( "Save to a single output file?: " );
	scanf( "%s", myString );
	if( myString[0] == 'Y' || myString[0] == 'y' ) {
		SingleFile = true;
		printf( "Output file base name: " );
		scanf( "%s", outBase );
		sprintf( outName, "%s.bshort", outBase );
		error = ck_fopen( &oFile, outName, "wb" );
		ILError( error, outName );
	} else {
		SingleFile = false;
	}

// allocate enough space for our image
	buff = (short *)ck_malloc( imSize * sizeof( short ), "image buffer" );

	for( i=1; i < argc; i++ ) {
		error = ck_fopen( &fp, argv[i], "rb" );
		ILError( error, "Opening files" );

		fseek( fp, 0, SEEK_SET );
		fileStart = ftell( fp );
		fseek( fp, 0, SEEK_END );
		fileEnd = ftell( fp );
		fileSize = fileEnd - fileStart;

		SkipBytes = fileSize - imSize * sizeof( short );

		fseek( fp, SkipBytes, SEEK_SET );

		error = ck_fread( buff, sizeof( short ), imSize, fp );
		ILError( error, "Reading image data" );

		error = ck_fclose( fp );
		ILError( error, "Closing image file" );

// Make sure that the files are in the correct byte order for this host

		if( !SingleFile ) {
			error = WriteMGHImage( buff, argv[i], ySize, xSize, 1, T_SHORT );
			ILError( error, "Writing bshort file" );
		} else {
			error = ck_fwrite( buff, sizeof( short ), imSize, oFile );
			ILError( error, "Concatenating to output file" );
		}
	}

	free( buff );
	fflush( stdout );

	if( SingleFile ) {
		error = ck_fclose( oFile );
		ILError( error, "Closing output file" );

		sprintf( outName, "%s.hdr", outBase );
		error = ck_fopen( &oFile, outName, "w" );
		ILError( error, outName );

		fprintf( oFile, "%d %d %d %d", ySize, xSize, argc-1, LittleEndianFile );
		error = ck_fclose( oFile );
		ILError( error, outName );
	}

	printf( "\nRemove originals?: " );
	scanf( "%s", myString );

	if( myString[0] == 'Y' || myString[0] == 'y' ) {
		for( i=1; i < argc; i++ ) {
			sprintf( myString, "rm %s\n", argv[i] );
			system( myString );
		}
	}
}
/* </pre></body></html> */
