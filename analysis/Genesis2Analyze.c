/* <html><head><title>Genesis2Analyze source</title></head><body>
<h2>How to make Genesis2Analyze</h2>
<ol>
<li>Build ImgLib.o analyzeUtil.o ImgLibError.o VLib.o (from
<a
href="http://airto.loni.ucla.edu/BMCweb/SharedCode/SharedSoftware.html#Anchor-UCLA-35829">www.brainmapping.org</a>)
<li>compile Genesis2Analyze, linking the objects above. You will need to have C++ extensions and the Math Library.
A typical command might be:<br><font face=courier>
cc ImgLib.o analyzeUtil.o ImgLibError.o VLib.o Genesis2Analyze.c -lm -o
Genesis2Analyze</font>
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
<a href="http://airto.loni.ucla.edu/BMCweb/HowTo/SpecialFormats.html">
Brain Mapping Center support pages</a>.
</td></tr></table></center>
<pre> */

/* $Author: mscohen $  $Revision: 1.21 $ $Date: 2006/06/09 18:40:34 $ $Log: Genesis2Analyze.c,v $
/* $Author: zrinka $  $Revision: 1.20 $ $Date: 2002/11/19 19:21:25 $ Revision 1.21  2006/06/09 18:40:34  mscohen
/* $Author: zrinka $  $Revision: 1.20 $ $Date: 2002/11/19 19:21:25 $ Added debug statements
/* $Author: zrinka $  $Revision: 1.20 $ $Date: 2002/11/19 19:21:25 $
/* $Author: mscohen $  $Revision: 1.21 $ $Date: 2006/06/09 18:40:34 $ Revision 1.20  2002/11/19 19:21:25  zrinka
/* $Author: mscohen $  $Revision: 1.21 $ $Date: 2006/06/09 18:40:34 $ fixed some more html
/* $Author: mscohen $  $Revision: 1.21 $ $Date: 2006/06/09 18:40:34 $
/* $Author: mscohen $  $Revision: 1.21 $ $Date: 2006/06/09 18:40:34 $ Revision 1.19  2002/11/19 19:18:16  zrinka
/* $Author: mscohen $  $Revision: 1.21 $ $Date: 2006/06/09 18:40:34 $ fixed some html
/* $Author: mscohen $  $Revision: 1.21 $ $Date: 2006/06/09 18:40:34 $
/* $Author: mscohen $  $Revision: 1.21 $ $Date: 2006/06/09 18:40:34 $ Revision 1.18  2002/09/10 22:17:06  mscohen
/* $Author: mscohen $  $Revision: 1.21 $ $Date: 2006/06/09 18:40:34 $ changed call to OpenProcFile
/* $Author: mscohen $  $Revision: 1.21 $ $Date: 2006/06/09 18:40:34 $
/* $Author: mscohen $  $Revision: 1.21 $ $Date: 2006/06/09 18:40:34 $ Revision 1.17  2002/09/09 22:43:18  nina
/* $Author: mscohen $  $Revision: 1.21 $ $Date: 2006/06/09 18:40:34 $ changed fopen mode from "r"/"w" to "rb"/"wb" where appropriate
/* $Author: mscohen $  $Revision: 1.21 $ $Date: 2006/06/09 18:40:34 $
/* $Author: mscohen $  $Revision: 1.21 $ $Date: 2006/06/09 18:40:34 $ Revision 1.16  2002/04/09 00:25:43  mscohen
/* $Author: mscohen $  $Revision: 1.21 $ $Date: 2006/06/09 18:40:34 $ Added html formatting.
/* $Author: mscohen $  $Revision: 1.21 $ $Date: 2006/06/09 18:40:34 $
 * Revision 1.15  2001/12/04  16:18:43  mscohen
 * Handling of temp files was corrupted. Tested and works on Mac unix now.
 *
 * Revision 1.14  2001/12/04  04:41:35  mscohen
 * upgraded all errfopen to ck_fopen
 *
 * Revision 1.13  2001/12/04  04:35:36  mscohen
 * Reverted/repaired file renumbering. Bad bug.
 * Added support with CreateTempFile
 * Upgraded to ck_fopen
 *
 * Revision 1.9  2001/10/06  11:04:39  mscohen
 * modified compilation instructions.
 *
 * Revision 1.8  2001/10/05  22:23:06  mscohen
 * Modified disclaimer notice.
 *
 * Added html code (but did not test compile)
 *
 * Revision 1.6  2001/09/27  02:19:06  mscohen
 * Improved commenting for support purposes.
 *
 * Revision 1.5  2001/09/27  02:08:02  mscohen
 * Turned off verbose mode
 *
 * Revision 1.4  2001/09/27  02:05:12  mscohen
 * Upgraded to current ImgLib version
 *
 * Revision 1.3  2000/03/23  13:09:41  mscohen
 * New version of ImgLib and ImgLib.h
 *
 * Revision 1.2  2000/03/14  20:58:24  mscohen
 * Added more help messages
 *
 * Revision 1.1  2000/02/15  23:29:42  mscohen
 * Initial revision
 *
 * Revision 1.1  2000/02/15  17:39:34  mscohen
 * Initial revision
 *
 * Revision 1.1  2000/02/15  14:51:20  mscohen
 * Initial revision
 *
 * Revision 1.1  2000/02/15  03:32:10  mscohen
 * Initial revision
 *
 * Revision 1.1  2000/02/15  03:01:32  mscohen
 * Initial revision
 * */

