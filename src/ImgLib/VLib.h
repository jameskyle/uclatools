/* <HTML><HEAD><TITLE>VLib routines</TITLE></HEAD><BODY><PRE>
/***************************************************************************
*	<font size=+2><b>VLib.h - Part of ImgLib</b></font>
*	<i>Please note last saved date below</i>
*   <font size=+1><b>$Date: 2006/03/25 08:46:33 $</b></font><p>
*   A set of primitives for use in image processing.
*               c 1996-2002  Mark S. Cohen
*   This software is distributed as is with no guarantees.
*   Please report any errors or bug fixes to mscohen@ucla.edu
*
*   For academic use only. Commercial users should contact
*   Mark Cohen for licensing information.
*
************************************************************************** <!-- */
/****************************************************************
* $Id: VLib.h,v 1.29 2006/03/25 08:46:33 mscohen Exp mscohen $
* $Log: VLib.h,v $
* Revision 1.29  2006/03/25 08:46:33  mscohen
* vfminmax
*
* Revision 1.27  2003/12/15 05:25:07  mscohen
* Added CountWords utility
*
* Revision 1.26  2002/12/17 06:22:17  mscohen
* Added vabs function
*
* Revision 1.25  2002/09/11 16:59:23  mscohen
* Removed CK_ functions - failed experiment.
*
* Revision 1.24  2002/09/04 17:41:01  mscohen
* Created GetVLibVer()
*
* Revision 1.23  2002/09/03 19:24:32  mscohen
* Upgraded to local RCS file.
*
* Revision 1.17  2002/08/29 07:42:45  mscohen
* maintenance checkin
*
 * Revision 1.22  2002/04/16  17:01:51  mscohen
 * copyright
 *
 * Revision 1.22  2002/04/16  17:01:51  mscohen
 * copyright
 *
 * Revision 1.21  2002/04/16  16:01:43  mscohen
 * html
 *
 * Revision 1.20  2002/04/12  05:51:04  mscohen
 * Added convert to percent function
 *
 * Revision 1.18  2001/12/11  04:14:11  mscohen
 * Added PercentCount utility
 *
 * Revision 1.17  2001/12/04  04:27:26  mscohen
 * Added function CreateTempFile
 *
 * Revision 1.16  2001/08/24  16:44:31  mscohen
 * added html commenting
 *
 * Revision 1.15  2001/08/23  19:55:17  mscohen
 * Added MoveSwap to facilitate header translation for unaligned structs
 *
 * Revision 1.14  2001/03/29  02:19:14  mscohen
 * added GetFileSize function.
 *
 * Revision 1.13  2001/03/17  05:54:42  mscohen
 * TYPO!
 *
 * Revision 1.12  2001/03/17  05:53:06  mscohen
 * renamed EqualString to equalString to avoid Macintosh name conflict.
 *
 * Revision 1.11  2001/03/17  04:08:41  mscohen
 * Ooops, typo
 *
 * Revision 1.10  2001/03/17  04:06:23  mscohen
 * Added vfill function
 *
 * Revision 1.9  2001/03/15  15:41:41  mscohen
 * Added equalString utility
 *
 * Revision 1.9  2000/08/28  21:57:34  mscohen
 * Added SetTypeRange function
 *
 * Revision 1.9  2000/08/28  21:57:34  mscohen
 * Added SetTypeRange function
 *
 * Revision 1.8  2000/07/06  22:22:41  mscohen
 * fixed bugs in memFree
 *
 * Revision 1.8  2000/07/06  22:22:41  mscohen
 * fixed bugs in memFree
 *
 * Revision 1.5  2000/03/08  19:37:03  mscohen
 *  Added more ck_f functions
 *
 * Revision 1.4  2000/02/15  15:55:04  mscohen
 * Rename BigEndian and LittleEndian functions
 *
 * Revision 1.3  1999/09/28  04:09:58  mscohen
 * Fixed set and clear flags for handing byte order
 *
 * Revision 1.2  1999/09/22  15:29:12  mscohen
 * Added DBG function.
 *
 * Revision 1.6  1999/07/14  05:53:10  mscohen
 * ANSI compatibility
 *
 * Revision 1.5  1999/05/30  12:23:49  mscohen
 * Patches to function names. vdiv now returns OSErr.
 *
 * Revision 1.4  1999/05/28  22:15:16  mscohen
 * Bug fixes in vsmul. Some reallocation of functions to MathLib
 *
 * Revision 1.3  1999/04/20  00:05:33  mscohen
 * Added SET and CLEAR macros. Added LogMessage routine
 *
 * Revision 1.2  1999/02/28  21:08:24  mscohen
 * Cleaned up handling of OSErr
 *
 * Revision 1.1  1999/02/28  19:58:01  mscohen
 * Initial revision
 *
* 
*	VLib.h
*	A basic vector processing library - mostly utilities
*****************************************************************/
#ifndef __VLIB__
#define __VLIB__
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <limits.h>
#include <ctype.h>

