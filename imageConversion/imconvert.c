/* <html><head><title>imconvert source</title></head><body>
<h1>imconvert.c</h1>
<i>Please note last saved date:</i>
<font size=+1><b>$Date: 2003/04/16 13:27:19 $</b><p></font><p>

<center>
<table bgcolor="black" cellpadding=5 border="1">
    <tr><td><font color="white">
General medical image type conversion utility.
    </font></td></tr></table><p>
    &copy;1996-2002  <a href="mailto:mscohen@ucla.edu">Mark S. Cohen, Ph.D.</a><br>
    <a href="http://www.brainmapping.org">UCLA Brain Mapping Division</a><br>
</center>
<h2>How to make imconvert</h2>
<ol>
    <li>Build ImgLib.o analyzeUtil.o ImgLibError.o VLib.o MathLib.o (from
    <a href=
"http://airto.loni.ucla.edu/BMCweb/SharedCode/SharedSoftware.html#Anchor-UCLA-35829">
www.brainmapping.org</a>)
    <li>compile imconvert, linking the objects above. You will need to have C++ extensions and the
Math Library.
    A typical command might be:<br><font face=courier><p>
    <center>
        cc ImgLib.o analyzeUtil.o ImgLibError.o VLib.o MathLib.o imconvert.c -o imconvert -lm</font>
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
<a href="http://airto.loni.ucla.edu/BMCweb/HowTo/imconvert.html">
Brain Mapping Center support pages</a>.
</td></tr></table></center>
<pre>
<!--
*  $Log: imconvert.c,v $
*  Revision 1.40  2003/04/16 13:27:19  mscohen
*  Repaired bug in cropping behavior that had invalidated the '-c' option
*
*  Revision 1.39  2003/04/16 11:37:10  mscohen
*  Added tr override feature
*
*  Revision 1.38  2002/11/19 19:03:15  zrinka
*  modified html
*
*  Revision 1.37  2002/10/30 21:19:51  zrinka
*  added function "MoveLines" that fixes the problem of (some)
*  functional datasets where the 'eyes are in the back of the head'
*  in the first slice of each time point. It requires number of
*  lines to move as input on the command line using the -l option
*
*  Revision 1.36  2002/09/10 19:46:08  nina
*  changed fopen mode from "r"/"w" to "rb"/"wb" as appropriate
*
*  Revision 1.35  2002/09/10 19:42:53  nina
*  no changes, consolidating RCS
*
 * Revision 1.34  2002/06/17  10:26:27  mscohen
 * For mdisplay mode, fixed bug where the wrong subimages were selected if the input and output data
 * had a different number of bytes/pixel.
 *
 * Revision 1.33  2002/06/04  20:45:38  mscohen
 * Per Richard DuBois: altered mem allocation for input buffer in SaveByTimePoint to size of input data (float) not output data.
 *
 * Revision 1.32  2002/06/01  02:15:56  mscohen
 * Clarified image sizes in bytes and pixels.
 * Fixed bug in multi-slice images where data offsets were calculated incorrectlly, causing problems for imageflip
 * Fixed bug in byteswap that caused errors for certain output data types. This was a problem with malloc of the wrong size.
 * Miscellaneous formatting cleanups.
 *
 * Revision 1.30  2002/04/25  02:36:40  mscohen
 * Check in with diagnostics removed.
 *
 * Revision 1.29  2002/04/19  04:07:28  mscohen
 * Edited html to improve documentation.
 *
 * Revision 1.28  2002/04/03  11:34:40  mscohen
 * Fixed an error in image flip for multiple slices.
 *
 * Revision 1.27  2002/01/23  20:38:17  mscohen
 * added output of version number on each use.
 *
 * Revision 1.26  2001/12/11  04:21:07  mscohen
 * Added percent completion indicators.
 *
 * Revision 1.26  2001/12/11  04:21:07  mscohen
 * Added percent completion indicators.
 *
 * Revision 1.25  2001/11/25  20:12:59  mscohen
 * Added support for field of view
 *
 * Revision 1.24  2001/10/06  11:07:15  mscohen
 * modified compilation instructions.
 *
 * Revision 1.23  2001/10/06  10:22:49  mscohen
 * Modified compilation instructions
 *
 * Revision 1.22  2001/10/06  03:47:40  mscohen
 * trying to reconcile versions. For some reason, the handling of the crop flag seems ambiguous.
 *
 * Revision 1.14.1.2  2001/09/27  14:46:16  mscohen
 * Added html formatting for distribution.
 *
 * Revision 1.19  2001/08/17  17:03:22  mscohen
 * Fixed a file naming bug in analyze format files.
 *
 * Revision 1.18  2001/08/16  22:00:27  mscohen
 * Fixed a bug that caused the analyze header to have the wrong values for glmin and glmax
 *
 * Revision 1.17  2001/03/19  19:27:34  mscohen
 * Fixed bug introduced into mdisplay by byte swapping.
 *
 * Revision 1.16  2001/03/17  05:07:45  mscohen
 * Added documentation for byte order options.
 *
 * Revision 1.15  2001/03/17  04:45:34  mscohen
 * Added support for forced byte order in output file
 *
 * Revision 1.14  2001/03/16  17:49:08  mscohen
 * Added user message for -m A option (was incomplete before)
 *
 * Revision 1.13  2001/02/28  14:40:02  mscohen
 * Update copyright notice
 *
 * Revision 1.12  2000/11/07  02:45:12  mscohen
 * Repaired after Wil destroyed user directories.
 *
 * Revision 1.18  2000/11/02  18:16:21  mscohen
 * Added image flip feature
 *
 * Revision 1.17  2000/10/27  15:50:45  mscohen
 * Added support for mdisplay of all slices
 *
 * Revision 1.16  2000/10/15  10:52:20  mscohen
 * Better error handling for RangeCheck
 * Better handling of normalization (change to VLib)
 *
 * Revision 1.15  2000/10/13  21:08:01  mscohen
 * Fixed a variety of bugs relating to RangeCheck.
 * Autorange does not work properly with unsigned short
 *
 * Revision 1.14  2000/09/19  02:47:14  mscohen
 * added support for mdisplay mode.
 *
 * Revision 1.12  2000/08/29  16:00:55  mscohen
 * Major (complete!) re-write for upgraded functions and easier support
 * Some questions remain about byte order.
 *
 * Revision 1.11  2000/03/23  13:23:30  mscohen
 * New version of ImgLib and ImgLib.h
 * Modified conversion flags (was !=, instead of |=)
 * Changed references to LittleEndian
 *
 * Revision 1.10  1999/09/30  01:07:51  mscohen
 * Added new handling for proc file and other ImgLib functions.
 *
 * Revision 1.9  1999/09/28  04:08:51  mscohen
 * Fixed set and clear flags for handing byte order
 *
 * Revision 1.8  1999/01/17  03:54:48  mscohen
 * Modified handling of epi2bshort style entries. File name does not have fixed field length.
 *
 * Revision 1.7  1998/11/13  08:05:46  mscohen
 * small bug fixes and cleanup
 *
 * Revision 1.6  1998/11/13  02:22:35  mscohen
 * added epi2bshort emulation
 *
 * Revision 1.5  1998/04/16  20:30:02  mscohen
 * Fixed multiplier bug
 *
 * Revision 1.4  1998/04/15  07:02:10  mscohen
 * add revision control in processing file
 *
 * Revision 1.3  1998/04/15  06:58:31  mscohen
 * Eror handling, eliminate goto, add option for AIR3.0 and make slice separation an option
 * pass glmax and glmin to analyze headers. Visual (layout) cleanup
 *
 * Revision 3-31-98: MMZ: Debugging, and recompiling with new libs
   It now inputs analyze.
   Work next on output analyze: change inner and outer loop order
*/

