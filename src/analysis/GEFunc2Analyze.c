#include <stdio.h>
#include <string.h>
#include <time.h>
#include "ImgLib.h"

int   gNumSlices;
int   gNumTimePts;
char  gVerbose;
FILE *gProcFile;
FILE  *gTempFile;
char  *gTempFName;
char  nameFileName[ L_tmpnam ];

void usage( char *progname );
OSErr RenameFiles( int argc, char *argv[] );
OSErr CopyImageData( int argc, char *argv[], IMAGE *im );

void usage( char *progname )
{
	OSErr error = noErr;

	printf( "Usage: %s OutFileBase ImageFiles\n", progname );
	printf( "E.g., %s MyOutFile E17776S4*.MR\n", progname );
	printf( "Convert Exxxxxx.MR to analyze 4D. Assumes multiple time points\n" );
	printf( "All images MUST be from the same series number\n" );
	printf( "Side effect: the files are renamed\n" );
	error = ck_fclose( gProcFile );
	ILError( error, "Proc file" );
	exit (1);
}

OSErr RenameFiles( int argc, char *argv[] )
{
	OSErr    error = noErr;
	char     command[256];
	int      number;
	char     newname[256], origname[256], *numstring;
	int      i;

	printf( "Renaming files...\n" );
	error = CreateTempFile( &gTempFile, &gTempFName );
	ILError( error, "Creating temp file" );

// rename the files and create name list into the file called TempName
	for( i=argc-1; i>1; i-- )
	{
		strcpy( origname, argv[i] );
		strcpy( newname, argv[i] );

		numstring = strrchr( newname, 'I' ) + 1;
		number = atoi( numstring );

		sprintf( numstring, "%0.3d.MR", number );
		fprintf( gTempFile, "%s\n", newname );

		if( strcmp( origname, newname )) {
			if( gVerbose ) {
			    printf( "\t%s --> %s\n", origname, newname );
			}
		    sprintf( command, "mv %s %s\n", origname, newname );
		    fprintf( gProcFile, "\tRenamed %s to %s\n", origname, newname );
		    system( command );
		}
	}

	error = ck_fclose( gTempFile );

	tmpnam( nameFileName );
	sprintf( command, "sort %s > %s\n", gTempFName, nameFileName );
	system( command );
	return error;
}

OSErr CopyImageData( int argc, char *argv[], IMAGE *im )
{
	OSErr          error = noErr;
	dsr            myDSR;
	int            theSlice;
	FILE           *GenesisFile;
	FILE           *OutFile;
	FILE           *NameFile;
	unsigned short *data;
	char           GenesisFileName[256];
	char           OutFileName[256];
	long           imSize;
	int            count = 0;

	sprintf( OutFileName, "%s.img", argv[1] );
	printf( "Data will be saved to %s\n", OutFileName );

	error = ck_fopen( &OutFile, OutFileName, "wb" );
	ILError( error, "Opening output file" );

	error = ck_fopen( &NameFile, nameFileName, "rb" );
	ILError( error, "Opening name file" );

	imSize = im->dim.x * im->dim.y * sizeof( unsigned short );
	data = (unsigned short *)ck_malloc( imSize, "data" );
	fprintf( gProcFile, "Image size: %ld\n", imSize );

	printf( "Copying data contents\n" );
	for( theSlice = 2; theSlice < argc; theSlice++ ) {
		fscanf( NameFile, "%s", GenesisFileName );

		error = ck_fopen( &GenesisFile, GenesisFileName, "rb" );
		ILError( error, GenesisFileName );

		fseek( GenesisFile, SIGHDRSIZE, SEEK_SET );

		error = ck_fread( data, imSize, 1, GenesisFile );
		ILError( error, GenesisFileName );

		error = ck_fclose( GenesisFile );
		ILError( error, GenesisFileName );

		error = ck_fwrite( data, imSize, 1, OutFile );
		ILError( error, OutFileName );

		if( gVerbose ) {
			printf( "%d of %d files\n", theSlice-1, argc-2 );
		} else {
			int ans;
			if( PercentCount( theSlice-1, argc-2, 10, &ans )) {
				printf( "\t%d%% complete\n", ans );
			}
		}
		fflush( stdout );
		count++;
	}

	printf( "\n%d files processed\n", count );

	error = ck_fclose( NameFile );
	ILError( error, gTempFName );

	error = ck_fclose( OutFile );
	ILError( error, OutFileName );

	free( data );

	return noErr;
}

int main( int argc, char **argv )
{
	IMAGE  im;
	dsr    myDSR;
	char   inName[256];
	int    numTimePts;
	unsigned short  *data;
	OSErr  error = noErr;
	static char id[] = "$Revision: 1.4 $$Date: 2002/09/10 21:58:40 $";


	gVerbose = false;

	error = OpenProcFile( argc, argv, argv[1], id, &gProcFile );
	ILError( error, "Opening proc file" );

	if( argc < 3 ) {
		usage( argv[0] );
	}

	strcpy( inName, argv[2] );

	error = UC_Readheader( inName, &im );
	ILError( error, "reading input header" );

	gNumSlices  = im.dim.n_acquired_slices;
	gNumTimePts = (argc-2) / gNumSlices;

// file size and consistency check
	if( (argc - 2) % gNumSlices ) {
		printf( "This does not look correct: the number of files (%d) divided by the\n", argc - 2 );
		printf( "number of slices in the headers (%d) is not an integer\n", gNumSlices );
	} else {
		printf( "You appear to have %d time points\n", gNumTimePts );
	}

// files must be in the proper order
	error = RenameFiles( argc, argv );
	ILError( error, "renaming files" );

// copy file contents
	error = CopyImageData( argc, argv, &im );
	ILError( error, "Copying Image Data" );

	im.dim.timePts  = gNumTimePts;
	im.dim.n_slices = gNumSlices;
	
// Make an analyze header
	error = CreateHeader( ANALYZE, &im, argv[1] );
	ILError( error, "Creating header" );

	error = ck_fclose( gProcFile );
	return 0;
}
