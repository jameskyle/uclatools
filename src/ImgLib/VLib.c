/* <HTML><HEAD><TITLE>VLib routines</TITLE></HEAD><BODY><PRE><!--
*******************************************************************
-->
* <font size=+2><b>VLib - vector processing and utility library</b></font>
* <i>Please note saving date below:</i>
* <font size=+1><b>$Date: 2007/06/27 17:33:26 $</b></font><p>
*    A set of primitives for use in image processing.
*               c 1996-2002  Mark S. Cohen
*    
*    Please do not distribute without express permission of the author.
*
*    Users making changes or extensions to this software are instructed
*    to notify the author of any such changes (in the interest of keeping
*    this material up to date and bug-free)
*
*    The author makes no representation of the suitability of this software
*    for any particular purposes, nor does he certify the accuracy of the
*    algorithms provided.
*
*    Please report any errors or bug fixes to mscohen@ucla.edu
*
*    For academic use only. Commercial users should contact
*    Mark Cohen for licensing information.
*
*    Keep track of the differences between vlen and npts as the description
*    of vector sizes. While vlen is supposed to be used to represent the
*    length of the input vector, npts is to represent the number of points
*    actually processed. Use might not be entirely consistent.
*
*    Most of the routines that ask for an output vector cannot be used as
*    in-place operations.
* <!--
* $Log: VLib.c,v $
* Revision 1.48  2007/06/27 17:33:26  mscohen
* Fixed type conversions for complex data
* Changed argument list for vmag (now assumes that input data are float)
*
* Revision 1.47  2006/06/09 18:48:00  mscohen
* code cleanup
*
* Revision 1.44  2006/03/26 06:08:47  mscohen
* type convert now works with complex
* added vfsmul
* complex vadd, vsub, vsmul, vminmax, vfminmax, vsqrt, vsq, vssq, vsum, vdiv and vmul
*
* Revision 1.45  2006/03/25 08:45:11  mscohen
* Complex math for multiply and divide
* Improved handling of complex data in type_convert
* vfminmax
*
* Revision 1.43  2004/01/30 21:55:35  mscohen
* Fixed a typo in countwords
*
* Revision 1.42  2003/12/15 05:23:35  mscohen
* Added CountWords utility
*
* Revision 1.41  2002/12/17 06:21:53  mscohen
* Added vabs function
*
* Revision 1.39  2002/09/11 16:59:46  mscohen
* Removed CK_ functions - failed experiment.
*
* Revision 1.38  2002/09/10 00:51:13  nina
* changed fopen mode from "r"/"w" to "rb"/"wb" as appropriate.
*
* Revision 1.37  2002/09/04 17:40:49  mscohen
* Created GetVLibVer()
*
* Revision 1.36  2002/09/03 19:25:36  mscohen
* Fixed a bug in GetFileSize that left files open.
*
* Revision 1.36  2002/09/03 19:24:02  mscohen
* Upgraded to local RCS file.
*
* Revision 1.24  2002/08/29 07:43:00  mscohen
* Fixed bug in GetFileSize which kept files open after checking size.
*
 * Revision 1.35  2002/06/26  13:18:09  mscohen
 * Changed unsigned short images to signed short: changed type_convert to support this
 * Numerous cosmetic and commenting changes.
 *
 * Revision 1.34  2002/05/01  03:02:17  mscohen
 * Make fill value in vfill local for speed.
 *
 * Revision 1.34  2002/05/01  03:02:17  mscohen
 * Make fill value in vfill local for speed.
 *
 * Revision 1.33  2002/04/17  13:56:55  mscohen
 * changed ck_fwrite to post errno errors on write failure.
 *
 * Revision 1.32  2002/04/17  03:05:38  mscohen
 * Fixed ck_fopen by removing extraneous debug and adding FILE_NOT_FOUND error.
 *
 * Revision 1.32  2002/04/17  03:05:38  mscohen
 * Fixed ck_fopen by removing extraneous debug and adding FILE_NOT_FOUND error.
 *
 * Revision 1.31  2002/04/16  17:01:41  mscohen
 * copyright
 *
 * Revision 1.30  2002/04/16  16:01:50  mscohen
 * html
 *
 * Revision 1.29  2002/04/16  15:49:57  mscohen
 * Made saving date more prominent in html
 *
 * Revision 1.28  2002/04/12  05:50:15  mscohen
 * Added convert to percent utility
 *
 * Revision 1.27  2002/01/03  21:51:18  mscohen
 * Minor upgrades
 *
 * Revision 1.26  2001/12/11  04:13:55  mscohen
 * Added PercentCout utility
 *
 * Revision 1.25  2001/12/04  04:26:52  mscohen
 * Added utility function CreateTempFile.
 *
 * Revision 1.23  2001/08/24  17:00:58  mscohen
 * Made Id part of viewable html
 *
 * Revision 1.22  2001/08/24  13:32:56  mscohen
 * HTML beautification
 *
 * Revision 1.21  2001/08/23  21:15:34  mscohen
 * Fixed html commenting
 *
 * Revision 1.21  2001/08/23  21:15:34  mscohen
 * Fixed html commenting
 *
 * Revision 1.20  2001/08/23  19:56:00  mscohen
 * Added MoveSwap to facilitate header translation for unaligned structs
 *
 * Revision 1.19  2001/07/03  20:00:33  zrinka
 * in vfill: p set to be incremented after memcpy
 *
 * Revision 1.18  2001/04/18  21:18:32  mscohen
 * Modified handling of ReadLastBytes and GetFileSize
 *
 * Revision 1.17  2001/03/29  02:19:02  mscohen
 * added GetFileSize function
 *
 * Revision 1.16  2001/03/17  06:21:05  mscohen
 * syntax error in equalString
 *
 * Revision 1.15  2001/03/17  05:52:31  mscohen
 * renamed EqualString to equalString to avoid Macintosh name conflict.
 *
 * Revision 1.14  2001/03/17  04:06:06  mscohen
 * Added vfill function
 *
 * Revision 1.13  2001/03/15  15:41:30  mscohen
 * Added equalString utility
 *
 * Revision 1.12  2001/02/28  14:38:44  mscohen
 * Update copyright notice
 *
 * Revision 1.17  2000/10/26  03:05:53  mscohen
 * fix range bug in type_convert for bshort
 *
 * Revision 1.16  2000/10/26  01:58:13  mscohen
 * Fixed bug that forced rescaling of buchar output in type_convert
 *
 * Revision 1.15  2000/10/15  10:53:21  mscohen
 * New scaling in SetTypeRange (unsigned short)
 *
 * Revision 1.14  2000/09/12  01:04:55  mscohen
 * Fixed a bug in type_convert that caused 0 images to be rescaled.
 *
 * Revision 1.13  2000/08/28  21:56:00  mscohen
 * Added SetTypeRange function
 * Removed unused variables in type_convert
 * Added support for null type conversion in type_convert
 *
 * Revision 1.12  2000/08/23  15:21:02  mscohen
 * added function ReportDataType
 *
 * Revision 1.9  2000/05/02  20:18:23  mscohen
 * Big fix in vminmax (thanks to M. Satterfield
 *
 * Revision 1.7  2000/03/26  11:49:22  mscohen
 * Modified handling of vsmul for more flexibel operation with multiple vectors.
 *
 * Revision 1.5  2000/03/08  19:36:46  mscohen
 * Added more ck_f functions
 *
 * Revision 1.4  2000/02/15  15:54:39  mscohen
 * Rename BigEndian and LittleEndian functions
 *
 * Revision 1.3  1999/09/28  04:10:09  mscohen
 * Fixed set and clear flags for handing byte order
 *
 * Revision 1.2  1999/09/22  15:28:57  mscohen
 * Added DBG function.
 *
 * Revision 1.7  1999/07/14  05:53:48  mscohen
 * ANSI C compatibilityANSI C compatibility
 *
 * Revision 1.6  1999/05/30  12:24:24  mscohen
 * Patches to function names. vdiv now returns OSErr.
 *
 * Revision 1.5  1999/05/28  22:14:58  mscohen
 * Bug fixes in vsmul
 *
 * Revision 1.4  1999/04/20  00:30:48  mscohen
 * Made gMessageStr a global from VLib
 *
 * Revision 1.3  1999/04/20  00:06:10  mscohen
 * Modified Error handling globally.
 *
 * Revision 1.2  1999/02/28  21:07:58  mscohen
 * Cleaned up handling of OSErr
 *
 * Revision 1.1  1999/02/28  19:57:41  mscohen
 * Initial revision
 *
* 
*	VLib.c
*	A basic vector processing library - mostly utilities
*	c1999-2001 Mark S. Cohen
*********************************************************************/
#include <errno.h>
#ifdef mactest
#define ENOENT 2
#endif

#include "VLib.h"
#include "MathLib.h"
#include <time.h>	

char gMessageStr[255];

#ifdef POST_DBG
char gDebugString[256];
#endif

/* because these externs are only defined if logging is initiated,
	ImgLib will not fault out at compile if no MessageLog global is defined */

/* --> *******************************************************************************
OSErr <B> InitMessageLog </b> ( void )
*   Set up a file based message log for debugging and user support purposes
<!--   *******************************************************************************/
OSErr	InitMessageLog( void )
{
#ifdef LogToDatedFile
	char str1[128];
	char str2[128]; /* file name */
	time_t now;
	struct tm *date;
	extern FILE *MessageLog;

/* for some reason, if this is included as part of main, it does not put up that damned SIOUX window */
	if( MessageLog == NULL ) {
	    now = time ( NULL );
	    date = localtime ( &now );
		strftime( str2, 128, "LOGFILE.%a", date );
		MessageLog = fopen( str2, "a" );
		if( MessageLog == NULL ) {
			return UNABLE_TO_OPEN_LOGFILE;
		}
	}
#endif
	return noErr;
}

/* --> *******************************************************************************
* <b> LogMessage </b> 
*  Unified Error logging
*
*	Requires that a global
*		FILE *MessageLog;
*
*	exist (i.e., in main() ) in order that logging can take place to
*	screen or to a dated weekly rotating file. Adds the contents of MessageString
*   to the log.
*
*   OSErr	LogMessage( char *MessageString )
*
<!--   *******************************************************************************/
OSErr	LogMessage( char *MessageString )
{
	OSErr error = noErr;
#ifdef NoLog
	return noErr;
#else
	Boolean dbg = true;  /* be silent by default */
	char str1[128];
	char str2[128]; /* file name */
	time_t now;
	struct tm *date;
	static Boolean opened = false;	/* set if the Message log is initialized */

#ifdef LogToDatedFile
	extern FILE *MessageLog;
#endif
#ifdef LogToScreen
#define MessageLog stdout
#endif
	
	if (dbg) {
	    now = time ( NULL );
	    date = localtime ( &now );
	    strftime( str1, 128, "%A, %B %d, %Y %I:%M:%S %p", date ); 

 /* LOGFILE.Mon */
		error = InitMessageLog();

/* Put the date and opening line in, the first time that Messaging is accessed */
		if( !opened ) {
			sprintf( str2,"\n\nHello World from LogMessage at : %s\n", str1);
			fprintf( MessageLog, str2 );
			opened = true;
		}

		fprintf( MessageLog, MessageString );
		if( MessageString[ strlen( MessageString ) -1 ] != '\n' ) {
			fprintf( MessageLog, "\n" );		
			fprintf( MessageLog, "\n" );		
		}
		fflush( MessageLog );
	}
#endif
	return error;
}

/* --> ***********************************************************************
*       Keep control of ImgLib revisions
*   <b>void GetVLibVer( char *buff )</b>
*
*   Returns the version number for VLib in buff
*
<!-- ************************************************************************/
void GetVLibVer( char *buff ) /* return a string describing VLib revision */
{
   static char id[] = "$Revision: 1.48 $$Date: 2007/06/27 17:33:26 $";
   char versionStr[31];
   char buffer[63];

/* extract version and date */
   sscanf( id, "%s%s%s%s", buffer, versionStr, buffer, buffer );
   strcat( versionStr, " " );
   strcat( versionStr, buffer );
   strcpy( buff, versionStr );
}

/* --> *******************************************************************************
* <b> DBG </b> 
*  Handy debugging utility.
*
*     void DBG ( char *msg )
*
*  Uses stdout to print debug statements. Increments a counter with each call
*  and displays the message string in the format:
*       ****	1: message	****
*
<!--   *******************************************************************************/
void DBG ( char *msg )
{
    static short count=1;
    
    printf( "****\t%hd: %s\t****\n", count++, msg );
    fflush( stdout );
}

/* --> *******************************************************************************
* <b> IsTrueorFalse </b> 
*  Return Boolean value as a string (true or false).
*    void IsTrueorFalse (Boolean b, char *s)
*
<!--   *******************************************************************************/
void IsTrueorFalse (Boolean b, char *s)
{
    if (b) strcpy (s,"true");
    else   strcpy (s,"false");
}

/* --> *******************************************************************************
* <b> ReportDataType </b> 
*  Return the name of the input file type as a string
*
*     char *ReportDataType( short data_type )
*
<!--   *******************************************************************************/
char *ReportDataType( short data_type )
{
	switch( data_type ) {
		case T_FLOAT:
			return "float";
		break;

		case T_SHORT:
			return "short";
		break;

		case T_USHORT:
			return "unsigned short";
		break;

		case T_CHAR:
			return "char";
		break;

		case T_UCHAR:
			return "unsigned char";
		break;

		case T_INT:
			return "int";
		break;

		case T_DOUBLE:
			return "double";
		break;

		case T_COMPLEX:
			return "complex";
		break;

		case T_LONG:
			return "long";
		break;

		default:
			return "Unrecognized data type";
	}
}

