/* <html><head><title>gen2bshort source</title></head><body>
<h2>How to make Ana3to4d</h2>
<ol>
<li>Build ImgLib.o analyzeUtil.o ImgLibError.o VLib.o (from
<a
href="http://airto.loni.ucla.edu/BMCweb/SharedCode/SharedSoftware.html#Anchor-UCLA-35829">www.brainmapping.org</a>)
<li>compile gen2bshort, linking the objects above. You will need to have C++
extensions and the Math Library.
A typical command might be:<br><font face=courier>
cc ImgLib.o analyzeUtil.o ImgLibError.o VLib.o gen2bshort.c -lm -o
gen2bshort</font>
</ol>
<center>
<table border=2 cellpadding=8 width="90%"><tr><td>
This software is made available <b>AS IS</b> and no warranty is made as to its
accuracy
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
<a href="http://airto.loni.ucla.edu/BMCweb/HowTo/SpecialFormats.html">
Brain Mapping Center support pages</a>.
</td></tr></table></center>
<pre> */
/*
*      Convert a series of analyze 3D files to a single
*      analyze 4D file.
*/

#include <stdio.h>
#include "ImgLib.h"

#define NAME_SIZE 256

static char id[] = "$Revision: 1.6 $$Date: 2002/11/19 20:34:15 $";

void print_usage(char *name);
void print_usage(char *name)
{
   fprintf(stderr,
   "Usage: %s  outfile Analyze3DFile(s)\n\
    Creates a single Analyze 4D file from a series of Analyze 3D files\n\
    User must supply the base name of  the output file.\n", name );
}

int main (int argc, char *argv[])
{
	int     argcount;
	int     timePts = argc-2;
	FILE    *inFile, *outFile;
	FILE    *procFile;
	void    *DataBuffer;
	char    basename[NAME_SIZE], inputname[NAME_SIZE], outname[NAME_SIZE];
	IMAGE   im;
	long    volSize;
	OSErr   error = noErr;


	error = OpenProcFile( argc, argv, argv[0], id, &procFile );
	RETURNONERROR;

	if (argc<=2)
	{
		print_usage(argv[0]);
		exit(-1);
	}

    printf ( "Processing %d files\n", timePts );

/* Open the output file as outputBase.img */
    strcpy( basename, argv[1] );
    strcpy( inputname, argv[2] );
    sprintf( outname,"%s.img",basename );
    printf( "Writing to %s\n", outname );

    outFile = errfopen(outname, "wb");
	
	error = UC_Readheader( inputname, &im );
	ILError( error, "Getting header data" );

	volSize = im.dim.isoX * im.dim.isoY * im.dim.n_slices * get_datasize( im.data_type );

	DataBuffer = ck_malloc( volSize, "storage for input data" );

/* Append contents of each file to output */
    for ( argcount=2; argcount < argc; argcount++ )
    {
    	strcpy( inputname, argv[argcount] );

		inFile = errfopen( inputname, "rb" );
		error = ck_fread( DataBuffer, volSize, 1, inFile );

		fclose(inFile);
		error = ck_fwrite( DataBuffer, volSize, 1, outFile );
		ILError( error, "output file" );
       
    }   /*  loop on input files using argcount */

    fclose(outFile);
    free( DataBuffer );
    
	im.dim.timePts = (short)timePts;

	error = CreateHeader( ANALYZE, &im, basename );

	error = ck_fclose( procFile );
	ILError( error, "proc file" );

	return 0;
} /* main */
/* </PRE> </CODE> */
/* </body></html> */
