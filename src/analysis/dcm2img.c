/* <html><head><title>dcm2img source</title></head><body>
<h1>dcm2img.c</h1>
<i>Please note last saved date:</i>
<font size=+1><b>$Date: 2003/02/13 01:28:31 $</b><p></font><p>
<center>
<table bgcolor="black" cellpadding=5 border="1">
<tr><td><font color="white">
File conversion utility from *dcm (Sonata DICOM) to Analyze format.
</font></td></tr></table><p>
&copy;2002  <a href="mailto:mscohen@ucla.edu">Mark S. Cohen, Ph.D.</a><br>
<a href="http://www.brainmapping.org">UCLA Brain Mapping Division</a><br>
</center>
<h2>How to make dcm2img</h2>
<ol>
<li>Build ImgLib.o analyzeUtil.o ImgLibError.o VLib.o (from
<a
href = "http://airto.bmap.ucla.edu/BMCweb/SharedCode/SharedSoftware.html#Anchor-UCLA-35829">
www.brainmapping.org</a>)
<li>compile dcm2img, linking the objects above. You will need to have C++ extensions
and the Math Library.
A typical command might be:<br><font face=courier>
cc ImgLib.o analyzeUtil.o ImgLibError.o VLib.o dcm2img.c -lm -o
dcm2img</font>
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

/* $Id: dcm2img.c,v 1.1 2003/02/13 01:28:31 mscohen Exp mscohen $ *
$Log: dcm2img.c,v $
Revision 1.1  2003/02/13 01:28:31  mscohen
Initial revision

 */
/* convert DICOM (*.dcm) short int files to Analyze files. */
/* Somewhat more generic than gen2bshort and friends, this architecture
	detects the header size, which is useful, because the header length is variable */
/* assumes 2 bytes/pixel */

#include <stdio.h>
#include "ImgLib.h"

main( int argc, char *argv[] )
{
	FILE  *fp, *oFile;
	int   i;
	short *inBuff, *outBuff;	// storage for image data
	char  myString[64];
	char  outBase[64], outName[64];
	Boolean LittleEndianFile = false;
	OSErr error = noErr;
	long  imSize;
	long  outSize;
	long  fileStart, fileEnd, fileSize;
	long  SkipBytes;
	short slices = 1;
	int   xs, ys;
	short rows, cols;
	Boolean SingleFile;
	Boolean TimeData = false;
	IMAGE im;

	if( argc < 2 ) {
		printf( "You must enter the names of the input files\n" );
		exit( -1 );
	}

	InitializeIm( &im );

// arbitrary default
	im.dim.dpixsize_X = im.dim.dpixsize_Y = 240.00/64;
	im.data_type = T_SHORT;

	printf( "Are the input files from a pc or a DEC alpha (Little endian)?: " );
	scanf( "%s", myString );
	if( myString[0] == 'Y' || myString[0] == 'y' ) {
		CLEAR( im.dim.flags, kmacByteOrder );
	} else {
		SET( im.dim.flags, kmacByteOrder );
	}

	printf( "What is the center to center spacing (thickness+gap) of the slices (mm)?: " );
	scanf( "%f", &im.dim.scanspacing );
	im.dim.slthick = im.dim.scanspacing;

	printf( "How many pixels in the x dimension (for each file)?: " );
	scanf( "%hd", &im.dim.x );
	printf( "How many pixels in the y dimension (for each file)?: " );
	scanf( "%hd", &im.dim.y );

	im.dim.isoX = im.dim.x;
	im.dim.isoY = im.dim.y;

	imSize = im.dim.x * im.dim.x;
	outSize = imSize;

	printf( "Is each file a separate time point (y) or a separate slice (n): " );
	scanf( "%s", myString );
	if( myString[0] == 'Y' || myString[0] == 'y' ) {
		TimeData = true;
		printf( "How many slices are in each file?: " );
		scanf( "%hd", &slices );

		error = getRowsCols( slices, &rows, &cols );
		ILError( error, "getRowsCols" );

		xs = im.dim.x / cols;
		ys = im.dim.y / rows;

		im.dim.x = im.dim.isoX = xs;
		im.dim.y = im.dim.isoY = ys;

		outSize = xs * ys * slices;

		printf( "\trows: %hd\tcols: %hd\n", rows, cols );
		printf( "\txs: %d\tys: %d\n", xs, ys );
	}
	im.dim.n_slices = slices;

	printf( "What tr was used (msec)?: " );
	scanf( "%ld", &im.acq.tr );
	im.acq.tr *= 1000;	/* tr is now in microseconds */

	printf( "Save to a single output file?: " );
	scanf( "%s", myString );
	if( myString[0] == 'Y' || myString[0] == 'y' ) {
		SingleFile = true;
		printf( "Output file base name: " );
		scanf( "%s", outBase );

		sprintf( outName, "%s.img", outBase );
		error = ck_fopen( &oFile, outName, "wb" );
		ILError( error, outName );

	} else {
		SingleFile = false;
	}

// allocate enough space for our image
	inBuff  = (short *)ck_malloc( imSize * sizeof( short ), "image buffer" );
	outBuff = (short *)ck_malloc( outSize * sizeof( short ), "image buffer" );

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

		error = ck_fread( inBuff, sizeof( short ), imSize, fp );
		ILError( error, "Reading image data" );

		error = ck_fclose( fp );
		ILError( error, "Closing image file" );

		if( slices > 1 ) {
			error = unmdisplay( (void *)inBuff, (void *)outBuff, xs, ys, (int)slices, T_SHORT );
			ILError( error, "unmdisplay" );
		} else {
			error = vmov( inBuff, 1, outBuff, 1, imSize, T_SHORT );
			ILError( error, "vmov" );
		}

		if( !SingleFile ) {
			sprintf(  outName, "%s_%0.3d.img", outBase, i );

			error = ck_fopen( &oFile, outName, "wb" );
			ILError( error, "Saving output file" );

			error = ck_fwrite( outBuff, sizeof( short ), outSize, oFile );
			ILError( error, "Writing to output file" );

			error = ck_fclose( oFile );
        	ILError( error, "Closing output file" );

			sprintf(  outName, "%s_%0.3d", outBase, i );
			error = CreateHeader( ANALYZE, &im, outName );
			ILError( error, "Creating header" );

		} else {
			error = ck_fwrite( outBuff, sizeof( short ), imSize, oFile );
			ILError( error, "Concatenating to output file" );
		}
	}

	free( inBuff );
	free( outBuff );
	fflush( stdout );

	if( SingleFile ) {
		error = ck_fclose( oFile );
		ILError( error, "Closing output file" );

		if( TimeData ) {
			im.dim.timePts = i-1;
		} else {
			im.dim.n_slices = i-1;
			im.dim.timePts = 1;
		}

		error = CreateHeader( ANALYZE, &im, outBase );
		ILError( error, "Creating Header" );
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