/* --> *******************************************************************************
*	<b>PercentCount</b>
*
*	Used to indicate percent completion on console output
*
*	Boolean PercentCount(  // returns true if the percent has changed by stepSize
*       int count          // event counter
*       int max            // maximum number of events
*       int stepSize       // what increment in percent change to display
*       int *ans )         // the current percent of completion
<!-- ********************************************************************************/
Boolean PercentCount( int count, int max, int stepSize, int *ans )
{
	static int prevAns;
	
	*ans = (100*count)/max;
	if( *ans != prevAns && !(*ans % stepSize) ) {
		prevAns = *ans;
		return true;
	} else {
		return false;
	}
}

/* --> *******************************************************************************
* <B> SetTypeRange </b> 
*		Set outMin and outMax to equal the data range of the data_type;
*
*       void SetTypeRange( float *outMax, float *outMin, short data_type )
*
<!--   *******************************************************************************/
void SetTypeRange( float *outMax, float *outMin, short data_type )
/* NOTE: not specified for floating point types! */
/* NOTE: at present, the full range of unsigned short is not used */
{
	switch( data_type ) {
		case T_CHAR:
			*outMin = CHAR_MIN;
			*outMax = CHAR_MAX;
		break;
		
		case T_UCHAR:
			*outMin = 0;
			*outMax = UCHAR_MAX;
		break;

		case T_SHORT:
			*outMin = SHRT_MIN;
			*outMax = SHRT_MAX;
		break;

		case T_USHORT:
			*outMin = 0;
			*outMax = SHRT_MAX;
		break;

		case T_INT:
			*outMin = INT_MIN;
			*outMax = INT_MAX;
		break;

		case T_LONG:
			*outMin = LONG_MIN;
			*outMax = LONG_MAX;
		break;
	}
}
/* --> *******************************************************************************
* <b> ShowError </b> 
*  Dump an error message to a log file. Assumes that the file has been opened.
*
*    void ShowError ( OSErr error, char *where, char *msg)
*
<!--   *******************************************************************************/

void ShowError ( OSErr error, char *where, char *msg)
{
#ifdef no_log_file
    const short dbg = 0; /* must be quiet */
#else
    const short dbg = 1;
#endif
   if (dbg) {
     sprintf( gMessageStr,"ShowError: %s(%d) on %s!\n", where, error, msg );
     LogMessage( gMessageStr );
   }
}

/* --> *******************************************************************************
* <b> errfopen </b> 
*  This function reports its own messages. Use ck_fopen for more standard approach
*  Checks for valid file names and returns appropriate error messages
*  A re-write of fopen with a small amount of error reportinf. No Mac equivalent.
*    ck_fopen is recommended for current and new projects
*
*    FILE *errfopen( char *fname, char *mode )
*
<!--   *******************************************************************************/
FILE *errfopen( char *fname, char *mode )
{
	char response[255];
	FILE *fp;

	fp = fopen( fname, mode );
	if ( !fp ) {
		sprintf( response, "ERROR: The file '%s' ", fname );
		if ( strcmp( mode, "r") == 0 ) {
		   strcat(response,"could not be read.\n");
		} else if ( strcmp( mode, "w") == 0 ) {
		   strcat(response,"could not be written.\n");
	   } else if ( strcmp( mode, "rw") == 0 ) {
	      strcat(response,"could not be opened for read/write.\n");
	   } else if ( strcmp( mode, "a") == 0 ) {
	      strcat(response,"could not be opened for appending.\n");
	   } else {
	      strcat(response,"could not be used.\n");
	   }
	   sprintf( gMessageStr,response );
	   LogMessage( gMessageStr );
#ifdef unix
	   ILError( FILE_ERROR, response );
#endif
      return(NULL);
   } else {
      return(fp);
   }
}   /* file_check */

/* using the ck_f functions:
char *errString;
FILE *f;

error = ck_fopen( &fp, "MyFile", "r" );
ILError( error, errString );
*/

/* --> *******************************************************************************
*  <B> ck_fopen </b> 
*
*	Checks for valid file names and returns appropriate error messages.
*
*    OSErr ck_fopen( FILE **f,      -- returns an open file to f
*                    char *fname,   -- file name to open
*                    char *mode )   -- standard C file mode
*
<!--   *******************************************************************************/
OSErr ck_fopen( FILE **f, char *fname, char *mode )
{
	*f = fopen( fname, mode );
	if ( !*f ) {
		if( errno == ENOENT ) {
			return FILE_NOT_FOUND;
		} else if( equalString( mode, "r" )) {
			return READ_OPEN_FAILURE;
		} else if( equalString( mode, "w" )) {
			return WRITE_OPEN_FAILURE;
		} else if( equalString( mode, "rb" )) {
			return READ_OPEN_FAILURE;
		} else if( equalString( mode, "wb" )) {
			return WRITE_OPEN_FAILURE;
		} else if( equalString( mode, "a" )) {
			return APPEND_OPEN_FAILURE;
		} else if( equalString( mode, "rw" )) {
			return WRITE_OPEN_FAILURE;
		} else {
			return noErr;
		}
	} else {
		return noErr;
	}
}

/* --> *******************************************************************************
*  <b> ck_fwrite </b> 
* 	A wrapper for fwrite that checks for completion and posts an
*	Error on failure. All arguments as in fwrite.
*
*    OSErr  ck_fwrite( void *data, size_t itemSize, size_t nmemb, FILE *f )
*
<!--   *******************************************************************************/
OSErr  ck_fwrite( void *data, size_t itemSize, size_t nmemb, FILE *f )
{
	long	count;

	count = fwrite( data, itemSize, nmemb, f );
	if( count != nmemb ) {
#ifdef POST_DBG
		printf( "errno reports: %s\n", strerror( errno ));
		sprintf( gDebugString, "ck_fwrite: Memory location: %ld\tRequested size: %ld\n%s",
		         data, nmemb*itemSize );
		DBG( gDebugString );
#endif
		return WRITE_ERROR;
	}
	return noErr;
}

/* --> *******************************************************************************
*  <b> ck_fread </b> 
* 	A wrapper for fread that checks for completion and posts an
*	Error on failure
*
*     OSErr  ck_fread( void *data, size_t itemSize, size_t nmemb, FILE *f )
*
<!--   *******************************************************************************/
OSErr  ck_fread( void *data, size_t itemSize, size_t nmemb, FILE *f )
{
	long	count;

	count = fread( data, itemSize, nmemb, f );
	if( count != nmemb ) {
#ifdef POST_DBG
		sprintf( gDebugString, "ck_fread: Memory location: %ld\tRequested size: %ld\n",
		         data, nmemb*itemSize );
		DBG( gDebugString );
#endif
		return READ_ERROR;
	}
	return noErr;
}

/* --> *******************************************************************************
*  <b> ck_fclose </b> 
* 	A wrapper for fclose that checks for completion
*
*    OSErr ck_fclose( FILE *f )
*
<!--   *******************************************************************************/
OSErr ck_fclose( FILE *f )
{
	if( fclose( f ) ) {
#ifdef POST_DBG
		sprintf( gDebugString, "ck_fclose: error on file close\n" );
		DBG( gDebugString );
#endif
		return FCLOSE_ERROR;
	}
	return noErr;
}

/* --> *******************************************************************************
* <b> ck_malloc </b>  and<b> ck_calloc </b> 
*
*   Really just a replacement for malloc and calloc with a small
*   amount of error reporting.
*
*   This is among the most crucial differences between MAC & UNIX --
*   The MAC must use its own memory manager to be stable. In this
*   version, all memory errors are to be treated as FATAL!
*
*     Ptr ck_malloc( long size, char *msg )
*     Ptr ck_calloc( long number_of_objects, int size, char *msg )
*
<!--   *******************************************************************************/
#ifdef UNIX
Ptr ck_malloc( long size, char *msg )
{
   void *rtn = (void *) malloc(size);
   if (!rtn) {
      sprintf( gMessageStr, "Error allocating memory for: %s!\n", msg );
      LogMessage( gMessageStr );
      abort();
   }
   return (char *)rtn;
}

Ptr ck_calloc( long number_of_objects, int size, char *msg )
{ 
   void *rtn = (void *) calloc(number_of_objects, size);
   if (!rtn) {
      sprintf( gMessageStr, "ck_malloc: Error allocating memory for: %s!\n", msg );
      LogMessage( gMessageStr );
      abort();
   }
   return (char *)rtn;
}
#endif

#ifdef MAC
Ptr ck_malloc( long size, char *msg )
{
	OSErr         error = noErr;
	Size          theSize, bigBlock, cbNeeded;  /* move those handles */
	Ptr           rtn;
	const short   ck_dbg = 1; /* 2 for always, 1 for routine dbg */

	cbNeeded = ( Size )size;	/* this is what we need */
	
	PurgeMem(cbNeeded);		/* ask for memory to be released */
	
	error = MemError();	/* check for memory error */
	if (error == memFullErr) {	/* incomplete? */
		if (ck_dbg) {
			sprintf( gMessageStr, "ck_malloc: memFullErr purging memory for: %s, size: %08x!\n", msg, cbNeeded );
			LogMessage( gMessageStr );
		}
//		ShowErrorHook(memFullErr);
		exit( -1 );
	} else if (error != noErr) {
		if (ck_dbg) {
			sprintf( gMessageStr, "ck_malloc: Error purging memory for: %s!\n", msg );
			LogMessage( gMessageStr );
		}
		ShowError( error, "ck_malloc: NewPtr returns nil.", msg );
//		ShowErrorHook(error);
		exit( -1 );
	}

	bigBlock = MaxMem( &theSize );
	if (ck_dbg == 2) {
		sprintf( gMessageStr, "ck_malloc: biggest block is: %ld, free mem is %ld.\n",
		                     bigBlock, FreeMem() );
		LogMessage( gMessageStr );
	}
	
	rtn = NewPtr( cbNeeded );
	if( rtn == nil ) {
		if (ck_dbg) {
			sprintf( gMessageStr, "ck_malloc: Error allocating memory for: %s!\n", msg );
			LogMessage( gMessageStr );
		}
		error = MemError();
		if (error == noErr) { /* what? no error??? we'll make one up!! */
		error = NIL_PTR;
	} else {
		error = MEMALLOCERROR; /* can only be memFullErr?? */
	}
	
	ShowError( error, "ck_malloc: NewPtr returns nil.", msg );
//	ShowErrorHook(error);
	exit( -1 );
	} else {
		if (ck_dbg == 2) {
			sprintf( gMessageStr, "ck_malloc: allocating memory [%08x (hex)] for: %s, size: %ld.\n",
					rtn, msg, cbNeeded );
			LogMessage( gMessageStr );
		}
	}
	return rtn;
}

Ptr ck_calloc( long number_of_objects, int size, char *msg )
{
	OSErr         error;
	Size          theSize, bigBlock, cbNeeded;  /* move those handles */
	Ptr           rtn;   
	const short   ck_dbg = 1; /* 2 for always, 1 for routine dbg */

	cbNeeded = (Size)number_of_objects * size;	/* this is what we need */
	
	PurgeMem(cbNeeded);		/* ask for memory to be released */
	
	error = MemError();	/* check for memory error */
	if (error == memFullErr) {	/* incomplete? */
		if (ck_dbg) {
			sprintf( gMessageStr, "ck_calloc: memFullErr purging memory for: %s!\n", msg );
			LogMessage( gMessageStr );
		}
		ShowError( error,  "ck_calloc: NewPtr got memFullErr.", msg );
//		ShowErrorHook(memFullErr);
		exit( -1 );
	} else if (error != noErr) {
		if (ck_dbg) {
			sprintf( gMessageStr, "ck_calloc: Error purging memory for: %s!\n", msg );
			LogMessage( gMessageStr );
		}
		ShowError( error, "ck_calloc: NewPtr returns nil.", msg );
//		ShowErrorHook(error);
		exit( -1 );
	}

	bigBlock = MaxMem( &theSize );
	
	if (ck_dbg == 2) {
		sprintf( gMessageStr, "ck_calloc: biggest block is: %ld, free mem is %ld.\n",
		                 bigBlock, FreeMem() );
		LogMessage( gMessageStr );
	}
	
	rtn = NewPtrClear( cbNeeded );
	
	if( rtn == nil ) {
		if (ck_dbg) {
			sprintf( gMessageStr, "ck_calloc: Error allocating memory for: %s!\n", msg );
			LogMessage( gMessageStr );
		}
		error = MemError();
		if (error == noErr) { /* what? no error??? we'll make one up!! */
			error = NIL_PTR;
		} else {
			error = MEMALLOCERROR; /* can only be memFullErr?? */
		}
		ShowError( error, "ck_calloc: NewPtrClear returns nil.", msg );
//		ShowErrorHook(error);
		exit( -1 );
	} else {
		if (ck_dbg == 2) {
			sprintf( gMessageStr, "ck_calloc: allocating memory [%08x (hex)] for: %s, size: %ld.\n",
			          rtn, msg, cbNeeded );
			LogMessage( gMessageStr );
		}
	}
	return rtn;
}
#endif

/* --> *******************************************************************************
*  <b> CreateTempFile </b> 
*   Open a temporary file for write and return its name
*	OSErr CreateTempFile( FILE *TempFile, char **itsName )
*
*   Typical calling sequence:
*
*   char  *TempName;
*   FILE  *TempFile;
*
*	error = CreateTempFile( &TempFile, &TempName );
*	ILError( error, "creating temporary file" );
*
*	error = ck_fclose( TempFile );
*
<!--   *******************************************************************************/
OSErr CreateTempFile( FILE **TempFile, char **itsName )
{
	OSErr   error = noErr;
	
	*itsName = (char *)malloc( L_tmpnam );
	tmpnam( *itsName );
	
	error = ck_fopen( TempFile, *itsName, "w" );
	
	return error;
}