/********************  Genesis2Analyze  ***************************
*	Convert a series of genesis (E*.MR) images to an analyze file
*	Save a 'NameFile' containing the names of all input images.
*
*	This program is designed as a front end for the EQ program
******************************************************************/
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <stdlib.h>
#include "ImgLib.h"
#include "ImgLibError.h"

FILE	*MessageLog;

/* This program converts a list of Signa *.MR files to an analyze file, and outputs a name list */
OSErr writeHeader( IMAGE *im, char *baseName );

OSErr writeHeader( IMAGE *im, char *basename )
/* creates an analyze style header based on the struct im */
{
	FILE  *f;
	char  buff[256];
	dsr   theDSR;
	OSErr error = noErr;
	
	sprintf( buff, "%s.hdr", basename );
	f = fopen( buff, "wb" );
	if( !f ) {
		return WRITE_ERROR;
	}
	error = EmptyAnaHdr( &theDSR, "From Signa", im->dim.scanspacing, im->dim.x,
				             im->dim.y, im->dim.n_slices, im->dim.timePts, axial );
		if( error ) ILError( error, "Creating analyze header" );

	if( pcByteOrder() ) {
		error = swapHdrBytes( &theDSR );
			if( error ) ILError( error, "swapping header bytes" );
		printf( "\nWARNING: Computer and data differ in byte order\n" );
	}
	
	error = ck_fwrite( &theDSR, sizeof( dsr ), 1, f );
		if( error ) ILError( error, buff );
		
	ck_fclose( f );
	return error;
}