#include "getopt.h"
#include "ImgLib.h"

#define false 0
#define true 1
#define kNoThickness -1
#ifndef noErr
#define noErr 0
#endif

#define kLE 2	// LittleEndian
#define kBE 4	// BigEndian

#define INCOMPATIBLE_OPTIONS -1

#ifdef mactest
#include <console.h>
#include <sioux.h>
#endif

/* globals */
typedef struct {
	char    OutFileName[255];
	char    InFileName[255];
	char    OutBaseName[256];
	int     Slices;
	short   FirstSlice, LastSlice;
	Boolean FlipY; 	// flip image top and bottom
	Boolean Verbose;
	Boolean SeparateTimePts;
	Boolean SeparateSlices;
	Boolean Emulation;	// emulate epi2bshort
	Boolean ForceFloat;
	Boolean Mult2Stack;
	Boolean UseAllSlices;
	Boolean Mdisplay;	// mdisplay output file
	short   RescaleMode;
	float   Multiplier;
	float   Thickness;
	float   OutMin, OutMax;
	float   XFOV, YFOV;	// field of view X and Y
	FILE    *ProcFile;
	short   OutByteOrder;
	Boolean SwapOutput;
	Boolean Crop; // allows the user (Berman) to override the crop flag
	short	numLines; //number of lines to move from the back to the front (first slice only)
	float   tr; // tr in msec. Overrides default
} imconvertGlobal;

imconvertGlobal g;

/* PROTOTYPES */
OSErr MoveLines( FILE *inFile, IMAGE inIm, FILE *tmpFile);
void  TestForSwap( void );
void  print_usage( char *name );
OSErr ProcessCommandLine( int argc, char *argv[] );
OSErr RangeCheck( IMAGE *im );
OSErr OutBaseNameFromOutName( char *OutFileName, char *OutBaseName );
OSErr DoConversion( void *vec1, void *vec2, IMAGE *im, short output_datatype, short *rules );
OSErr Rescale( void *inData, IMAGE *im, float multiplier, short RescaleMode, short output_datatype );
OSErr SaveByTimePoint( IMAGE *im, int file_type, short output_datatype, 
                       FILE *inFile, short *ConversionRules );
OSErr SaveBySlice( IMAGE *im, int file_type, short output_datatype,
                   FILE *inFile, short *ConversionRules );
OSErr SetHeaderDimensions( IMAGE *im, IMAGE *hdrIm, int file_type, short data_type );
OSErr ReportConversionError( OSErr error, short rules );
OSErr SetFoV( IMAGE *im );
void initGlobal( imconvertGlobal *g );

void initGlobal( imconvertGlobal *g ) {
	g->Slices = 0;
	g->FlipY = false;
	g->SeparateTimePts = false;
	g->SeparateSlices = false;
	g->Emulation = false;
	g->ForceFloat = false;
	g->Mult2Stack = false;
	g->UseAllSlices = false;
	g->Mdisplay = false;
	g->RescaleMode = kNoScale;
	g->Multiplier = 1;
	g->Thickness = 0;
	g->numLines = 0;
	g->tr = 0.0;
}

OSErr SetFoV( IMAGE *im )
/* Determine 3d field of view and pixel size from user input.
Assume square if only one of X or Y FOV is given */	
{
	if( g.XFOV > 0.0 || g.YFOV > 0.0 ) {
		if( g.YFOV > 0.0 && g.XFOV < 0.0 ) {
			g.XFOV = g.YFOV;
		}
		if( g.XFOV > 0.0 ) {
			if( g.YFOV < 0.0 ) {
				g.YFOV = g.XFOV;
			}
		}

		im->dim.acqfov = g.XFOV;
		im->dim.dpixsize_X = im->dim.acqpixsize_X = g.XFOV/im->dim.isoX;
		im->dim.acqfov_rect = g.YFOV;
		im->dim.dpixsize_Y = im->dim.acqpixsize_Y = g.YFOV/im->dim.isoY;
	}

	if( g.UseAllSlices ) {
		g.FirstSlice = 0;
		g.LastSlice = im->dim.n_slices;
	}

	if( g.Slices == 0 ) {
		g.Slices = im->dim.n_slices;
	}

	if( g.Thickness != 0 ) {
		im->dim.slthick = g.Thickness;
	}

	return noErr;
}
	