/* --> *******************************************************************************
*  <b> GetFileSize </b> 
*   Determine the length of a specified file
*	OSErr    GetFileSize( long *filesize,  -- the calculated file size
*                         char *fname )    -- the file name (unaltered)
<!--   *******************************************************************************/
OSErr    GetFileSize( long *filesize, char *fname )
{
	OSErr error = noErr;
	FILE  *fp;

	error = ck_fopen( &fp, fname, "r" );
	RETURNONERROR;

	fseek( fp, 0, SEEK_END );
	*filesize = ftell( fp );

	error = ck_fclose( fp );
	return error;
}

/* --> *******************************************************************************
*  <b> ReadLastBytes </b> 
*   Read at the end of a file
*	OSErr    ReadLastBytes( char *fname, char *buff, long nBytes )
<!--   *******************************************************************************/
OSErr    ReadLastBytes( char *fname, char *buff, long nBytes )
{
	FILE  *f;
	OSErr error = noErr;

	error = ck_fopen( &f, fname, "rb" );
	RETURNONERROR;

	fseek( f, -nBytes, SEEK_END );

	error = ck_fread( buff, sizeof( char ), nBytes, f );
	RETURNONERROR;

	error = ck_fclose( f );
	return error;
}


#ifdef MAC
/* --> *******************************************************************************
*   <b> LockHandleToPointer </b> 
*    Lock a handle and return a pointer to its data (Macintosh).
*    This is used to address memory using standard C constructs (e.g., ImgLib)
*    within the Mac memory management.
*
*    OSErr LockHandleToPointer( Handle h,           -- an active Handle
*                               void **p,           -- a pointer to use
*                               SignedByte *state ) -- the handle state
*
<!--   *******************************************************************************/
OSErr LockHandleToPointer( Handle h, void **p, SignedByte *state )
{	
/* get current state of handle so caller can restore it to original state */
	*state = HGetState(h);

/* move it high in heap and lock it (redundant if already locked, but no problem) */
	HLockHi(h);
	if( h == nil || *h == nil ) {
		return INVALID_ADDRESS;
	}
	
	*p = (void *)*h;
	
	return noErr;
}		

/* --> *******************************************************************************
*  <b> ck_AllocateHandle </b> 
*
	Unified memory allocation for handles. Whenever possible, make
	this call instead of calling NewHandle() directly. This will
	ensure that all is well.

OSErr	ck_AllocateHandle(long size, Handle *handleAddress)

	Requires: nothing
	Receives: desired size, pointer to Handle to store results
	Changes:  nothing
	Returns:  error value
	NOTE: does not check memory reserve (use EasyMallocateHandle for that)
<!--   *******************************************************************************/
OSErr	ck_AllocateHandle(long size, Handle *handleAddress)
{
	OSErr	error = noErr;
	long	totalSize, contigSize;
	
/* see if we have enough room in memory */
	PurgeSpace(&totalSize, &contigSize);
	
/* if a purge would result in the largest block being too small */
	if (contigSize < size) { 
		error = memFullErr;	/* so we're out of memory */
	}
	if (error) {
		*handleAddress = nil;
		return error;
	}	
	
/* allocate handle */
	*handleAddress = NewHandle(size);
	if(!*handleAddress) {
		error = MemError();
	}

	return error;
}
#endif

/* --> *******************************************************************************
*  <b> memFree </b> 
*
*   OSErr memFree( void **theMem, char *msg  )
*
*   Because of the differences in pointer handling between Mac
*   and unix environments, disposing of pointers requires a separate
*   routine for each system. This is it:
*   We zero the pointer as well, so that no one can point to la-la land.
<!--   *******************************************************************************/
OSErr memFree( void **theMem, char *msg  )
{
	OSErr error = noErr;

	if ( *theMem == nil) {
		error = NIL_PTR;
		return error;
	}

/*
	if (ck_dbg) {
		sprintf( gMessageStr,"memFree: deallocating memory [%08x] for: %s, size: %08x.\n", 
		         *theMem, msg, GetPtrSize((char *)*theMem) );
		LogMessage( gMessageStr );
	}
*/
#ifdef MAC
	if( *theMem ) {
		DisposePtr( (Ptr)*theMem );
		*theMem = nil;
	}
#endif

#ifdef UNIX
	if( *theMem ) {
		free( *theMem );
		*theMem = nil;
	}
#endif
	return noErr;
}

/* --> *******************************************************************************
*  <b> cpy_alloc </b> 
*	Copy memory addresses to new name, implemented as a special
*	function so that we can track it.
*
*   OSErr cpy_alloc(void **destPtr, char *msg1, void *srcPtr, char *msg2)
<!--   *******************************************************************************/
OSErr cpy_alloc(void **destPtr, char *msg1, void *srcPtr, char *msg2)
{
        const short   cpy_dbg = 1; /* 1 for routine dbg */ /* 1 */
        OSErr   error = noErr;

   /* we're about to bury a pts to some alloc'd memory! */
   /* we can clean this up; but we may complain */
#ifdef MAC		
 OSErr   DisposeWindowMemoryItem  (Ptr *p, char *msg);

        if (*destPtr != nil) {
                error = DisposeWindowMemoryItem( (Ptr *)*destPtr, msg1 );
                if( error != noErr ) {
                	return error;
                }
        }
#endif

        if (cpy_dbg) {
                sprintf( gMessageStr, "cpy_alloc: %s = %s (%08x = %08x)\n",
                        msg1, msg2, *destPtr , srcPtr );
                LogMessage( gMessageStr );
        }
        *destPtr = srcPtr;
        return error;
}

/* --> *******************************************************************************
*  <b> YorN </b> 
*   Yield a string (Yes or No) based on a boolean
*
*    char *YorN( int( theFlag ))
<!--   *******************************************************************************/

char *YorN( int( theFlag ))
{
   if( theFlag == 0 )
      return("No");
   else
      return("Yes");
}

/* --> *******************************************************************************
*  <b> Boolean pcByteOrder() </b>     -- true if Little Endian
*  <b> Boolean macByteOrder() </b>    -- true if Big Endian
*   Functions to determine the byte order on the present platform.
<!--   *******************************************************************************/
/* determine the byte swapping order */
Boolean pcByteOrder()
/* returns true if this machine is pcByteOrder */
{
	union u {
		unsigned short int ui;
		unsigned char uc[2];
	}uv;

	uv.ui=1;
	return(uv.uc[0]); /* if this sets the second byte, we are little-Endian */
}

Boolean macByteOrder()
/* returns true if this machine is macByteOrder */
{
	union u {
		unsigned short int ui;
		unsigned char uc[2];
	}uv;

	uv.ui=1;
	return(uv.uc[1]); /* if this sets the second byte, we are Big-Endian */
}


/* --> *******************************************************************************
*   <b> lowerString </b>  
*    void lowerString( char *theString )
*
*    Force a string to lower case
<!--   *******************************************************************************/
void	lowerString( char *theString )
{
	char *p;
	
	p = theString;
	while( *p ) {
		*p = tolower( *p );
		p++;
	}
}

/* --> *******************************************************************************
*  <b> vmov </b> 
*   Copy the contents of invec to outvec, leaving invec intact.
*   vectors must be of same type!
*
*   CALLING SEQUENCE
*   OSErr vmov( *invec, in_step1, *outvec, out_step, vlen, data_type )
*
*   INPUTS:   *invec    -- input vector
*             *outvec   -- output vector
*             in_step   -- rate to step through input vector
*             out_step  -- rate to step through output vector
*             vlen      -- number of data points to process
*                NOTE: calling routine must ensure that this doesn't exceed
*                        the lengths of the input vectors
*             data_type -- standard UCLA data type from ImgLib.h
*
*   OUTPUTS:  returns contents of invec in outvec
<!--   *******************************************************************************/
OSErr  vmov( void *invec, int instep, void *outvec, int ostep, long vlen, int data_type)
{
	char  *vec1, *vec2;
	int   i,j, sizeof_element;

	if( invec==nil || outvec==nil ) {
		return INVALID_ADDRESS;
	}
	sizeof_element = get_datasize( data_type );

	/* memcpy should be faster */  
	if( instep==ostep==1 ) { 
		memcpy( outvec, invec, sizeof_element * vlen );
	} else {
		vec1 = (char *)invec;
		vec2 = (char *)outvec;
		for( i=0; i< vlen*sizeof_element; i+=sizeof_element ) {
			for( j=0; j<sizeof_element; j++ ) {
				vec2[i*ostep + j] = vec1[i*instep + j];
			}
		}
	}
	return noErr;
}

/* --> *******************************************************************************
*  <b> vfill </b> 
*   Fill a vector with a constant value
*
*   CALLING SEQUENCE
*   OSErr vfill( *invec, fill, vlen, data_type )
*
*   INPUTS:   *invec    -- input vector
*             fill      -- constant fill value
*             vlen      -- number of data points to process
*                NOTE: calling routine must ensure that this doesn't exceed
*                        the lengths of the input vectors
*             data_type -- standard UCLA data type from ImgLib.h
*
<!--   *******************************************************************************/
OSErr  vfill( void *invec,  void *fill, long vlen, int data_type)
{
	long   i;
	char   *p = (char *)invec;
	int    sizeof_element;
	char   myFill[8];	// allocate the largest data size

	if( invec==nil ) {
		return INVALID_ADDRESS;
	}
	sizeof_element = get_datasize( data_type );

	memcpy( myFill, (char *)fill, sizeof_element );

	for( i=0; i<vlen; i++ ) {
		memcpy( p, myFill, sizeof_element );
		p += sizeof_element;
	}
	return noErr;
}

/* --> *******************************************************************************
*  <b> vfsmul </b> 
*   Multiply each element in the vector by a floating point scalar.
*   vector and scalar must be of same type!
*	May be used in place.
*
*   CALLING SEQUENCE
*   OSErr  vfsmul(void *src, int instep, void *dst, int ostep,
*                 float scalar, long vlen, int data_type )
*
*   INPUTS:   *src      -- input vector
*             instep    -- rate to step through vector
*             *dst      -- destination vector
*             outstep   -- rate to step through vector
*             scalar    -- float scalar multiplier
*             vlen      -- number of elements in input vector to process
*             data_type -- standard data type of vector and scalar from above
*
*   OUTPUTS:  Returns each element in the vector multiplied by a float. Follows
*             standard arithmetic rounding rules.
*
*   This, and many following returns, require typecasting of the input vectors
<!--   *******************************************************************************/
OSErr  vfsmul(void *src, int instep, void *dst, int ostep,
              float scalar, long vlen, int data_type )
{
	OSErr           error = noErr;
	int             i, j;
	char            *csrc, *cdst;
	short           *ssrc, *sdst;
	int             *isrc, *idst;
	float           *fsrc, *fdst;
	double          *dsrc, *ddst;
	unsigned char   *ucsrc, *ucdst;
	unsigned short  *uisrc, *uidst;

	if( src == nil || dst == nil ) {
		return INVALID_ADDRESS;
	}	
	switch( data_type )  {
		case T_CHAR:
			csrc = (char *)src;
			cdst = (char *)dst;
			for( i=0, j=0; i<vlen*instep; i+=instep, j+=ostep ) {
				*(cdst+j) = (char)(scalar * *(csrc+i) + 0.5);
			}
		break;   
		case T_SHORT:
			ssrc = (short *)src;
			sdst = (short *)dst;
			for( i=0, j=0; i<vlen*instep; i+=instep, j+=ostep ) {
				*(sdst+j) = (short)(scalar * *(ssrc+i) + 0.5);
			}
		break;   
		case T_INT:
			isrc = (int *)src;
			idst = (int *)dst;
			for( i=0, j=0; i<vlen*instep; i+=instep, j+=ostep ) {
				*(idst+j) = (int)(scalar * *(isrc+i) + 0.5);
			}
		break;   
		case T_FLOAT:
		case T_COMPLEX:
			if( data_type == T_COMPLEX ) {
				vlen *= 2;
			}
			fsrc = (float *)src;
			fdst = (float *)dst;
			for( i=0, j=0; i<vlen*instep; i+=instep, j+=ostep ) {
				*(fdst+j) = (float)(scalar * *(fsrc+i));
			}
		break;   
			fsrc = (float *)src;
			fdst = (float *)dst;
			for( i=0, j=0; i<vlen*instep; i+=instep, j+=ostep ) {
				*(fdst+j) = (float)(scalar * *(fsrc+i));
			}
		break;   
		case T_DOUBLE:
			dsrc = (double *)src;
			ddst = (double *)dst;
			for( i=0, j=0; i<vlen*instep; i+=instep, j+=ostep ) {
				*(ddst+j) = (double)(scalar * *(dsrc+i));
			}
		break;   
		case T_UCHAR:
			ucsrc = (unsigned char *)src;
			ucdst = (unsigned char *)dst;
			for( i=0, j=0; i<vlen*instep; i+=instep, j+=ostep ) {
				*(ucdst+j) = (unsigned char)(scalar * *(ucsrc+i) + 0.5);
			}
		break;   
		case T_USHORT:
			uisrc = (unsigned short *)src;
			uidst = (unsigned short *)dst;
			for( i=0, j=0; i<vlen*instep; i+=instep, j+=ostep ) {
				*(uidst+j) = (unsigned short)(scalar * *(uisrc+i) + 0.5);
			}
		break;
		default:
			sprintf( gMessageStr, "Undefined type in vsmul\n" );
			LogMessage( gMessageStr );
			error = UNKNOWNTYPE;
	}
	return error;
}