void print_usage( char *progName )
{
	char	response[25];

	printf( "Usage: %s basename*.MR\n", progName );
	printf( "Renumber all files ending in \'*.MR\' with 3 digit numbers\n" );
	printf( "Create an analyze 4D file with these contents\n" );
	printf( "\n\nNote that you must keep a copy of the NameFile (which ends in .NAME_FILE)\n" );
	printf( "\n\t and the headers file, which ends in: .GenesisHeaders, in order to\n" );
	printf( "\n\tsuccessfully reconstruct the genesis headers using Analyze2Genesis\n" );

	printf( "More (y/n)?: " );
	scanf( "%s", response );
	
	if( response[0] == 'y' || response[0] == 'Y' ) {
		printf( "\nThis is part of a set of utilities to pre-process signa files for intensity correction,\n" );
		printf( "and to post-process the resulting files to convert them to a valid Signa format.\n" );
		printf( "==============\n" );
		printf( "The tool Genesis2Analyze takes a series of ExxxxSxxxIxxx.MR files and makes them into a single\n" );
		printf( "4-D analyze file. It also saves a record of the original names, and the complete header for each\n\n" );
		printf( "Typing Genesis2Analyze (my way of providing simple help) gives:\n\n" );
		printf( "Usage: Genesis2Analyze basename*.MR\n" );
		printf( "Renumber all files ending in '*.MR' with 3 digit numbers\n" );
		printf( "Create an analyze 4D file with these contents\n\n\n" );
		printf( "Note that you must keep a copy of the NameFile (which ends in .NAME_FILE)\n\n" );
		printf( "        and the headers file, which ends in: .GenesisHeaders, in order to\n\n" );
		printf( "        successfully reconstruct the genesis headers using Analyze2Genesis\n" );
		printf( "===============\n" );
		printf( "Analyze2Genesis does the reverse operation, extracting each slice in an analyze file, and giving\n" );
		printf( "it a name and header from the output derived from Genesis2Analyze.\n\n" );
		printf( "Typing Analyze2Genesis gives:\n\n" );
		printf( "Usage:\n" );
		printf( "Analyze2Genesis -i AnalyzeInputFile -n NameFile\n\n" );
		printf( "        Analyze2Genesis will convert a (4D) analyze file to a series of genesis\n" );
		printf( "        files having names corresponding to the contents of the NameFile\n\n\n" );
		printf( "Note that you must keep a copy of the NameFile (which ends in .NAME_FILE)\n\n" );
		printf( "         and the headers file, which ends in: .GenesisHeaders, in order to\n\n" );
		printf( "        successfully reconstruct the genesis headers using Analyze2Genesis\n" );
		printf( "        The user is responsible for ensuring that the name file has a separate\n" );
		printf( "        entry for each slice location in the analyze file\n\n" );
		printf( "================\n\n" );
		printf( "A complete session transcript follows for RF correction. Note that the warning messages for mv\n" );
		printf( "and differing byte order are expected.\n\n" );
		printf( " Genesis2Analyze E*.MR\n" );
		printf( "Renaming files\n" );
		printf( "mv: E2123S2I124.MR and E2123S2I124.MR are identical.\n\n" );
		printf( "... etc ...\n\n" );
		printf( "mv: E2123S2I100.MR and E2123S2I100.MR are identical.\n\n" );
		printf( "WARNING: Computer and data differ in byte order\n\n" );
		printf( "        Patience for a moment\n\n\n" );
		printf( "Created Analyze (4D) file: E2123S2.img\n" );
		printf( "        Save these files!: E2123S2.NAME_FILE and E2123S2.GenesisHdrs\n\n" );
		printf( "To create corresponding genesis images from an analyze file named XXX.img, use:\n" );
		printf( "        Analyze2Genesis -n E2123S2.NAME_FILE -i XXX.img\n" );
		printf( "All done\n\n" );
		printf( "--------------------\n\n" );
		printf( "> EQ -i E2123S2.img -o eqTst\nEQ\n" );
		printf( "        $Revision: 1.21 $$Date: 2006/06/09 18:40:34 $\n" );
		printf( "Working\n\n" );
		printf( "Done. Thank you for using EQ.\n" );
		printf( "mscohen@ucla.edu -- http://www.brainmapping.org\n" );
		printf( "\n--------------------\n\n" );
		printf( " Analyze2Genesis -n E2123S2.NAME_FILE -i eqTst.img\n" );
		printf( "Using E2123S2.GenesisHdrs for genesis headers\n\n" );
		printf( "......................................................................\n\n" );
		printf( "> ls Ec*\n" );
		printf( "Ec2123S2I001.MR   Ec2123S2I026.MR   Ec2123S2I051.MR   Ec2123S2I076.MR Ec2123S2I101.MR\n" );
		printf( " ... etc ...\n" );
		printf( "Ec2123S2I025.MR   Ec2123S2I050.MR   Ec2123S2I075.MR   Ec2123S2I100.MR\n\n" );
		printf( "Mark S. Cohen, Ph.D.\n" );
		printf( "UCLA Brain Mapping Division\n" );
		printf( "660 Charles Young Dr. S.\n" );
		printf( "Los Angeles, CA 90095\n\n" );
		printf( "office: 310/897-1690\n" );
		printf( "lab:    310/825-9142\n" );
		printf( "http://www.brainmapping.org/\n" );
	}
	exit(-1);
}