int main( int argc, char **argv)
{
	OSErr   error = noErr;
	static  char id[] = "$Revision: 1.40 $$Date: 2003/04/16 13:27:19 $";
	char    versionStr[31];
	char    errString[256];
	IMAGE   im;
	short   output_datatype;
	short   ofile_type;	/* of output file */
	FILE    *inFile;
	short   ConversionRules = 0;

#ifdef mactest
	SIOUXSettings.autocloseonquit = false;
	SIOUXSettings.asktosaveonclose = false;
	argc = ccommand(&argv);
#endif

	g.Crop = true;
	g.XFOV = g.YFOV = -1.0;	// initialize to an unused value.
	g.SwapOutput = false;	// swapping is only turned on as needed
	if( macByteOrder() ) {
		g.OutByteOrder = kBE;
	} else {
		g.OutByteOrder = kLE;
	}
	sscanf( id, "%s%s", versionStr, versionStr );
	printf( "%s -- Version %s\n", argv[0], versionStr );

	initGlobal( &g );	// initialize values of imconvertGlobal
	
// Gather all user input parameters
	error = ProcessCommandLine( argc, argv );
	ILError( error, "error in command line" );

// Open a proc file to store user inputs and messages
	error = OpenProcFile( argc, argv, g.OutFileName, id, &g.ProcFile );
	ILError( error, "Opening \"proc\" file." );

// Determine the base name for the output file
	error = OutBaseNameFromOutName( g.OutFileName, g.OutBaseName );
	sprintf( errString, "Could not determine output name base from %s.\n", g.OutFileName );
	ILError( error, errString );

/* imconvert supports syntax emulation for an older more limited program that forces output to
bshort. Default behavior uses ImgLib function calls to determine the desired output type
based on the output file name. */
	if( !g.Emulation ) {
// find the user desired output type and make this the readimage default.
		error = FindOutputType( g.OutFileName, &ofile_type, &output_datatype );
		sprintf( errString, "Determining output type for %s. Function: FindOutputType\n", g.OutFileName );
		ILError( error, errString );

		if( g.ForceFloat && ofile_type == ANALYZE ) {
			output_datatype = T_FLOAT;
		}
	} else {
		output_datatype = T_USHORT;
		ofile_type = BSHORT;
	}

// read in the input image header
	error = UC_Readheader( g.InFileName, &im );
	sprintf( errString,"Could not process image header for %s. Function: UC_Readheader\n", g.InFileName );
	ILError( error, errString );

// Determine 3 dimensional field of view and pixel size from user input.
	error = SetFoV( &im );
	ILError( error, "Error setting output image field of view (Function: SetFoV)" );
	
// Determine need for byte swapping
	TestForSwap();

	error = RangeCheck( &im );
	ILError( error, "Function: RangeCheck" );

	if( g.tr != 0.0 ) {
		im.acq.tr = g.tr * 1000.0;
	}

// Set up cropping
	if( g.Crop ) {
		SET( im.dim.flags, kNeedXCrop );
	} else {
		CLEAR( im.dim.flags, kNeedXCrop );
	}

// always handle input data as float
	im.data_type = T_FLOAT;

// Advise the user
	if( g.Verbose ) {
		printf( "Converting from %s to %s file type\n",
                 ReportFileType( im.file_type ), ReportFileType( ofile_type ));
		if( g.Mult2Stack ) {
			printf( "Converting from multiple display to stack format\n" );
			fflush( stdout );
		}
		if( g.tr != 0.0 ) {
			printf( "tr was set to %.3f msec\n", g.tr );
		}
	}

	error = ck_fopen( &inFile, g.InFileName, "rb" );
	if( error ) {
		sprintf( errString, "Could not open %s.\n", g.InFileName );
		ILError( error, errString );
	}
	
	
/* imconvert supports two basic output modes: SaveBySlice collects all time points in a slice
as a single file. SaveByTimePoint keeps all slices at a given time point together */
	if( !g.SeparateSlices ) {	/* loop on time points */
		error = SaveByTimePoint( &im, ofile_type, output_datatype, inFile, &ConversionRules );
		ILError( error, "Function: SaveByTimePoint" );
	} else {
		error = SaveBySlice( &im, ofile_type, output_datatype, inFile, &ConversionRules );
		ILError( error, "Function: SaveBySlice" );
	}		

	error = ck_fclose( inFile );
	ILError( error, "infile" );
	
/* now, fix the first slice (in each volume) by moving the required number of lines
	if ( g.numLines != 0  ) {
printf( "fixing the lines \n" );

		// open output file for reading and read the corresponding header file
		error = ck_fopen( &inFile, g.OutFileName, "rb" );
		ILError( error, "Opening output file for reading" );
		
		error = UC_Readheader( g.OutFileName, &im );
		ILError( error, "Output image header" );
		
		// open temporary file to store fixed image data
		error = ck_fopen( &tmpFile, tmpFileName, "wb" );
		ILError( error, "Opening temp file" );
		
		//Fix it and save it to the tmpFile
		error = MoveLines( inFile, im, tmpFile );
		ILError( error, argv[0] );
		
		//close tmpFile and inFile
		error = ck_fclose( tmpFile );
		ILError( error, "Closing tmpFile" );
		
		error = ck_fclose( inFile );
		ILError( error, "Closing input File" );
		
		//make temp file the output file
		rename( tmpFileName, g.OutFileName );
		
	}
 */

/* dump processing file contents */
	if( g.Verbose ) fprintf( g.ProcFile, "\n" );
	
	fprintf( g.ProcFile,"TIME POINTS:       %d\n", im.dim.timePts );
	fprintf( g.ProcFile,"NUM SLICES:        %d\n", im.dim.n_slices );
	fprintf( g.ProcFile,"CONVERT MDISPLAY:  %s\n", YorN( g.Mult2Stack ));
	
	if ( g.numLines != 0 ) {
		fprintf( g.ProcFile, "LINES MOVED UP:	%hd\n", g.numLines );
	}

	if( g.Mult2Stack ) {
		short rows, cols;

		error = getRowsCols( g.Slices, &rows, &cols );
		ILError( error, "Getting number of rows and columns" );

		fprintf( g.ProcFile,"   ROWS:           %hd\n", rows );
		fprintf( g.ProcFile,"   COLUMNS:        %hd\n", cols );
		fprintf( g.ProcFile,"   OUTPUT X SIZE:  %hd\n", im.dim.isoX / cols );
		fprintf( g.ProcFile,"   OUTPUT Y SIZE:  %hd\n", im.dim.isoY / rows );
	}			

	fprintf( g.ProcFile,"SEPARATE TIME PTS: %s\n", YorN( g.SeparateTimePts ) );
	fprintf( g.ProcFile,"SEPARATE SLICES:   %s\n", YorN( g.SeparateSlices ) );
	fprintf( g.ProcFile,"THICKNESS USED:    %0.4f\n", g.Thickness );
	
	if( g.Mdisplay ) {
		fprintf( g.ProcFile,"MDISPLAY MODE:     %s\n", YorN( g.SeparateTimePts ) );
		fprintf( g.ProcFile,"   1st Slice:      %hd\n", g.FirstSlice );
		fprintf( g.ProcFile,"   Last Slice:     %hd\n", g.LastSlice );
	}

	fprintf( g.ProcFile,"\noutput file type:  %s\n", ReportFileType( ofile_type ) );
	fprintf( g.ProcFile,"output data type:  %s\n", ReportDataType( output_datatype ) );
	fprintf( g.ProcFile,"Byte swapping:     %s\n", YorN( g.SwapOutput ));

	error = ck_fclose( g.ProcFile );
	ILError( error, "Problem closing \"proc\" file." );
	
	return error;
}