/* --> *******************************************************************************
*  <b> vsmul </b> 
*   Multiply each element in the vector by a scalar.
*   vector and scalar must be of same type!
*	May be used in place.
*
*   CALLING SEQUENCE
*   OSErr  vsmul(void *src, int instep, void *dst, int ostep,
*                void *scalar, long vlen, int data_type )
*
*   INPUTS:   *src      -- input vector
*             instep    -- rate to step through vector
*             *dst      -- destination vector
*             outstep   -- rate to step through vector
*             *scalar   -- pointer the scalar multiplier
*             vlen      -- number of elements in input vector to process
*             data_type -- standard data type of vector and scalar from above
*
*   OUTPUTS:  Returns each element in the vector multiplied by a scalar
*
*   This, and many following returns, require typecasting of the input vectors
<!--   *******************************************************************************/
OSErr  vsmul(void *src, int instep, void *dst, int ostep,
             void *scalar, long vlen, int data_type )
{
	OSErr           error = noErr;
	int             i, j;
	char            *csrc, *cdst, cs;
	short           *ssrc, *sdst, ss;
	int             *isrc, *idst, is;
	float           *fsrc, *fdst, fs;
	double          *dsrc, *ddst, ds;
	unsigned char   *ucsrc, *ucdst, ucs;
	unsigned short  *uisrc, *uidst, uis;

	if( src == nil || dst == nil ) {
		return INVALID_ADDRESS;
	}	

	switch( data_type )  {
		case T_CHAR:
			csrc = (char *)src;
			cdst = (char *)dst;
			cs = *(char *)scalar;
			for( i=0, j=0; i<vlen*instep; i+=instep, j+=ostep ) {
				*(cdst+j) = cs * *(csrc+i);
			}
		break;   

		case T_SHORT:
			ssrc = (short *)src;
			sdst = (short *)dst;
			ss  = *(short *)scalar;
			for( i=0, j=0; i<vlen*instep; i+=instep, j+=ostep ) {
				*(sdst+j) = ss * *(ssrc+i);
			}
		break;   

		case T_INT:
			isrc = (int *)src;
			idst = (int *)dst;
			is = *(int *)scalar;
			for( i=0, j=0; i<vlen*instep; i+=instep, j+=ostep ) {
				*(idst+j) = is * *(isrc+i);
			}
		break;   

		case T_FLOAT:
		case T_COMPLEX:
			if( data_type == T_COMPLEX ) {
				vlen *= 2;
			}
			fsrc = (float *)src;
			fdst = (float *)dst;
			fs = *(float *)scalar;
			for( i=0, j=0; i<vlen*instep; i+=instep, j+=ostep ) {
				*(fdst+j) = fs * *(fsrc+i);
			}
		break;   

		case T_DOUBLE:
			dsrc = (double *)src;
			ddst = (double *)dst;
			ds = *(double *)scalar;
			for( i=0, j=0; i<vlen*instep; i+=instep, j+=ostep ) {
				*(ddst+j) = ds * *(dsrc+i);
			}
		break;   

		case T_UCHAR:
			ucsrc = (unsigned char *)src;
			ucdst = (unsigned char *)dst;
			ucs = *(unsigned char *)scalar;
			for( i=0, j=0; i<vlen*instep; i+=instep, j+=ostep ) {
				*(ucdst+j) = ucs * *(ucsrc+i);
			}
		break;   
		case T_USHORT:
			uisrc = (unsigned short *)src;
			uidst = (unsigned short *)dst;
			uis = *(unsigned short *)scalar;
			for( i=0, j=0; i<vlen*instep; i+=instep, j+=ostep ) {
				*(uidst+j) = uis * *(uisrc+i);
			}
		break;
		default:
			sprintf( gMessageStr, "Undefined type in vsmul\n" );
			LogMessage( gMessageStr );
			error = UNKNOWNTYPE;
	}
	return error;
}

/* --> *******************************************************************************
*  <b> vadd </b> 
*   Add the elements in two vectors and place the sum in a third.
*   All vectors must be of the same type!
*
*   CALLING SEQUENCE
*   OSErr vadd ( void *invec1, int step1, void *invec2, int step2,
*                void *outvec, int out_step, long vlen, int data_type )
*
*   INPUTS:   *invec1 and 2 -- input vectors
*             step          -- rate to step through vectors
*             *outvec       -- output vector
*             vlen          -- number of data points to process
*                  NOTE: calling routine must ensure that this doesn't exceed
*                        the lengths of the input vectors
*             data_type     -- standard data type of vector and scalar from above
*
*   OUTPUTS:  returns sum of invec1 and invec2 in outvec
<!--   *******************************************************************************/
OSErr vadd ( void *invec1, int step1, void *invec2, int step2,
           void *outvec, int out_step, long vlen, int data_type )
{
   OSErr  error = noErr;              /* for errors */
   int                i,j,k,m;
   char               *cv1, *cv2, *cov;
   short              *sv1, *sv2, *sov;
   int                *iv1, *iv2, *iov;
   float              *fv1, *fv2, *fov;
   double             *dv1, *dv2, *dov;
   unsigned char      *ucv1, *ucv2, *ucov;
   unsigned short     *uiv1, *uiv2, *uiov;

   if( invec1==nil || invec2==nil || outvec==nil ) {
		return INVALID_ADDRESS;
   }	
	switch( data_type )  {
		case T_CHAR:
			cv1 = (char *)invec1; cv2 = (char *)invec2; cov = (char *)outvec;
			for( i=j=k=m=0; i<vlen; i++, j+=step1, k+=step2, m+=out_step ) {
				*(cov+m) = *(cv1+j) + *(cv2+k);
			}
		break;   

		case T_SHORT:
			sv1 = (short *)invec1; sv2 = (short *)invec2; sov = (short *)outvec;
			for( i=j=k=m=0; i<vlen; i++, j+=step1, k+=step2, m+=out_step ) {
				*(sov+m) = *(sv1+j) + *(sv2+k);
			}
		break;   

		case T_INT:
			iv1 = (int *)invec1; iv2 = (int *)invec2; iov = (int *)outvec;
			for( i=j=k=m=0; i<vlen; i++, j+=step1, k+=step2, m+=out_step ) {
				*(iov+m) = *(iv1+j) + *(iv2+k);
			}
		break;   

		case T_FLOAT:
		case T_COMPLEX:
			if( data_type == T_COMPLEX ) {
				vlen *= 2;
			}
			fv1 = (float *)invec1; fv2 = (float *)invec2; fov = (float *)outvec;
			for( i=j=k=m=0; i<vlen; i++, j+=step1, k+=step2, m+=out_step ) {
				*(fov+m) = *(fv1+j) + *(fv2+k);
			}
		break;   

		case T_DOUBLE:
			dv1 = (double *)invec1; dv2 = (double *)invec2; dov = (double *)outvec;
			for( i=j=k=m=0; i<vlen; i++, j+=step1, k+=step2, m+=out_step ) {
				*(dov+m) = *(dv1+j) + *(dv2+k);
			}
		break;   

		case T_UCHAR:
			ucv1 = (unsigned char *)invec1; ucv2 = (unsigned char *)invec2;
			ucov = (unsigned char *)outvec;
			for( i=j=k=m=0; i<vlen; i++, j+=step1, k+=step2, m+=out_step ) {
				*(ucov+m) = *(ucv1+j) + *(ucv2+k);
			}
		break;   

		case T_USHORT:
			uiv1 = (unsigned short *)invec1; uiv2 = (unsigned short *)invec2;
			uiov = (unsigned short *)outvec;
			for( i=j=k=m=0; i<vlen; i++, j+=step1, k+=step2, m+=out_step ) {
				*(uiov+m) = *(uiv1+j) + *(uiv2+k);
			}
		break;

		default:
			sprintf( gMessageStr, "Undefined type in vadd\n" );
			LogMessage( gMessageStr );
			error = UNKNOWNTYPE;
	}
	return error;
}

/* --> *******************************************************************************
*  <b> vsub </b> 
*   Take the difference of the elements in two vectors and place it in a third.
*   All vectors must be of the same type!
*
*   CALLING SEQUENCE
*     OSErr vsub( void *invec1, int step1, void *invec2, int step2,
*                 void *outvec, int out_step, long vlen, int data_type )
*
*   INPUTS:   *invec1 and 2 -- input vectors
*             step          -- rate to step through vectors
*             *outvec       -- output vector
*             vlen          -- number of data points to process
*                NOTE: calling routine must ensure that this doesn't exceed
*                        the lengths of the input vectors
*             data_type     -- standard data type of vector and scalar from above
*
*   OUTPUTS:  returns difference of invec1 and invec2 in outvec
<!--   *******************************************************************************/
OSErr vsub( void *invec1, int step1, void *invec2, int step2,
            void *outvec, int out_step, long vlen, int data_type )
{
   OSErr  error = noErr;              /* for errors */
   int                i,j,k,m;
   char               *cv1, *cv2, *cov;
   short              *sv1, *sv2, *sov;
   int                *iv1, *iv2, *iov;
   float              *fv1, *fv2, *fov;
   double             *dv1, *dv2, *dov;
   unsigned char      *ucv1, *ucv2, *ucov;
   unsigned short     *uiv1, *uiv2, *uiov;

   if( invec1==nil || invec2==nil || outvec==nil ) {
		return INVALID_ADDRESS;
   }	
	switch( data_type )  {
		case T_CHAR:
			cv1 = (char *)invec1; cv2 = (char *)invec2; cov = (char *)outvec;
			for( i=j=k=m=0; i<vlen; i++, j+=step1, k+=step2, m+=out_step ) {
				*(cov+m) = *(cv1+j) - *(cv2+k);
			}
		break;

		case T_SHORT:
			sv1 = (short *)invec1; sv2 = (short *)invec2; sov = (short *)outvec;
			for( i=j=k=m=0; i<vlen; i++, j+=step1, k+=step2, m+=out_step ) {
				*(sov+m) = *(sv1+j) - *(sv2+k);
			}
		break;

		case T_INT:
			iv1 = (int *)invec1; iv2 = (int *)invec2; iov = (int *)outvec;
			for( i=j=k=m=0; i<vlen; i++, j+=step1, k+=step2, m+=out_step ) {
				*(iov+m) = *(iv1+j) - *(iv2+k);
			}
		break;

		case T_FLOAT:
		case T_COMPLEX:
			if( data_type == T_COMPLEX ) {
				vlen *= 2;
			}
			fv1 = (float *)invec1; fv2 = (float *)invec2; fov = (float *)outvec;
			for( i=j=k=m=0; i<vlen; i++, j+=step1, k+=step2, m+=out_step ) {
				*(fov+m) = *(fv1+j) - *(fv2+k);
			}
		break;

		case T_DOUBLE:
			dv1 = (double *)invec1; dv2 = (double *)invec2; dov = (double *)outvec;
			for( i=j=k=m=0; i<vlen; i++, j+=step1, k+=step2, m+=out_step ) {
				*(dov+m) = *(dv1+j) - *(dv2+k);
			}
		break;

		case T_UCHAR:
			ucv1 = (unsigned char *)invec1; ucv2 = (unsigned char *)invec2;
			ucov = (unsigned char *)outvec;
			for( i=j=k=m=0; i<vlen; i++, j+=step1, k+=step2, m+=out_step ) {
				*(ucov+m) = *(ucv1+j) - *(ucv2+k);
			}
		break;

		case T_USHORT:
			uiv1 = (unsigned short *)invec1; uiv2 = (unsigned short *)invec2;
			uiov = (unsigned short *)outvec;
			for( i=j=k=m=0; i<vlen; i++, j+=step1, k+=step2, m+=out_step ) {
				*(uiov+m) = *(uiv1+j) - *(uiv2+k);
			}
		break;

		default:
			sprintf( gMessageStr, "Undefined type in vsub\n" );
			LogMessage( gMessageStr );
			error = UNKNOWNTYPE;
	}
	return error;
}

/* --> *******************************************************************************
*  <b> vmul </b> 
*   Multiply the elements in two vectors and place the product in a third.
*   All vectors must be of the same type!
*
*   CALLING SEQUENCE
*   OSErr vmul( void *invec1, int step1, void *invec2, int step2,
*              void *outvec, int out_step, long vlen, int data_type )
*
*   INPUTS:   *invec1 and 2 -- input vectors
*             step          -- rate to step through vectors
*             *outvec       -- output vector
*             vlen          -- number of data points to process
*                NOTE: calling routine must ensure that this doesn't exceed
*                      the lengths of the input vectors
*             data_type     -- standard data type
*
*   OUTPUTS:  returns product of invec1 and invec2 in outvec
<!--   *******************************************************************************/
OSErr vmul( void *invec1, int step1, void *invec2, int step2,
           void *outvec, int out_step, long vlen, int data_type )
{
   OSErr  error = noErr;              /* for errors */
   int                i,j,k,m;
   char               *cv1, *cv2, *cov;
   short              *sv1, *sv2, *sov;
   int                *iv1, *iv2, *iov;
   float              *fv1, *fv2, *fov;
   double             *dv1, *dv2, *dov;
   unsigned char      *ucv1, *ucv2, *ucov;
   unsigned short     *uiv1, *uiv2, *uiov;

   if( invec1==nil || invec2==nil || outvec==nil ) {
		return INVALID_ADDRESS;
   }	
	switch( data_type )  {
		case T_CHAR:
			cv1 = (char *)invec1; cv2 = (char *)invec2; cov = (char *)outvec;
			for( i=j=k=m=0; i<vlen; i++, j+=step1, k+=step2, m+=out_step ) {
				*(cov+m) = *(cv1+j) * *(cv2+k);
			}
		break;
		case T_SHORT:
			sv1 = (short *)invec1; sv2 = (short *)invec2; sov = (short *)outvec;
			for( i=j=k=m=0; i<vlen; i++, j+=step1, k+=step2, m+=out_step ) {
				*(sov+m) = *(sv1+j) * *(sv2+k);
			}
		break;
		case T_INT:
			iv1 = (int *)invec1; iv2 = (int *)invec2; iov = (int *)outvec;
			for( i=j=k=m=0; i<vlen; i++, j+=step1, k+=step2, m+=out_step ) {
				*(iov+m) = *(iv1+j) * *(iv2+k);
			}
		break;
		case T_FLOAT:
			fv1 = (float *)invec1; fv2 = (float *)invec2; fov = (float *)outvec;
			for( i=j=k=m=0; i<vlen; i++, j+=step1, k+=step2, m+=out_step ) {
				*(fov+m) = *(fv1+j) * *(fv2+k);
			}
		break;
		case T_COMPLEX:
			fv1 = (float *)invec1; fv2 = (float *)invec2; fov = (float *)outvec;
			for( i=j=k=m=0; i<vlen; i++, j+=2*step1, k+=2*step2, m+=2*out_step ) {
				*(fov+m) = *(fv1+j) * *(fv2+k);
				fov[m]   = fv1[j] * fv2[k] - fv1[j+1] * fv2[k+1]; 
				fov[m+1] = fv1[j] * fv2[k+1] + fv1[j+1] * fv2[k];
			}
		break;
		case T_DOUBLE:
			dv1 = (double *)invec1; dv2 = (double *)invec2; dov = (double *)outvec;
			for( i=j=k=m=0; i<vlen; i++, j+=step1, k+=step2, m+=out_step ) {
				*(dov+m) = *(dv1+j) * *(dv2+k);
			}
		break;
		case T_UCHAR:
			ucv1 = (unsigned char *)invec1; ucv2 = (unsigned char *)invec2;
			ucov = (unsigned char *)outvec;
			for( i=j=k=m=0; i<vlen; i++, j+=step1, k+=step2, m+=out_step ) {
				*(ucov+m) = *(ucv1+j) * *(ucv2+k);
			}
		break;
		case T_USHORT:
			uiv1 = (unsigned short *)invec1; uiv2 = (unsigned short *)invec2;
			uiov = (unsigned short *)outvec;
			for( i=j=k=m=0; i<vlen; i++, j+=step1, k+=step2, m+=out_step ) {
				*(uiov+m) = *(uiv1+j) * *(uiv2+k);
			}
		break;
		default:
			sprintf( gMessageStr, "Undefined type in vmul\n" );
			LogMessage( gMessageStr );
			error UNKNOWNTYPE;
	}
   return error;
}