int G2A (int argc, char *argv[] )
{
	char origname[128];	// original name
	char newname [128];	// renumbered name
	char command [255];	// command buffer
	char *TempName;
	char *numstring;
	char ImageName[128];
	int  counter = 0;
	int  number;
	char *p;
	char nameFileName[255];
	char analyzeFileName[255];
	char headerFileName[255];
	char baseName[255];
	short verbose = true;
	FILE *nameFile;
	FILE *TempFile;
	FILE *dataFile;
	FILE *procFile;
	FILE *headerFile;
	FILE *inFile;
	int  i;
	IMAGE im;
	short count;
	short *dataBuff;
	Boolean DebugMode = false;
	
	static char id[] = "$Revision: 1.21 $$Date: 2006/06/09 18:40:34 $";
	OSErr error = noErr;

	if( argc < 2 ) {
		print_usage( argv[0] );
	}

// setup an output file name, based on the last input file name
	sprintf( baseName, "%s", argv[argc-1] );	// output file name

	p = strrchr( baseName, 'I' );
	*p = '\0';	// null terminate
	
	error = OpenProcFile( argc, argv, baseName, id, &procFile );
	ILError( error, "Opening Proc file" );
	
	sprintf( nameFileName, "%s.NAME_FILE", baseName );
	fprintf( procFile, "Associated name file ... %s\n", nameFileName );
	sprintf( headerFileName, "%s.GenesisHdrs", baseName );
	fprintf( procFile, "Associated headers file ... %s\n", headerFileName );

// corrected names will be stored temporarily in nameFile	
	error = CreateTempFile( &TempFile, &TempName );
	ILError( error, TempName );
	
// use UC_ReadHeader to determine image dimensions, etc...
	error = UC_Readheader( argv[argc-1], &im );
	if( error ) ILError( error, "Read Image" );

// rename the files and create name list into the file called TempName
   for( i=argc-1; i>0; i-- )
   {
		strcpy( origname, argv[i] );
		strcpy( newname, argv[i] );

		numstring = strrchr( newname, 'I' ) + 1;
		number = atoi( numstring );


		sprintf( numstring, "%0.3d.MR", number );

		fprintf( TempFile, "%s\n", newname );

		if( strcmp( origname, newname ) || DebugMode ) {
			if( verbose ) {
			    printf( "i:%d\t%s --> %s\n", i, origname, newname );
			}
		    sprintf( command, "mv %s %s\n", origname, newname );
		    fprintf( procFile, "\tRenamed %s to %s\n", origname, newname );
		    system( command );
		}
	}
	error = ck_fclose( TempFile );
	ILError( error, TempName );

// Sort the contents of the temporary nameFile and place it into the permanent version
	sprintf( command, "sort %s > %s", TempName, nameFileName );
	system( command );

	if( false ) {
		DBG( command );
		printf( "\nContents of name file\n" );
		sprintf( command, "cat %s", nameFileName );
		DBG( command );
		system( command );
	}
	
// now, place the name of the header file at the beginning of a file */
	error = CreateTempFile( &TempFile, &TempName );
	ILError ( error, TempName );

	fprintf( TempFile, "%s\n", headerFileName );
	error = ck_fclose( TempFile );
	ILError( error, TempName );
	
// add the file names to the end
	sprintf( command, "cat %s >> %s\n", nameFileName, TempName );
	system( command );
	
// and rename this file
	sprintf( command, "mv %s %s\n", TempName, nameFileName );
	system( command );
	
	if( false ) {
		printf( "new Contents of name file" );
		sprintf( command, "cat %s", nameFileName );
		DBG( command );
		system( command );
	}

	im.dim.n_slices = argc - 1;

	error = CreateHeader( ANALYZE, &im, baseName );
	if( error ) ILError( error, "Writing Header" );

	sprintf( analyzeFileName, "%s.img", baseName );
	error = ck_fopen( &dataFile, analyzeFileName, "wb" );
	ILError( error, "data file" );
	error = ck_fopen( &nameFile, nameFileName, "rb" );
	ILError( error, "name file" );
	error = ck_fopen( &headerFile, headerFileName, "wb" );
	ILError( error, "header file" );
	
	if( im.dim.x * im.dim.y * sizeof(short) < SIGHDRSIZE ) {
		dataBuff = (short *)ck_malloc( SIGHDRSIZE, "data buffer" );
	} else {
		dataBuff = (short *)ck_malloc( im.dim.x * im.dim.y * sizeof(short), "data buffer" );
	}
	
	count = fscanf( nameFile, "%s", headerFileName );	// just read past this.
	count = fscanf( nameFile, "%s", ImageName );

	if( false ) { DBG( ImageName ); }

	printf( "\n\tPatience for a moment\n" );

	while( count>0 ) {
		printf( "%d of %d complete\n", counter++, argc - 1 );
		error = ck_fopen( &inFile, ImageName, "rb" );
			ILError( error, "Opening image" );
		error = ck_fread ( dataBuff, sizeof( char ), SIGHDRSIZE, inFile );
			ILError( error, "Reading header" );
		error = ck_fwrite( dataBuff, sizeof( char ), SIGHDRSIZE, headerFile );
			ILError( error, "Writing image header" );
		error = ck_fread ( dataBuff, sizeof( short ), im.dim.x * im.dim.y, inFile );
			ILError( error, "Reading image data" );
		error = ck_fwrite( dataBuff, sizeof( short ), im.dim.x * im.dim.y, dataFile );
			ILError( error, "Writing image data" );
		ck_fclose( inFile );
		count = fscanf( nameFile, "%s", ImageName );
	}

	printf( "\n\nCreated Analyze (4D) file: %s\n", analyzeFileName );
	printf( "\tSave these files!: %s and %s\n\n", nameFileName, headerFileName );
	printf( "To create corresponding genesis images from an analyze file named XXX.img, use:\n" );
	printf( "\tAnalyze2Genesis -n %s -i XXX.img\n", nameFileName );

	free( dataBuff );	
	error = ck_fclose( headerFile );
		if( error ) ILError( error, headerFileName );
	error = ck_fclose( nameFile );
		if( error ) ILError( error, nameFileName );
	error = ck_fclose( dataFile );
		if( error ) ILError( error, analyzeFileName );
	error = ck_fclose( procFile );
		if( error ) ILError( error, "proc file" );

	return error;
}

int main( int argc, char *argv[] )
{
	short error;

	error = G2A( argc, argv );
	if( error ) ILError( error, "G2A" );
	
	return 0;
}
/* </pre></body></html> */