#include "Target.h"
#include "ImgLibError.h"

#ifdef MAC
#undef POST_DBG
#endif

#ifndef MAC
#undef POST_DBG
/* add a bunch of macintosh constructs */
#ifndef OSErr
#define OSErr short
#endif
#ifndef noErr
#define noErr 0
#endif
#ifndef Ptr
#define Ptr char *
#endif
#ifndef Boolean
#define Boolean unsigned char
#endif
#ifndef nil
#define nil NULL
#endif

#endif
#ifdef SCANSTAT
#include "EasyApp.h"
/*
#include "scanSTAT.h"
*/
#endif

#ifndef true
#define true 1
#endif
#ifndef false
#define false 0
#endif

/* NO LONGER SAME AS USED IN MGH READ_IMAGE */
/* Types are in order of increasing resolution... */
#define T_CHAR       1
#define T_UCHAR      2
#define T_SHORT      3
#define T_USHORT     4
#define T_INT        5
#define T_LONG       6
#define T_ULONG      7
#define T_FLOAT      8
#define T_DOUBLE     9
#define T_COMPLEX   10
#define T_NATIVE    11

/* on the alpha station (sun) MAC:
short int           2 (2)	2
int                 4 (4)	4
long int            8 (4)	4
unsigned short int  2 (2)	2
unsigned int        4 (4)	4
unsigned long int   8 (4)	4
char                1 (1) 1
unsigned char       1 (1) 1
float               4 (4) 4
long float          8 (8)
double              8 (8) 8
*/
#define SunINT 4

#define RETURNONERROR if (error != noErr ) return error
#define RETURNTHISERROR(x) if (error != noErr ) return (x)

#define max(a,b)        (a)>(b) ? (a) : (b) /* macro for finding the larger of two */
#define min(a,b)        (a)<(b) ? (a) : (b) /* macro for finding the smaller of two */

#define SET(a,b)        a |= b
#define CLEAR(a,b)      a &= ~b
#define IsSET(a,b)      (a&b ? true : false)

#ifdef MAC
#define RETURNONNIL(h) if( h == nil ) return MEMORY_ALLOCATION_ERR
#else
#define RETURNONNIL(h) if( h == nil ) return MEMALLOCERROR
#endif

/* type_con results and control */
#define kNeverRescale          0 // not yet supported
#define	kAlwaysRescale         2 // not yet supported
#define kScaleToSpecifiedRange 4 // not yet supported
#define	kScaleToOutputRange    8 // not yet supported
#define kNeed256Values        16

#define kLossLess             0
#define kResolutionLost       1
#define kDataRescaled         2
#define kRescaledNarrowRange  4
#define kUnknownConversion    8