/* --> *******************************************************************************
*  <b> vdiv </b> 
*   Divide the elements of numvec1 by denvec2 and place the ratio in outvec.
*   All vectors must be of the same type!
*   NOTE:  RETURNS outvec[i] UNCHANGED IF denvec[i] IS 0 !!!!
*
*   CALLING SEQUENCE
*   OSErr vdiv( void *numvec, int step1, void *denvec, int step2,
*               float *outvec, int out_step, long vlen, int data_type )
*
*   INPUTS:   *numvec   -- numerator vector
*             *denvec   -- denominator vector
*             step      -- rate to step through vectors
*             *outvec   -- output vector
*             vlen      -- number of data points to process
*                  NOTE: calling routine must ensure that this doesn't exceed
*                        the lengths of the input vectors
*             data_type -- standard data type of vector and scalar from above
*
*   OUTPUTS:  Returns error condition
<!--   *******************************************************************************/
OSErr vdiv( void *numvec, int step1, void *denvec, int step2,
           float *outvec, int out_step, long vlen, int data_type )
{
	int                i,j,k,m;
	char               *cnumvec, *cdenvec;
	short              *snumvec, *sdenvec;
	int                *inumvec, *idenvec;
	float              *fnumvec, *fdenvec;
	double             *dnumvec, *ddenvec;
	unsigned char      *ucnumvec, *ucdenvec;
	unsigned short     *uinumvec, *uidenvec;
	OSErr              error = noErr;

	if( numvec==nil || denvec==nil || outvec==nil ) {
		return INVALID_ADDRESS;
	}	

	switch( data_type )  {
		case T_CHAR:
			cnumvec = (char *)numvec;
			cdenvec = (char *)denvec;
			for( i=j=k=m=0; i<vlen; i++, j+=step1, k+=step2, m+=out_step ) {
				if( *(cdenvec+k) !=0 ) {
					*(outvec+m) = (float) *(cnumvec+j) / *(cdenvec+k);
				} else {
					if( cnumvec[j] == 0 ) {
						outvec[m] = 0;
					} else {
						return DIVIDE_BY_ZERO_ERR;
					}
				}
			}
		break;   

		case T_SHORT:
			snumvec = (short *)numvec;
			sdenvec = (short *)denvec;
			for( i=j=k=m=0; i<vlen; i++, j+=step1, k+=step2, m+=out_step ) {
				if( *(sdenvec+k) !=0 ) {
					*(outvec+m) = (float) *(snumvec+j) / *(sdenvec+k);
				} else {
					if( snumvec[j] == 0 ) {
						outvec[m] = 0;
					} else {
						return DIVIDE_BY_ZERO_ERR;
					}
				}
			}
		break;   

		case T_INT:
			inumvec = (int *)numvec;
			idenvec = (int *)denvec;
			for( i=j=k=m=0; i<vlen; i++, j+=step1, k+=step2, m+=out_step ) {
				if( *(idenvec+k) !=0 ) {
					*(outvec+m) = (float) *(inumvec+j) / *(idenvec+k);
				} else {
					if( inumvec[j] == 0 ) {
						outvec[m] = 0;
					} else {
						return DIVIDE_BY_ZERO_ERR;
					}
				}
			}
		break;   

		case T_FLOAT:
			fnumvec = (float *)numvec;
			fdenvec = (float *)denvec;
			for( i=j=k=m=0; i<vlen; i++, j+=step1, k+=step2, m+=out_step ) {
				if( *(fdenvec+k) !=0 ) {
					*(outvec+m) = *(fnumvec+j) / *(fdenvec+k);
				} else {
					if( fnumvec[j] == 0 ) {
						outvec[m] = 0;
					} else {
						return DIVIDE_BY_ZERO_ERR;
					}
				}
			}
		break;   

		case T_COMPLEX:
			fnumvec = (float *)numvec;
			fdenvec = (float *)denvec;
			for( i=j=k=m=0; i<vlen; i+=2, j+=2*step1, k+=2*step2, m+=2*out_step ) {
				if( fnumvec[j] == 0 || ( fdenvec[k] + fdenvec[k+1] ==0 )) {
					outvec[m]   = 0;
					outvec[m+1] = 0;
				} else {
					outvec[m]   = (fnumvec[j] * fdenvec[k] + fnumvec[j+1] * fdenvec[k+1]) /
						(fdenvec[k]*fdenvec[k] + fdenvec[k+1]*fdenvec[k+1] );
						
					outvec[m+1] = (fnumvec[j+1] * fdenvec[k] - fnumvec[j] * fdenvec[k+1]) /
						(fdenvec[k]*fdenvec[k] + fdenvec[k+1]*fdenvec[k+1] );						
/*				} else {
					printf( "point: %ld\n", j );
					return DIVIDE_BY_ZERO_ERR; */
				}
			}
		break;   

		case T_DOUBLE:
			dnumvec = (double *)numvec;
			ddenvec = (double *)denvec;
			for( i=j=k=m=0; i<vlen; i++, j+=step1, k+=step2, m+=out_step ) {
				if( *(ddenvec+k) !=0 ) {
					*(outvec+m) = (float) *(dnumvec+j) / *(ddenvec+k);
				} else {
					if( dnumvec[j] == 0 ) {
						outvec[m] = 0;
					} else {
						return DIVIDE_BY_ZERO_ERR;
					}
				}
			}
		break;   

		case T_UCHAR:
			ucnumvec = (unsigned char *)numvec;
			ucdenvec = (unsigned char *)denvec;
			for( i=j=k=m=0; i<vlen; i++, j+=step1, k+=step2, m+=out_step ) {
				if( *(ucdenvec+k) !=0 ) {
					*(outvec+m) = (float) *(ucnumvec+j) / *(ucdenvec+k);
				} else {
					if( ucnumvec[j] == 0 ) {
						outvec[m] = 0;
					} else {
						return DIVIDE_BY_ZERO_ERR;
					}
				}
			}
		break;   

		case T_USHORT:
			uinumvec = (unsigned short *)numvec;
			uidenvec = (unsigned short *)denvec;
			for( i=j=k=m=0; i<vlen; i++, j+=step1, k+=step2, m+=out_step ) {
				if( *(uidenvec+k) !=0 ) {
					*(outvec+m) = (float) *(uinumvec+j) / *(uidenvec+k);
				} else {
					if( uinumvec[j] == 0 ) {
						outvec[m] = 0;
					} else {
						return DIVIDE_BY_ZERO_ERR;
					}
				}
			}
		break;

		default:
			sprintf( gMessageStr, "vdiv -- Undefined type\n" );
			LogMessage( gMessageStr );
	}
	return( error );
}

/* --> *******************************************************************************
*  <b> vminmax </b> 
*
*   Determine the minimum and maximum in the vector.
*   CALLING SEQUENCE:
*     OSErr vminmax( void *invec, long vlen, void *vecmax, void *vecmin, int data_type )
*   INPUTS:
*         invec     -- properly typed input vector
*         vlen      -- number of points in vector
*         data_type -- from #defines above
*   OUTPUTS:
*         vecmax, vecmin  -- addresses of min and max outputs
<!--   *******************************************************************************/
OSErr vminmax( void *invec, long vlen, void *vecmax, void *vecmin, int data_type )
{
	OSErr  error = noErr;              /* for errors */
	int i;
	char               *cvec, cmax, cmin;
	short              *svec, smax, smin;
	int                *ivec, imax, imin;
	float              *fvec, fmax, fmin;
	double             *dvec, dmax, dmin;
	unsigned char      *ucvec, ucmax, ucmin;
	unsigned short     *uinvec, uimax, uimin=0;

	if( invec == nil ) {
		return INVALID_ADDRESS;
	}	
	switch( data_type )  {
		case T_CHAR:
			cvec = (char *)invec;
			cmax=cmin= *cvec;
			for( i=1; i< vlen; i++ )  {
				cmax = cmax > *(cvec+i) ? cmax : *(cvec+i);
				cmin = cmin < *(cvec+i) ? cmin : *(cvec+i);
			}
			vmov(&cmax,1,vecmax,1,1,T_CHAR);
			vmov(&cmin,1,vecmin,1,1,T_CHAR);
		break;   

		case T_SHORT:
			svec = (short *)invec;
			smax=smin= *svec;
			for( i=0; i< vlen; i++ )  {
				smax = smax >* (svec+i) ? smax : *(svec+i);
				smin = smin <* (svec+i) ? smin : *(svec+i);
			}
			vmov(&smax,1,vecmax,1,1,T_SHORT);
			vmov(&smin,1,vecmin,1,1,T_SHORT);
		break;   

		case T_INT:
			ivec = (int *)invec;
			imax=imin= *ivec;
			for( i=0; i< vlen; i++ )  {
				imax = imax >* (ivec+i) ? imax : *(ivec+i);
				imin = imin <* (ivec+i) ? imin : *(ivec+i);
			}
			vmov(&imax,1,vecmax,1,1,T_INT);
			vmov(&imin,1,vecmin,1,1,T_INT);
		break;   

		case T_FLOAT:
		case T_COMPLEX:
			if( data_type == T_COMPLEX ) {
				vlen *= 2;
			}
			fvec = (float *)invec;
			fmax=fmin= *fvec;
			for( i=0; i< vlen; i++ )  {
				fmax = fmax >* (fvec+i) ? fmax : *(fvec+i);
				fmin = fmin <* (fvec+i) ? fmin : *(fvec+i);
			}
			vmov(&fmax,1,vecmax,1,1,T_FLOAT);
			vmov(&fmin,1,vecmin,1,1,T_FLOAT);
		break;   

		case T_DOUBLE:
			dvec = (double *)invec;
			dmax=dmin= *dvec;
			for( i=0; i< vlen; i++ )  {
				dmax = dmax >* (dvec+i) ? dmax : *(dvec+i);
				dmin = dmin <* (dvec+i) ? dmin : *(dvec+i);
			}
			vmov(&dmax,1,vecmax,1,1,T_DOUBLE);
			vmov(&dmin,1,vecmin,1,1,T_DOUBLE);
		break;

		case T_UCHAR:
			ucvec = (unsigned char *)invec;
			ucmax=ucmin= *ucvec;
			for( i=0; i< vlen; i++ )  {
				ucmax = ucmax >* (ucvec+i) ? ucmax : *(ucvec+i);
				ucmin = ucmin <* (ucvec+i) ? ucmin : *(ucvec+i);
			}
			vmov(&ucmax,1,vecmax,1,1,T_UCHAR);
			vmov(&ucmin,1,vecmin,1,1,T_UCHAR);
		break;   

		case T_USHORT:
			uinvec = (unsigned short *)invec;
			uimax=uimin= *uinvec;
			for( i=0; i< vlen; i++ )  {
				uimax = uimax >* (uinvec+i) ? uimax : *(uinvec+i);
				uimin = uimin <* (uinvec+i) ? uimin : *(uinvec+i);
			}
			vmov(&uimax,1,vecmax,1,1,T_USHORT);
			vmov(&uimin,1,vecmin,1,1,T_USHORT);
		break;

		default:
			sprintf( gMessageStr, "Undefined type in vminmax\n" );
			LogMessage( gMessageStr );
			error = UNKNOWNTYPE;
	}
	return error;
}