/* functions */

/******************************    MoveLines     *************************************
*   This function fixes the problem that occurs in some of the functional datasets
*   where a few lines that should be on top, are wrapped to the bottom of the image 
*   (happens in the first slice only). The number of lines to move up is specified 
*   on the command line using the -l option.
*	zrinka 10/30/02
*************************************************************************************/
OSErr MoveLines( FILE *inFile, IMAGE inIm, FILE *tmpFile)
{
	OSErr   error = noErr;
	void    *inData, *slice;
	long    VolSize, inVolSize, SliceSize, inSliceSize;
	long	offset;
	short 	theTimePt;

	VolSize = inIm.dim.x * inIm.dim.y * inIm.dim.n_slices;

	inVolSize = VolSize * get_datasize( inIm.data_type );
	
	SliceSize = inIm.dim.isoX * inIm.dim.isoY;
	
	inSliceSize = SliceSize * get_datasize( inIm.data_type );

	inData    = ck_malloc( inVolSize,  "Storage for input data" );
	slice     = ck_malloc( inSliceSize,  "Storage for input slice" );
	
	offset = ( inIm.dim.y - g.numLines ) * inIm.dim.x * get_datasize( inIm.data_type );
	
	for ( theTimePt = 0; theTimePt < inIm.dim.timePts; theTimePt++ ) {
		error = GetSelectedVolume( inFile, &inIm, inData, theTimePt );
		RETURNONERROR;
	
/* copy the last u.numLines lines first */
		vmov( inData+offset, 1, slice, 1, inIm.dim.x * g.numLines * 2, get_datasize( inIm.data_type ) );
	
/* now copy the first (u.inIm.dim.y - u.numLines) */
		vmov( inData, 1, slice+inIm.dim.x*g.numLines*get_datasize( inIm.data_type ), 1, inIm.dim.x * (inIm.dim.y-g.numLines) * 2, get_datasize( inIm.data_type ) );

/* now copy the slice back to the volume */
		
		error = ck_fwrite( slice, 1, inSliceSize, tmpFile );
		error = ck_fwrite( inData+inSliceSize, 1, inVolSize - inSliceSize, tmpFile );
		RETURNONERROR;
	}

	free( slice );
	free( inData );

	return error;
}


void TestForSwap( void )
{
	if( (g.OutByteOrder == kLE) && macByteOrder() ) {
		if( g.Verbose ) {
			printf( "Output file is Little Endian and computer is Big Endian\nByte swapping enabled\n" );
		}
		g.SwapOutput = true;
	}

	if( (g.OutByteOrder == kBE) && !macByteOrder() ) {
		if( g.Verbose ) {
			printf( "Output file is Big Endian and computer is Little Endian\nByte swapping enabled\n" );
		}
		g.SwapOutput = true;
	}
	return;
}

void print_usage( char *name )
{
	printf( "USAGE: %s -i infile.ext1 -o outfile.ext2 [ M, T, A, #, t, s, m, O, X, Y, R, e, f, F, c, V]\n\n", name );
	printf( "General purpose file format conversion. File types are interpreted from the names\n" );
	printf( "The first two arguments are required\n" );
	printf( "\n\tDon't be put off, ONLY the first two are required, the rest are special purpose options\n" );
	printf( "\t-M scalar intensity multiplier\n" );
	printf( "\t-T slice thickness (for Analyze output)\n" );
	printf( "\t-A Analyze 3D format. Follow by slice thickness\n" );
	printf( "\t-# number of slices in 'mdisplay' format for infile\n" );
	printf( "\t-t separate file for each time point\n" );
	printf( "\t-s separate file for each slice\n" );
	printf( "\t-N Normalize full range of input intensities to full range of output type\n" );
	printf( "\t-m F-L save output files in mdisplay format (multiple slices/image)\n" );	
	printf( "\t       F and L are the first and last slice for mdisplay\n" );	
	printf( "\t       The option '-m A' will use all slices\n" );
	printf( "\t-O output byte order.\n" );
	printf( "\t        Enter -O L for pc/dec (little endian) or\n" );
	printf( "\t        -O B for mac/Sun/SGI (big endian)\n" );
	printf( "\t-X field of view (mm) in X\n" );
	printf( "\t-Y field of view (mm) in Y - enter only if different from field of view in X!\n" );
	printf( "\t-R Auto window input intensities to full range of output type\n" );
	printf( "\t-e emulate epi2bshort (but without offset artifacts!)\n" );
	printf( "\t-f force output type to float for Analyze files\n" );
	printf( "\t-F flip image top and bottom\n" );
	printf( "\t-c suppress cropping off the edges of images\n" );
	printf( "\t-V verbose operation\n" );
	printf( "saves a processing file \"outFile.%s.proc\"\n\n", name );
	printf( "Input files may be:\n\t.bfloat\n\t.bshort\n\t.buchar\n\t.img (Analyze)\n");
	printf( "\t.img (APD)\n\t.MR (Signa Genesis)\n" );
	printf( "Output files may be:\n\t.bfloat\n\t.bshort\n\t.buchar\n\t.img (Analyze)\n" );
	printf( "Output byte order is the same as that of the computer on which %s is run\n", name );
	printf( "\nEXAMPLES:\nConvert the file myIn.bshort to float\n" );
	printf( "> %s -i myIn.bshort -o myOut.bfloat\n\n", name );
	printf( "\t------------------\n\n" );
	printf( "Convert the mdisplay file myIn.bfloat, containing fifteen 5 mm thick slices\n" );
	printf( "to Analyze 3D format. Multiply all values by 100.\n" );
	printf( "> %s -i myIn.bfloat -o myOut.img -A 5 -# 15 -M 100\n", name );
	printf( "Output files are named myOut.tXXX.img, where XXX is the time point\n\n" );
	printf( "\t------------------\n\n" );
	printf( "Emulate epi2bshort, converting APD files to all time points at each slice\n" );
	printf( "> %s -i myIn_0123_001_003.img -e\n", name );
	printf( "Output files are named A1.sXX.bshort, where XX is the slice number\n\n" );
	printf( "\t------------------\n\n" );
	printf( "Convert a Signa genesis file to an Analyze file\n" );
	printf( "> %s -i E02334S5I11.MR -o Signa1.img\n\n", name );
	printf( "\t------------------\n\n" );
	printf( "Rescale data for use with 8-bit display programs (e.g., Photoshop)\n" );
	printf( "> %s -i myIn.bfloat -o EightBit.buchar -R\n\n", name );
	printf( "\t------------------\n\n" );
	printf( "Convert an input file into an mdisplay format using slices 3, 4, 5 and 6\n" );
	printf( "> %s -i myIn_0123_001_003.img -o myMDisplay.bshort -m 3-6\n\n", name );
	printf( "\t------------------\n\n" );
	printf( "Convert an input file into an mdisplay format using ALL slices\n" );
	printf( "> %s -i myIn_0123_001_003.img -o myMDisplay.bshort -m A\n\n", name );
	printf( "\t------------------\n\n" );
	printf( "More information can be found on the Brain Mapping lab HowTo pages\n" );
}

