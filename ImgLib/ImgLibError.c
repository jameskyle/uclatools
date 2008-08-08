/*<HTML><HEAD><TITLE>ImgLib Error routines</TITLE></HEAD><BODY><PRE>
*<font size=+2><b>ImgLibError.c</b></font>
*	<i>Please note last saved date below:</i>
*	<font size=+1><b>$Date: 2002/09/04 12:13:56 $</b></font><p>
*    A set of primitives for use in image processing.
*               c 1996-2002  Mark S. Cohen
*    This software is distributed as is with no guarantees.
*    Please report any errors or bug fixes to mscohen@ucla.edu
*
*    For academic use only. Commercial users should contact
*    Mark Cohen for licensing information.
*
<!-- **********************************************************************
*  $Log: ImgLibError.c,v $
*  Revision 1.21  2002/09/04 12:13:56  mscohen
*  Added AFNI supprt (Zrinka)
*
 *  Revision 1.21  2002/09/04 12:12:19  mscohen
 *  Added AFNI support (Zrinka)
 *
 * Revision 1.20  2002/04/16  17:02:05  mscohen
 * copyright
 *
 * Revision 1.19  2002/04/16  16:05:41  mscohen
 * html
 * */
#include "ImgLibError.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>

#define noErr 0
/* --> ***********************************************************************
prints a formatted error message to stdout
void ILError( short theError,       -- error code from calling routine
              char *errText )       -- desired message text
<!-- ************************************************************************/
void ILError( short theError, char *errText )
{
	char errString[256];
	char unixErr[128];

	if( theError == noErr ) {
		return;
	}

	printf( "\n\n***** ImbLib Error handler *****\n" );
	sprintf( unixErr, "\n===> unix error: %s", strerror( errno) );

	switch( theError )
	{
		case UNKNOWNTYPE:
			strcpy( errString, "An ImgLib function received an unrecognized data type as an argument." );
			break;
		case NOTAFNITYPE:
			strcpy( errString, "AFNI does not support the image data type. Recognized types: char, short, float, complex." );
			break;

/* math and memory */
		case DIVIDE_BY_ZERO_ERR:
			strcpy( errString, "Divide by zero error." );
			break;
		case INVALID_ADDRESS:
			strcpy( errString, "A function returned an invalid memory address." );
			break;
		case ARRAY_RANGE_ERR:
			strcpy( errString, "Array range error." );
			break;
		case NIL_PTR:
			strcpy( errString, "An ImgLib function received a pointer to NIL." );
			break;
		case NILIMGPTR:
			strcpy( errString, "The image pointer is NIL." );
			break;
		case OUT_OF_MEMORY:
			strcpy( errString, "There is not enough memory to complete this operation." );
			break;
		case MEMALLOCERROR:
			strcpy( errString, "Memory error - usually allocation." );
			break;
		case NEG_DATA_NOT_ALLOWED:
			strcpy( errString, "This function accepts only positive inputs." );
			break;
		case MEM_ALREADY_ALLOCATED:
			strcpy( errString, "Attempted to reallocated already assigned pointer." );
			break;

/* functions */
		case OVERLAY_FAILURE:
			strcpy( errString, "The overlay routine failed." );
			break;
		case MDISPLAY_ERROR:
			strcpy( errString, "The mdisplay routine failed." );
			break;
		case NEG_DATA_IN_AUTORANGE:
			strcpy( errString, "Auto range does not function properly with negative input data." );
			break;
		case CANT_HAVE_NEG_THRESHOLD:
			strcpy( errString, "The threshold cannot be negative." );
			break;
		case SCALE_FACTOR_ZERO:
			strcpy( errString, "A scale factor of zero was found." );
			break;
		case VOLUME_DISPLAY_INVALID:
			strcpy( errString, "This volume display is not available." );
			break;
		case DATA_OUT_OF_RANGE:
			strcpy( errString, "The data exceeds the valid range for this function." );
			break;
		case TIMEOUTERROR:
			strcpy( errString, "This functions halted while waiting for data." );
			break;
		case CONVERSION_ERROR:
			strcpy( errString, "There was a problem during data type conversion." );
			break;
		case SPARSE_DATA_VALUES:
			strcpy( errString, "The input data contained a limited range of values:" );
			strcat( errString, " auto detection of intensity range may be poor." );
			break;
		case RESCALE_NEEDED:
			strcpy( errString, "To preserve image quality, your data were rescaled; the resolution" );
			strcat( errString, " of the output type was less than that of the original data." );
			break;
		case UNKNOWN_DISPLAY_MODE:
			strcpy( errString, "An unrecognized display mode was selected." );
			break;

/* headers */
		case IMG_HEADER_ERROR:
			strcpy( errString, "There was a problem opening an image header" );
			strcat( errString, unixErr );
			break;
		case COULD_NOT_OPEN_HEADER:
			strcpy( errString, "The header could not be opened" );
			strcat( errString, unixErr );
			break;
		case INVALID_GENESIS_HDR:
			strcpy( errString, "The file does not containa valid genesis (Signa) header" );
			break;
		case HEADER_SIZE_MISMATCH:
			strcpy( errString, "The header is the wrong size." );
			break;
		case INVALID_HDR:
			strcpy( errString, "The header contains invalid data." );
			break;
		case MISSING_IMG_HEADER:
			strcpy( errString, "The image file does not have a corresponding header." );
			strcat( errString, unixErr );
			break;
		case HEADER_INTERP_FLAG:
			strcpy( errString, "The header requests interpolation in both X and Y. " );
			strcat( errString, "It is probably corrupted." );
			break;
		case COULD_NOT_OPEN_IMAGE:
			strcpy( errString, "The Image could not be opened." );
			strcat( errString, unixErr );
			break;
		case IMG_FILE_ERROR:
			strcpy( errString, "There is a problem with the image file." );
			strcat( errString, unixErr );
			break;
		case PARADIGM_FILE_ERROR:
			strcpy( errString, "Having trouble with that paradigm file\n" );
			strcat( errString, unixErr );
			break;
		case BEYOND_EOF:
			strcpy( errString, "Premature end of file." );
			strcat( errString, unixErr );
			break;
		case FILE_NOT_FOUND:
			strcpy( errString, "A required file could not be found." );
			strcat( errString, unixErr );
			break;
		case UNABLE_TO_OPEN_LOGFILE:
			strcpy( errString, "The log file could not be opened." );
			strcat( errString, unixErr );
			break;
		case UNKNOWNFILETYPE:
			strcpy( errString, "This image file type is not recognized." );
			break;
		case WRITE_ERROR:
			strcpy( errString, "A file could not be written successfully." );
			strcat( errString, unixErr );
			break;
		case READ_ERROR:
			strcpy( errString, "A file could not be read successfully." );
			strcat( errString, unixErr );
			break;
		case FOPEN_ERROR:
			strcpy( errString, "A file could not be opened successfully." );
			strcat( errString, unixErr );
			break;
		case FCLOSE_ERROR:
			strcpy( errString, "A file could not be closed successfully." );
			strcat( errString, unixErr );
			break;
		case FILE_ERROR:
			strcpy( errString, "There is a problem with the selected file");
			strcat( errString, unixErr );
			break;
		case DATATYPE_RANGE_ERROR:
			strcpy( errString, "The data values exceed the range for the selected data type." );
			break;
		case INVALID_FILENAME:
			strcpy( errString, "The file name is not valid." );	/* allows calling routine to control message */
			break;
		case READ_OPEN_FAILURE:
			strcpy( errString, "The file could not be opened for reading." );
			break;
		case WRITE_OPEN_FAILURE:
			strcpy( errString, "The file could not be opened for writing." );
			break;
		case APPEND_OPEN_FAILURE:
			strcpy( errString, "The file could not be opened for appending." );
			break;
		case SILENT_ERR:
			strcpy( errString, "" );	/* allows calling routine to control message */
			break;
		default:
			strcpy( errString, "An unknown error occured." );
			strcat( errString, unixErr );
			break;
	}
	printf( "\nAn internal program error occured in an ImgLib routine.\n" );
	printf( "The reported error was:\n===> %s\n", errString );
	if( strlen( errText )) {
		printf( "\nThe error message was:\n===> %s\n\n", errText );
	}
	printf( "Sometimes, you can find the source of the error by examining the processing\n" );
	printf( "files created by the program you are running (they typically end in .proc )\n" );
	printf( "\nIf you cannot resolve the problem, contact Mark Cohen (mscohen@ucla.edu)\n" );
	fflush( stdout );
	abort();
}
/* --> </BODY></HTML> */