/* --> *******************************************************************************
*  <b> vfminmax </b> 
*
*   Determine the minimum and maximum in the vector.
*   CALLING SEQUENCE:
*     OSErr vminmax( void *invec, long vlen, void *vecmax, void *vecmin, int data_type )
*   INPUTS:
*         invec     -- properly typed input vector
*         vlen      -- number of points in vector
*         data_type -- from #defines above
*   OUTPUTS:
*         vecmax, vecmin  -- addresses of min and max outputs as float
<!--   *******************************************************************************/
OSErr vfminmax( void *invec, long vlen, float *vecmax, float *vecmin, int data_type )
{
	OSErr  error = noErr;              /* for errors */
{
	OSErr  error = noErr;              /* for errors */
	int i;
	char               *cvec;
	short              *svec;
	int                *ivec;
	float              *fvec, fmax, fmin;
	double             *dvec;
	unsigned char      *ucvec;
	unsigned short     *uinvec;

	if( invec == nil ) {
		return INVALID_ADDRESS;
	}
	
	switch( data_type )  {
		case T_CHAR:
			cvec = (char *)invec;
			fmax = fmin = (float)cvec[0];
			for( i=1; i< vlen; i++ )  {
				fmax = fmax > (float)cvec[i] ? fmax : (float)cvec[i];
				fmin = fmin < (float)cvec[i] ? fmin : (float)cvec[i];
			}
		break;   

		case T_SHORT:
			svec = (short *)invec;
			fmax = fmin = (float)svec[0];
			for( i=1; i< vlen; i++ )  {
				fmax = fmax > (float)svec[i] ? fmax : (float)svec[i];
				fmin = fmin < (float)svec[i] ? fmin : (float)svec[i];
			}
		break;   

		case T_INT:
			ivec = (int *)invec;
			fmax = fmin = (float)ivec[0];
			for( i=1; i< vlen; i++ )  {
				fmax = fmax > (float)ivec[i] ? fmax : (float)ivec[i];
				fmin = fmin < (float)ivec[i] ? fmin : (float)ivec[i];
			}
		break;   

		case T_FLOAT:
		case T_COMPLEX:
			if( data_type == T_COMPLEX ) {
				vlen *= 2;
			}
			fvec = (float *)invec;
			fmax = fmin = fvec[0];
			for( i=1; i< vlen; i++ )  {
				fmax = fmax > (float)fvec[i] ? fmax : (float)fvec[i];
				fmin = fmin < (float)fvec[i] ? fmin : (float)fvec[i];
			}
		break;   

		case T_DOUBLE:
			dvec = (double *)invec;
			fmax = fmin = (float)dvec[0];
			for( i=1; i< vlen; i++ )  {
				fmax = fmax > (float)dvec[i] ? fmax : (float)dvec[i];
				fmin = fmin < (float)dvec[i] ? fmin : (float)dvec[i];
			}
		break;

		case T_UCHAR:
			ucvec = (unsigned char *)invec;
			fmax = fmin = (float)ucvec[0];
			for( i=1; i< vlen; i++ )  {
				fmax = fmax > (float)ucvec[i] ? fmax : (float)ucvec[i];
				fmin = fmin < (float)ucvec[i] ? fmin : (float)ucvec[i];
			}
		break;   

		case T_USHORT:
			uinvec = (unsigned short *)invec;
			fmax = fmin = (float)uinvec[0];
			for( i=1; i< vlen; i++ )  {
				fmax = fmax > (float)uinvec[i] ? fmax : (float)uinvec[i];
				fmin = fmin < (float)uinvec[i] ? fmin : (float)uinvec[i];
			}
		break;

		default:
			sprintf( gMessageStr, "Undefined type in vminmax\n" );
			LogMessage( gMessageStr );
			error = UNKNOWNTYPE;
		}
	}
	return error;
}
/* --> *******************************************************************************
*  <b> vsqrt </b> 
*   Square root of each element of a vector in place.
*
*   CALLING SEQUENCE
*   OSErr  vsqrt( void *invec, int step, long vlen, int data_type )
*
*   INPUTS:   *invec    -- input vector
*             vlen      -- input vector length
*             data_type -- standard data type from above
*
*   OUTPUTS:  returns square root (invec) in-place
<!--   *******************************************************************************/
OSErr  vsqrt( void *invec, int step, long vlen, int data_type )
{
	OSErr  error = noErr;              /* for errors */
	int                i;
	char               *cp;
	short              *sp;
	int                *ip;
	float              *fp;
	double             *dp;
	unsigned char      *ucp;
	unsigned short     *uip;

   if( invec == nil ) {
		return INVALID_ADDRESS;
   }	
	switch( data_type )  {
		case T_CHAR:
			cp = (char *)invec;
			for( i=0; i< vlen; i+= step ) {
				if( cp[i] > 0 ) {
					cp[i] = (char)sqrt( (float)cp[i] );
				} else {
					cp[i] = 0;
				}
			}
		break;

		case T_SHORT:
			sp = (short *)invec;
			for( i=0; i< vlen; i+= step ) {
				if( sp[i] > 0 ) {
					sp[i] = (short)sqrt( (float)sp[i] );
				} else {
					sp[i] = 0;
				}
			}
		break;

		case T_INT:
			ip = (int *)invec;
			for( i=0; i< vlen; i+= step ) {
				if( ip[i] > 0 ) {
					ip[i] = (int)sqrt( (float)ip[i] );
				} else {
					ip[i] = 0;
				}
			}
		break;

		case T_FLOAT:
		case T_COMPLEX:
			if( data_type == T_COMPLEX ) {
				vlen *= 2;
			}
			fp = (float *)invec;
			for( i=0; i< vlen; i+= step ) {
				if( fp[i] > 0 ) {
					fp[i] = (float)sqrt( fp[i] );
				} else {
					fp[i] = 0;
				}
			}
		break;

		case T_DOUBLE:
			dp = (double *)invec;
			for( i=0; i< vlen; i+= step ) {
				if( dp[i] > 0 ) {
					dp[i] = (double)sqrt( dp[i] );
				} else {
					dp[i] = 0;
				}
			}
		break;

		case T_UCHAR:
			ucp = (unsigned char *)invec;
			for( i=0; i< vlen; i+= step ) {
				if( ucp[i] > 0 ) {
					ucp[i] = (unsigned char)sqrt( (float)ucp[i] );
				} else {
					ucp[i] = 0;
				}
			}
		break;

		case T_USHORT:
			uip = (unsigned short *)invec;
			for( i=0; i< vlen; i+= step ) {
				if( uip[i] > 0 ) {
					uip[i] = (unsigned short)sqrt( (float)uip[i] );
				} else {
					uip[i] = 0;
				}
			}
		break;

		default:
			sprintf( gMessageStr, "Undefined type in vsq\n" );
			LogMessage( gMessageStr );
			error = UNKNOWNTYPE;
	}
	return error;
}

/* --> *******************************************************************************
*  <b> vsq </b> 
*   Square each element of a vector in place.
*
*   CALLING SEQUENCE
*   OSErr vsq( void *invec, int step, long vlen, int data_type )
*
*   INPUTS:   *invec    -- input vector
*             vlen      -- input vector length
*             data_type -- standard data type from above
*
*   OUTPUTS:  returns squared invec*invec in-place
<!--   *******************************************************************************/
OSErr vsq( void *invec, int step, long vlen, int data_type )
{
	OSErr  error = noErr;              /* for errors */
	int                i;
	char               *cp;
	short              *sp;
	int                *ip;
	float              *fp;
	double             *dp;
	unsigned char      *ucp;
	unsigned short     *uip;

	if( invec == nil ) {
		return INVALID_ADDRESS;
	}	

	switch( data_type )  {
		case T_CHAR:
			cp = (char *)invec;
			for( i=0; i< vlen; i+= step ) {
				*(cp+i) *= *(cp+i);
			}
		break;

		case T_SHORT:
			sp = (short *)invec;
			for( i=0; i< vlen; i+= step ) {
				*(sp+i) *= *(sp+i);
			}
		break;

		case T_INT:
			ip = (int *)invec;
			for( i=0; i< vlen; i+= step ) {
				*(ip+i) *= *(ip+i);
			}
		break;

		case T_FLOAT:
		case T_COMPLEX:
			if( data_type == T_COMPLEX ) {
				vlen *= 2;
			}
			fp = (float *)invec;
			for( i=0; i< vlen; i+= step ) {
				*(fp+i) *= *(fp+i);
			}
		break;

		case T_DOUBLE:
			dp = (double *)invec;
			for( i=0; i< vlen; i+= step ) {
				*(dp+i) *= *(dp+i);
			}
		break;

		case T_UCHAR:
			ucp = (unsigned char *)invec;
			for( i=0; i< vlen; i+= step ) {
				*(ucp+i) *= *(ucp+i);
			}
		break;

		case T_USHORT:
			uip = (unsigned short *)invec;
			for( i=0; i< vlen; i+= step ) {
				*(uip+i) *= *(uip+i);
			}
		break;

		default:
			sprintf( gMessageStr, "Undefined type in vsq\n" );
			LogMessage( gMessageStr );
			error = UNKNOWNTYPE;
	}

	return error;
}

/* --> *******************************************************************************
*  <b> vssq </b> 
*   Return the sum of squares of a vector. Return is always a float.
*   Calling program should typecast if needed.
*
*   CALLING SEQUENCE
*   double vssq( void *invec, int step, long vlen, int data_type )
*
*   INPUTS:   *vec      -- input vector
*             step      -- rate to step through input vector
*             vlen      -- length of input vector
*             data_type -- standard data type from above
*
*   OUTPUTS:  returns sum of all elements of invec*invec, stepping by step
<!--   *******************************************************************************/
double vssq( void *invec, int step, long vlen, int data_type )
{
	int                i;
	double             ssq = 0;
	char               *cp;
	short              *sp;
	int                *ip;
	float              *fp;
	double             *dp;
	unsigned char      *ucp;
	unsigned short     *uip;

	switch( data_type )  {
		case T_CHAR:
			cp = (char *)invec;
			for( i=0; i< vlen; i+= step ) {
				ssq += (*(cp+i) * *(cp+i));
			}
		break;

		case T_SHORT:
			sp = (short *)invec;
			for( i=0; i< vlen; i+= step ) {
				ssq += (*(sp+i) * *(sp+i));
			}
		break;

		case T_INT:
			ip = (int *)invec;
			for( i=0; i< vlen; i+= step ) {
				 ssq += (*(ip+i) * *(ip+i));
			}
		break;

		case T_FLOAT:
		case T_COMPLEX:
			if( data_type == T_COMPLEX ) {
				vlen *= 2;
			}
			fp = (float *)invec;
			for( i=0; i< vlen; i+= step ) {
				ssq += (*(fp+i) * *(fp+i));
			}
		break;

		case T_DOUBLE:
			dp = (double *)invec;
			for( i=0; i< vlen; i+= step ) {
				ssq += (*(dp+i) * *(dp+i));
			}
		break;

		case T_UCHAR:
			ucp = (unsigned char *)invec;
			for( i=0; i< vlen; i+= step ) {
				ssq += (*(ucp+i) * *(ucp+i));
			}
		break;

		case T_USHORT:
			uip = (unsigned short *)invec;
			for( i=0; i< vlen; i+= step ) {
				ssq += (*(uip+i) * *(uip+i));
			}
		break;

		default:
			sprintf( gMessageStr, "Undefined type in vssq\n" );
			LogMessage( gMessageStr );
			ssq = 0;
	}
	return( ssq );
}

/* --> *******************************************************************************
*  <b> vsum </b> 
*   Return the sum all elements in a vector. Return is always a float.
*   Calling program should typecast if needed.
*
*   CALLING SEQUENCE
*   double vsum( void *invec, int step, long vlen, int data_type )
*
*   INPUTS:   *vec      -- input vector
*             step      -- rate to step throught input vector
*             vlen      -- number of data points to process
*             data_type -- standard data type from above
*
*   OUTPUTS:  returns sum of all elements of vec, stepping by step
<!--   *******************************************************************************/
double vsum( void *invec, int step, long vlen, int data_type )
{
	int                i;
	double             sum=0.0;
	char               *cp;
	short              *sp;
	int                *ip;
	float              *fp;
	double             *dp;
	unsigned char      *ucp;
	unsigned short     *uip;

	switch( data_type )  {
		case T_CHAR:
			cp = (char *)invec;
			for( i=0; i< vlen; i+= step ) {
				sum += (double)(*(cp+i));
			}
		break;

		case T_SHORT:
			sp = (short *)invec;
			for( i=0; i< vlen; i+= step ) {
				sum += (double)(*(sp+i));
			}
		break;

		case T_INT:
			ip = (int *)invec;
			for( i=0; i< vlen; i+= step ) {
				 sum += (double)(*(ip+i));
			}
		break;

		case T_FLOAT:
		case T_COMPLEX:
			if( data_type == T_COMPLEX ) {
				vlen *= 2;
			}
			fp = (float *)invec;
			for( i=0; i< vlen; i+= step ) {
				sum += (double)(*(fp+i));
			}
		break;

		case T_DOUBLE:
			dp = (double *)invec;
			for( i=0; i< vlen; i+= step ) {
				sum += (double)(*(dp+i));
			}
		break;

		case T_UCHAR:
			ucp = (unsigned char *)invec;
			for( i=0; i< vlen; i+= step ) {
				sum += (double)(*(ucp+i));
			}
		break;

		case T_USHORT:
			uip = (unsigned short *)invec;
			for( i=0; i< vlen; i+= step ) {
				sum += (double)(*(uip+i));
			}
		break;

		default:
			sprintf( gMessageStr, "Undefined type in vsum\n" );
			LogMessage( gMessageStr );
			sum = 0;
	}
	return( sum );
}