/******************************    ProcessCommandLine     ***************************
*   capture the user input, mostly globals
*************************************************************************************/
OSErr ProcessCommandLine( int argc, char *argv[] )
{
	OSErr error = noErr;

	int argp = EOF;
	char  MyArgString[256];

	while ( (argp=getopt(argc,argv,"o:i:M:T:A:#:m:O:X:Y:l:r:cRNetsfFSNVh?")) != EOF )
	switch( argp )   {
		case 'o':
			strcpy( g.OutFileName, optarg );
		break;

		case 'i':
			strcpy( g.InFileName, optarg );
		break;

		case 'M':
			g.Multiplier = atof(optarg);
			g.RescaleMode = kNoScale;
		break;

		case 'T':
			g.Thickness = atof( optarg );
		break;
		
		case 'A':
			g.Thickness = atof( optarg );
			g.SeparateTimePts = true;	/* provided for backward compatibility */
		break;
		
		case '#':
			g.Mult2Stack = true;
			g.Slices = atoi( optarg );
		break;
		
		case 'm':	/* mdisplay output */
			g.Mdisplay = true;
			strcpy( MyArgString, optarg );
			if( equalString( MyArgString, "A" )) {
				g.UseAllSlices = true;
			} else {
				sscanf( MyArgString, "%hd-%hd", &g.FirstSlice, &g.LastSlice );
				g.FirstSlice -= 1;
				g.UseAllSlices = false;
			}
		break;

		case 'O':
			strcpy( MyArgString, optarg );
			if( equalString( optarg, "L" )) {
				g.OutByteOrder = kLE;
			} else if( equalString( optarg, "B" )) {
				g.OutByteOrder = kBE;
			}
		break;

		case 'X':
			g.XFOV = atof( optarg );
		break;

		case 'Y':
			g.YFOV = atof( optarg );
		break;

		case 'r':
			g.tr = atof( optarg );
		break;

		case 'R':
			g.RescaleMode = kAutoRange;
		break;

		case 'N':
			g.RescaleMode = kFileMinMax;
		break;

		case 'c':
			g.Crop = false;
		break;

		case 't':
			g.SeparateTimePts = true;
		break;

		case 's':
			g.SeparateSlices = true;
		break;

		case 'e':	/* provided for backward compatibility */
			g.SeparateSlices = true;
			strcpy( g.OutFileName, "A1.bshort" );
			g.Emulation = true;
		break;

		case 'V':
			g.Verbose = true;
		break;

		case 'f':
			g.ForceFloat = true;
		break;

		case 'F':
			g.FlipY = true;
		break;
		
		case 'l':	/* number of lines to bring to front */
			g.numLines = atoi( optarg );
		break;

		case 'h':
		case '?':
		default:
			print_usage( argv[0] );
			exit( -1 );
	}
	
	if( !strlen( g.InFileName ) || !strlen( g.OutFileName )) {
		print_usage( argv[0] );
		exit( -1 );
	}

	if( g.SeparateTimePts && g.SeparateSlices ) {
		printf( "Sorry, the s and t (options) cannot be applied simultaneously!\n" );
		print_usage( argv[0] );
		exit( -1 );
	}

	if( g.Mdisplay && g.SeparateSlices ) {
		printf( "Sorry, the s (separate slices) and m (mdisplay) options cannot be applied simultaneously!\n" );
		print_usage( argv[0] );
		exit( -1 );
	}

	return error;
}