/* --> */
OSErr    InitMessageLog( void );
OSErr    LogMessage( char *ErrorString );
void     GetVLibVer( char *buff );
void     DBG ( char *msg );
Boolean  PercentCount( int count, int max, int stepSize, int *ans );
char     *ReportDataType( short data_type );
void     SetTypeRange( float *outMax, float *outMin, short data_type );
void     IsTrueorFalse (Boolean b, char *s);
void     ShowError ( OSErr error, char *where, char *msg);
FILE     *errfopen( char *fname, char *mode );
OSErr    ck_fopen( FILE **f, char *fname, char *mode );
OSErr    ck_fwrite( void *data, size_t itemSize, size_t nmemb, FILE *f );
OSErr    ck_fread ( void *data, size_t itemSize, size_t nmemb, FILE *f );
OSErr    ck_fclose( FILE *f );
Ptr      ck_malloc( long size, char *msg );
Ptr      ck_calloc( long number_of_objects, int size, char *msg );
OSErr    CreateTempFile( FILE **TempFile, char **itsName );
OSErr    GetFileSize( long *filesize, char *fname );
OSErr    ReadLastBytes( char *fname, char *buff, long nBytes );
#ifdef MAC
OSErr    LockHandleToPointer( Handle h, void **p, SignedByte *state );
OSErr    ck_AllocateHandle(long size, Handle *handleAddress);
#endif
OSErr    memFree( void **theMem, char *msg );
OSErr    cpy_alloc(void **destPtr, char *msg1, void *srcPtr, char *msg2);
char     *YorN( int theFlag );
Boolean  pcByteOrder();	// i.e., LittleEndian
Boolean  macByteOrder();// i.e., BigEndian
void     lowerString( char *theString );

/* GENERIC VECTOR ROUTINES */
OSErr  vmov( void *invec, int instep, void *outvec, int ostep, long vlen, int data_type);
OSErr  vfill(void *invec, void *fill, long vlen, int data_type);
OSErr  vfsmul(void *invec, int instep, void *outvec, int ostep,
             float scalar, long vlen, int data_type );
OSErr  vsmul(void *invec, int instep, void *outvec, int ostep,
             void *scalar, long vlen, int data_type );
OSErr  vadd( void *invec1, int step1, void *invec2, int step2,
             void *outvec, int out_step, long vlen, int data_type );
OSErr  vsub( void *invec1, int step1, void *invec2, int step2,
             void *outvec, int out_step, long vlen, int data_type );
OSErr  vmul( void *invec1, int step1, void *invec2, int step2,
             void *outvec, int out_step, long vlen, int data_type );
OSErr  vdiv( void *numvec, int step1, void *denvec, int step2,
             float *outvec, int out_step, long vlen, int data_type );
OSErr  vminmax( void *invec, long vlen, void *fmax, void *fmin, int data_type );
OSErr  vfminmax( void *invec, long vlen, float *vecmax, float *vecmin, int data_type );
OSErr  vsq( void *invec, int step, long vlen, int data_type );
OSErr  vsqrt( void *invec, int step, long vlen, int data_type );
double vssq( void *invec, int step, long vlen, int data_type );
double vsum( void *invec, int step, long vlen, int data_type );
OSErr vabs( void *invec, int istep, void *ovec, int ostep, long vlen, int data_type );
OSErr  vclr( void * vec, long vlen, int data_type);
int    get_datasize(int data_type);
OSErr  vbyteswap(void *invec, long vlen, int data_type);
OSErr  vbyteNswap(void *invec, long vlen, short n_bytes);
OSErr  ConvertToPercent( void *invec, void *refvec, void *outvec, long vlen, int data_type );
OSErr  type_convert( void *inData, int inType, void *convertedData, int outType,
                    long nPts, float *fmax, float *fmin, short *rules );
int    swI( int *RevInt );
float  swF( float *RevFloat );
short  swS( short *RevShort );
unsigned short swUS( unsigned short *RevUShort );
unsigned int   swUI( unsigned int *RevUInt );
double swD( double *RevDouble );
long   swL( long *RevLong );
OSErr  MoveSwap( void *Src, void *Dst, Boolean swap, short data_type );
int    equalString( char *s1, char *s2 );
OSErr  CountWords( char *InputString, int *NumWords );

#endif
/* </PRE></BODY></HTML> */