/* --> *******************************************************************************
*  <b> vabs </b> 
*   Return the absolute value of each element in a vector.
*
*   CALLING SEQUENCE
*   OSErr vabs( void *invec, int istep, void *ovec, int ostep,
*               long vlen, int data_type )
*
*   INPUTS:   *vec      -- input vector
*             step      -- rate to step throught input vector
*             *outvec   -- output vector
*             vlen      -- number of data points to process
*                  NOTE: calling routine must ensure that this doesn't exceed
*                        the lengths of the input vectors
*             data_type -- standard data type of vector and scalar from above
*
*   OUTPUTS:  Error code
<!--   *******************************************************************************/
OSErr vabs( void *invec, int istep, void *ovec, int ostep, long vlen, int data_type )
{
	long               i, j;
	char               *cp1, *cp2;
	short              *sp1, *sp2;
	int                *ip1, *ip2;
	float              *fp1, *fp2;
	double             *dp1, *dp2;
	unsigned char      *ucp1, *ucp2;
	unsigned short     *uip1, *uip2;
	OSErr              error = noErr;

	switch( data_type )  {
		case T_CHAR:
			cp1 = (char *)invec;
			cp2 = (char *)ovec;
			for( i=0, j=0; i< vlen; i+= istep, j+=ostep ) {
				cp2[i] = cp1[i] < 0 ? -cp1[i] : cp1[i];
			}
		break;

		case T_SHORT:
			sp1 = (short *)invec;
			sp2 = (short *)ovec;
			for( i=0, j=0; i< vlen; i+= istep, j+=ostep ) {
				sp2[i] = sp1[i] < 0 ? -sp1[i] : sp1[i];
			}
		break;

		case T_INT:
			ip1 = (int *)invec;
			ip2 = (int *)ovec;
			for( i=0, j=0; i< vlen; i+= istep, j+=ostep ) {
				ip2[i] = ip1[i] < 0 ? -ip1[i] : ip1[i];
			}
		break;

		case T_FLOAT:
			fp1 = (float *)invec;
			fp2 = (float *)ovec;
			for( i=0, j=0; i< vlen; i+= istep, j+=ostep ) {
				fp2[i] = fp1[i] < 0 ? -fp1[i] : fp1[i];
			}
		break;

		case T_DOUBLE:
			dp1 = (double *)invec;
			dp2 = (double *)ovec;
			for( i=0, j=0; i< vlen; i+= istep, j+=ostep ) {
				dp2[i] = dp1[i] < 0 ? -dp1[i] : dp1[i];
			}
		break;

		case T_UCHAR:
			ucp1 = (unsigned char *)invec;
			ucp2 = (unsigned char *)ovec;
			error = vmov( ucp1, istep, ucp2, ostep, vlen, T_UCHAR );
		break;

		case T_USHORT:
			uip1 = (unsigned short *)invec;
			uip2 = (unsigned short *)ovec;
			error = vmov( uip1, istep, uip1, ostep, vlen, T_USHORT );
		break;

		default:
			sprintf( gMessageStr, "Undefined type in vsum\n" );
			LogMessage( gMessageStr );
	}

	return error;
}

/* --> *******************************************************************************
*  <b> vclr </b> 
*   Clear the contents of a vector
*
*   CALLING SEQUENCE:
*           OSErr vclr(void * vec, long vlen, int data_type)
*
*   INPUTS: vec    -- vector of arbitrary data type
*           vlen   -- number of bytes in vector
*           data_type  -- standard UC data type
*   OUTPUTS:  None
<!--   *******************************************************************************/
OSErr vclr(void * vec, long vlen, int data_type)
{
	OSErr  error = noErr;              /* for errors */
	unsigned long i;
	unsigned char *p;
	int dataSize;

	if( vec == nil ) {
		return INVALID_ADDRESS;
	}	

	dataSize = get_datasize( data_type );
	p=(unsigned char *)vec;

	for(i=0;i<vlen*dataSize;i++) {
		*p++ = 0; /* p[i] = 0 */
	}
	return error;
}

/* --> *******************************************************************************
*  <b> get_datasize </b> 
*   Get the size (in bytes) of the local data types.
*
*   CALLING SEQUENCE:
*   int     get_datasize(    -- returns the size in bytes
*   int         data_type);  -- accepts any of the UC standard types
<!--   *******************************************************************************/
int     get_datasize(int data_type)
{
   int size;

switch( data_type ) {
   case T_CHAR:    size = sizeof(char);  break;
   case T_SHORT:   size = sizeof(short);  break;
#ifdef ALPHA
   case T_LONG:    size = sizeof(int);  break;
#else
   case T_LONG:    size = sizeof(long);  break;
#endif
   case T_FLOAT:   size = sizeof(float);  break;
   case T_DOUBLE:  size = sizeof(double);  break;
   case T_COMPLEX: size = 2 * sizeof(float);  break;
   case T_UCHAR:   size = sizeof(unsigned char);  break;
   case T_USHORT:  size = sizeof(unsigned short);  break;
   case T_INT:     size = sizeof(int);  break;
   default:
      return( UNKNOWNTYPE );
   }
   return( size );
}

/* --> *******************************************************************************
* <b> vbyteswap </b> 
*    Byte swapping utility for managing the order on DEC and Sun
*
*    OSErr    vbyteswap(void *invec, long vlen, int data_type)
<!--   *******************************************************************************/
OSErr    vbyteswap(void *invec, long vlen, int data_type)
#define  twoBytes  2
#define  fourBytes 4
{
	int i,j,k, sizeof_element;
	char tmp;
	char *vec;

	if( invec == nil ) {
		return INVALID_ADDRESS;
	}

	vec = (char *)invec;
	sizeof_element = get_datasize( data_type );

	switch( sizeof_element ) {
		case twoBytes:
			for( i=0; i<vlen*2; i+= 2 )  {
				tmp = *(vec+i);
				*(vec+i) = *(vec+i+1);
				*(vec+i+1) = tmp;
			}
		break;

		case fourBytes:
			for( i=0; i<vlen*4; i+= 4 )  {
				tmp = *(vec+i);
				*(vec+i) = *(vec+i+3);
				*(vec+i+3) = tmp;
				tmp = *(vec+i+1);
				*(vec+i+1) = *(vec+i+2);
				*(vec+i+2) = tmp;
			}
		break;

		default: /* May as well guess */
			for( i=0; i<vlen*sizeof_element; i+= sizeof_element )  {
				for( j=0, k=sizeof_element-1; j<sizeof_element/2; j++, k-- )  {
					tmp = *(vec+i+k);
					*(vec+i+k) = *(vec+i+j);
					*(vec+i+j) = tmp;
				}
			}
	}
	return noErr;
}
#undef twoBytes
#undef fourBytes

/* --> *******************************************************************************
* <b> vbyteNswap </b> 
*  Byte swapping utility for items of known length
*
*   OSErr    vbyteNswap(void *invec, long vlen, short n_bytes)
<!--   *******************************************************************************/
OSErr    vbyteNswap(void *invec, long vlen, short n_bytes)
{
    int i,j,k;
    char tmp;
    char *vec;

    if( invec == nil ) {
		return INVALID_ADDRESS;
	}
	
    vec = (char *)invec;
	{
		for( i=0; i<vlen*n_bytes; i+= n_bytes )  {
			for( j=0, k=n_bytes-1; j<n_bytes/2; j++, k-- )  {
				tmp = *(vec+i+k);
				*(vec+i+k) = *(vec+i+j);
				*(vec+i+j) = tmp;
			}
		}
	}
    return noErr;
}

/* --> *******************************************************************************
*  <B> ConvertToPercent </b> 
*   Express invec as a percent change from refvec. If refvec is zero, output is zero
*
*    OSErr ConvertToPercent( void *invec,        -- pointer to input vector
*                            void *refvec,       -- pointer to reference vector
*                            void *outvec,       -- output vector
*                            long vlen,          -- total number of values to convert
*                            int  data_type )    -- take all comers
*
<!--   *******************************************************************************/
OSErr ConvertToPercent( void *invec, void *refvec, void *outvec, long vlen, int data_type )
{
	OSErr error = noErr;
	long  i;
	char           *cv1, *cv2, *cov;
	short          *sv1, *sv2, *sov;
	int            *iv1, *iv2, *iov;
	float          *fv1, *fv2, *fov;
	double         *dv1, *dv2, *dov;
	unsigned char  *ucv1, *ucv2, *ucov;
	unsigned short *uiv1, *uiv2, *uiov;

	if( invec==nil || refvec==nil || outvec==nil ) {
		return INVALID_ADDRESS;
	}

	switch( data_type )  {
		case T_CHAR:
			cv1 = (char *)invec; cv2 = (char *)refvec; cov = (char *)outvec;
			for( i=0; i<vlen; i++ ) {
				if( cv2[i] == 0 ) {
					cov[i] = 0;
				} else {
					cov[i] = (100 * ( cv1[i] - cv2[i] )) / cv2[i];
				}
			}
		break;
		
		case T_SHORT:
			sv1 = (short *)invec; sv2 = (short *)refvec; sov = (short *)outvec;
			for( i=0; i<vlen; i++ ) {
				if( sv2[i] == 0 ) {
					sov[i] = 0;
				} else {
					sov[i] = (100 * ( sv1[i] - sv2[i] )) / sv2[i];
				}
			}
		break;

		case T_INT:
			iv1 = (int *)invec; iv2 = (int *)refvec; iov = (int *)outvec;
			for( i=0; i<vlen; i++ ) {
				if( iv2[i] == 0 ) {
					iov[i] = 0;
				} else {
					iov[i] = (100 * ( iv1[i] - iv2[i] )) / iv2[i];
				}
			}
		break;

		case T_FLOAT:
			fv1 = (float *)invec; fv2 = (float *)refvec; fov = (float *)outvec;
			for( i=0; i<vlen; i++ ) {
				if( fv2[i] == 0 ) {
					fov[i] = 0;
				} else {
					fov[i] = (100.0 * ( fv1[i] - fv2[i] )) / fv2[i];
				}
			}
		break;

		case T_DOUBLE:
			dv1 = (double *)invec; dv2 = (double *)refvec; dov = (double *)outvec;
			for( i=0; i<vlen; i++ ) {
				if( dv2[i] == 0 ) {
					dov[i] = 0;
				} else {
					dov[i] = (100.0 * ( dv1[i] - dv2[i] )) / dv2[i];
				}
			}
		break;

		case T_UCHAR:
			ucv1 = (unsigned char *)invec; ucv2 = (unsigned char *)refvec; ucov = (unsigned char *)outvec;
			for( i=0; i<vlen; i++ ) {
				if( ucv2[i] == 0 ) {
					ucov[i] = 0;
				} else {
					ucov[i] = (100 * ( ucv1[i] - ucv2[i] )) / ucv2[i];
				}
			}
		break;

		case T_USHORT:
			uiv1 = (unsigned short *)invec; uiv2 = (unsigned short *)refvec; uiov = (unsigned short *)outvec;
			for( i=0; i<vlen; i++ ) {
				if( uiv2[i] == 0 ) {
					uiov[i] = 0;
				} else {
					uiov[i] = (100 * ( uiv1[i] - uiv2[i] )) / uiv2[i];
				}
			}
		break;

		default:
			sprintf( gMessageStr, "Undefined type in vsub\n" );
			LogMessage( gMessageStr );
			error = UNKNOWNTYPE;
	}
	return error;
}