OSErr  RangeCheck( IMAGE *im )
{
	OSErr error = noErr;

	if( g.Mdisplay ) {
		if( im->dim.n_slices < (g.LastSlice - g.FirstSlice) ) {
			printf( "Sorry, the input file has only %hd slices, but you have asked for %hd.\n",
			        im->dim.n_slices, g.LastSlice - g.FirstSlice );
			error = DATA_OUT_OF_RANGE;
		}
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

/* Rescale the image intensities based on user settings, values in file and type range.
Input must be float */
OSErr Rescale( void *inData, IMAGE *im, float multiplier, short RescaleMode, short output_datatype )
{
	OSErr error = noErr;
	long  nPts = im->dim.isoX * im->dim.isoY * im->dim.n_slices;
	long  nonNoise;
	float fmax, fmin, outMax, outMin;
	
// Determine the possible outMin and outMax based on the data type
	SetTypeRange( &outMax, &outMin, output_datatype );
	
	switch( RescaleMode )
	{
		case kAutoRange:
			error = imageDataRange( inData, nPts, &fmax, &fmin, &nonNoise, T_FLOAT );
			multiplier = (float)vRescale( inData, nPts, &fmax, &fmin, &outMax, &outMin, T_FLOAT );
			if( g.Verbose ) {
				printf( "\tAutoRange: Data rescaled to fall between %0.2f and %0.2f\n", outMin, outMax);
				printf( "\tAutoRange: non-noise pts: %ld\n", nonNoise );
			}
			fprintf( g.ProcFile, "AUTORANGE: Data rescaled to fall between %0.2f and %0.2f\n", outMin, outMax);
			fprintf( g.ProcFile, "AUTORANGE: non-noise pts: %ld\n", nonNoise );
		break;
		
		case kFileMinMax:
			multiplier = (float)vRescale( inData, nPts, &im->theMaxf, &im->theMinf, &outMax, &outMin, T_FLOAT );
			if( g.Verbose ) {
				printf( "\tAutoRange: Data rescaled to fall between %0.2f and %0.2f\n", outMin, outMax);
			}
			fprintf( g.ProcFile, "AUTORANGE: Data rescaled to fall between %0.2f and %0.2f\n", outMin, outMax);
		break;
		
		case kManualRange:
			multiplier = (float)vRescale( inData, nPts, &im->theMaxf, &im->theMinf, &g.OutMax, &g.OutMin, T_FLOAT );			
		break;
		
		case kNoScale:
			if( multiplier != 1 ) {
				if( g.Verbose ) {
					printf( "Multiplying signal intensities by: %f\n", multiplier );
				}
				error = vfsmul( inData, 1, inData, 1, multiplier, nPts, T_FLOAT );
			}
		break;
		
		default:
			error = UNKNOWN_DISPLAY_MODE;
	}

	return error;
}

OSErr DoConversion( void *vec1, void *vec2, IMAGE *im, short output_datatype, short *rules )
/* data come in in vec1 and leave in vec2 */
{
	OSErr error = noErr;
	float  fmax, fmin;
	long nPts;
	short rows = 1;
	short cols = 1;
	short imXsize, imYsize;

	if( g.Mult2Stack ) {
		nPts = im->dim.isoX * im->dim.isoY;
	} else {
		nPts = im->dim.isoX * im->dim.isoY * g.Slices;
	}

	error = type_convert( vec1, im->data_type, vec2, output_datatype,
	    	                  nPts, &fmax, &fmin, rules );
	error = ReportConversionError( error, *rules );
	RETURNONERROR;

    if( g.Mult2Stack ) {

		error = getRowsCols( g.Slices, &rows, &cols );
		RETURNONERROR;;

		imXsize = im->dim.isoX / cols;
		imYsize = im->dim.isoY / rows;

		if( im->dim.isoX % cols || im->dim.isoY % rows ) {
			printf( "Something is wrong with the dimensions you entered!\n" );
			return DATA_OUT_OF_RANGE;
		}

		error = unmdisplay( vec2, vec1, imXsize, imYsize, g.Slices, output_datatype );
		RETURNONERROR;

		error = vmov( vec1, 1, vec2, 1, imXsize * imYsize * g.Slices, output_datatype );
		RETURNONERROR;

	}

	return error;
}

OSErr SaveByTimePoint( IMAGE *im, int file_type, short output_datatype, 
                       FILE *inFile, short *ConversionRules )
{
	OSErr   error = noErr;
	long    theTimePt;
	char    TimePointFileName[256];
	FILE    *outFile;
	void    *inData, *outData, *mdispBuff;
	long    inSliceSize, outSliceSize, outVolSize, inVolSize;
	long    inVolBytes, outVolBytes, inSliceBytes, outSliceBytes;
	long    nMDispPixels;
	long    VolYLines;	// total number of Y lines in an input volume
	short   Rows, Cols;
	long 	offset; 	//zrinka 10-29-02 - for firs slice fix
	void	*slice; 	//zrinka 10-29-02 - for firs slice fix

/* these variables are used for mdisplay: numSlices is the number of mdisplay'd slices
	off is the offset into the input array */
	short   numSlices = g.LastSlice - g.FirstSlice;
	void    *pv; // pv points to the offset of the first mdisplay'd slice in the output buffer
	long    offs = 0;

	IMAGE   hdrIm;

/* these are the sizes in pixels, not bytes... */
	VolYLines     = im->dim.isoY * im->dim.n_slices;
	inSliceSize   = im->dim.isoX * im->dim.isoY;
	outSliceSize  = im->dim.isoX * im->dim.isoY;
	inVolSize     = inSliceSize  * im->dim.n_slices; 
	outVolSize    = outSliceSize * im->dim.n_slices;

	inSliceBytes  = inSliceSize  * get_datasize( im->data_type );
	outSliceBytes = outSliceSize * get_datasize( output_datatype );
	inVolBytes    = inVolSize    * get_datasize( im->data_type );
	outVolBytes   = outVolSize   * get_datasize( output_datatype );
	
	inData  = ck_malloc( inVolBytes,  "Storage for input data" );
	outData = ck_malloc( outVolBytes, "Storage for output data" );
	
	if ( g.numLines != 0 ) {
		slice = ck_malloc( inSliceBytes, "Storage for slice" );
		offset = ( im->dim.isoY - g.numLines ) * im->dim.isoX * get_datasize( im->data_type );
	}

	error = SetHeaderDimensions( im, &hdrIm, file_type, output_datatype );
		RETURNONERROR;
	hdrIm.data_type = output_datatype;

/* To set up mdisplay, we need to know the number of Rows and Columns to use for the matrix'd
	view. This is determined using getRowsCols. We then allocate mdispBuff, as a place
	to hold the correct number of output images. 'pv' is set up to point into the
	type-converted array to find the first mdisplay'd slice. */

	if( g.Mdisplay ) {	
		error = getRowsCols( numSlices, &Rows, &Cols );
		RETURNONERROR;

		nMDispPixels = im->dim.isoX * im->dim.isoY * Rows * Cols;
		
		mdispBuff = ck_malloc( nMDispPixels * get_datasize( output_datatype ),
		                       "Storage for output data" );

		offs = g.FirstSlice * (outSliceBytes);
		pv = (char *)outData + offs;

/* The Header for the output image has to be be resized for mdisplay */
		hdrIm.dim.isoX = hdrIm.dim.isoX * Cols;
		hdrIm.dim.isoY = hdrIm.dim.isoY * Rows;
		hdrIm.dim.n_slices = 1;
	}

	if( !g.SeparateTimePts ) {
		error = ck_fopen( &outFile, g.OutFileName, "wb" );
			RETURNONERROR;
	} else { 
		hdrIm.dim.timePts = 1;
	}
	
	if( g.OutByteOrder == kLE ) {
		CLEAR( hdrIm.dim.flags, kmacByteOrder );
	} else {
		SET( hdrIm.dim.flags, kmacByteOrder );
	}

	for( theTimePt = 0; theTimePt < im->dim.timePts; theTimePt++ ) {
		int ans;

		if( PercentCount( theTimePt, im->dim.timePts, 10, &ans )) {
			printf( "\t%d%% complete\n", ans );
		}

		if( g.SeparateTimePts ) {	/* create a file for each time point */
			sprintf( TimePointFileName, "%s.t%0.3d", g.OutBaseName, theTimePt+1 );

			strcat( TimePointFileName, strrchr( g.OutFileName, '.') );
			error = ck_fopen( &outFile, TimePointFileName, "wb" );
			RETURNONERROR;
		}

/* data are always read in their current type */
		error = GetSelectedVolume( inFile, im, inData, theTimePt );
			RETURNONERROR;
			
/* fix the first slice in volume if specified, by movid lines from the bottom to the top of first slice */
		if ( g.numLines != 0 ) {
			// copy the last g.numLines lines first
			vmov( inData+offset, 1, slice, 1, im->dim.isoX * g.numLines * 2, get_datasize( im->data_type ) );
	
			// now copy the first (im.dim.y - g.numLines)
			vmov( inData, 1, slice+im->dim.isoX*g.numLines*get_datasize( im->data_type ), 1, im->dim.isoX * (im->dim.isoY-g.numLines) * 2, get_datasize( im->data_type ) );
			
			// and finally copy the slice back to inData
			vmov( slice, 1, inData, 1, inSliceSize * 2, get_datasize( im->data_type ) );
		}

		if( g.FlipY ) {	// flip each slice in the input file
			int  mySlice;
			char *SliceOffset;

			for( mySlice=0; mySlice<im->dim.n_slices; mySlice++ ) {
				SliceOffset = (char *)inData + mySlice * inSliceBytes;
				error = ImageFlipY( (void *)SliceOffset, im->dim.isoX, im->dim.isoY,
				                    (void *)SliceOffset, im->data_type );
				ILError( error, "Image flip");
			}
		}

		error = Rescale( inData, im, g.Multiplier, g.RescaleMode, output_datatype );
		RETURNONERROR;

		error = DoConversion( inData, outData, im, output_datatype, ConversionRules );
		RETURNONERROR;

		if( g.Mdisplay ) {
			error = mdisplay( mdispBuff, pv, im->dim.isoX, im->dim.isoY, numSlices, output_datatype );
			RETURNONERROR;
			
			if( g.SwapOutput ) {
				error = vbyteswap( mdispBuff, nMDispPixels, output_datatype );
				RETURNONERROR;

			}
			error = ck_fwrite( mdispBuff, 1, nMDispPixels * get_datasize( output_datatype ), outFile );
			RETURNONERROR;

		} else {
			if( g.SwapOutput ) {
				error = vbyteswap( outData, outVolSize, output_datatype );
				RETURNONERROR;
			}

			error = ck_fwrite( outData, 1, outVolBytes, outFile );
		}

		if( g.SeparateTimePts ) {
			char myName[256];

			error = ck_fclose( outFile );
			ILError( error, "out file" );
			if( g.Verbose ) {
				printf( "\t%s: ", TimePointFileName );
				fprintf( g.ProcFile, "%s: time point: %d\n", TimePointFileName, theTimePt + 1 );
			}
			hdrIm.theMinf = im->theMinf * g.Multiplier;
			hdrIm.theMaxf = im->theMaxf * g.Multiplier;

			sprintf( myName, "%s.t%0.3d", g.OutBaseName, theTimePt+1 );

			error = CreateHeader( file_type, &hdrIm, myName );
			RETURNONERROR;
		}

		if( g.Verbose ) {
			printf( "\tTime point %d of %d\n", theTimePt+1, im->dim.timePts );
		}
	}

	if( !g.SeparateTimePts ) {
		error = ck_fclose( outFile );
		ILError( error, "outfile" );

		hdrIm.theMinf = im->theMinf * g.Multiplier;
		hdrIm.theMaxf = im->theMaxf * g.Multiplier;
		error = CreateHeader( file_type, &hdrIm, g.OutBaseName );
		RETURNONERROR;
	}

	memFree( (void **)&inData, "inData" );
	memFree( (void **)&outData, "outData" );
	
	if ( g.numLines != 0 ) memFree( (void **)&slice, "slice" );
	
	if( g.Mdisplay ) {
		memFree( (void **)&mdispBuff, "mdispBuff" );
	}

	return error;
}	/* SaveByTimePoint */

/************************	SaveBySlice   ***************************
* SaveBySlice
*
* 	Primarily to support emulation of epi2bshort, this routine moves
*	the data from the input file one slice at a time to create a file
*	containing all time points at each slice location.
*********************************************************************/
OSErr SaveBySlice( IMAGE *im, int file_type, short output_datatype, 
                       FILE *inFile, short *ConversionRules )
{
	OSErr  error = noErr;
	long   theSlice, theTimePt;
	char   SliceFileBaseName[256];
	char   SliceFileName[256];
	FILE   *outFile;
	IMAGE  hdrIm, sizeIm;	/* slice-wise file saving would require confusing changes to the global im */
	void   *inData, *outData;
	int    SaveSlices = g.Slices;	
	long   outSliceSize, inSliceSize, inSliceBytes, outSliceBytes;
	
	long 	offset; 	//zrinka 10-29-02 - for first slice fix
	void	*slice; 	//zrinka 10-29-02 - for first slice fix
	
	inSliceSize  = im->dim.isoX * im->dim.isoY;
	outSliceSize = im->dim.isoX * im->dim.isoY;

	inSliceBytes  = inSliceSize * get_datasize( im->data_type );
	outSliceBytes = outSliceSize * get_datasize( output_datatype );

	inData  = ck_malloc( inSliceBytes,  "Storage for input data" );
	outData = ck_malloc( outSliceBytes, "Storage for output data" );
	error = SetHeaderDimensions( im, &hdrIm, file_type, output_datatype );
		RETURNONERROR;
		
	if ( g.numLines != 0 ) {
		slice = ck_malloc( inSliceBytes, "Storage for slice" );
		offset = ( im->dim.isoY - g.numLines ) * im->dim.isoX * get_datasize( im->data_type );
	}
	
/* sizeIm is used to keep track of dimensions during conversions */
	memcpy( (void *)&sizeIm, im, sizeof( IMAGE ));
	sizeIm.dim.n_slices = 1;

	g.Slices = 1; /* over-ride for local use */
	
	for( theSlice = 0; theSlice < im->dim.n_slices; theSlice++ ) {
		int ans;

		if( PercentCount( theSlice, im->dim.n_slices, 10, &ans )) {
			printf( "\t%d%% complete\n", ans );
		}

		if( g.Emulation ) {
			sprintf( SliceFileBaseName, "A1.s%d", theSlice+1 );
		} else {
			sprintf( SliceFileBaseName, "%s.s%0.3d", g.OutBaseName, theSlice+1 );
		}

		if( g.OutByteOrder == kLE ) {
			CLEAR( hdrIm.dim.flags, kmacByteOrder );
		} else {
			SET( hdrIm.dim.flags, kmacByteOrder );
		}

		sprintf( SliceFileName, "%s%s", SliceFileBaseName, strrchr( g.OutFileName, '.') );
		error = ck_fopen( &outFile, SliceFileName, "wb" );

		for( theTimePt = 0; theTimePt < im->dim.timePts; theTimePt++ ) {
			error = UC_Readimage( inFile, im, inData, theSlice, theTimePt );

			ILError( error, "Read Image" );
			
/* fix the first slice in volume if specified, by movid lines from the bottom to the top of first slice */
			if ( g.numLines != 0 && theSlice == 0 ) {
				// copy the last g.numLines lines first
				vmov( inData+offset, 1, slice, 1, im->dim.isoX * g.numLines * 2, get_datasize( im->data_type ) );
	
				// now copy the first (im.dim.y - g.numLines)
				vmov( inData, 1, slice+im->dim.isoX*g.numLines*get_datasize( im->data_type ), 1, im->dim.isoX * (im->dim.isoY-g.numLines) * 2, get_datasize( im->data_type ) );
			
				// and finally copy the slice back to inData
				vmov( slice, 1, inData, 1, inSliceSize * 2, get_datasize( im->data_type ) );
			}


			if( g.FlipY ) {	// flip each slice in the input file
				int  mySlice;
				char *SliceOffset;

				for( mySlice=0; mySlice<im->dim.n_slices; mySlice++ ) {
					SliceOffset = (char *)inData + mySlice * inSliceBytes;
					error = ImageFlipY( (void *)SliceOffset, im->dim.isoX, im->dim.isoY,
					                    (void *)SliceOffset, im->data_type );
					ILError( error, "Image flip");
				}
			}

			error = Rescale( inData, &sizeIm, g.Multiplier, g.RescaleMode, output_datatype );
			RETURNONERROR;
	
			error = DoConversion( inData, outData, &sizeIm, output_datatype, ConversionRules );
			RETURNONERROR;
	
			if( g.SwapOutput ) {
				error = vbyteswap( outData, outSliceSize, output_datatype );
				RETURNONERROR;
			}

			error = ck_fwrite( outData, 1, outSliceBytes, outFile );
			RETURNONERROR;
		}

		hdrIm.data_type = output_datatype;
		hdrIm.theMinf *= g.Multiplier;
		hdrIm.theMaxf *= g.Multiplier;
		error = CreateHeader( file_type, &hdrIm, SliceFileBaseName );
		RETURNONERROR;

		if( g.Verbose ) {
			printf( "%s: Slice %d of %d\n", SliceFileName, theSlice+1, im->dim.n_slices );
			fprintf( g.ProcFile, "%s: Slice %d of %d\n", SliceFileName, theSlice+1, im->dim.n_slices );
		}

		error = ck_fclose( outFile );
		RETURNONERROR;
	}
	
	if( g.Verbose ) {
		fprintf( g.ProcFile, "%s %d %d\n", SliceFileName, theSlice+1, im->dim.n_slices );
	}

	g.Slices = SaveSlices;	/* restore (not necessary at this time) */

	memFree( (void **)&inData, "inData" );
	memFree( (void **)&outData, "outData" );
	
	if ( g.numLines != 0 ) memFree( (void **)&slice, "slice" );

	return error;
}	/* SaveBySlice */

/***********************    SetHeaderDimensions      *******************
*	SetHeaderDimensions
*
*	Establish dimensions to use for this header, based on the file type
*	Conventions used:
*		isoX     - xsize once pixels have been made isotropic
*		isoY     - ysize once pixels have been made isotropic
*		n_slices - If multidisplay, volume is compressed to a single image
*				so n_slices is 1. Otherwise it is the actual number of input slices
*
*	im          - IMAGE struct of input image
*	hdrIm       - IMAGE struct of output image
*	file_type   - file type for output file
*	dat_type    - desired output data type.
*
***********************************************************************/
OSErr SetHeaderDimensions( IMAGE *im, IMAGE *hdrIm, int file_type, short data_type )
{
	OSErr  error = noErr;
	short  rows = 1;
	short  cols = 1;
	
	memcpy( hdrIm, im, sizeof( IMAGE ));
	hdrIm->data_type = data_type;
	
	if( g.Mult2Stack ) {
		error = getRowsCols( g.Slices, &rows, &cols );
		RETURNONERROR;

		hdrIm->dim.isoX = hdrIm->dim.x = im->dim.isoX / cols;
		hdrIm->dim.isoY = hdrIm->dim.y = im->dim.isoY / rows;
		hdrIm->dim.n_slices = 1;

	} else {	
		if( file_type == BSHORT || file_type == BFLOAT || file_type == BUCHAR ) {
			if( !g.SeparateSlices && !g.Mdisplay ) {

/* This is the source of much confusion. In MGH image types, we must multiply the number of y
lines by the number of slices. This is done in CreateHeader (not here ) */
				hdrIm->dim.isoY = hdrIm->dim.y;
			}
		}
	}

	if( g.SeparateTimePts ) {
		hdrIm->dim.timePts = 1;
	}
	
	if( g.SeparateSlices ) {
		hdrIm->dim.n_slices = 1;
	}

	return error;
}

OSErr ReportConversionError( OSErr error, short rules )
{
	if( error != CONVERSION_ERROR ) {
		return error;
	}

	if( g.Verbose ) {
		if( rules & kDataRescaled ) {
			printf( "The input data was rescaled to maintain an adequate intensity resolution.\n" );
		}
		if( rules & kRescaledNarrowRange ) {
			printf( "The input data range was too narrow, and was rescaled accordingly.\n" );
		}
	}

	return noErr;
}
/* </pre></body></html> */
