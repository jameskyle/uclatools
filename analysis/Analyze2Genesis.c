/* <html><head><title>Analyze2Genesis source</title></head><body>
<h2>How to make Analyze2Genesis</h2>
<ol>
<li>Build ImgLib.o analyzeUtil.o ImgLibError.o VLib.o (from
<a
href="http://airto.loni.ucla.edu/BMCweb/SharedCode/SharedSoftware.html#Anchor-UCLA-35829
">www.brainmapping.org</a>)
<li>compile Analyze2Genesis, linking the objects above. You will need to have C++ extensions
and the Math Library.
A typical command might be:<br><font face=courier>
cc ImgLib.o analyzeUtil.o ImgLibError.o VLib.o Analyze2Genesis.c -lm -o
Analyze2Genesis</font>
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
<pre>
$Log: Analyze2Genesis.c,v $
Revision 1.14  2002/11/19 19:38:03  zrinka
fixed some html

Revision 1.13  2002/09/10 22:18:46  mscohen
changed call to OpenProcFile

Revision 1.12  2002/09/09 21:05:51  nina
changed fopen mode from "w"/"r" to "wb"/"rb" where applicable

Revision 1.11  2002/09/09 20:51:13  nina
no modifications, RCS consolidation

 * Revision 1.10  2002/04/09  00:25:29  mscohen
 * Added html formatting.
 *
 * Revision 1.9  2001/10/06  11:05:19  mscohen
 * modified compilation instructions.
 *
 * Revision 1.9  2001/10/06  11:05:19  mscohen
 * modified compilation instructions.
 *
 * Revision 1.8  2001/10/06  10:32:25  mscohen
 * Modified compilation instructions.
 *
 * Revision 1.7  2001/10/05  22:22:42  mscohen
 * Modified disclaimer notice.
 *
 * Revision 1.6  2001/10/05  21:56:40  mscohen
 * Added rcs notations
 * */

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include "ImgLib.h"
#include "ImgLibError.h"
#include "MathLib.h"
#include "getopt.h"

#define OSErr short
#define noErr 0

FILE	*MessageLog;

void print_usage( char *progName )
{
	char	response[25];

	printf( "Usage:\n%s -i AnalyzeInputFile -n NameFile\n\n", progName );
	printf( "\t%s will convert a (4D) analyze file to a series of genesis\n", progName );
	printf( "\tfiles having names corresponding to the contents of the NameFile\n\n" );
	printf( "\n\nNote that you must keep a copy of the NameFile (which ends in .NAME_FILE)\n" );
	printf( "\n\t and the headers file, which ends in: .GenesisHeaders, in order to\n" );
	printf( "\n\tsuccessfully reconstruct the genesis headers using Analyze2Genesis\n" );
	printf( "\tThe user is responsible for ensuring that the name file has a separate\n" );
	printf( "\tentry for each slice location in the analyze file\n" );

	printf( "More (y/n): " );
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
		printf( "        $Revision: 1.14 $$Date: 2002/11/19 19:38:03 $\n" );
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
}

int main( int argc, char *argv[] )
{
	short argp;
	char  *p;
	char  *nameFileName = NULL;
	char  *analyzeFName = NULL;
	char  genesisInFName[256];
	char  genesisOutFName[256];
	char  headerFileName[256];
	FILE  *nameFile;
	FILE  *analyzeFile;
	FILE  *genesisOutFile;
	FILE  *headerFile;
	FILE  *procFile;
	char  baseName[256];
	char  *dataBuff;
	short numImages, count, verbose = false;
	IMAGE im;
	static char id[] = "$Revision: 1.14 $$Date: 2002/11/19 19:38:03 $";
	OSErr error = noErr;
	
// capture user options
	while ( (argp=getopt(argc,argv,"i:n:Vh?")) != EOF ) {
		switch( argp )   {
			case 'i':
				analyzeFName = optarg;
				break;
			case 'n':
				nameFileName = optarg;
				break;
			case 'V': verbose = true; break;
			case 'h':
			case '?':
			default:
				print_usage( argv[0] );
				exit( -1 );
		}
	}

	if( !analyzeFName || !nameFileName ) {
		print_usage( argv[0] );
		return -1;
	}

	strcpy( baseName, nameFileName );
	p = strrchr( baseName, '.' );
	*p = '\0';
	if( !strlen( baseName ) ) {
		strcpy( baseName, nameFileName );
	}
	
	error = UC_Readheader( analyzeFName, &im );
		if( error ) ILError( error, "Opening header file" );

	numImages = im.dim.n_slices;
	dataBuff = (char *)ck_malloc( im.dim.x * im.dim.y * sizeof( short ), "dataBuff" );
	
	nameFile = errfopen( nameFileName, "rb" );
	analyzeFile = errfopen( analyzeFName, "rb" );
	fscanf( nameFile, "%s", headerFileName );
	printf( "Using %s for genesis headers\n", headerFileName );
	headerFile = errfopen( headerFileName, "rb" );
	error = OpenProcFile( argc, argv, baseName, id, &procFile );
	ILError( error, "Opening Proc file" );
	
	fprintf( procFile, "Headers were read from ... %s\n", headerFileName );
	fprintf( procFile, "Names were constructed from ... %s\n", nameFileName );
	if( verbose ) {
		printf( "Using name file: %s\n", nameFileName );
	}
	
	
	for( count=0; count < numImages; count++ ) {
		fscanf( nameFile, "%s", genesisInFName );

		sprintf( genesisOutFName, "Ec" );
		strcpy( genesisOutFName+2, genesisInFName+1 );

		genesisOutFile = errfopen( genesisOutFName, "wb" );
		
		ck_fread ( dataBuff, sizeof( char ), SIGHDRSIZE, headerFile );
		ck_fwrite( dataBuff, sizeof( char ), SIGHDRSIZE, genesisOutFile );
		
		ck_fread ( dataBuff, sizeof( short ), im.dim.x*im.dim.y, analyzeFile );
		ck_fwrite( dataBuff, sizeof( short ), im.dim.x*im.dim.y, genesisOutFile );

		fclose( genesisOutFile );
		printf( "." );
		fflush( stdout );
		if( verbose ) {
			printf( "\t%s\n", genesisOutFName );
		}
	}
	printf( "\n" );
	fprintf( procFile, "\nCreated %d Genesis image files...\n", numImages );
	fclose( headerFile );
	fclose( nameFile );
	fclose( analyzeFile );
	fclose( procFile );
}

/* </pre></body></html> */