/* --> *******************************************************************************
*  <B> type_convert </b> 
*   Conversion between float, unsigned short and unsigned char
*	and, as of 6-18-02, signed short.
*
*	Sets the error value if the conversion was non-trivial, and reports
*	the rules for use by the calling routine.
*
*    OSErr type_convert( void *inData,        -- pointer to original data
*                        int inType,          -- original data type
*                        void *convertedData, -- pointer to converted data
*                        int outType,         -- converted data type
*                        long nPts,           -- total number of pixels to convert
*                        float *fmax,         -- calculated max and min of input data
*                        float *fmin,
*                        short *rules )
*
*	<I>FUTURE</I>: The rules for resolution and range scaling are passed in 'rules'.
*	Desired max and min of output are passed in fmax and fmin.
<!--   *******************************************************************************/
OSErr type_convert( void *inData, int inType, void *convertedData, int outType,
                    long nPts, float *fmax, float *fmin, short *rules )
		/* *inData;        pointer to original data */
		/* inType;         original data type */
		/* *convertedData; pointer to converted data */
		/* outType;        converted data type */
		/* nPts;           total number of pixels to convert */
		/* *rules         what happened to the data? */
{

	long           i;          /* counter */
	unsigned short *pus;       /* working pointer to unsigned short data */
	short          *ps;        /* pointer to signed short data */
	float          *pf, *pCOM; /* working pointer to float or complex data */
	double         *pd;
	unsigned char  *pc;        /* working pointer to unsigned char data */
	float          typemax, minAsFloat, range;     /* max, min and max-min data value, as float */
	unsigned char  cmax, cmin;
	unsigned short usmax, usmin;  /* max/min of unsigned short input data */
	short          smax, smin;    /* max/min of signed short input data */
	OSErr          error = noErr;
	
	if( inType == outType ) {
		error = vmov( inData, 1, convertedData, 1, nPts, inType );
		return error;
	}

/* The heart of this function is here. For each input type, make a conversion to each output type.
chars are simply copied. If output type is higher resolution and equal or greater range, just copy.
float and short types must be rescaled to convert to char or short. */ 

	switch( inType ) {
		case T_UCHAR:	 /* input is unsigned char, no scaling, no loss of accuracy */
			pc = (unsigned char *)inData;
			vminmax( inData, nPts, &cmax, &cmin, T_UCHAR );
			*fmax = cmax;
			*fmin = cmin;

/* UCHAR to USHORT */
			if( outType==T_USHORT ) {       /* output is unsigned short */
				pus = (unsigned short *)convertedData;
				for(i=0; i<nPts; i++) {
					pus[i] = (unsigned short)pc[i];
				}

/* UCHAR to SHORT */
			} else if( outType==T_SHORT ) {       /* output is unsigned short */
				ps = (short *)convertedData;
				for(i=0; i<nPts; i++) {
					ps[i] = (short)pc[i];
				}

/* UCHAR to FLOAT */
			} else if(outType == T_FLOAT) { /* output is float */
				pf=(float *)convertedData;
				for( i=0; i<nPts; i++ ) {
					pf[i] = (float)pc[i];
				} 
			
/* UCHAR to COMPLEX */
			} else if(outType == T_COMPLEX) { /* output is float */
				pf=(float *)convertedData;
				for( i=0; i<nPts; i++ ) {
					pf[i*2]     = (float)pc[i];
					pf[i*2 + 1] = 0.0;
				} 
			}
			
		break;

		case T_USHORT:			
			pus = (unsigned short *)inData; /* input is unsigned short */
			error = vminmax( inData, nPts, &usmax, &usmin, T_USHORT );
				RETURNONERROR;
			*fmax = usmax;
			*fmin = usmin;
			range = usmax - usmin;

/* USHORT to FLOAT */
			if( outType==T_FLOAT ) { /* output is float, no scaling */
				pf = ( float * )convertedData;
				for( i=0; i<nPts; i++) {
					pf[i] = ( float )pus[i];
				}

/* USHORT to COMPLEX */
			} else if( outType==T_COMPLEX ) { /* output is float, no scaling */
				pf = ( float * )convertedData;
				for( i=0; i<nPts; i++) {
					pf[i*2]   = ( float )pus[i];
					pf[i*2+1] = 0.0;
				}

/* USHORT to SHORT */
			} else if( outType == T_SHORT ) {
				ps = (short *)convertedData;
				
				if( usmax > SHRT_MAX ) { /* must rescale if input exceeds SHRT_MAX */
					short HalfMax = SHRT_MAX/2;
					for( i=0; i<nPts; i++ ) {
						ps[i] = pus[i] - HalfMax;
					}
					SET( *rules, kDataRescaled );
					error = CONVERSION_ERROR;
				} else { 
					for( i=0; i<nPts; i++ ) {
						ps[i] = pus[i];
					}
				}
										
/* USHORT to UCHAR */
			} else if( outType==T_UCHAR ) { /* output is unsigned char, scale to 0 - UCHAR_MAX, if needed */
				pc = (unsigned char *)convertedData;

				if( (usmax<UCHAR_MAX) && (usmin>0) ) {
					for( i=0; i<nPts; i++) {
						pc[i] = (unsigned char) pus[i];
					}
				} else { /* data range exceeds output range: rescale */
					typemax = UCHAR_MAX;
					for( i=0; i<nPts; i++) {
						pc[i] = (unsigned char)((float)pus[i]*( typemax/range));
					}
					SET( *rules, kResolutionLost + kDataRescaled );
					error = CONVERSION_ERROR;
				} 
			} 
		break;

		case T_SHORT:			
			ps = (short *)inData; /* input is signed short */
			error = vminmax( inData, nPts, &smax, &smin, T_SHORT );
				RETURNONERROR;
			*fmax = smax;
			*fmin = smin;
			range = smax - smin;

/* SHORT to FLOAT */
			if( outType==T_FLOAT ) { /* output is float, no scaling */
				pf = ( float * )convertedData;
				for( i=0; i<nPts; i++) {
					pf[i] = ( float )ps[i];
				}

/* SHORT to COMPLEX */
			} else if( outType==T_COMPLEX) { /* output is float, no scaling */
				pf = ( float * )convertedData;
				for( i=0; i<nPts; i++) {
					pf[i*2]   = ( float )ps[i];
					pf[i*2+1] = 0.0;
				}

/* SHORT to U_SHORT */
			} else if( outType == T_USHORT ) {
				unsigned short HalfMax;
				pus = (unsigned short *)convertedData;
				
				if( smin < 0 ) { /* negative input values, must rescale */
					HalfMax = 1 + USHRT_MAX/2;
					for( i=0; i<nPts; i++) {
						pus[i] = ps[i] + HalfMax;
					}
				} else {
					for( i=0; i<nPts; i++) {
						pus[i] = ps[i];
					}					
				}
				
/* SHORT to UCHAR */
			} else if( outType==T_UCHAR ) { /* output is unsigned char, scale to 0 - UCHAR_MAX, if needed */
				pc = (unsigned char *)convertedData;

				if( (smax<UCHAR_MAX) && (smin>0) ) {
					for( i=0; i<nPts; i++) {
						pc[i] = (unsigned char) ps[i];
					}
				} else { /* data range exceeds output range: rescale */
					typemax = UCHAR_MAX;
					for( i=0; i<nPts; i++) {
						pc[i] = (unsigned char)((float)ps[i]*( typemax/range ));
					}
					SET( *rules, kResolutionLost + kDataRescaled );
					error = CONVERSION_ERROR;
				} 
			} 
		break;

		case T_COMPLEX:
		case T_FLOAT:
			pf = (float *)inData; /* input is float */
			if( inType == T_COMPLEX ) {
				error = vmag( pf, 2, pf, 1, nPts );
				RETURNONERROR;
			}
			error = vminmax( pf, nPts, fmax, fmin, T_FLOAT );
				RETURNONERROR;
			range = *fmax - *fmin;
			minAsFloat = *fmin;	// speeds up processing.

			SET( *rules, kResolutionLost);
			error = CONVERSION_ERROR;
			
/* COMPLEX (FLOAT) to FLOAT */
			if( outType == T_FLOAT ) {
				error = vmov( pf, 1, convertedData, 1, nPts, T_FLOAT );
				RETURNONERROR;
			}
			
/* FLOAT to DOUBLE */
			if( outType == T_DOUBLE ) {
				pd = (double *)convertedData;
				for( i=0; i<nPts; i++) {
					pd[i] = (double)pf[i];
				}
			}
/* FLOAT to USHORT */
			if( outType==T_USHORT ) {
				pus = (unsigned short *)convertedData;

	/* special case for all equal (e.g., all points are zero) */
				if( *fmax == *fmin ) {	/* can't rescale */
					for( i=0; i<nPts; i++) {
						pus[i] = (unsigned short)pf[i];
					}

	/* Special case for stat images */
				} else if( (*fmax<=1.0) && (*fmin>=-1.0) ) {
					for( i=0; i<nPts; i++) {
						pus[i] = USHRT_MAX/2 + (unsigned short)pf[i] * USHRT_MAX/2;
					}
					SET( *rules, kResolutionLost + kRescaledNarrowRange + kDataRescaled );

				} else if( (*fmax < (float)USHRT_MAX) && ( *fmin>=0 ) ) {
					if( *rules & kNeed256Values ) {
						for( i=0; i<nPts; i++) {
							pus[i] = (unsigned short)( (pf[i] - minAsFloat) * (256/range ));
						}
						SET( *rules, kDataRescaled );

					} else { /* copy */
						for( i=0; i<nPts; i++) {
							pus[i] = (unsigned short)pf[i];
						}
					}
				} else { /* data range exceeds output range: rescale */
					typemax = USHRT_MAX;

					for( i=0; i<nPts; i++) {
						pus[i] = (unsigned short)( (pf[i] - minAsFloat) * ( typemax/range ));
					}
					error = CONVERSION_ERROR;
				}

/* FLOAT to SHORT */
			} else if( outType==T_SHORT ) {
				ps = (short *)convertedData;

	/* special case for all equal (e.g., all points are zero) */
				if( *fmax == *fmin ) {	/* can't rescale */
					for( i=0; i<nPts; i++) {
						ps[i] = (short)pf[i];
					}

	/* Special case for stat images */
				} else if( (*fmax<=1.0) && (*fmin>=-1.0) )  {
					for( i=0; i<nPts; i++) {
						ps[i] = SHRT_MAX/2 + (short)pf[i] * SHRT_MAX/2;
					}
					SET( *rules, kResolutionLost + kRescaledNarrowRange + kDataRescaled );

				} else if( (*fmax < (float)SHRT_MAX) && ( *fmin>=(float)SHRT_MIN ) ) {
					if( *rules & kNeed256Values ) {
						for( i=0; i<nPts; i++) {
							ps[i] = (short)( (pf[i] - minAsFloat) * (256/range ));
						}
						SET( *rules, kDataRescaled );

					} else { /* copy */
						for( i=0; i<nPts; i++) {
							ps[i] = (short)pf[i];
						}
					}
				} else { /* data range exceeds output range: rescale */
					typemax = SHRT_MAX;
					for( i=0; i<nPts; i++) {
						ps[i] = (short)( (pf[i] - minAsFloat) * (typemax/range ));
					}
					error = CONVERSION_ERROR;
				}

/* FLOAT to COMPLEX */
			} else if( outType == T_COMPLEX ) {
				pCOM=(float *)convertedData;
				for( i=0; i<nPts; i++) {
					pCOM[i*2]   = pf[i];
					pCOM[i*2+1] = 0.0;
				}
/* FLOAT to UCHAR */
			} else if( outType==T_UCHAR ) {
				pc = (unsigned char *)convertedData;

	/* special case for all equal (e.g., all points are zero) */
				if( *fmax == *fmin ) {	/* can't rescale */
					for( i=0; i<nPts; i++) {
						pc[i] = (unsigned char)pf[i];
					}

	/* special case for statistical images which vary from 1 to -1 */
				} else if( (*fmax<=1.0) && (*fmin>=-1.0) )  { /*  */
					for( i=0; i<nPts; i++) {
						pc[i] = UCHAR_MAX/2 + (unsigned short)pf[i] * UCHAR_MAX/2;
					}
					SET( *rules, kRescaledNarrowRange + kDataRescaled );
					error = CONVERSION_ERROR;

	/* the more or less standard case */
				} else if( (*fmax<=(float)UCHAR_MAX) && (*fmin>=0) ) {
					if( *rules & kNeed256Values ) {
						for( i=0; i<nPts; i++) {
							pc[i] = (unsigned char)( (pf[i] - minAsFloat) * (256.0/range ));
						}
						SET( *rules, kDataRescaled );

					} else { /* copy */
						for( i=0; i<nPts; i++) {
							pc[i] = (unsigned char)pf[i];
						}
					}

	/* force rescale if data range exceeds type range for unsigned char */
				} else {
					typemax = UCHAR_MAX;
					for( i=0; i<nPts; i++) {
						pc[i] = (unsigned char)( (pf[i] - minAsFloat) * (typemax/range ));
					}
					SET( *rules, kDataRescaled );
					error = CONVERSION_ERROR;

				}
			}
		break;

		default:
			SET( *rules, kUnknownConversion );
			break;
	}	/* main switch */
	return error;
}

/* --> *******************************************************************************
*     Swapping for simple types:<b> swI swF swS swUS swUI swD swL </b> 
*     Each swaps the original in place and returns the swapped value in place
*
*     int            swI( int *RevInt )
*     float          swF( float *RevFloat )
*     short          swS( short *RevShort )
*     unsigned short swUS( unsigned short *RevUShort )
*     unsigned int   swUI( unsigned int *RevUInt )
*     double         swD( double *RevDouble )
*     long           swL( long *RevLong )
<!--   *******************************************************************************/
int    swI( int *RevInt )
{
	vbyteswap( RevInt, 1, T_INT );
	return *RevInt;
}

float  swF( float *RevFloat )
{
	vbyteswap( RevFloat, 1, T_FLOAT );
	return *RevFloat;
}

short  swS( short *RevShort )
{
	vbyteswap( RevShort, 1, T_SHORT );
	return *RevShort;
}

unsigned short swUS( unsigned short *RevUShort )
{
	vbyteswap( RevUShort, 1, T_USHORT );
	return *RevUShort;
}

unsigned int swUI( unsigned int *RevUInt )
{
	vbyteswap( RevUInt, 1, T_INT );
	return *RevUInt;
}

double swD( double *RevDouble )
{
	vbyteswap( RevDouble, 1, T_DOUBLE );
	return *RevDouble;
}

long swL( long *RevLong )
{
	vbyteswap( RevLong, 1, T_LONG );
	return *RevLong;
}

/* --> *******************************************************************************
<B> MoveSwap </b>  
MoveSwap moves data from Src to Dst, swapping the bytes, if requested
OSErr MoveSwap( void *Src,        // The location of a simple data type
				void *Dst,        // output location for the data (swapped or unswapped)
				Boolean swap,     // if true, swap byte order
				short data_type ) // standard data_type delcarator
<!--   *******************************************************************************/
OSErr MoveSwap( void *Src, void *Dst, Boolean swap, short data_type )
{
	OSErr error = noErr;

	error = vmov( Src, 1, Dst, 1, 1, data_type );
	if( error ) {
		return error;
	}
	if( swap ) {
		switch( data_type ) {
			case T_CHAR:
			case T_UCHAR:
			break;
			
			case T_SHORT:
			case T_USHORT:
				swS( (short *)Dst );
			break; 
	
			case T_INT:
				swI( (int *)Dst );
			break;
	
			case T_FLOAT:
				swF( (float *)Dst );
			break;
	
			case T_DOUBLE:
				swD( (double *)Dst );
			break;
	
			case T_LONG:
				swL( (long *)Dst );
			break;
	
			default:
				error = UNKNOWNTYPE;
		}
	}
	return error;
}

/* --> *******************************************************************************
*<b> equalString </b> 
* Test to see that two strings are equal, and return a Boolean
* true if s1 and s2 are the same. false otherwise
*
* int equalString( char *s1, char *s2 )
<!--   *******************************************************************************/
int equalString( char *s1, char *s2 )
{
	if( !strcmp( s1, s2 )) {
		return true;
	} else {
		return false;
	}
}

/* --> *******************************************************************************
*<b> CountWords </b> 
* Count the number of words in a string.
*
* int CountWords( char *InputString, int *NumWords )
<!--   *******************************************************************************/
OSErr CountWords( char *InputString, int *NumWords )
{
	OSErr error = noErr;
	char *pc;
	long count = 0;
	long InputLength;
	char *theString;

	if( !InputString ) {
		return INVALID_ADDRESS;
	}

	if( InputLength = strlen( InputString ) == 0 ) {
		*NumWords = 0;
	}

	theString = (char *)malloc( InputLength );

	pc = InputString;
	do {
		sscanf( pc, "%s", theString );
		count++;
		pc+= strlen( theString )+1;
	} while( pc - InputString < InputLength );

	free( theString );

	*NumWords = count;

	return error;
}
