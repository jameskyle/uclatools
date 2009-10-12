/*<HTML><HEAD><TITLE>ImgLib routines</TITLE></HEAD><BODY><PRE>
* <font size=+2><b>ImgLib.c</b></font>
* <i>Please note last saved date below:</i><p>
*  <font size=+1><b>$Date: 2008/05/16 17:19:08 $</b><p></font>
*    A set of primitives for use in image processing.
*               c 1996-2002  Mark S. Cohen
*    This software is distributed as is with no guarantees.
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
*
*              ORIGINAL REVISION DATE: 9/15/96
*              MAJOR REVISION: 9/21/96
*              BACK CONVERSION TO UNIX 3/24/98
* <!--
* $Id: ImgLib.c,v 2.96 2008/05/16 17:19:08 mscohen Exp mscohen $
*  $Log: ImgLib.c,v $
*  Revision 2.96  2008/05/16 17:19:08  mscohen
*  Added function CalcT2
*
*  Revision 2.93  2006/06/09 18:48:09  mscohen
*  code cleanup
*
*  Revision 2.92  2006/05/20 04:15:19  mscohen
*  Modified UC_ReadHeader so that isoX and isoY are set correctly when reading from non-ANMR files
*
*  Revision 2.89  2006/03/26 06:07:08  mscohen
*  Fixed UC_Readimage handling of fileDatatype
*  UC_Readimage now reliably reads complex data
*  Modified dsrFromImage to fix problems with complex numbers
*
*  Revision 2.90  2006/03/25 08:48:43  mscohen
*  WritAnalyzeFile() (still not debugged)
*  Upgraded UC_Readimage to handle complex data
*
*  Revision 2.89  2005/05/06 16:30:32  mscohen
*  Changed convention for tr in analyze struct to seconds (was microseconds).
*
*  Revision 2.88  2003/02/05 23:09:42  mscohen
*  Removed superfluous checking of number of dimensions in ParseDSR. Checking of header size is adequate.
*
*  Revision 2.87  2003/01/22 20:42:41  mscohen
*  Detect 0 slice to slice distance and populate with guesses if encountered.
*
*  Revision 2.86  2003/01/15 02:02:02  mscohen
*  Fixed bug that blocked reading of analyze float files with less than 3 dimensions.
*
*  Revision 2.85  2002/09/10 01:04:05  nina
*  changed fopen mode from "r"/"w" to "rb"/"wb" as appropriate
*
*  Revision 2.84  2002/09/04 17:40:30  mscohen
*  Added version information for VLib and MathLib in OpenProcFile
*
*  Revision 2.83  2002/09/04 12:12:04  mscohen
*  Added afni support
*
*  Revision 2.82  2002/09/03 19:26:41  mscohen
*  Modified default values of bits/pixel
*  Upgraded to local RCS file.
*
 * Revision 2.81  2002/06/26  13:06:26  mscohen
 * Major change: redefined most short images as signed short. This alters:
 *     FindOutputType, InitializeIm, UC_Readheader, ParseDSR, interp2D UC_Readimage
 * Changed default behavior of CreateHeader to clear cropping flags.
 * Numerous cosmetic, formatting and commenting changes.
 *
 * Revision 2.80  2002/06/06  03:52:59  mscohen
 * Added UCFlagToString
 *
 * Revision 2.79  2002/06/01  02:12:33  mscohen
 * Altered dsrFromImage to duplicate UCLA flags. Misc. formatting changes.
 *
 * Revision 2.79  2002/05/13  15:58:31  mscohen
 * Altered CreateHeader to yield correct ysize in MGH formats
 * Altered default behavior of CreateHeader to determine number of rows in MGH headers from n_slices
 *
 * Revision 2.78  2002/05/13  15:44:09  mscohen
 * Changed call line for OpenProcFile to support standard error handling.
 *
 * Revision 2.77  2002/05/08  14:47:42  mscohen
 * removed irritiating diagnostic in image flip function
 *
 * Revision 2.76  2002/04/26  03:32:37  mscohen
 * Removed nonsense debug message
 * Direct support of UserData area of genesis header.
 *
 * Revision 2.75  2002/04/25  02:11:55  mscohen
 * Empty functions added for Varian file formats
 * Manual optimization of FloatToCImage to improve speed.
 *
 * Revision 2.74  2002/04/19  03:47:41  mscohen
 * Changes to parseSignaHeader to use GE-type header offsets.
 *
 * Revision 2.73  2002/04/17  03:53:01  mscohen
 * Removed dummy slice thickness and tr from parseANMR, as this was confusing to users.
 *
 * Revision 2.72  2002/04/17  03:05:22  mscohen
 * Replaced all instances of erropen with ck_fopen. Made bugs. Probably fixed them.
 *
 * Revision 2.71  2002/04/16  18:41:47  mscohen
 * Stuff 'tr' into analyze pixdim[4] for fsl compatibility.
 *
 * Revision 2.70  2002/04/16  18:20:48  mscohen
 * More complete handling of degenerate headers - Analyze and MGH - to enter needed parameters
 *
 * Revision 2.69  2002/04/16  17:31:04  mscohen
 * Modified comments on ParseXXXHeader
 *
 * Revision 2.68  2002/04/16  17:01:55  mscohen
 * copyright
 *
 * Revision 2.67  2002/04/16  16:17:00  mscohen
 * more comments in GetSelectedVolume
 *
 * Revision 2.66  2002/04/16  16:02:02  mscohen
 * html
 *
 * Revision 2.65  2002/04/16  15:46:00  mscohen
 * Even more html
 *
 * Revision 2.64  2002/04/16  15:43:30  mscohen
 * Made saving date more prominent in html
 *
 * Revision 2.63  2002/04/16  15:10:25  mscohen
 * Modified dsrFromImage to dimensions more fields (pixdim was not fully initialized)
 *
 * Revision 2.62  2002/04/10  23:24:26  mscohen
 * fixed bug that gave incorrect dimensions for multi-slice data when CreateHeader
 * made MGH-style headers.
 *
 * Revision 2.59  2002/01/08  16:52:16  mscohen
 * Fixed a problem in GetSelectedVolume where reading Signa images failed, as the number of slices in the file (1) may be different
 * than the total number of slices acquired. Added new parameter in ImgLib.h for n_acquired_slices.
 *
 * Revision 2.57  2001/12/04  04:22:04  mscohen
 * Fixed handling of se_contrastOffset and sctimeOffset in ParseSignaHeader
 * Changed parameter data types in FindOutputType
 *
 * Revision 2.55  2001/11/27  21:11:58  mscohen
 * Over-wrote the cropping flags in ParseANMRHeader. This tends to obsolete this
 * version of real time imaging, but is more robust for the present.
 * c'est la guerre.
 *
 * Revision 2.54  2001/11/25  20:43:33  mscohen
 * Changed sctime to T_FLOAT (was T_SHORT, an error)
 *
 * Revision 2.53  2001/11/25  20:26:58  mscohen
 * Support for fov moved from InitializeIm to Parse routines
 * Cleaned up calculation of cropping in ParseANMRHeader
 * Non sqware FOV in Signa images still not correct
 *
 * Revision 2.52  2001/10/18  04:20:43  mscohen
 * Renamed 'overlay' to 'Overlay' to eliminate Mach name conflict.
 *
 * Revision 2.51  2001/08/24  20:30:17  zrinka
 * changed calling sequence for GetSelectedVolume,
 * UC_ReadImage and UC_ReadImage2D - removed crop flag.
 *
 * Revision 2.50  2001/08/24  16:59:44  mscohen
 * Changed html
 *
 * Revision 2.49  2001/08/24  15:03:32  mscohen
 * Altered type designations for Analyze to maintain compatibility with FSL
 *
 * Revision 2.48  2001/08/24  11:44:56  mscohen
 * Added more html decorations
 *
 * Revision 2.47  2001/08/23  21:17:04  mscohen
 * Fixed html comments (more to come)
 *
 * Revision 2.45  2001/08/23  20:02:12  mscohen
 * Fixed ParseSignaHeader
 *
 * Revision 2.44  2001/08/17  15:07:39  mscohen
 * Fixed a silly comment formatting error.
 *
 * Revision 2.43  2001/08/17  13:31:01  mscohen
 * Added html documentation codes.
 *
 * Revision 2.42  2001/08/16  22:12:11  mscohen
 * Fixed a (stupid) bug in dsrFromImage that set glmin to a large positive number.
 *
 * Revision 2.41  2001/08/16  21:53:29  mscohen
 * Initialized fileMax and fileMin so that im.theMaxf and im.theMinf are properly calculated
 *
 * Revision 2.40  2001/04/18  21:19:06  mscohen
 * up-rev'd functions that call ck_fopen
 *
 * Revision 2.39  2001/03/19  19:28:07  mscohen
 * Modified CreateHeader so that byte order of header is the same as that of the input IMAGE struct
 *
 * Revision 2.38  2001/03/17  04:05:49  mscohen
 * Added notes about BigEndian/LittleEndian
 *
 * Revision 2.37  2001/02/28  14:38:22  mscohen
 * Update copyright notice
 *
 * Revision 2.45  2000/11/02  18:01:43  mscohen
 * Added ImageFlipY
 *
 * Revision 2.44  2000/10/20  17:29:50  mscohen
 * fixed integer range error in dsrFromImage
 *
 * Revision 2.43  2000/10/19  15:35:06  mscohen
 * Removed FTP functions from ImgLib
 *
 * Revision 2.42  2000/09/10  02:21:25  mscohen
 * Fixed bug in dsrFromImage in adding byte order to analyze header.
 *
 * Revision 2.41  2000/08/29  15:59:02  mscohen
 * changed handling of byte order in dsrFromImage
 *
 * Revision 2.40  2000/08/28  21:48:25  mscohen
 * added terms in finterp2D to suppress error reporting
 * removed unused variables in imageDataRange
 * removed rows and cols from command line of mdisplay
 * cosmetic mods to CreateHeader
 * modified dsrFromImage - datatype is interpreted from IMAGE struct
 * modified creation of time/date string in dsrFromImage
 * UC_Readheader now correctly determines data type for Signa images
 * ParseDSR now includes pixel dimensions
 * UC_Readimage variable name change (was imax, now InMax)
 *
 * Revision 2.39  2000/08/24  19:05:35  mscohen
 * minor mods to unmdisplay
 *
 * Revision 2.38  2000/08/24  15:29:50  mscohen
 * new call line for unmdisplay
 *
 * Revision 2.37  2000/08/23  15:12:52  mscohen
 * added function ReportFileType
 * added function CreateHeader
 * bug fix in memfree calls (pointers to void)
 * added function FindOutputType
 * pass more dimensions to header on UC_Readheader
 * TimePt is now int in readimage
 * Modified GetSelectedVolume
 * added FTPRead function (not completed)
 *
 * Revision 2.35  2000/05/08  17:30:36  mscohen
 * Modified call line for UC_Readheader (MAC)
 *
 * Revision 2.33  2000/05/02  20:17:49  mscohen
 * Modified imageDataRange algorithm for better control of contrast
 * Generalize type_conversion
 *
 * Revision 2.29  2000/03/26  11:50:08  mscohen
 * Many changes to imageDataRange and fixed the maxf minf functions (finally).
 * More changes to imageDataRange are pending.
 *
 * Revision 2.28  2000/03/25  13:32:58  mscohen
 * Bug fixes in Maxf and Minf (UC_Readimage)
 *
 * Revision 2.27  2000/03/23  13:07:46  mscohen
 * Fix bug in file min max (UC_Readimage)
 *
 * Revision 2.26  2000/03/23  12:09:21  mscohen
 * Fixed data size bug in ImageDataRange
 * Changed handling of IMAGE struct to consolidate dimensional info.
 * Populate im.isoX and isoY fields
 * Save xmt and receive gains in IMAGE
 * Fix bug in type_con for conversion to char
 *
 * Revision 2.24  2000/03/08  19:33:29  mscohen
 * Problems (unknown source) in memory allocation and deallocation in imageDataRange. Works now.
 *
 * Revision 2.23  2000/03/08  06:12:25  mscohen
 * Improved OpenProcFile to fix illegal file names derived from paths.
 * Add detection of file min and max in UC_ReadImage
 * Add ImageToAnalyzeHeader conversion

 * Revision 2.22  2000/02/15  15:54:39  mscohen
 * Rename BigEndian and LittleEndian functions
 *
 * Revision 2.21  1999/10/07  13:39:08  mscohen
 * Over-range error calculating num_bins in ImageDataRange.
 * /
 *
 * Revision 2.19  1999/09/29  17:27:47  mscohen
 * Major modifications to handling of ImageDataRange: now use a more statistically sound algorithm
 * that looks for a local minimum in the smoothed histogram of pixel intensities
 *
 * Revision 2.16  1999/09/22  15:24:18  mscohen
 * United various header files into a single ImgLib.h
 * Modified (slightly) the contents of ParseANMRHeader
 * Added OpenProcFile function.
 *
 * Revision 2.15  1999/08/20  21:03:24  mscohen
 * Modified handling of suffix calculation. Expanded size of name fields
 * in UCHeader.
 *
 * Revision 2.12  1999/07/14  05:51:17  mscohen
 * ANSI C compatibility
 *
 * Revision 2.10  1999/05/28  22:15:43  mscohen
 * Fix errors in ParseSignaHeader
 *
 * Revision 2.7  1999/04/20  00:04:40  mscohen
 * Big improvements to Readimage and type con. Modified global error handling
 *
 * Revision 2.6  1999/04/13  00:07:15  mscohen
 * Check in of modified Mac version
 *
 * Revision 2.5  1999/04/12  18:59:19  mscohen
 * Complete rebuild of UC_Readimage and UC_Readheader
 * Inclusion of UCHeader.h to redefine IMAGE struct
 * Full consistency with Macintosh versions
 *
 * Revision 2.4  1999/02/28  21:06:59  mscohen
 * extracted components of VLib
 *
 * Revision 2.3  1998/04/20  19:02:59  mscohen
 * Small changes to ReadImage and ReadHeader
 *
 * Revision 2.1  1998/03/28  09:18:04  mscohen
 * Maintenance
 *
 * Revision 2.0  1998/03/26  18:10:12  mscohen
 * This is the first multi-platform version
 * for Macintosh and unix.
 *
* ver 2.2 wls. removed case sensitivity to matching .img or .IMG
***************************************************************************/
#ifdef mactest
#define ENOENT 2
#endif

#include "ImgLib.h"
#include "MathLib.h"
#ifdef SCANSTAT
#include "EasyApp.h"
#include "scanSTAT.h"
#endif

#ifdef no_log_file
#define default_warn false
#else
#define default_warn true
#endif

/* this must be defined in the main for the program utilizing ImgLib */
extern char gMessageStr[255];

/* --> ***********************************************************************
*       Keep control of ImgLib revisions
*   <b>void GetImgLibVer( char *buff )</b>
*
*   Returns the version number for ImgLib in buff
*
<!-- ************************************************************************/
void GetImgLibVer( char *buff ) /* return a string describing ImgLib revision */
{
   static char id[] = "$Revision: 2.96 $$Date: 2008/05/16 17:19:08 $";
   char versionStr[31];
   char buffer[63];

/* extract version and date */
   sscanf( id, "%s%s%s%s", buffer, versionStr, buffer, buffer );
   strcat( versionStr, " " );
   strcat( versionStr, buffer );
   strcpy( buff, versionStr );
}


/* --> ***********************************************************************
*   <b>OpenProcFile</b>
*
*   Open a standard proc file for tracking user inputs and settings
*
*   TYPICAL CALLING SEQUENCE
*
*   static char id[] = "$Revision: 2.96 $$Date: 2008/05/16 17:19:08 $";
*   error = OpenProcFile( argc, argv, outFilename, id, &ProcFile );
*
*   INPUTS:   argc         -- from main command line
*             argv         -- from main command line
*             outFilename  -- basename of output file (typical)
*             ProcFile     -- the freshly opened ProcFile
*
*   FUTURE:  capture command line in users home directory
<!-- ************************************************************************/
OSErr OpenProcFile( int argc, char **argv, char *name, char *id, FILE **ProcFile )
{
	char      buffer[256];
	struct tm *date;
	time_t    now;
	short     i;
	char      versionStr[31];
	char      *progName, *last;
	OSErr     error = noErr;

// fix illegal names
	progName = argv[0];
        last = strrchr( progName, '/' ); //zrinka 08/20/02 - added to handle program
        if ( last != NULL ) 		//names such as /usr/local/bin/imconvert
            progName = last;
	while( *progName == '.' || *progName == '/' ) {
		progName++;
	}

/* extract version and date */
	sscanf( id, "%s%s%s%s", buffer, versionStr, buffer, buffer );
	strcat( versionStr, " " );
	strcat( versionStr, buffer );
	sprintf( buffer, "%s.%s.proc", name, progName );
	error = ck_fopen( ProcFile, buffer, "w" );
	if( error ) {
		*ProcFile = NULL;
		return error;
	}
	
	fprintf( *ProcFile, "%s version ...... %s\n", argv[0], versionStr );

	GetImgLibVer( buffer );
	fprintf( *ProcFile, "ImgLib version ....... %s\n", buffer );
#ifdef __MATHLIBH__
	GetMathLibVer( buffer );
	fprintf( *ProcFile, "MathLib version ...... %s\n", buffer );
#endif
	GetVLibVer( buffer );
	fprintf( *ProcFile, "VLib version ......... %s\n", buffer );

	now = time ( NULL );
	date = localtime ( &now );
	strftime( buffer, 128, "%c", date );
	fprintf( *ProcFile, "Executed on .......... %s\n", buffer );
	
	fprintf( *ProcFile, "\nCommand line:\n\t" );
	for( i=0; i<argc; i++ ) {
		fprintf( *ProcFile, "%s ", argv[i] );
	}
	
	fprintf( *ProcFile, "\n\n" );
	
	return error;
}

/* --> ************************************************************************
*  <b>ReportFileType</b>
*  Return the name of the input file type, returning it as a text string
*
*    char *ReportFileType( int file_type )
*
<!-- ************************************************************************/
char *ReportFileType( int file_type )
{
	switch( file_type ) {
		case ANALYZE:
			return "Analyze";
		break;

		case EPI:
			return "APD2";
		break;

		case SIGNA:
			return "Signa Genesis";
		break;

		case BFLOAT:
			return "MGH bfloat";
		break;

		case BSHORT:
			return "MGH bshort";
		break;

		case BUCHAR:
			return "MGH buchar";
			break;

		case VARIAN:
			return "Varian";
		break;
                
        case AFNI:	/* zrinka 08/20/02 - added for AFNI support */
			return "AFNI";
		break;

		default:
			return "unrecognized file type";
	}
}

char *UCFlagString( unsigned short int flags )
{
	char *theString;
	
	theString = (char *)malloc( 256 );

	if( flags ) {
		sprintf( theString, "( " );

		if( flags & kNeedXInterp ) {
			strcat( theString, "NeedXInterp " );
		}
		
		if( flags & kNeedYInterp ) {
			strcat( theString, "NeedYInterp " );
		}
		
		if( flags & kmacByteOrder ) {
			strcat( theString, "macByteOrder " );
		}
		
		if( flags & kNeedXCrop ) {
			strcat( theString, "NeedXCrop " );
		}
		
		if( flags & kNeedYCrop ) {
			strcat( theString, "kNeedYCrop " );
		}
		
		if( flags & kTimeOrder ) {
			strcat( theString, "TimeOrder " );
		}
		
		if( flags & kScanning ) {
			strcat( theString, "Scanning " );
		}
	
		strcat( theString, ")" );
		
		return theString;

	} else	{
		return "";
	}
}




/* --> ************************************************************************
*   <b>xinterp and yinterp</b>
*       Bilinear interpolation of a 2 D image along a single dimension
*       Only integer interpolation ratios are supported.
*
*   CALLING SEQUENCE: yinterp(*invec, xs, ys, Ratio, *outvec, data_type);
*                     xinterp(*invec, xs, ys, Ratio, *outvec, data_type);
*
*   INPUTS: invec  -- vector of arbitrary data type
*           xs     -- number of points in x dimension (input)
*           ys     -- number of points in y dimension (input)
*           Ratio  -- amount by which the image should be enlarged
*           data_type  -- standard UC data type
*   OUTPUTS:  None
*           outvec -- output vector of same data type as input
*
*   Note that xinterp and yinterp are the same, but for a swap of x & y variables.
*   finterpx and finterpy are floating point versions
<!-- ************************************************************************/
OSErr yinterp(void *invec, int xs, int ys, int Ratio, void *outvec, int data_type)
{
	OSErr	error = noErr;
	float	*ftmp, *fout, *fin;
	short	*stmp, *sout, *s_in;
	unsigned short *ustmp, *usout, *usin;
	unsigned char  *uctmp, *ucout, *ucin;
	char	*ctmp, *cout, *cin;
	int		*itmp, *iout, *iin;
	double	*dtmp, *dout, *din;
	int     r, c, j, k, rxsR, xsR, jxs;
	short	Toffs, Boffs;
	
	Boffs = Ratio/2;
	Toffs = Ratio - Boffs - 1;
	xsR  = xs * Ratio;
	
	switch( data_type )
	{
		case T_FLOAT:	/* comments are in T_FLOAT only. Algorithms are the same for all types */
			ftmp = (float *)ck_malloc( (ys-1) * get_datasize( T_FLOAT ), "yinterp: ftmp");
			
			for( c=0; c<xs; c++ )
			{
				fout = (float *)outvec;	/* the trick is to point to the beginning of each row */
				fout +=c;
				fin  = (float *)invec;
				fin +=c;
				
				/* pad the top of the output vector */
				for( r=0; r<Toffs*xs; r+=xs )	{
					fout[r] = fin[0];
				}	/* fout now points to past the top offset */
				
				fout += xs*Toffs;
				/* fill in the preserved points */
				for( r=0, rxsR=0; r<ys*xs; r+=xs, rxsR+=xsR )	{
					fout[rxsR] = fin[r];
				}
				
				/* build a vector containing the deltas in intensity */
				for( r=0, j=0; r<(ys-1)*xs; r+=xs, j++ )	{
					ftmp[j] = (fin[r+xs]-fin[r])/Ratio;
				}
				
				/* add the deltas to the points on the bottom */
				for( r=0, k=0, rxsR=0; r<(ys-1)*xs; r+=xs, k++, rxsR+=xsR )	{
					for( j=1, jxs=xs; j<Ratio; j++, jxs+=xs )	{
						fout[rxsR + jxs] = fin[r]+j*ftmp[k];
					}
				}
				/* pad the bottom */
				for(r=(ys-1)*Ratio; r<(ys-1)*Ratio+Boffs; r++ )	{
					fout[(r+1)*xs] = fin[(ys-1)*xs];
				}
			}
			error = memFree( (void **)&ftmp, "yinterp: ftmp" );
			break;
		case T_SHORT:
			stmp = (short *)ck_malloc( (ys-1) * get_datasize( T_SHORT ), "yinterp: stmp");
			
			for( c=0; c<xs; c++ )
			{
				sout = ((short *)outvec) + c;
				s_in = ((short *)invec) + c;
				for( r=0; r<Toffs*xs; r+=xs )
					sout[r] = s_in[0];
				sout += xs *Toffs;
				for( r=0, rxsR=0; r<ys*xs; r+=xs, rxsR+=xsR )
					sout[rxsR] = s_in[r];
				for( r=0, j=0; r<(ys-1)*xs; r+=xs, j++ )
					stmp[j] = (s_in[r+xs]-s_in[r])/Ratio;
				for( r=0, k=0, rxsR=0; r<(ys-1)*xs; r+=xs, k++, rxsR+=xsR )
					for( j=1, jxs=xs; j<Ratio; j++, jxs+=xs )
						sout[rxsR + jxs] = s_in[r]+j*stmp[k];
				for(r=(ys-1)*Ratio; r<(ys-1)*Ratio+Boffs; r++ )
					sout[(r+1)*xs] = s_in[(ys-1)*xs];
			}
			error = memFree( (void **)&stmp, "yinterp: stmp" );
			break;
		case T_USHORT:
			ustmp = (unsigned short *)ck_malloc( (ys-1) * get_datasize( T_USHORT ), "yinterp: ustmp");
			
			for( c=0; c<xs; c++ )
			{
				usout = ((unsigned short *)outvec) + c;
				usin  = ((unsigned short *)invec) + c;
				
				for( r=0; r<Toffs*xs; r+=xs )
					usout[r] = usin[0];
				usout += xs*Toffs;
				for( r=0, rxsR=0; r<ys*xs; r+=xs, rxsR+=xsR )
					usout[rxsR] = usin[r];
				for( r=0, j=0; r<(ys-1)*xs; r+=xs, j++ )
					ustmp[j] = (usin[r+xs]-usin[r])/Ratio;
				for( r=0, k=0, rxsR=0; r<(ys-1)*xs; r+=xs, k++, rxsR+=xsR )
					for( j=1, jxs=xs; j<Ratio; j++, jxs+=xs )
						usout[rxsR + jxs] = usin[r]+j*ustmp[k];
				for(r=(ys-1)*Ratio; r<(ys-1)*Ratio+Boffs; r++ )
					usout[(r+1)*xs] = usin[(ys-1)*xs];
			}
			error = memFree( (void **)&ustmp, "yinterp: ustmp" );
			break;
		case T_CHAR:
			ctmp = (char *)ck_malloc( (ys-1) * get_datasize( T_CHAR ), "yinterp: ctmp");
			
			for( c=0; c<xs; c++ )
			{
				cout = ((char *)outvec) + c;
				cin  = ((char *)invec) + c;
				for( r=0; r<Toffs*xs; r+=xs )
					cout[r] = cin[0];
				cout += xs*Toffs;
				for( r=0, rxsR=0; r<ys*xs; r+=xs, rxsR+=xsR )
					cout[rxsR] = cin[r];
				for( r=0, j=0; r<(ys-1)*xs; r+=xs, j++ )
					ctmp[j] = (cin[r+xs]-cin[r])/Ratio;
				for( r=0, k=0, rxsR=0; r<(ys-1)*xs; r+=xs, k++, rxsR+=xsR )
					for( j=1, jxs=xs; j<Ratio; j++, jxs+=xs )
						cout[rxsR + jxs] = cin[r]+j*ctmp[k];
				for(r=(ys-1)*Ratio; r<(ys-1)*Ratio+Boffs; r++ )
					cout[(r+1)*xs] = cin[(ys-1)*xs];
			}
			error = memFree( (void **)&ctmp, "yinterp: ctmp" );
			break;
		case T_UCHAR:
			uctmp = (unsigned char *)ck_malloc( (ys-1) * get_datasize( T_UCHAR ), "yinterp: uctmp");
			
			for( c=0; c<xs; c++ )
			{
				ucout = ((unsigned char  *)outvec) + c;
				ucin  = ((unsigned char  *)invec) + c;
				for( r=0; r<Toffs*xs; r+=xs )
					ucout[r] = ucin[0];
				ucout += xs*Toffs;
				for( r=0, rxsR=0; r<ys*xs; r+=xs, rxsR+=xsR )
					ucout[rxsR] = ucin[r];
				for( r=0, j=0; r<(ys-1)*xs; r+=xs, j++ )
					uctmp[j] = (ucin[r+xs]-ucin[r])/Ratio;
				for( r=0, k=0, rxsR=0; r<(ys-1)*xs; r+=xs, k++, rxsR+=xsR )
					for( j=1, jxs=xs; j<Ratio; j++, jxs+=xs )
						ucout[rxsR + jxs] = ucin[r]+j*uctmp[k];
				for(r=(ys-1)*Ratio; r<(ys-1)*Ratio+Boffs; r++ )
					ucout[(r+1)*xs] = ucin[(ys-1)*xs];
			}
			error = memFree( (void **)&uctmp, "yinterp: uctmp" );
			break;
		case T_INT:
			itmp = (int *)ck_malloc( (ys-1) * get_datasize( T_INT ), "yinterp: itmp");
			
			for( c=0; c<xs; c++ )
			{
				iout = ((int *)outvec) + c;
				iin  = ((int *)invec) + c;
				for( r=0; r<Toffs*xs; r+=xs )
					iout[r] = iin[0];
				iout += xs*Toffs;
				for( r=0, rxsR=0; r<ys*xs; r+=xs, rxsR+=xsR )
					iout[rxsR] = iin[r];
				for( r=0, j=0; r<(ys-1)*xs; r+=xs, j++ )
					itmp[j] = (iin[r+xs]-iin[r])/Ratio;
				for( r=0, k=0, rxsR=0; r<(ys-1)*xs; r+=xs, k++, rxsR+=xsR )
					for( j=1, jxs=xs; j<Ratio; j++, jxs+=xs )
						iout[rxsR + jxs] = iin[r]+j*itmp[k];
				for(r=(ys-1)*Ratio; r<(ys-1)*Ratio+Boffs; r++ )
					iout[(r+1)*xs] = iin[(ys-1)*xs];
			}
			error = memFree( (void **)&itmp, "yinterp: itmp" );
			break;
		case T_DOUBLE:
			dtmp = (double *)ck_malloc( (ys-1) * get_datasize( T_DOUBLE ), "yinterp: dtmp");
			
			for( c=0; c<xs; c++ )
			{
				dout = ((double *)outvec) + c;
				din  = ((double *)invec) + c;
				for( r=0; r<Toffs*xs; r+=xs )
					dout[r] = din[0];
				dout += xs*Toffs;
				for( r=0, rxsR=0; r<ys*xs; r+=xs, rxsR+=xsR )
					dout[rxsR] = din[r];
				for( r=0, j=0; r<(ys-1)*xs; r+=xs, j++ )
					dtmp[j] = (din[r+xs]-din[r])/Ratio;
				for( r=0, k=0, rxsR=0; r<(ys-1)*xs; r+=xs, k++, rxsR+=xsR )
					for( j=1, jxs=xs; j<Ratio; j++, jxs+=xs )
						dout[rxsR + jxs] = din[r]+j*dtmp[k];
				for(r=(ys-1)*Ratio; r<(ys-1)*Ratio+Boffs; r++ )
					dout[(r+1)*xs] = din[(ys-1)*xs];
			}
			error = memFree( (void **)&dtmp, "yinterp: dtmp" );
			break;
		default:
			return UNKNOWNTYPE;
	}
	return error;
}

OSErr xinterp(void *invec, int xs, int ys, int Ratio, void *outvec, int data_type)
{
	OSErr	error = noErr;
	float	*ftmp, *fout, *fin;
	short	*stmp, *sout, *s_in;
	unsigned short *ustmp, *usout, *usin;
	unsigned char  *uctmp, *ucout, *ucin;
	char	*ctmp, *cout, *cin;
	int		*itmp, *iout, *iin;
	double	*dtmp, *dout, *din;
	int		r, c, j;
	short	Loffs, Roffs;
	
	Roffs = Ratio/2;
	Loffs = Ratio - Roffs - 1;
	
	switch( data_type )
	{
		case T_FLOAT:	/* comments are in T_FLOAT only. Algorithms are the same for all types */
			ftmp = (float *)ck_malloc( (xs-1) * get_datasize( T_FLOAT ), "xinterp: ftmp");
			
			for( r=0; r<ys*xs; r+=xs )
			{
				fout = (float *)outvec;	/* the trick is to point to the beginning of each row */
				fout += r*Ratio;
				fin  = (float *)invec;
				fin += r;
				
				/* pad the left side of the output vector */
				for( c=0; c<Loffs; c++ )	{
					fout[c] = fin[0];
				}	/* fout now points to past the left offset */
				
				fout += Loffs;
				/* fill in the preserved points */
				for( c=0; c<xs; c++ )	{
					fout[c*Ratio] = fin[c];
				}
				
				/* build a vector containing the deltas in intensity */
				for( c=0; c<xs-1; c++ )	{
					ftmp[c] = (fin[c+1]-fin[c])/Ratio;
				}
				
				/* add the deltas to the points to the right */
				for( c=0; c<xs-1; c++ )	{
					for( j=1; j<Ratio; j++ )	{
						fout[c*Ratio+j] = fin[c]+j*ftmp[c];
					}
				}
				
				/* pad the right side */
				for(c=(xs-1)*Ratio; c<(xs-1)*Ratio+Roffs; c++ )	{
					fout[c+1] = fin[xs-1];
				}
			}
			error = memFree( (void **)&ftmp, "xinterp: ftmp" );
		break;
			
		case T_UCHAR:
			uctmp = (unsigned char *)ck_malloc( (xs-1) * get_datasize( T_UCHAR ), "xinterp: uctmp");
			
			for( r=0; r<ys*xs; r+=xs )
			{
				ucout = ((unsigned char *)outvec) + r*Ratio;
				ucin  = ((unsigned char *)invec) + r;
				
				for( c=0; c<Loffs; c++ )
					ucout[c] = ucin[0];
				ucout += Loffs;
				for( c=0; c<xs; c++ )
					ucout[c*Ratio] = ucin[c];
				for( c=0; c<xs-1; c++ )
					uctmp[c] = (ucin[c+1]-ucin[c])/Ratio;
				for( c=0; c<xs-1; c++ )
					for( j=1; j<Ratio; j++ )
						ucout[c*Ratio+j] = ucin[c]+j*uctmp[c];
				for(c=(xs-1)*Ratio; c<(xs-1)*Ratio+Roffs; c++ )
					ucout[c+1] = ucin[xs-1];
			}
			error = memFree( (void **)&uctmp, "xinterp: uctmp" );
		break;
		
		case T_CHAR:
			ctmp = (char *)ck_malloc( (xs-1) * get_datasize( T_CHAR ), "xinterp: ctmp");
			
			for( r=0; r<ys*xs; r+=xs )
			{
				cout = ((char *)outvec) + r*Ratio;
				cin  = ((char *)invec) + r;
				
				for( c=0; c<Loffs; c++ )
					cout[c] = cin[0];
				cout += Loffs;
				for( c=0; c<xs; c++ )
					cout[c*Ratio] = cin[c];
				for( c=0; c<xs-1; c++ )
					ctmp[c] = (cin[c+1]-cin[c])/Ratio;
				for( c=0; c<xs-1; c++ )
					for( j=1; j<Ratio; j++ )
						cout[c*Ratio+j] = cin[c]+j*ctmp[c];
				for(c=(xs-1)*Ratio; c<(xs-1)*Ratio+Roffs; c++ )
					cout[c+1] = cin[xs-1];
			}
			error = memFree( (void **)&ctmp, "xinterp: ctmp" );
		break;
		
		case T_USHORT:
			ustmp = (unsigned short *)ck_malloc( (xs-1) * get_datasize( T_USHORT ), "xinterp: ustmp");
			
			for( r=0; r<ys*xs; r+=xs )
			{
				usout = ((unsigned short *)outvec) + r*Ratio;
				usin  = ((unsigned short *)invec) + r;
				
				for( c=0; c<Loffs; c++ )
					usout[c] = usin[0];
				usout += Loffs;
				for( c=0; c<xs; c++ )
					usout[c*Ratio] = usin[c];
				for( c=0; c<xs-1; c++ )
					ustmp[c] = (usin[c+1]-usin[c])/Ratio;
				for( c=0; c<xs-1; c++ )
					for( j=1; j<Ratio; j++ )
						usout[c*Ratio+j] = usin[c]+j*ustmp[c];
				for(c=(xs-1)*Ratio; c<(xs-1)*Ratio+Roffs; c++ )
					usout[c+1] = usin[xs-1];
			}
			error = memFree( (void **)&ustmp, "xinterp: ustmp" );
		break;
		
		case T_SHORT:
			stmp = (short *)ck_malloc( (xs-1) * get_datasize( T_SHORT ), "xinterp: stmp");
			
			for( r=0; r<ys*xs; r+=xs )
			{
				sout = ((short *)outvec) + r*Ratio;
				s_in  = ((short *)invec) + r;
				
				for( c=0; c<Loffs; c++ )
					sout[c] = s_in[0];
				sout += Loffs;
				for( c=0; c<xs; c++ )
					sout[c*Ratio] = s_in[c];
				for( c=0; c<xs-1; c++ )
					stmp[c] = (s_in[c+1]-s_in[c])/Ratio;
				for( c=0; c<xs-1; c++ )
					for( j=1; j<Ratio; j++ )
						sout[c*Ratio+j] = s_in[c]+j*stmp[c];
				for(c=(xs-1)*Ratio; c<(xs-1)*Ratio+Roffs; c++ )
					sout[c+1] = s_in[xs-1];
			}
			error = memFree( (void **)&stmp, "xinterp: stmp" );
		break;
		
		case T_INT:
			itmp = (int *)ck_malloc( (xs-1) * get_datasize( T_INT ), "xinterp: itmp");
			
			for( r=0; r<ys*xs; r+=xs )
			{
				iout = ((int *)outvec) + r*Ratio;
				iin  = ((int *)invec) + r;
				
				for( c=0; c<Loffs; c++ )
					iout[c] = iin[0];
				iout += Loffs;
				for( c=0; c<xs; c++ )
					iout[c*Ratio] = iin[c];
				for( c=0; c<xs-1; c++ )
					itmp[c] = (iin[c+1]-iin[c])/Ratio;
				for( c=0; c<xs-1; c++ )
					for( j=1; j<Ratio; j++ )
						iout[c*Ratio+j] = iin[c]+j*itmp[c];
				for(c=(xs-1)*Ratio; c<(xs-1)*Ratio+Roffs; c++ )
					iout[c+1] = iin[xs-1];
			}
			error = memFree( (void **)&itmp, "xinterp: itmp" );
		break;
		
		case T_DOUBLE:
			dtmp = (double *)ck_malloc( (xs-1) * get_datasize( T_DOUBLE ), "xinterp: dtmp");
			
			for( r=0; r<ys*xs; r+=xs )
			{
				dout = ((double *)outvec) + r*Ratio;
				din  = ((double *)invec) + r;
				
				for( c=0; c<Loffs; c++ )
					dout[c] = din[0];
				dout += Loffs;
				for( c=0; c<xs; c++ )
					dout[c*Ratio] = din[c];
				for( c=0; c<xs-1; c++ )
					dtmp[c] = (din[c+1]-din[c])/Ratio;
				for( c=0; c<xs-1; c++ )
					for( j=1; j<Ratio; j++ )
						dout[c*Ratio+j] = din[c]+j*dtmp[c];
				for(c=(xs-1)*Ratio; c<(xs-1)*Ratio+Roffs; c++ )
					dout[c+1] = din[xs-1];
			}
			error = memFree( (void **)&dtmp, "xinterp: dtmp" );
		break;
		
		default:
			return UNKNOWNTYPE;
	}
	return error;
}

/* --> *****************************************************************
*   <b>interp2D</b>
*   2-dimensional (x and y) interpolation for images
*
*   CALLING SEQUENCE:
*   int interp2D(
*           void *invec           // pointer to input data
*           int xsize, int ysize  // image dimensions (input)
*           int Ratio             // interpolation ratio - integers only
*           void *outvec          // pointer to output data. Calling routine
*                                      must allocate sufficient space...                        
*           int data_type )       // standard UC type T_FLOAT, T_SHORT & T_USHORT supported
* 
<!-- *****************************************************************/
OSErr interp2D(void *invec, int xsize, int ysize, int Ratio, void *outvec, int data_type)
{  
	OSErr  error = noErr;              /* for errors */
	float *fptr = nil;
	unsigned short *uiptr;
	unsigned char *ucptr;
	short *sptr;
	long size;
   
   /* only one dimension of interpolation goes here... */
	size =  xsize*ysize*Ratio*get_datasize( data_type );
	switch( data_type )   {
		case( T_USHORT ):
			uiptr = (unsigned short *)ck_malloc( size, "interp2D: uiptr" );
			xinterp( invec, xsize, ysize, Ratio, uiptr, data_type );
			yinterp( uiptr, xsize*Ratio, ysize, Ratio, outvec, data_type );
			error = memFree( (void **)&uiptr,"interp2D: uiptr" );
		break;

		case( T_SHORT ):
			sptr = (short *)ck_malloc( size, "interp2D: sptr" );
			xinterp( invec, xsize, ysize, Ratio, sptr, data_type );
			yinterp( sptr, xsize*Ratio, ysize, Ratio, outvec, data_type );
			error = memFree( (void **)&sptr,"interp2D: sptr" );
		break;

		case( T_UCHAR ):
			ucptr = (unsigned char *)ck_malloc( size, "interp2D: ucptr" );
			xinterp( invec, xsize, ysize, Ratio, ucptr, data_type );
			yinterp( ucptr, xsize*Ratio, ysize, Ratio, outvec, data_type );
			error = memFree( (void **)&ucptr, "interp2D: ucptr" );
		break;

		case( T_FLOAT ):
			fptr = (float *)ck_malloc( size, "interp2D: fptr" );
			xinterp( invec, xsize, ysize, Ratio, fptr, data_type );
			yinterp( fptr, xsize*Ratio, ysize, Ratio, outvec, data_type );
			error = memFree( (void **)&fptr, "interp2D: fptr"  );
		break;

		default:
			 return( UNKNOWNTYPE );
			 break;
      }
   return error;
}

/* --> *****************************************************************
<b>OSErr  finterpX</b>(void *invec, int xs, int ys, int outxs, void *outvec, int data_type)
<!-- *****************************************************************/
OSErr  finterpX(void *invec, int xs, int ys, int outxs, void *outvec, int data_type)
{
	OSErr	error = noErr;
	float	*fin;
	float	*fout;
	long	ctr;
	short	row;
	long	index = -1L;
	
	fin = (float *)invec;
	fout = (float *)outvec;
	
	if( outxs < xs ) {
		return DATA_OUT_OF_RANGE;
	}
	
	if( data_type != T_FLOAT ) {
		error = UNKNOWNTYPE;
		return error;
	}
	
	for( row=0; row<ys; row++ ) {
		for( ctr=0; ctr<outxs; ctr++ ) {
			if( xs * ctr/outxs == index ) {	/* use integer math to detect interpolated locations */
				if( index != xs ) {
					fout[ctr] = 0.5 * ( fin[xs*ctr/outxs] + fin[index+1] );
				} else {
					fout[ctr] = fin[index];
				}
			} else {
				fout[ctr] = fin[index];
			}
		}
	}

	return error;
}

/* --> *****************************************************************
<B>OSErr  finterpY</b>(void *invec, int xs, int ys, int outys, void *outvec, int data_type)
<!-- *****************************************************************/
OSErr  finterpY(void *invec, int xs, int ys, int outys, void *outvec, int data_type)

{
	OSErr error = noErr;
	long imSize;
	
	imSize = xs * ys * outys;
	
	if( (outvec==NULL) || (invec == NULL) ) {
		return DATA_OUT_OF_RANGE;
	}
	
	if( data_type != T_FLOAT ) {
		error = UNKNOWNTYPE;
		return error;
	}
	
	return error;
}

/* --> *****************************************************************
<B>OSErr  finterp2D</b>(void *invec, int xs, int ys, int outxs, int outys, void *outvec, int data_type)
<!-- *****************************************************************/
OSErr  finterp2D(void *invec, int xs, int ys, int outxs, int outys, void *outvec, int data_type)
{
	OSErr error = noErr;
	long imSize;
	
	imSize = xs * ys * outys & outxs;
	
	if( (outvec==NULL) || (invec == NULL) ) {
		return DATA_OUT_OF_RANGE;
	}
	
	if( data_type != T_FLOAT ) {
		error = UNKNOWNTYPE;
		return error;
	}
	
	return error;
}

/* --> *****************************************************************
*   <B>pixreplicate</B>
*   2-dimensional (x and y) pixel replication for images
*
*   CALLING SEQUENCE:
*   int pixreplicate(
*           void        *invec    // pointer to input data
*           int xsize, int ysize  // image dimensions
*           int Ratio             // interpolation ratio - integers only
*           void        *outvec   //     pointer to output data. Calling routine
*                                 // must allocate sufficient space...
*           int data_type )       // standard UC type
*
<!-- *****************************************************************/
OSErr pixreplicate(void *invec, int xsize, int ysize, int Ratio, void *outvec, int data_type)
{ 
   OSErr  error = noErr;              /* for errors */
   int i, j;
   int theSize;
   int XS;
   int Y;
   char *cIN, *cOUT;

   theSize = get_datasize( data_type );
   XS = theSize * xsize;

   cIN = (char *)invec;
   cOUT = (char *)outvec;
   for( Y=0; Y<ysize; Y++ )       /* once for each row */
      for(j=0; j<Ratio; j++)      /* Replicate the column elements */
         for(i=0; i<Ratio; i++)   /* Replicate the row elements */
         /* this might be faster with memcpy */
            vmov( cIN + (Y*XS), 1, cOUT + (Y*Ratio+j) * XS * Ratio + i * theSize, Ratio,
                  xsize, data_type );
   return error;
}

/* --> *****************************************************************
*   <B>pixrepX</B>
*   1-dimensional (x) pixel replication for images
*
*   CALLING SEQUENCE:
*   int pixrepX(
*           void        *invec     //  pointer to input data
*           int xsize, int ysize   //  image dimensions
*           int Ratio              //  interpolation ratio - integers only
*           void        *outvec    //  pointer to output data. Calling routine
*                                  //  must allocate sufficient space...
*           int data_type )        //  standard UC type
*
<!-- *****************************************************************/
OSErr pixrepX(void *invec, int xsize, int ysize, int Ratio, void *outvec, int data_type)
{ 
   OSErr  error = noErr;              /* for errors */
   int j;
   int theSize;
   int Y;
   int XS;
   char *cIN, *cOUT;

   theSize = get_datasize( data_type );
   XS = xsize * theSize;

   cIN = (char *)invec;
   cOUT = (char *)outvec;
   for( Y=0; Y<ysize; Y++ )       /* once for each row */
      for(j=0; j<Ratio; j++ )      /* Replicate the column elements */
         vmov( cIN + Y * XS, 1,
               cOUT + (Y*Ratio) * XS + j*theSize, Ratio,
               xsize, data_type );
   return error;
}

/* --> *****************************************************************
*   <B>pixrepY</B>
*   1-dimensional (y) pixel replication for images
*
*   CALLING SEQUENCE:
*   int pixrepY(
*           void        *invec     //  pointer to input data
*           int xsize, int ysize   //  image dimensions
*           int Ratio              //  interpolation ratio - integers only
*           void        *outvec    //  pointer to output data. Calling routine
*                                  //  must allocate sufficient space...
*           int data_type )        //  standard UC type
*
<!-- *****************************************************************/
OSErr pixrepY(void *invec, int xsize, int ysize, int Ratio, void *outvec, int data_type)
{ 
   OSErr  error = noErr;              /* for errors */
   int j;
   int theSize;
   int Y;
   int XS;
   char *cIN, *cOUT;

   theSize = get_datasize( data_type );
   XS = theSize * xsize;

   cIN = (char *)invec;
   cOUT = (char *)outvec;
   for( Y=0; Y<ysize; Y++ )       /* once for each row */
      for(j=0; j<Ratio; j++)      /* Replicate the column elements */
         vmov( cIN+Y*XS, 1, cOUT+(Y*Ratio+j)*XS, 1, XS, T_UCHAR );
   return error;
}
	
/* --> *****************************************************************
*   <B>DownSample</B>
*   2-dimensional matrix reduction for images
*
*   CALLING SEQUENCE:
*   OSErr DownSample(
*                   void     *invec       // pointer to input data
*                   int      xsize        // image dimensions for original image
*                   int      ysize
*                   int      xFactor, yFactor     reduction ratio - positive integers only
*                   void     *outvec     //  pointer to output data. Calling routine
*                                        //  must allocate sufficient space...
*                   int      data_type ) //  standard UC type
*
<!-- *****************************************************************/
OSErr DownSample(void *invec, int xsize, int ysize, int xFactor, int yFactor, void *outvec, int data_type)
{ 
	OSErr  error = noErr;              /* for errors */
	int Y;
	int xyFactor = xFactor * yFactor;
	char *cIN, *cOUT;
	unsigned char *ucIN, *ucOUT;
	short *sIN, *sOUT;
	unsigned short *usIN, *usOUT;
	int	*iIN, *iOUT;
	float	*fIN, *fOUT;
	double	*dIN, *dOUT;

	if( xFactor <= 0 || yFactor<=0 || xsize<1 || ysize<1) {
		return DATA_OUT_OF_RANGE;
	}

	if( invec == nil || outvec == nil ) {
		return INVALID_ADDRESS;
	}
	
	for( Y = 0; Y < xsize * ysize; Y += yFactor*xsize ) {
		switch( data_type )
		{
			case T_CHAR:
				cIN  = (char *)invec;
				cOUT = (char *)outvec;

				error = vmov( cIN  + Y + xFactor/2, xFactor,
				              cOUT + Y/xyFactor, 1,
				              xsize/xFactor, data_type );
			break;				
			
			case T_UCHAR:
				ucIN  = (unsigned char *)invec;
				ucOUT = (unsigned char *)outvec;

				error = vmov( ucIN  + Y + xFactor/2, xFactor,
				              ucOUT + Y/xyFactor, 1,
				              xsize/xFactor, data_type );
			break;
			
			case T_SHORT:
				sIN  = (short *)invec;
				sOUT = (short *)outvec;

				error = vmov( sIN  + Y + xFactor/2, xFactor,
				              sOUT + Y/xyFactor, 1,
				              xsize/xFactor, data_type );
			break;
			
			case T_USHORT:
				usIN  = (unsigned short *)invec;
				usOUT = (unsigned short *)outvec;

				error = vmov( usIN  + Y + xFactor/2, xFactor,
				              usOUT + Y/xyFactor, 1,
				              xsize/xFactor, data_type );
			break;
			
			case T_INT:
				iIN  = (int *)invec;
				iOUT = (int *)outvec;

				error = vmov( iIN  + Y + xFactor/2, xFactor,
				              iOUT + Y/xyFactor, 1,
				              xsize/xFactor, data_type );
			break;
			
			case T_FLOAT:
				fIN  = (float *)invec;
				fOUT = (float *)outvec;

				error = vmov( fIN  + Y + xFactor/2, xFactor,
				              fOUT + Y/xyFactor, 1,
				              xsize/xFactor, data_type );
			break;
			
			case T_DOUBLE:
				dIN  = (double *)invec;
				dOUT = (double *)outvec;

				error = vmov( dIN  + Y + xFactor/2, xFactor,
				              dOUT + Y/xyFactor, 1,
				              xsize/xFactor, data_type );
			break;
			
			default:
				return UNKNOWNTYPE;
		}
	}  // Y loop
	return error;
}

/* --> **********************************************************************
*   <B>ScaleImageSize</b>
*	Interpolate an image and place it into newly created memory. Calling routine
*	should free this memory as soon as possible
*
*	Requires:
*   float *ScaleImageSize(
*       float    *theImage        source image
*		IMAGE    im               descriptive header info
*         int    interpFactor     interpolation factor: Positive number for increase, negative for decrease
*         int    displayMode      how should interpolation be done?
*      *OSErr    error            error return value
*         int    data_type )
*
*	Changes:     Allocates memory for interpbuf and returns image in it.
<!-- **************************************************************************/
float *ScaleImageSize( float *theImage, IMAGE *im, int interpFactor,
                       int displayMode, OSErr *error, int data_type )
{
	long     interpVolSize;
	float    *interpBuff;
	long     VolSize = im->dim.isoX * im->dim.isoY * im->dim.n_slices;

	if( interpFactor > 0 ) {
		interpVolSize = interpFactor * interpFactor * VolSize;
	} else {
		interpVolSize = VolSize / ( interpFactor * interpFactor );
	}

	interpBuff = (float *)ck_malloc( interpVolSize * get_datasize( data_type ), "ScaleImageSize: interpBuff" );

// interpolate the new window according to the user-specified interpolation options
	if( interpFactor > 1 ) {
		if( displayMode & kBilinear ) {
			*error = interp2D    ( theImage, im->dim.isoX, im->dim.isoY * im->dim.n_slices,
						interpFactor, interpBuff, data_type );
		} else if( displayMode & kReplication ) {
			*error = pixreplicate( theImage, im->dim.isoX, im->dim.isoY * im->dim.n_slices,
						interpFactor, interpBuff, data_type );
		} else {
			*error = UNKNOWN_DISPLAY_MODE;
		}
	} else if( interpFactor < 0 ) {
		*error = DownSample( theImage, im->dim.isoX, im->dim.isoY * im->dim.n_slices,
						-interpFactor, -interpFactor, interpBuff, data_type );
	} else {
		*error = vmov( theImage, 1, interpBuff, 1, interpVolSize, data_type );
	}
	/* theImage is now sized and in interpBuf */

	if( *error != noErr ) {
		memFree( (void **)&interpBuff, "ScaleImageSize: interpBuff" );
	}

	return interpBuff;
}

/* --> *****************************************************************
*   <B>ImageFlipY</B>
*   Flip image top and bottom
*	OK to use in-place..
*
*   CALLING SEQUENCE:
*   OSErr ImageFlipY(
*                   void     *invec      // pointer to input data
*                   int      xsize       // image dimensions for original image
*                   int      ysize
*                   void     *outvec     // pointer to output data. Calling routine
*                                        // must allocate sufficient space...
*                   int      data_type ) // standard UC type
*
<!-- *****************************************************************/
OSErr ImageFlipY( void *invec, int xsize, int ysize, void *outvec, int data_type )
{
	void *tmp;
	int  theLine;
	int  xbytes;
	char *pIn;
	char *pOut;
	OSErr error = noErr;
	
	xbytes = xsize * get_datasize( data_type );
	if( xbytes == 0 ) {
		return MEMALLOCERROR;
	}

	pIn  = (char *)invec;
	pOut = (char *)outvec;

	tmp = ck_malloc( xbytes, "ImageFlipY: scratch space" );
	for( theLine=0; theLine<ysize/2; theLine++ ) {
// copy the upper line to tmp
		memcpy( tmp,			
				pIn + theLine*xbytes,
				xbytes );
// replace the upper line with the lower line
		memcpy( pOut + theLine*xbytes,
		        pIn + ysize * xbytes - (theLine+1) * xbytes,
		        xbytes );
// replace the lower line with the copied upper line
		memcpy( pOut + ysize * xbytes - (theLine+1) * xbytes,
		        tmp,
		        xbytes );
	}

	free( tmp );

	return error;
}	

/* --> *******************************************************************\
*	<B>CropImageSides</B>
*
*	Remove the specified number of points from the left and right sides
*	of an image, returning the data in the first part of the input buffer
*
*	Calling routine is responsible for disposing of, or resizing memory
*
*   void CropImageSides (
*	void *ImageData        // the original data
*	short UnCropdXSize     // number of pixels per x line before cropping
*	short ysize            // number of y lines
*	short LeftSkip         // points to skip on image left
*	short RightSkip        // points to skip on image right
*   short bytesPix )       // number of bytes/pixel
*
*	CHANGES:
*		Contents of image data
<!-- *********************************************************************/
void   CropImageSides( void *ImageData, short UnCropdXSize, short ysize,
                       short LeftSkip, short RightSkip, short bytesPix )
{
	int y;
	short UnCropdXBytes = UnCropdXSize * bytesPix;
	short LeftSkipBytes = RightSkip * bytesPix;
	long  ValidXBytes;
	void  *cImageData = ImageData;

	if( LeftSkip == 0 && RightSkip == 0 ) {
		return;	// don't waste our time
	}

	ValidXBytes = UnCropdXBytes - LeftSkipBytes - RightSkip * bytesPix;
	
	for( y=0; y<ysize; y++ ) {
		memcpy( (char *)cImageData + y * ValidXBytes,
		        (char *)cImageData + y * UnCropdXBytes + LeftSkipBytes,
		        ValidXBytes );
	}
}

/* --> ************************************************************************
*    <B>xcrop</B> - remove the left and right quarter images from a large image
*
*    CALLING SEQUENCE:
*    xcrop(
*        *InImg,      Input Image vector
*        xs, ys,      x and y dimensions in ints
*        *CropImg,    Output Image vector
*        data_type )  Standard UC data type
*
<!-- *************************************************************************/
void xcrop( void *InImg, int xs, int ys, void *CropImg, int data_type )
/* remove the left and right quarter image */
{  
   int qxsize, hxsize, xsize, size;
   int i;
   char *cInImg, *cCropImg;

   cInImg = (char *)InImg;
   cCropImg = (char *)CropImg;

   size = get_datasize( data_type );

   qxsize = size * xs/4;
   hxsize = size * xs/2;
   xsize = size * xs;

   cInImg = (char *)InImg;
   cCropImg = (char *)CropImg;
   for( i=0; i<ys; i++ ) {
      memcpy( cCropImg + i * hxsize, cInImg + qxsize + i * xsize, hxsize );
   }
}

/* --> *******************************************************************************
*   <B>Overlay</B>
*
*   merge two float images to make a uchar composite
*
*   CALLING SEQUENCE:
*   Overlay( float         *baseImage,           <I>Image to use as base. Must already be</I>
*                                                <I>rescaled to range from BMAX to BMIN</I>
*            float         *OverlayImage,        <I>Image to overlay onto base</I>
*            unsigned char *composite,           <I>Output</I>
*            long          imSize,
*            float         overlay_max,          <I>Maximum value to be used from overlay</I>
*            float         overlay_cutoff,       <I>Minimum value to be used from overlay</I>
*            short         overlayMode )         <I>Pos/Neg/Whatever...</I>
*
<!-- *******************************************************************************/
OSErr Overlay( float *baseImage, float *OverlayImage, unsigned char *composite,
                 long imSize, float overlay_max, float overlay_cutoff, short OvlMode )
{
   int    i;
   float  oscale;
   OSErr  error = noErr;

   if( baseImage==nil || OverlayImage==nil || composite==nil ) {
   	return INVALID_ADDRESS;
   }
   /* calculate the scaling factor for the overlay set */
   oscale = (POSMAX-POSMIN)/(overlay_max-overlay_cutoff);

   /* replace all values in the output image with overlays, if over threshold */
   switch( OvlMode ) {
    case kPosNeg:
        for( i=0; i< imSize; i++ ) { /* create the color map */
            if ( OverlayImage[i] > overlay_cutoff ) {
                if(  OverlayImage[i] >= overlay_max ) {
                    composite[i] = POSMAX; /* imglib.h */
	         } else {
	            composite[i] = POSMIN + oscale * ( OverlayImage[i] - overlay_cutoff );
	         }
            } else if ( OverlayImage[i] < -overlay_cutoff ) {
	         if(  OverlayImage[i] <= -overlay_max ) {
	            composite[i] = NEGMAX; /* imglib.h */
	         } else {
	            composite[i] = NEGMIN - oscale * ( OverlayImage[i] + overlay_cutoff );
	         }
            } else {
	         composite[i] = baseImage[i];
            }
        }
        break;
    case kPosOnly:
        for( i=0; i< imSize; i++ ) { /* create the color map */
            if ( OverlayImage[i] > overlay_cutoff ) {
                if(  OverlayImage[i] >= overlay_max ) {
	            composite[i] = POSMAX; /* imglib.h */
                } else {
	            composite[i] = POSMIN + oscale * ( OverlayImage[i] - overlay_cutoff );
                }
            } else {
                composite[i] = baseImage[i];
            }
        }
        break;
    case kNegOnly:
        for( i=0; i< imSize; i++ ) { /* create the color map */
            if ( OverlayImage[i] < -overlay_cutoff ) {
                if(  OverlayImage[i] <= -overlay_max ) {
	            composite[i] = NEGMAX; /* imglib.h */
                } else {
	            composite[i] = NEGMIN - oscale * ( OverlayImage[i] + overlay_cutoff );
                }
            } else {
                composite[i] = baseImage[i];
            }
        }
        break;
    default:
        error = UNKNOWN_DISPLAY_MODE;
        break;
   }

   return error;
}

/* --> *******************************************************************************
*   <B>vGaussSmooth</B>
*   Use convolution to smooth using a Gaussian kernel
*   Accepts and returns only T_FLOAT at this time
*
*   CALLING SEQUENCE:
*   vGaussSmooth( void *invec         -- only T_FLOAT now
*                   int width         -- Width of Gaussian kernel in pixels
*                    int step         -- Step size through input vector
*                   long npts         -- Number of points to process
*               int data_type )       -- Only Accepts T_FLOAT
*   OUTPUTS:
*      This is an in-place routine. returns the smoothed vector
*
<!-- ******************************************************************************/
OSErr vGaussSmooth( void *invec, int width, int step, long npts, int data_type )
{
	OSErr   error = noErr;              /* for errors */
	int     intlimit;
	int     i, j;
	int     ksize;
	float   limit;    /* at this point, the correction is less than 1% */
	float   sigma;
	float   norm;
	float   *kernel = nil;      /* this is the smoothing vector */
	float   *local_in = nil;
	float   *smooth = nil;     /* contains the smoothed data */

	if( data_type != T_FLOAT )  {
		return( UNKNOWNTYPE );
	}

	limit = 1.1 * width;
	ksize = (int)(2*limit+1);
	intlimit = (int)limit;
	kernel = (float *)ck_malloc( get_datasize(T_FLOAT) * (2*intlimit + 1), "gaussian kernel" );
	smooth = (float *)ck_calloc( npts, get_datasize(T_FLOAT), "smoothed vector" );
	sigma = 2/(float)width;

/* The local version of the input vector is enlarged at both ends to
	allow convolution of the end points */

	local_in = (float *)ck_calloc( npts, get_datasize( T_DOUBLE ), "convolution vector" );
	vmov( invec, step, local_in+intlimit, 1, npts, T_FLOAT );

/* make the smoothing kernel - a gaussian */
	for( i= -intlimit; i<=intlimit; i++ ) {
		kernel[intlimit + i] = exp( -((float)i*sigma) * ((float)i*sigma) );
	}

/* make sure that the the area is equal to one */
	norm = (float)vsum( kernel,1, ksize, T_FLOAT );

	for( i=0; i<ksize; i++ ) {
		kernel[i] /= norm;
	}

/* make symmetric */
	for(i=0; i<intlimit; i++) {
		local_in[i] = local_in[intlimit];
		local_in[npts+intlimit+i] = local_in[npts+intlimit-1];
	}

/* convolution */
	for( i=0; i<npts; i++) {
		for( j=-intlimit; j<=intlimit; j++ ) {
			smooth[i] += local_in[i+j+intlimit] * kernel[j+intlimit];
		}
	}

/* return to input location (for an in-place routine) */
	vmov( smooth, 1, invec, step, npts, T_FLOAT );

	error = memFree( (void **)&kernel, "kernel"  );
		RETURNONERROR;
	error = memFree( (void **)&smooth, "smooth"  );
		RETURNONERROR;
	error = memFree( (void **)&local_in, "local_in"  );
	return error;
}

/* --> ************************************************************************
*   <B>HanningSmooth</B>
*   Performs a very rapid in place 2-dimensional convolution smoothing with a Hanning Kernel
*
*   The Hanning filter is a convolution with a 0.25, 0.5, 0.25 kernel. We handle this
*   as integer math, by using a 1-2-1 kernel and dividing the sum by four.
*
*   CALLING SEQUENCE:
*        HanningSmooth( short Kernel,                   // smoothing kernel
*                       unsigned short *image,          // input data
*                       unsigned short *smoother,       // storage buffer as large as image
*                       int xsize,
*                       int ysize,
*                       int data_type )
*
*   INPUTS:  *image -- pointer to a 2D image array of short ints
*            xsize, ysize -- x and y dimensions
*			Kernel == kSmooth9 or kSmooth25 to control degree of smoothing.
*			Larger smoothing radii should be performed in k-space
*
*   OUTPUTS: Smoothed image is returned in place.
*
<!-- *************************************************************************/   
OSErr HanningSmooth( short Kernel, unsigned short *image, unsigned short *smoother, int xsize, int ysize, int data_type )
{
	OSErr  error = noErr;              /* for errors */
	int i,j;

	if( data_type != T_USHORT ) {
		return( UNKNOWNTYPE );
	}

	if( Kernel == kNoSmooth ) {
		return noErr;
	}
	
/* smooth along the x axis into 'smoother' */
	for ( j=0; j<xsize*ysize; j+=xsize )
	{
		int xm1 = xsize - 1;
		if( Kernel == kSmooth9 ) {
			smoother[j] = image[j];                    /* pad the left */
			smoother[j+xm1] = image[j+xm1];            /* pad the right */
			for( i=1; i<xm1; i++ )  {                  /* do the convolution */
				int k = j+i;
				smoother[k] = (image[k-1] + ( image[k] << 1 ) + image[k+1]) >> 2;
			}
		} else if( Kernel == kSmooth25 ) {
			int xm2 = xsize - 2;
			smoother[j] = image[j];                    /* pad the left */
			smoother[j+1] = image[j+1];                /* pad the left */
			smoother[j+xm2] = image[j+xm2];            /* pad the right */
			smoother[j+xm1] = image[j+xm1];            /* pad the right */
			for( i=2; i<xm2; i++ )  {                  /* do the convolution */
				int k = j+i;
				smoother[k] = (image[k-2] + (image[k-1] << 1) + ( image[k] * 3 ) + (image[k+1] << 1) + image[k+2]) >> 3;
			}
		}
	}

/* smooth along the y axis back into 'image' */
	for ( i=0; i<xsize; i++ )
	{
		int ym1 = ysize-1;
		if( Kernel == kSmooth9 ) {
			image[i] = smoother[i];                     /* pad the top and bottom */
			image[ ym1*xsize + i ] = smoother[ ym1*xsize + i ];
			for( j=xsize; j<ym1*xsize; j+=xsize )  {    /* convolve */
				int k = j+i;
				image[k] = (smoother[k-xsize] + ( smoother[k] << 1 ) + smoother[k+xsize]) >> 2;
			}
		} else if( Kernel == kSmooth25 ) {
			int ym2 = ysize - 2;
			image[i] = smoother[i];                     /* pad the top and bottom */
			image[i+1] = smoother[i+1];                 /* pad the top and bottom */
			image[ ym2*xsize + i ] = smoother[ ym2*xsize + i ];
			image[ ym1*xsize + i ] = smoother[ ym1*xsize + i ];
			for( j=xsize+1; j<ym2*xsize; j+=xsize )  {    /* convolve */
				int k = j+i;
				image[k] = (smoother[k-xsize-1] + (smoother[k-xsize] << 1) + ( smoother[k] * 3 ) + (smoother[k+xsize] << 1) + smoother[k+xsize+1]) >> 3;
			}
		}
	}
	return error;
}

/* --> ************************************************************************
*               <B>CalcT2</B>
*
*  Determine the T2 and Rho of a location based on intensity values and
*  return the Chi2 value for goodness of fit.
*
*  Data dimensions are passed in the IMAGE struct
*
*  CALLING SEQUENCE
*  OSErr CalcT2( IMAGE *im,    // struct with image dimensions. data_type is IGNORED.
*                float *data,  // set of image volumes arranged as time points in order of te
*				 char *Mask,   // binary mask indicating which locations are to be fitted
*				 float *TEs,   // vector of te values in msec
*				 float *T2,    // image vector for T2 outputs
*				 float *Rho,   // image vector for Rho outputs
*				 float *chi2 ) // image vector for fit values
*
*  RETURNS: error code
*
<!-- ************************************************************************/

OSErr CalcT2( IMAGE *im, float *data, unsigned char *Mask, float *TEs, float *T2, float *Rho, float *chi2 )
{
	int   nt = im->dim.timePts;
	long  i, j, thePt;
	long  VolSize = im->dim.x * im->dim.y * im->dim.n_slices;
	float *lnSI;	// log of the input data
	float sx=0;
	float sx2=0;
	float sy, M, r;
	float num, den, O_E;
	OSErr error = noErr;

	error = vclr( T2, VolSize, T_FLOAT);   RETURNONERROR;
	error = vclr( chi2, VolSize, T_FLOAT); RETURNONERROR;
	lnSI = (float *)ck_malloc( VolSize * nt * sizeof( float ), "ln(SI)" );
	
	for( i=0; i<VolSize; i++ ) {
		for( j=0; j<nt*VolSize; j+=VolSize ) {
			if(Mask[i]) {
				lnSI[i+j] = log(data[i+j]);
			} else {
				lnSI[i+j] = 0;
			}
		}
	}

	for (i=0; i<nt; i++) {
		sx2 += TEs[i] * TEs[i];
		sx  += TEs[i];
	}
	den = sx2 - sx*sx/nt;	// sum of squared x values - the average of the squares

	for( thePt=0; thePt<VolSize; thePt++ ) {
		chi2[thePt] = 0;
		if( Mask[thePt] ) {
			sy = M = 0;
			for( i=0; i<nt; i++ ) {
				sy += lnSI[thePt + i*VolSize];
				M  += lnSI[thePt + i*VolSize] * TEs[i];
			}
			num = M - sy*sx/nt;
			
			if( den==0 ) {
				T2[thePt] = 0;
				Rho[thePt] = 0;
			} else {
				M = num/den;
				T2[thePt] = -1/M;
				r = (sy -sx*M) / nt;
				Rho[thePt] = exp(r);
				for( i=0; i<nt; i++ ) {
					O_E = lnSI[thePt + i*VolSize] - M*TEs[i] - r;
					chi2[thePt] += O_E * O_E / ( M * TEs[i] + r );
				}
			}
		} else {
			T2[thePt] = 0;
			Rho[thePt] = 0;
		}
	}
	
	return error;
}

/* --> ************************************************************************
*               <B>imageDataRange</B>
*
*  Determine the noise level and lower fraction of the nonNoise data
*  range based on a histogram of pixel intensities
*
*  Assume that the noise is a commonly occuring value in the bottom
*  NRANGE of values. Use a histogram of HBINS size and threshold at NMULT
*  times the peak in the lower NRANGE.
*
*  If the data ranges to negative values, simply return the max and min.
*
*  CALLING SEQUENCE
*  OSErr imageDataRange( void *vec,
*                        long npts,
*                        void *hicutoff,
*                        void *noiseLevel,
*                        long *nonNoise,
*                        int data_type )
*
*  INPUTS:
*     vec       -- input vector
*     npts      -- vector length (int)
*     data_type -- output vector of char of same length as input
*  OUTPUTS:
*     noiseLevel is updated to contain the noise threshold
*     hiCutoff contains the level below which fraction of pixel intensities are found
*     nonNoise is the number of points above threshhold    
*  RETURNS: error code
*
<!-- ************************************************************************/
OSErr imageDataRange( void *vec, long npts, void *hicutoff,
                       void *noiseLevel, long *nonNoise, int data_type )
{
   OSErr    error = noErr;              /* for errors */
   Boolean	degenerate = false;						// indicating that there are too few points to process
   long     i, *hist=nil, *smoothHist=nil, MIN;
   short    minbin=0, num_bins;
   int      imax, imin, *ivec, iTh, intCut;
   char     cmax, cmin, *cvec, cTh, cCut;
   short    smax, smin, *svec, sTh, sCut, index;
   float    fmax, fmin, *fvec, fTh, fCut;
   double   dmax, dmin, *dvec, dTh, dCut;
   unsigned char ucmax, ucmin, *ucvec, ucTh, ucCut;
   unsigned short uimax, uimin, *uivec, uiTh, uiCut;
   long     control, sum;
   double   dbinsize=0.0;

#undef CheckHistogram
#ifdef CheckHistogram
FILE *myfile;
#endif
   
   float fdiff;
   
   float CUTOFF = 0.985;	/* fraction of non-noise pixels to show */
   float NRANGE = 0.15; /* fraction of intensity range to search for histogram min */
   float NullPointReduction = 0.6;  /* amount below the minimum in the histogram to set the Threshold */
   
   *nonNoise = 0;
 
// make a bin size with at least 16 points/bin
	if( npts/16 > 256 ) {
		num_bins = 256;
	} else if( npts > 256) {
		num_bins = npts/16;
	} else {
		degenerate = true;
	}

/* Calculate the bin width and make a histogram of signal intensities */
	if( !degenerate ) {
		hist = (long *)ck_calloc( num_bins,       sizeof( long ), "histogram" );
		smoothHist = (long *)ck_calloc( num_bins, sizeof( long ), "Smooth hist" );
	}
	switch( data_type ) {
		case T_CHAR:
			cvec = (char *)vec;
			error = vminmax( vec, npts, &cmax, &cmin, data_type );
			RETURNONERROR;
			
			if( !degenerate ) {
				if( cmin >= 0 ) {
					dbinsize = (double)(cmax-cmin)/num_bins;
					for( i=0; i<npts; i++ ) {
						index = (int)( (cvec[i]-cmin)/dbinsize );
						if( index > num_bins - 1 ) {
							index = num_bins-1;
						}
						hist[index]++;
					}
				} else {
					vmov(&cmin,1,noiseLevel,1,1,data_type);
					vmov(&cmax,1,hicutoff,  1,1,data_type);
				}
			}
		break;
	
		case T_SHORT:
			svec = (short *)vec;
			error = vminmax( vec, npts, &smax, &smin, data_type );
			RETURNONERROR;
			if( !degenerate ) {
				if( smin>=0 ) {
					dbinsize = (double)(smax-smin)/num_bins;
					for( i=0; i<npts; i++ ) {
						index = (int)( (svec[i]-smin)/dbinsize );
						if( index > num_bins - 1 ) {
							index = num_bins-1;
						}
						hist[index]++;
					}
				} else {
					vmov(&smin,1,noiseLevel,1,1,data_type);
					vmov(&smax,1,hicutoff,  1,1,data_type);
					return NEG_DATA_IN_AUTORANGE;
				}
			}
		break;

		case T_UCHAR:
			ucvec = (unsigned char *)vec;
			error = vminmax( vec, npts, &ucmax, &ucmin, data_type );
			RETURNONERROR;
			if( !degenerate ) {
				dbinsize = (double)(ucmax-ucmin)/num_bins;
				for( i=0; i<npts; i++ ) {
					index = (int)( (ucvec[i]-ucmin)/dbinsize );
					if( index > num_bins - 1 ) {
						index = num_bins-1;
					}
					hist[index]++;
				}
			}
		break;

		case T_USHORT:
			uivec = (unsigned short *)vec;
			error = vminmax( vec, npts, &uimax, &uimin, data_type );
			RETURNONERROR;
			if( !degenerate ) {
				dbinsize = (double)(uimax-uimin)/num_bins;
				for( i=0; i<npts; i++ ) {
					index = (int)( (uivec[i]-uimin)/dbinsize );
					if( index > num_bins - 1 ) {
						index = num_bins-1;
					}
					hist[index]++;
				}
			}
		break;

		case T_INT:
			ivec = ( int *)vec;
			error = vminmax( vec, npts, &imax, &imin, data_type );
			RETURNONERROR;
			if( !degenerate ) {
				if( imin>=0 ) {
					dbinsize = (double)(imax-imin)/num_bins;
					for( i=0; i<npts; i++ ) {
						index = (int)( (ivec[i]-imin)/dbinsize );
						if( index > num_bins - 1 ) {
							index = num_bins-1;
						}
						hist[index]++;
					}
				} else {
					vmov(&imin,1,noiseLevel,1,1,data_type);
					vmov(&imax,1,hicutoff,  1,1,data_type);
					return NEG_DATA_IN_AUTORANGE;
				}
			}
		break;

		case T_FLOAT:
			fvec = (float *)vec;
			error = vminmax( vec, npts, &fmax, &fmin, data_type );
			RETURNONERROR;
			if( !degenerate ) {
				if( fmin>=0.0 ) {
					dbinsize = (fmax-fmin)/num_bins;
					for( i=0; i<npts; i++ ) {
						fdiff = fvec[i] - fmin;
						index = (int)( fdiff/dbinsize );
						if( index > num_bins - 1 ) {
							index = num_bins-1;
						}
						hist[index]++;
					}
				} else {
					vmov(&fmin,1,noiseLevel,1,1,data_type);
					vmov(&fmax,1,hicutoff,  1,1,data_type);
					return NEG_DATA_IN_AUTORANGE;
				}
			}
		break;

		case T_DOUBLE:
			dvec = (double *)vec;
			error = vminmax( vec, npts, &dmax, &dmin, data_type );
			if( !degenerate ) {
				RETURNONERROR;
				if( dmin>=0.0 ) {
					dbinsize = (dmax-dmin)/num_bins;
					for( i=0; i<npts; i++ ) {
						index = (int)( (dvec[i]-dmin)/dbinsize );
						if( index > num_bins - 1 ) {
							index = num_bins-1;
						}
						hist[index]++;
					}
				} else {
					vmov(&dmin,1,noiseLevel,1,1,data_type);
					vmov(&dmax,1,hicutoff,  1,1,data_type);
					return NEG_DATA_IN_AUTORANGE;
				}
			}
		break;

		default:
			return( UNKNOWNTYPE );
			break;
	}
	RETURNONERROR;

// smooth the histogram with a modified gaussian kernel
	if( !degenerate ) {
		smoothHist[0] = (4*hist[0] + 2*hist[1] + hist[2])/7;
		smoothHist[1] = (3*hist[0] + 4*hist[1] + 2*hist[2] + hist[3])/10;
		smoothHist[num_bins-1] = (4*hist[num_bins-1] + 2*hist[num_bins-2] + hist[num_bins-3])/7;
		smoothHist[num_bins-2] = (3*hist[num_bins-1] + 4*hist[num_bins-2] + 2*hist[num_bins-3] + hist[num_bins-4])/10;
		for( i=2; i<num_bins-2; i++ ) {
			smoothHist[i] = (hist[i-2] + 2*hist[i-1] + 4*hist[i] + 2* hist[i+1] + hist[i+2])/10;
		}

// find the min in the bottom part of the histogram
// calculate the number of pixels in the smoothed histogram
// determine the number of pixels classified as noise in both smoothed and unsmoothed histograms
		sum = 0L;
		
		minbin = 0;
		for( i=0,MIN=smoothHist[0]; i < (long)(NRANGE*num_bins); i++ ) {
			if( smoothHist[i] < MIN ) {
				MIN = smoothHist[i];
				minbin = i;
			}
		}

		error = memFree( (void **)&smoothHist, "smoothed histogram"  );	
			RETURNONERROR;

		for( i=0, sum=0L; i < minbin+1; i++ ) {
			sum += hist[i];
		}

		*nonNoise = npts - sum;
		control = (long)( CUTOFF * (npts - sum) );

// set the noise threshold to the value at the minimum
		if( minbin==0 ) {
			dTh=dbinsize;
		} else {
			dTh = minbin * dbinsize * NullPointReduction;
		}

// determine the hi cutoff as the value that contains CUTOFF fraction of data
// where the data are pixels above the noise cutoff.

		i = minbin;
		sum = 0;
		while ( sum <= control ) {
			sum +=  hist[i++];
		}

		error = memFree( (void **)&hist, "histogram"  );	
			RETURNONERROR;

/*
if the data set contains few actual different values (e.g., a data set with a range of less
than 256 different intensities) the counter, i, may exceed the number of bins. This is not
exactly an error, so we post only a warning.
*/
		if( i > num_bins ) {
			dCut = fmax;
			error = SPARSE_DATA_VALUES;
		} else {
			dCut = i * dbinsize;
		}

/* Use vmov for type-independent return of the calculated levels */
	}	// not degenerate

	switch( data_type ) {
		case T_CHAR:
			cTh = (char) dTh; cCut = (char) dCut;
			vmov(&cTh,1,noiseLevel,1,1,data_type);
			vmov(&cCut,1,hicutoff, 1,1,data_type);
		break;

		case T_SHORT:
			sTh = (short) dTh; sCut = (short) dCut;
			vmov(&sTh,1,noiseLevel,1,1,data_type);
			vmov(&sCut,1,hicutoff, 1,1,data_type);
		break;

		case T_UCHAR:
			ucTh = (unsigned char) dTh; ucCut = (unsigned char) dCut;
			vmov(&ucTh,1,noiseLevel,1,1,data_type);
			vmov(&ucCut,1,hicutoff, 1,1,data_type);
		break;

		case T_USHORT:
			uiTh = (unsigned short) dTh; uiCut = (unsigned short) dCut;
			vmov(&uiTh,1,noiseLevel,1,1,data_type);
			vmov(&uiCut,1,hicutoff, 1,1,data_type);
		break;

		case T_FLOAT:
			fTh = (float) dTh; fCut = (float) dCut;
			vmov(&fTh,1,noiseLevel,1,1,data_type);
			vmov(&fCut,1,hicutoff, 1,1,data_type);
		break;

		case T_DOUBLE:
			vmov(&dTh,1,noiseLevel,1,1,data_type);
			vmov(&dCut,1,hicutoff, 1,1,data_type);
		break;

		case T_INT:
			iTh = (int) dTh; intCut = (int) dCut;
			vmov(&iTh,1,noiseLevel,1,1,data_type);
			vmov(&intCut,1,hicutoff, 1,1,data_type);
		break;

		default:
			return UNKNOWNTYPE;
	}
	return error;
}

/* --> ************************************************************************
*   <B>ThreshMask & vThreshold</B>
*   Form an image mask having values of 0 for all pixels below threshold and 1 for all
*   pixels greater than the threshold
*
*   Input must be of type unsigned short.
*
*   CALLING SEQUENCE
*   long ThreshMask( void *myImage,        // form a mask of points above threshold
*                    unsigned char *theMask,
*                    long imageSize,
*                    void *threshold, 
*                    int data_type )
*
*   long vThreshold( void *myImage,        // zero out points below threshhold
*                    long vlen,
*                    void *threshold,
*                    int data_type )
*
*   INPUTS:   *myImage  -- image vector
*             imageSize -- size of image
*             threshold -- value above which points are treated as valid
*
*   OUTPUTS:  *theMask  --  vector of unsigned char representing the image mask (ThreshMask)
*             vThreshold zeroes all points in vec greater than threshold
*             returns number of points over threshold.
<!-- ************************************************************************/
long ThreshMask( void *myImage,  unsigned char *theMask, long imageSize, void *threshold, int data_type )
{
	int   i;
	long  numNotNoise = 0;
	char  *cThr, *cImVec;
	short *sThr, *sImVec;
	int   *iThr, *iImVec;
	float *fThr, *fImVec;
	double *dThr, *dImVec;
	unsigned char *ucThr, *ucImVec;
	unsigned short *uiThr, *uiImVec;
   
	switch ( data_type ) {
		case T_CHAR:
			cImVec = (char *)myImage;
			cThr = (char *)( threshold );
			for ( i=0, numNotNoise=0; i<imageSize; i++ ) {
				if(cImVec[i] >= *cThr ) {
					numNotNoise++;
					theMask[i] = 1;
				} else {
					theMask[i] = 0;
				}
			}
		break;

		case T_SHORT:
			sImVec = (short *)myImage;
			sThr = (short *)( threshold );
			for ( i=0, numNotNoise=0; i<imageSize; i++ ) {
				if(sImVec[i] >= *sThr )  {
					numNotNoise++;
					theMask[i] = 1;
				} else {
					theMask[i] = 0;
				}
			}
		break;

		case T_INT:
			iImVec = (int *)myImage;
			iThr = (int *)( threshold );
			for ( i=0, numNotNoise=0; i<imageSize; i++ ) {
				if(iImVec[i] >= *iThr )  {
					numNotNoise++;
					theMask[i] = 1;
				} else {
					theMask[i] = 0;
				}
			}
		break;

		case T_FLOAT:
			fImVec = (float *)myImage;
			fThr = (float *)( threshold );
			for ( i=0, numNotNoise=0; i<imageSize; i++ ) {
			if(fImVec[i] >= *fThr )  {
					numNotNoise++;
					theMask[i] = 1;
				} else {
					theMask[i] = 0;
				}
			}
		break;

		case T_DOUBLE:
			dImVec = (double *)myImage;
			dThr = (double *)( threshold );
			for ( i=0, numNotNoise=0; i<imageSize; i++ ) {
				if(dImVec[i] >= *dThr )  {
					numNotNoise++;
					theMask[i] = 1;
				} else {
					theMask[i] = 0;
				}
			}
		break;

		case T_UCHAR:
			ucImVec = (unsigned char *)myImage;
			ucThr = (unsigned char *)( threshold );
			for ( i=0, numNotNoise=0; i<imageSize; i++ ) {
				if(ucImVec[i] >= *ucThr ) {
					numNotNoise++;
					theMask[i] = 1;
				} else {
					theMask[i] = 0;
				}
			}
		break;

		case T_USHORT:
			uiImVec = (unsigned short *)myImage;
			uiThr = (unsigned short *)( threshold );
			for ( i=0, numNotNoise=0; i<imageSize; i++ ) {
				if(uiImVec[i] >= *uiThr ) {
					numNotNoise++;
					theMask[i] = 1;
				} else {
					theMask[i] = 0;
				}
			}
		break;

		default:
		return( UNKNOWNTYPE );
		break;
	}
	return( numNotNoise );
}

long vThreshold( void *myImage, long vlen, void *threshold, int data_type )
{
	int           i;
	long          numNotNoise = 0;
	char          *cThr, *cImVec;
	short         *sThr, *sImVec;
	int           *iThr, *iImVec;
	float         *fThr, *fImVec;
	double        *dThr, *dImVec;
	unsigned char *ucThr, *ucImVec;
	unsigned short *uiThr, *uiImVec;

	switch ( data_type ) {
		case T_CHAR:
			cImVec = (char *)myImage;
			cThr = (char *)( threshold );
			for ( i=0, numNotNoise=0; i<vlen; i++ ) {
				if(cImVec[i] >= *cThr ) {
					numNotNoise++;
				} else {
					cImVec[i] = 0;
				}
			}
		break;

		case T_SHORT:
			sImVec = (short *)myImage;
			sThr = (short *)( threshold );
			for ( i=0, numNotNoise=0; i<vlen; i++ ) {
				if(sImVec[i] >= *sThr ) {
					numNotNoise++;
				} else {
					sImVec[i] = 0;
				}
			}
		break;

		case T_INT:
			iImVec = (int *)myImage;
			iThr = (int *)( threshold );
			for ( i=0, numNotNoise=0; i<vlen; i++ ) {
				if(iImVec[i] >= *iThr ) {
					numNotNoise++;
				} else {
					iImVec[i] = 0;
				}
			}
		break;

		case T_FLOAT:
			fImVec = (float *)myImage;
			fThr = (float *)( threshold );
			for ( i=0, numNotNoise=0; i<vlen; i++ ) {
				if(fImVec[i] >= *fThr ) {
					numNotNoise++;
				} else {
					fImVec[i] = 0;
				}
			}
		break;

		case T_DOUBLE:
			dImVec = (double *)myImage;
			dThr = (double *)( threshold );
			for ( i=0, numNotNoise=0; i<vlen; i++ ) {
				if(dImVec[i] >= *dThr ) {
					numNotNoise++;
				} else {
					dImVec[i] = 0;
				}
			}
		break;

		case T_UCHAR:
			ucImVec = (unsigned char *)myImage;
			ucThr = (unsigned char *)( threshold );
			for ( i=0, numNotNoise=0; i<vlen; i++ ) {
				if(ucImVec[i] >= *ucThr ) {
					numNotNoise++;
				} else {
					ucImVec[i] = 0;
				}
			}
		break;

		case T_USHORT:
			uiImVec = (unsigned short *)myImage;
			uiThr = (unsigned short *)( threshold );
			for ( i=0, numNotNoise=0; i<vlen; i++ ) {
				if(uiImVec[i] >= *uiThr ) {
					numNotNoise++;
				} else {
					uiImVec[i] = 0;
				}
			}
		break;

		default:
			return( UNKNOWNTYPE );
		break;
	}
   return( numNotNoise );
}

/* --> ************************************************************************
*               <B>autoMask & autoThresh</B>
*
*  Determine the noise level and threshold based
*  on histogram determination
*
*  Assume that the noise is a commonly occuring
*  value in the bottom NRANGE of values. Use a
*  histogram of HBINS size and threshold at
*  NMULT times the peak in the lower NRANGE.
*
*  CALLING SEQUENCE <B>autoMask</B>:          // create a mask of pixels over a calculated threshhold
*     OSErr autoMask( void *invec,
*                     unsigned char *theMask,
*                     long npts,
*                     void *threshold,
*                     long *nonNoise,
*                     int data_type )
*
*  INPUTS:
*     invec  -- image vector
*     mask   -- output vector of short of same length as input
*     npts   -- vector size
*     vlen   -- vector length (long)
*  OUTPUTS:
*     theMask   -- pre-allocated vector that will contain 1 for points above thresh and 0 for points below
*     threshold -- updated to contain the value used
*     nonNoise  -- the number of points above threshold
*
*  CALLING SEQUENCE <B>autoThresh</B>:              // determine the noise threshold
*     OSErr autoThresh( void *invec,
*                       long npts,
*                       void *threshold,
*                       long *nonNoise,
*                       int data_type )
*
*  INPUTS:
*     invec  -- input vector. T_USHORT and T_FLOAT supported
*     npts   -- vector length (int)
*  OUTPUTS:
*     threshold --  contains the calculated value
*     nonNoise  --  the number of points above threshold
<!-- ************************************************************************/
OSErr autoMask( void *invec, unsigned char *theMask, long npts, void *threshold, long *nonNoise, int data_type )
{
   OSErr    error = noErr;
   char     *cvec, cHiCut, cNoise;
   short    *svec, sHiCut, sNoise;
   int      *ivec, iHiCut, iNoise;
   float    *fvec, fHiCut, fNoise;
   double   *dvec, dHiCut, dNoise;
   unsigned char *ucvec, ucHiCut, ucNoise;
   unsigned short *uivec, uiHiCut, uiNoise;   

	switch( data_type ) {
		case T_CHAR:
			cvec = (char *)invec;
			error = imageDataRange( cvec, npts, &cHiCut, &cNoise, nonNoise, data_type );     /* get the data range */
				RETURNONERROR;
			*nonNoise = ThreshMask( cvec, theMask, npts, &cNoise, data_type);  /* create the mask */
			vmov( &cNoise,1,threshold,1,1,T_CHAR );
		break;

		case T_SHORT:
			svec = (short *)invec;
			error = imageDataRange( svec, npts, &sHiCut, &sNoise, nonNoise, data_type );
				RETURNONERROR;
			*nonNoise = ThreshMask( svec, theMask, npts, &sNoise, data_type);
			vmov( &sNoise,1,threshold,1,1,T_SHORT );
		break;

		case T_INT:
			ivec = (int *)invec;
			error = imageDataRange( ivec, npts, &iHiCut, &iNoise, nonNoise, data_type );
				RETURNONERROR;
			*nonNoise = ThreshMask( ivec, theMask, npts, &iNoise, data_type );
			vmov( &iNoise,1,threshold,1,1,T_INT );
		break;

		case T_FLOAT:
			fvec = (float *)invec;
			error = imageDataRange( fvec, npts, &fHiCut, &fNoise, nonNoise, data_type );
				RETURNONERROR;
			*nonNoise = ThreshMask( fvec, theMask, npts, &fNoise, data_type);
			vmov( &fNoise,1,threshold,1,1,T_FLOAT );
		break;

		case T_DOUBLE:
			dvec = (double *)invec;
			error = imageDataRange( dvec, npts, &dHiCut, &dNoise, nonNoise, data_type );
				RETURNONERROR;
			*nonNoise = ThreshMask( dvec, theMask, npts, &dNoise, data_type);
			vmov( &dNoise,1,threshold,1,1,T_DOUBLE );
		break;

		case T_UCHAR:
			ucvec = (unsigned char *)invec;
			error = imageDataRange( ucvec, npts, &ucHiCut, &ucNoise, nonNoise, data_type );
				RETURNONERROR;
			*nonNoise = ThreshMask( ucvec, theMask, npts, &ucNoise, data_type);
			vmov( &ucNoise,1,threshold,1,1,T_UCHAR );
		break;

		case T_USHORT:
			uivec = (unsigned short *)invec;
			error = imageDataRange( uivec, npts, &uiHiCut, &uiNoise, nonNoise, data_type );
				RETURNONERROR;
			*nonNoise = ThreshMask( uivec, theMask, npts, &uiNoise, data_type);
			vmov( &uiNoise,1,threshold,1,1,T_USHORT );
		break;

		default:
			return( UNKNOWNTYPE );
		break;
	}
	return( error );
}

OSErr autoThresh( void *invec, long npts, void *threshold, long *nonNoise, int data_type )
{
   OSErr    error = noErr;
   char     *cvec, cHiCut, cNoise;
   short    *svec, sHiCut, sNoise;
   int      *ivec, iHiCut, iNoise;
   float    *fvec, fHiCut, fNoise;
   double   *dvec, dHiCut, dNoise;
   unsigned char *ucvec, ucHiCut, ucNoise;
   unsigned short *uivec, uiHiCut, uiNoise;  
   
	switch( data_type ) {
		case T_CHAR:
			cvec = (char *)invec;
			error = imageDataRange( cvec, npts, &cHiCut, &cNoise, nonNoise, data_type );  /* get the data range */
				RETURNONERROR;
			*nonNoise = vThreshold( cvec, npts, &cNoise, data_type);     /* threshold the image */
			vmov( &cNoise,1,threshold,1,1,T_CHAR );
		break;

		case T_SHORT:
			svec = (short *)invec;
			error = imageDataRange( svec, npts, &sHiCut, &sNoise, nonNoise, data_type );
				RETURNONERROR;
			*nonNoise = vThreshold( svec, npts, &sNoise, data_type);
			vmov( &sNoise,1,threshold,1,1,T_SHORT );
		break;

		case T_INT:
			ivec = (int *)invec;
			error = imageDataRange( ivec, npts, &iHiCut, &iNoise, nonNoise, data_type );
				RETURNONERROR;
			*nonNoise = vThreshold( ivec, npts, &iNoise, data_type);
			vmov( &iNoise,1,threshold,1,1,T_INT );
		break;

		case T_FLOAT:
			fvec = (float *)invec;
			error = imageDataRange( fvec, npts, &fHiCut, &fNoise, nonNoise, data_type );
				RETURNONERROR;
			*nonNoise = vThreshold( fvec, npts, &fNoise, data_type);
			vmov( &fNoise,1,threshold,1,1,T_FLOAT );
		break;

		case T_DOUBLE:
			dvec = (double *)invec;
			error = imageDataRange( dvec, npts, &dHiCut, &dNoise, nonNoise, data_type );
				RETURNONERROR;
			*nonNoise = vThreshold( dvec, npts, &dNoise, data_type);
			vmov( &dNoise,1,threshold,1,1,T_DOUBLE );
		break;

		case T_UCHAR:
			ucvec = (unsigned char *)invec;
			error = imageDataRange( ucvec, npts, &ucHiCut, &ucNoise, nonNoise, data_type );
				RETURNONERROR;
			*nonNoise = vThreshold( ucvec, npts, &ucNoise, data_type);
			vmov( &ucNoise,1,threshold,1,1,T_UCHAR );
		break;

		case T_USHORT:
			uivec = (unsigned short *)invec;
			error = imageDataRange( uivec, npts, &uiHiCut, &uiNoise, nonNoise, data_type );
				RETURNONERROR;
			*nonNoise = vThreshold( uivec, npts, &uiNoise, data_type);
			vmov( &uiNoise,1,threshold,1,1,T_USHORT );
		break;

		default:
			return( UNKNOWNTYPE );
		break;
	}
	return error;
}

/* --> **************************************************************
*  <B>FloatToCImage</b>
*
*  Convert a floating point vector to a vector of char
*
*  void FloatToCImage(
*           inImage  --  pointer to the input image
*            cimage  --  pointer to the output (char) image
*               len  --  the number of image points
*        *max, *min  --  Calculated or input max and min. Same type as inImage
*           rescale  --  kAutoRange, kFileMinMax, kManualRange, kNoScale
*         data_type )--  data type of input image
*
<!-- **************************************************************/
OSErr FloatToCImage( float *inImage, void *cimage, long len,
                   float *fmax, float *fmin, float newMax, float newMin, short rescale )
/* Convert a vector of floating point data to a properly scaled
   vector of unsigned char  */
{
	long           i;
	unsigned char  *out;
	float          scale;	// was double MSC 4-24-02
	OSErr          error = noErr;
	long           nonNoise;
	float          FMax, FMin; //	local copies.
	
	out = (unsigned char *)cimage;
	FMax = *fmax;
	FMin = *fmin;
	
	if( rescale == kNoScale ) {
			for( i=0; i<len; i++ ) {
			out[i] = (unsigned char)inImage[i] + 0.5;
		}
		return noErr;
	}
	
	if( rescale == kFileMinMax ) {
		FMax = FMin = inImage[0];
		for( i=0; i<len; i++ ) {
			out[i] = (unsigned char)inImage[i] + 0.5;
			FMax = FMax>inImage[i] ? FMax : inImage[i];
			FMin = FMin<inImage[i] ? FMin : inImage[i];
		}
	} else if( rescale == kAutoRange ) {
		error = imageDataRange( inImage, len, fmax, fmin, &nonNoise, T_FLOAT );
		FMax = *fmax;
		FMin = *fmin;
	}	/* default is to use min and max as passed to routine */
	
	if( error!=noErr && error!=NEG_DATA_IN_AUTORANGE ) {
		return error;
	}	
	
	/* default behavior supports rescale == kMinMax already */
	scale = (double)(newMax - newMin)/(FMax - FMin);
	
	/* So that this doesn't need extra memory for the type conversion, do not use vrescale */
	for( i=0; i<len; i++ ) {
		if( inImage[i] <= FMin ) {
			out[i] = newMin;
		} else if( inImage[i] >= FMax ) {
			out[i] = newMax;
		} else {
			out[i] = (unsigned char)( 0.5 + scale*( inImage[i] - FMin ) );
		}
	}
	return error;
}

/* --> **********************************************************************
*
*  <B>ImageChangeDetect</B>
*
*  Count the number of image points that change from above to below
*  threshold or vice versa.
*
*  OSErr ImageChangeDetect(
*  INPUTS
*          myImage   --  pointer to the input image
*           theMask  --  pointer to the previously calculated image mask
*         threshold  --  intensity threshold used to make the mask
*               npts --  number of points in the image and mask
*  RETURNS
*        numlowerPts --  number of points that have dropped below threshold 
*      numGreaterPts -- number of points that have increased above threshold
*
<!-- ***********************************************************************/
OSErr ImageChangeDetect( unsigned short *myImage, unsigned char *theMask, float threshold, long npts,
                    long *numLowerPts, long *numGreaterPts )
{
	OSErr	error = noErr;
	long	i;
	
	*numGreaterPts = *numLowerPts = 0;
	
	for( i=0; i<npts; i++ ) {
		if( !theMask[i] && myImage[i]>=threshold ) { /* gone from noise to signal */
			(*numGreaterPts)++;
		} else if( theMask[i] && myImage[i]<threshold ) { /* change from signal to noise */
			(*numLowerPts)++;
		}
	}
	return error;
}	

/* --> **********************************************************************
*
*  <B>MaskChangeDetect</B>
*
*  Count the number of points that differ between two masks.
*
*  long MaskChangeDetect(
*  INPUTS
*            oldMask   --  pointer to the previously calculated image mask
*            newMask   --  pointer to the current image mask
*               npts ) --  number of points in the image and mask
*  RETURNS
*        number of points that have changed 
*
<!-- ***********************************************************************/
long MaskChangeDetect( unsigned char *oldMask, unsigned char *newMask, long npts )
{
	long numChangePts = 0;
	long i;
	
	for( i=0; i<npts; i++ ) {
		if( oldMask[i] != newMask[i] )
			numChangePts++;
	}
	return numChangePts;
}	

/* --> ************************************************************************
*   <B>vRescale</B>
*   Adjust the values of the input vector to fall between newMin and newMax
*
*   CALLING SEQUENCE:
*   double vRescale( void *invec, -- input (and output) data vector
*          long vlen,             -- length of input and ouput vectors
*          void *oldMax,          -- Maximum value of input vector to be used
*          void *oldMin,          -- Minimum value of input to be used
*          void *newMax,          -- New Maximum for data range
*          void *newMin,          -- New Minimum for data range
*          int data_type );       -- standard data type
*
*   RETURNS:
*          calculated scale factor.
*
*   Values less than or equal to oldMin in the input vector are set to newMin
*   Those greater than or equal to oldMax are set to newMax
*   The rest are scaled linearly between these endpoints.
<!-- ************************************************************************/
double     vRescale( void *invec, long vlen, void *oldMax, void *oldMin,
                     void *newMax, void *newMin, int data_type )
{
/* update: 06/07/02 Zrinka - due to performance issues of scanSTAT on
   OS X this function was changed such that newMax, newMin, oldMax and
   OldMin local variables (of different types) are no longer pointers. 
   By removing pointers, image resizing on OS X has improved significantly */
   
   int i;
   char     *cvec, cnewMax, cnewMin, cOldMax, cOldMin;
   short    *svec, snewMax, snewMin, sOldMax, sOldMin;
   int      *ivec, inewMax, inewMin, iOldMax, iOldMin;
   float    *fvec, fnewMax, fnewMin, fOldMax, fOldMin;
   double   *dvec, dnewMax, dnewMin, dOldMax, dOldMin, scale;
   unsigned char *ucvec, ucnewMax, ucnewMin, ucOldMax, ucOldMin;
   unsigned short *uivec, uinewMax, uinewMin, uiOldMax, uiOldMin;   
  
 
switch( data_type ) {
   case T_CHAR:
         cvec = (char *)invec;
         cnewMax = *(char *)(newMax); cnewMin = *(char *)(newMin);
         cOldMax = *(char *)(oldMax); cOldMin = *(char *)(oldMin);
         scale = (double)(cnewMax - cnewMin)/(cOldMax - cOldMin);
         for( i=0; i< vlen; i++ )
            if( cvec[i] <= cOldMin )
               cvec[i] = cnewMin;
            else if( cvec[i] >= cOldMax )
               cvec[i] = cnewMax;
            else cvec[i] = (char)( scale*(cvec[i] - cOldMin) );
      break;
   case T_SHORT:
         svec = (short *)invec;
         snewMax = *(short *)(newMax); snewMin = *(short *)(newMin);
         sOldMax = *(short *)(oldMax); sOldMin = *(short *)(oldMin);
         scale = (double)(snewMax - snewMin)/(sOldMax - sOldMin);
         for( i=0; i< vlen; i++ )
            if( svec[i] <= sOldMin )
               svec[i] = snewMin;
            else if( svec[i] >= sOldMax )
               svec[i] = snewMax;
            else svec[i] = (short)( scale*(svec[i] - sOldMin) );
      break;
   case T_INT:
         ivec = (int *)invec;
         inewMax = *(int *)(newMax); inewMin = *(int *)(newMin);
         iOldMax = *(int *)(oldMax); iOldMin = *(int *)(oldMin);
         scale = (double)(inewMax - inewMin)/(iOldMax - iOldMin);
         for( i=0; i< vlen; i++ )
            if( ivec[i] <= iOldMin )
               ivec[i] = inewMin;
            else if( ivec[i] >= iOldMax )
               ivec[i] = inewMax;
            else ivec[i] = (int)( scale*(ivec[i] - iOldMin) );
      break;
   case T_FLOAT:
         fvec = (float *) invec;
         fnewMax = *(float *)(newMax); fnewMin = *(float *)(newMin);
         fOldMax = *(float *)(oldMax); fOldMin = *(float *)(oldMin);
         scale = (double)(fnewMax - fnewMin)/(fOldMax - fOldMin);
         for( i=0; i< vlen; i++ )
            if( fvec[i] <= fOldMin )
               fvec[i] = fnewMin;
            else if( fvec[i] >= fOldMax )
               fvec[i] = fnewMax;
            else fvec[i] = (float)( scale*(fvec[i] - fOldMin) );
      break;
   case T_DOUBLE:
         dvec = (double *)invec;
         dnewMax = *(double *)(newMax); dnewMin = *(double *)(newMin);
         dOldMax = *(double *)(oldMax); dOldMin = *(double *)(oldMin);
         scale = (double)(dnewMax - dnewMin)/(dOldMax - dOldMin);
         for( i=0; i< vlen; i++ )
            if( dvec[i] <= dOldMin )
               dvec[i] = dnewMin;
            else if( dvec[i] >= dOldMax )
               dvec[i] = dnewMax;
            else dvec[i] = (double)( scale*(dvec[i] - dOldMin) );
      break;
   case T_UCHAR:
         ucvec = (unsigned char *)invec;
         ucnewMax = *(unsigned char *)(newMax); ucnewMin = *(unsigned char *)(newMin);
         ucOldMax = *(unsigned char *)(oldMax); ucOldMin = *(unsigned char *)(oldMin);
         scale = (double)(ucnewMax - ucnewMin)/(ucOldMax - ucOldMin);
         for( i=0; i< vlen; i++ )
            if( ucvec[i] <= ucOldMin )
               ucvec[i] = ucnewMin;
            else if( ucvec[i] >= ucOldMax )
               ucvec[i] = ucnewMax;
            else ucvec[i] = (unsigned char)( scale*(ucvec[i] - ucOldMin) );
      break;
   case T_USHORT:
         uivec = (unsigned short *)invec;
         uinewMax = *(unsigned short *)(newMax); uinewMin = *(unsigned short *)(newMin);
         uiOldMax = *(unsigned short *)(oldMax); uiOldMin = *(unsigned short *)(oldMin);
         scale = (double)(uinewMax - uinewMin)/(uiOldMax - uiOldMin);
         for( i=0; i< vlen; i++ )
            if( uivec[i] <= uiOldMin )
               uivec[i] = uinewMin;
            else if( uivec[i] >= uiOldMax )
               uivec[i] = uinewMax;
            else uivec[i] = (unsigned short)( scale*(uivec[i] - uiOldMin) );
      break;

   default:
      return( UNKNOWNTYPE );
      break;
   }
   return( scale );
}

/* --> *****************************************************************
*    <B>getRowsCols</B> - calculate the number of rows and columns in a multidisplay
*                  image of known xs, ys and slice
*
*    CALLING SEQUENCE:
*          getRowsCols(short slices,     -- Number of slices in input volume
*                      short &Rows,
*                      short &Cols )     -- Returns number of rows and cols in multi display
<!-- *************************************************************************/
OSErr getRowsCols( short slices, short *ROWS, short *COLS )
/* takes a vertical stack of images and converts it to a multi-display format */
{
	OSErr error = noErr;

	short rows=0, cols=1;

	rows = ( short )sqrt( (double)slices );
	while( rows * cols < slices ) {
		cols++;
	}

	*ROWS = rows;
	*COLS = cols;

	if( rows == 0 || cols == 0 ) {
		return DATA_OUT_OF_RANGE;
	}

	return error;
}

/* --> *************************************************************************
*    <B>mdisplay</B> - take a vertical stack of images and rearrange into a single
*               multi-slice display. Memory for the output image must be
*               pre-allocated. Typically, this will mean a call to getRowsCols
*
*    e.g.,  getRowsCols( slices, &rows, &cols );
*           multImg = malloc( rows*cols*dataSize );
*           error = mdisplay( multImg, in, xs, ys, slices, rows, cols, data_type );
*
*    Returns an error if the number of slices does not fit into the Row x Col space
*    CALLING SEQUENCE:
*    OSErr mdisplay(
*               OutImg  -- Storage for output image
*               InImg   -- Vertically arranged image stack or volume
*               xs, ys  -- Dimension of slices in input volume
*               slices  -- Number of slices in input volume
*            data_type  -- Standard UC data_type for input and output
*                     )
*   NOT TO BE USED AS AN IN PLACE ROUTINE!
<!-- *************************************************************************/

/* A brief history of pointers in the new mdisplay */
/* p is the source, q is the destination.
   We walk through the input vector line by line and put each
   on the correct part of the multi-display.
   We don't over-run the display or the input vector because
   we use the slices to stop us. (smile :-)
   
/*       source                   destination: an MDisplay
       < - XS - >            < -  XScols: (XS * cols) - - - - - - >
        +-------+             +--------+--------+--------+--------+ ^
i0  p0->|  L0   | ^       q0->|     L0 |        |        |        | |
        |-------| |  r=0  q1->|  i0 L1 |   1    |   2    |   3    | ys
    p1->|  L1   | ys          |     LN |        |        |        | |
        |-------| |           +--------+--------+--------+--------+ v
        |  LN   | v           |        |        |        |        |  
        +-------+    r=1      |   4    |   5    |   6    |   7    |  
i1      |  L0   |             |        |        |        |        |  
        |-------|             +--------+--------+--------+--------+
        |  L1   |             |        |        |        |        |  
        |-------|    r=2      |   8    |   9    |  10    |  11    |
        |  LN   |             |        |        |        |        |  
        +-------+             +--------+--------+--------+--------+
        |       |             |        |        |        |        |  
                     r=3      |  12    |  13    |  14    |  15    |
        |       |             |        |        |        |        |  
        +-------+             +--------+--------+--------+--------+
i18 pn->|  LN   |             |        |        |        | may    |  
        +-------+    r=rows-1 |  16    |   17   |  18    | be     |
                              |        |        |        | empty  |
       slices = 19            +--------+--------+--------+--------+
                                c=0      c=1      c=2      c=cols-1
                                
     ysXScols is images 4, 5, 6, & 7 (the size of a whole row).
     used to move the q pointer to the next row (q0 for each r)
                                
*/

OSErr  mdisplay( void *OutImg, void *InImg, int xs, int ys, int slices, int data_type )
{
	OSErr	error = noErr;
	int		theCol, theRow;	/* which row and col will be accepting this slice? */
	int		y, slice;
	int		XS;			/* xsize in bytes */
	long	imSize;		/* image size in bytes */
	char	*inputOffs;
	char	*outputOffs;
	short	Rows, Cols;

/* clear the display space */
	error = getRowsCols( slices, &Rows, &Cols );
		RETURNONERROR;

	vclr( OutImg, Rows*Cols*xs*ys, data_type );
	
	XS = xs*get_datasize( data_type );
	imSize = XS*ys;
	
	theCol = theRow = 0;
	
	for( slice=0; slice<slices; slice++ ) {
		theCol = slice%Cols;
		theRow = slice/Cols;

		outputOffs = (char *)OutImg + theRow*Cols*imSize + theCol*XS;
		inputOffs  = (char *)InImg  + imSize*slice;

		for( y=0; y<imSize; y+=XS ) {
			memcpy( outputOffs + y*Cols,
					inputOffs  + y,
					XS );
		}
	}
	return error;
}

/* --> ******************************************************************
*   <b>unmdisplay</b>
*	Take multi-display data set and convert it into a
*	'volume' structure (xs x ys*slices )
*
*   OSErr  unmdisplay( void *InImg,     -- input image
*                      void *OutImg,    -- pre-allocated memory for output
*                      int xs, int ys,  -- image dimensions of output images
*                      int slices,      -- number of valid slices in the input set
*                      int data_type )
*
*	xs and ys are the sizes of the final images (after unmdisplay)
<!-- ******************************************************************/
OSErr  unmdisplay( void *InImg, void *OutImg, int xs, int ys, int slices, int data_type )
{
	OSErr	error = noErr;
	long	theCol, theRow;	/* which row and col will be accepting this slice? */
	long	y, slice;
	long	XS;			/* xsize in bytes */
	short   rows, cols;
	long	imSize;		/* image size in bytes */
	char	*inputOffs;
	char	*outputOffs;

	error = getRowsCols( slices, &rows, &cols );
		RETURNONERROR;

	XS = xs*get_datasize( data_type );
	imSize = XS*ys;
	
	for( slice=0; slice<slices; slice++ ) {
		theCol = slice%cols;
		theRow = slice/cols;

		inputOffs  = (char *)InImg  + theRow * cols * imSize + theCol * XS;
		outputOffs = (char *)OutImg + imSize * slice;

		for( y=0; y<imSize; y+=XS ) {
			memcpy( outputOffs + y,
			        inputOffs  + y*cols,
					XS );
		}
	}
	return error;
}

/* --> ************************************************************************
*  <B>waitfopen</B>
*  Read successive items from a file. Waits for items to exist, if necessary.
*
*  CALLING SEQUENCE:
*      FILE *waitfopen( char *name,   --  the filename to open
*                       char *mode    --  the file mode
*                     int WaitTime )  --  time to wait for file existence (seconds)
<!-- ************************************************************************/
#ifdef UNIX
FILE  *waitfopen( char *name, char *mode, int WaitTime )
{
// #define ENOENT 2
#ifdef no_log_file
    const short dbg = 0; /* must be quiet */
#else
    const short dbg = 1;
#endif
   FILE *fp;
   int i;
   char sleepCommand[] = "sleep 1";

   i = WaitTime;

   fp = fopen( name, mode );
   while( fp == NULL ) {
      if( errno == ENOENT ) {
            if(( i%5 == 0 ) && dbg) {
               printf( "Waiting for file %s for %d more seconds\n", name, i );
            }
            fp = fopen( name, mode );
            i--;
            if( i<= 0 ) {
               printf( "Timed out waiting for %s.\n", name );
               exit(-1);
            }
            system( sleepCommand );
      } else {
         exit(-1);
      }
   }
   return( fp );
}

#endif	/* unix only */

/* --> ************************************************************************
*      <B>ReadNextFileString</B>
*
*      Sort of a stripped down fscanf for reading strings from Mac files
*
*      OSErr ReadNextFileString( short fileID, char *myString )
*
<!-- ************************************************************************/
#ifdef MAC
OSErr ReadNextFileString( short fileID, char *myString )
{
   // Presumes that the file remains at the current mark
   OSErr error = noErr;
   long one = 1;
   char c;
   Ptr  myPtr = myString;

   myString[0] = '\0';       // Empty the string
   do {  /* skip leading white space */
      one = 1; /* if we get an error, one will be zero! */
      error = FSRead(fileID, &one, &c);
   }
   while ((error == noErr) && (c == ' ') || (c == '\t') || (c == '\r') || (c == '\n'));
 
   do {       /* cat the successive non-terminal characters onto the string */
      *myPtr++ = c;
      one = 1; /* if we get an error, one will be zero! */
      error = FSRead(fileID, &one, &c);
      RETURNONERROR;    /* most likely an eofErr */
         /* in the event that we encountered an eof before the next white space
            we COULD call WaitNextEvent, or something, to avoid sending invalid data */
   }
   while( (c != ' ') && (c != '\t') && (c != '\r') && (c != '\n'));
   *myPtr++ = '\0';   /* terminate the string */
   return error;
}   
#endif

/* --> ************************************************************************
*  <B>WaitNextItemFromList</B>
*  Read successive items from a (paragigm) file. Waits for items to exist, if necessary.
*
*  CALLING SEQUENCE:
*  (unix)
*  OSErr WaitNextItemFromList( char *Item,   --  the returned item (string)
*                                FILE *fp,   --  pointer to an open file
*                              char *fname   --  the filename for an error message
*                           int WaitTime )   --  Time to wait for item to be valid ( seconds );
*  (mac)
*  OSErr  WaitNextItemFromList( char *Item, short fileID, int WaitSec )
*
<!-- ************************************************************************/
#ifdef UNIX
OSErr  WaitNextItemFromList( char *Item, FILE *fp, char *fname, int WaitTime )
{
/* Reads an element from fp. If it is not (yet) there, then wait up to one minute */
#ifdef no_log_file
	const short dbg = 0; /* must be quiet */
#else
	const short dbg = 1;
#endif
	int scan_result;
	int  i;
	char sleepCommand[] = "sleep 1";

	i = WaitTime;
	scan_result = fscanf( fp, "%s", Item);

	while( (scan_result != 1) && ( i>0 ) ) {
		if( (i%5 == 0 ) && dbg) {
			printf( "Waiting for data from %s: %d more seconds\n", fname, i );
		}

		scan_result = fscanf( fp, "%s", Item);
		i--;
		if( i<= 0 ) {
			if (dbg) {
				printf( "Timed out reading from list\n" );
				return COULD_NOT_OPEN_IMAGE;
			}
		}
		system( sleepCommand );
	}
	return scan_result;
	}
#endif
#ifdef MAC
OSErr  WaitNextItemFromList( char *Item, short fileID, int WaitSec )
{
/* Reads an element from fileID. If it is not (yet) there, then wait up to one minute */
	OSErr         scan_result;
	EventRecord   theEvent;
	long          SleepTime = 60;     /* wait for one second per pass */
	int           i= WaitSec;

	scan_result = ReadNextFileString( fileID, Item );
	while( (scan_result != eofErr ) && (i<WaitSec) ) {
		WaitNextEvent( everyEvent, &theEvent, SleepTime, nil );
		scan_result = ReadNextFileString( fileID, Item );
		i--;
	}
	if( i <= 0 ) {
		return( TIMEOUTERROR );
	}
	return scan_result;
}
#endif

/* --> ************************************************************************
*   <B>get_para_file_size</B>
*   get_paradigm
*
*   OSErr get_para_file_size( char *fname )
*   OSErr get_paradigm( char *fname, float *pv, int npts )
*
*   get_para_file_size -- returns the number of points in a
*   "paradigm file" of the type used to process scanSTAT images.
*   INPUTS:  char *fname
*   OUTPUTS:  return value is the number of elements.
*
*   get_paradigm -- is usually called after get_para_file_size.
*   It takes the ascii input in the text file and converts it
*   to a vector of floats.
*   INPUTS:  char *fname -- name of the paradigm file
*            float *pv -- a pointer to the vector that will hold the
*                         paradigm file values.
*            int npts -- number of points in the paradigm file.
<!-- ***********************************************************************/
#ifdef UNIX
OSErr get_para_file_size( char *fname )
{
	char buffer[128];
	FILE *fp;
	int npts;

	ck_fopen( &fp, fname,"r");

	if( fp==NULL ) {
		ILError( PARADIGM_FILE_ERROR, "get_para_file_size" );
	}
	npts = 0;

	while( fgets( buffer, 128, fp ) ) {
		npts++;
	}
	fclose(fp);
	return( npts );
}

OSErr get_paradigm( char *fname, float *pv, int npts )
{
	int i;
	char pval[64];
	FILE *fp;
	OSErr error = noErr;

   ck_fopen( &fp, fname,"r");

	for (i=0; i<npts; i++) {
		fscanf(fp,"%s",pval);
		*pv++ = atof( pval );
	}

	fclose(fp);
	return error;
}
#endif

/* --> ******************************************************************************
*   <B>irpStringFromIMAGE</B>
*	Create a string containing the valid contents for an APD header file
*	These files end in .irp and have names that are four characters shorter 
*	than the corresponding image file.
*
*	Note that more than one image file can be supported by a single .irp
*
*   char *irpStringFromIMAGE( IMAGE *im )
*	INPUTS:
*		a pointer to a populated IMAGE struct
*	OUTPUTS
*		a freshly minted string.
*	Calling routine is responsible for disposing this memory
<!-- ******************************************************************************/
char *irpStringFromIMAGE( IMAGE *im )
{
// this is really not exactly right, as it assumes that phase encode is along Y
	char *irpString;	// i.e., long enough

	Boolean partPhase = im->acq.nPhaseLines < im->dim.acq_Height ? true : false;
	Boolean partRead  = im->acq.nReadPoints < im->dim.acq_Width ? true : false;
	Boolean overSample = true;	// we are not keeping track
	short xextra = (short)(im->acq.nReadPoints/im->dim.acq_Width) - im->dim.acq_Width/2;
	short xover = 0;	// we are not keeping track
	Boolean magnitude = false;
	Boolean I_image = false;
	Boolean Q_image = false;
	Boolean phase_image = false;
	Boolean time_data = false;	// for now

	short DisplayType = MAGNITUDE;	// Just for now

	switch( DisplayType ) {
		case MAGNITUDE: magnitude = true; break;
		case REAL:      I_image = true; break;
		case IMAGINARY: Q_image = true; break;
		case PHASE: phase_image = true; break;
		default: magnitude = true;
	}
		
	irpString = ck_malloc( 1024, "irpString" );

sprintf( irpString,
"num_slices     %d\n\
images_per_slice     %d\n\
NEX     %f\n\
xsize     %d\n\
ysize     %d\n\
part_phase     %d\n\
part_read     %d\n\
oversample     %d\n\
interleave     %d\n\
read_fov     %d\n\
phase_fov     %d\n\
xextra     %hd\n\
xover     %hd\n\
magnitude_image     %d\n\
I_image     %d\n\
Q_image     %d\n\
phase_image     %d\n\
time_data     %d\n\
rotation     0\n\
reflection_x     1\n\
reflection_y     0",
im->dim.n_slices, 
im->dim.timePts, 
im->acq.nex,
im->dim.x, 
im->dim.y, 
partPhase, 
partRead, 
overSample,
(int)im->acq.interleave, 
(int)im->dim.acqfov, 
(int)im->dim.acqfov_rect, 
xextra, 
xover, 
magnitude,
I_image, 
Q_image, 
phase_image, 
time_data );

	return irpString;
}

OSErr calc_orientationFromIMAGE( IMAGE *im, char *orient );
OSErr calc_orientationFromIMAGE( IMAGE *im, char *orient )
{
	if( im->dim.x ) {
		*orient = axial;	// axial
	}
	return noErr;
}

/* --> ***************************************************************************
*  <B>SetImDatatype</B>
*
*  Sets the data type for an image to the selected type
*
*  Calling String:
*  SetImDatatype( short data_type	// base name of the output image
*            IMAGE *im );
*
*  Indirect accessor to ensure data consistency.
<!-- ******************************************************************************/
void  SetImDatatype( IMAGE *im, short data_type )
{
	im->data_type = data_type;
	im->dim.BytesPix = get_datasize( data_type );
}

/* --> ***************************************************************************
*  <B>WriteAnalyzeFile</B>
*
*  Save image file with the selected name
*
*  Calling String:
*  OSErr = ( char  *basename	// base name of the output image
*            IMAGE *im,
*            void  *imageData );
*
*  Output image type is determined from file name and data type.
*  Data are rescaled if needed
<!-- ******************************************************************************/
OSErr WriteAnalyzeFile( char  *basename, IMAGE *im, void *imageData )
{
	OSErr   error = noErr;
	FILE    *imFile;
	char    *filename;
	float   datamax, datamin;
	float   datarange, datascale;
	long    imSize = im->dim.x * im->dim.y;

	filename = ck_malloc( strlen( basename ) + 4, "filename" );
	strcpy( filename, basename );
	strcat( filename, ".img" );
	
	error = ck_fopen( &imFile, filename, "wb");
	RETURNONERROR;
	
	error = CreateHeader( ANALYZE, im, basename );
	RETURNONERROR;
	
	if( im->data_type != T_CHAR ) {
		error = ck_fwrite( imageData, get_datasize( im->data_type ), imSize, imFile );
		RETURNONERROR;
	} else {
		error = vfminmax( imageData, imSize, &datamax, &datamin, T_CHAR );
		RETURNONERROR;
		if( datarange > CHAR_MAX || datarange <= 64 ) {
			datascale = CHAR_MAX/datarange;
		}
		error = vfsmul(imageData, 1, imageData, 1, datascale, imSize, im->data_type ); 
		RETURNONERROR;	
		error = ck_fwrite( imageData, get_datasize( T_CHAR ), imSize, imFile );
		RETURNONERROR;
	}
	
	error = ck_fclose( imFile );
	return noErr;
}

/* --> ***************************************************************************
*  <B>CreateHeader</B>
*
*  Write a valid header of the type associated with the selected file type
*
*  Calling String:
*  OSErr = ( int FileType,		// BSHORT, BUCHAR, BFLOAT, ANALYZE, EPI (Genesis not yet supported)
*         IMAGE im,
*         char *basename )  // base name of the output image
*
*  CreateHeader will clear the cropping flags that would appear in Analyze header,
*  as we save the images in cropped format.
<!-- ******************************************************************************/
/* note that here, as should be generally true, we interpret the Z dimension
	of the MGH header as TIME

	CreateHeader will create a valid image header with contents based on the struct IMAGE
*/
OSErr  CreateHeader( int FileType, IMAGE *im, char *baseName )
{
	FILE  *hdrFile;
	dsr   *myDSR;
	char  HeaderName[63];
	short isBigEndian;
	OSErr error = noErr;

	switch ( FileType ) {
		case BSHORT:
		case BUCHAR:
		case BFLOAT:
			if( im->dim.flags & kmacByteOrder ) {
				isBigEndian = 1;
			} else {
				isBigEndian = 0;
			}
			strcpy( HeaderName, baseName );
			strcat( HeaderName, ".hdr" );
			error = ck_fopen( &hdrFile, HeaderName,"w");
			RETURNONERROR;
			fprintf( hdrFile, "%d %d %d %hd", im->dim.isoY * im->dim.n_slices, im->dim.isoX,
					im->dim.timePts, isBigEndian );
			error = ck_fclose( hdrFile );
			break;
			
		case ANALYZE:
			strcpy( HeaderName, baseName );
			strcat( HeaderName, ".hdr" );

			myDSR = (dsr *)malloc( sizeof( dsr ));
			
			error = dsrFromIMAGE( im, myDSR );
				ILError( error, "Creating Analyze Header" );

			if( !( im->dim.flags & kmacByteOrder ) && macByteOrder()) {
				error = swapHdrBytes( myDSR );
				RETURNONERROR;
			}
			if(( im->dim.flags & kmacByteOrder ) && pcByteOrder()) {
				error = swapHdrBytes( myDSR );
				RETURNONERROR;
			}

/* we assume that the images have been cropped before saving */
			CLEAR( myDSR->dime.flags, kNeedXCrop );
			CLEAR( myDSR->dime.flags, kNeedYCrop );

			error = ck_fopen( &hdrFile, HeaderName,"wb");
			RETURNONERROR;
			error = ck_fwrite( (void *)myDSR, sizeof( dsr ), 1, hdrFile );
				if( error ) {
					fclose( hdrFile );
					return error;
				}
			error = ck_fclose( hdrFile );
			break;
			
		case EPI:
			strcpy( HeaderName, baseName );
			strcat( HeaderName, ".irp" );

			error = ck_fopen( &hdrFile, HeaderName,"w");
			RETURNONERROR;
			fprintf( hdrFile, "%s", irpStringFromIMAGE( im ) );
			error = ck_fclose( hdrFile );
			break;

// This was removed so that AFNI headers get created independently of ImgLib functions
// so that ImgLib does not depend on any AFNI libraries/header files.                        
//              case AFNI:	// added on 08/15/02 - zrinka
//                      error = CreateAFNIHeader( im, baseName );
//                      break;

		case VARIAN:
			return UNKNOWNTYPE;
/* this will need work */
//			Determine the needed headername;
//			Open the Varian-type header for writing (use ck_fopen...)
//			error = VarianFromIMAGE( im, myVarianHeader );
			RETURNONERROR;

//			Write the header file
//			break;
		default:
			return UNKNOWNTYPE;
	}
	
	return error;
}

/* --> ******************************************************************************
*   <B>FindOutputType</B>
*	Use the file name and extension to determine the output file type
*   OSErr FindOutputType( char *fname, int *itsType, short *data_type )
<!-- *******************************************************************************/
OSErr FindOutputType( char *fname, short *itsType, short *data_type )
{
	OSErr error = noErr;
	char  *extension;
	
	extension = strrchr( fname, '.' );
	
	if( !strcmp( extension, ".bfloat" )) {
		*itsType = BFLOAT;
		*data_type = T_FLOAT;
	} else if( !strcmp( extension, ".bshort" )) {
		*itsType = BSHORT;
		*data_type = T_SHORT;	/* changed 6-18-02. Was T_USHORT */
	} else if( !strcmp( extension, ".buchar" )) {
		*itsType = BUCHAR;
		*data_type = T_UCHAR;
	} else if( !strcmp( extension, ".img" )) {
		*itsType = ANALYZE;
		*data_type = T_SHORT;
	} else if( !strcmp( extension, ".MR" )) {
		*itsType = SIGNA;
		*data_type = T_USHORT;
    } else if( !strcmp( extension, ".BRIK" )) {	/* added on 08-14-02 zrinka */
		*itsType = AFNI;
		*data_type = T_SHORT;
	} else {
		*itsType = (short)No_TYPE;
		error = UNKNOWNFILETYPE;
	}
// Need Varian specifiers
	return error;
}

/* --> *****************************************************************************
*	<B>dsrFromIMAGE</B>
*   Populate all fields of a dsr from the IMAGE struct
*
*   OSErr  dsrFromIMAGE( IMAGE *im, dsr *theDSR )
*
*	INPUTS:
*		a pointer to a populated IMAGE struct
*		a pointer to a dsr;
*	OUTPUTS:
*		error
*	CHANGES:
*		contents of dsr
<!-- ******************************************************************************/
OSErr  dsrFromIMAGE( IMAGE *im, dsr *theDSR )
{
	char    nowstr[32];
	struct  tm *nowstruct;

	OSErr error = noErr;

	error = vclr( theDSR, sizeof( dsr ), T_CHAR );

	theDSR->hk.sizeof_hdr = sizeof( dsr );	// 348 bytes, last time I looked
	strcpy( theDSR->hk.db_name, "none" );
	theDSR->hk.extents = 16384;
	theDSR->hk.regular = 'r';


	theDSR->dime.dim[0] = 4;	
	theDSR->dime.dim[1] = im->dim.isoX;
	theDSR->dime.dim[2] = im->dim.isoY;
	theDSR->dime.dim[3] = im->dim.n_slices;
	theDSR->dime.dim[4] = im->dim.timePts;
	strcpy(	theDSR->dime.vox_units, "mm" );

	theDSR->dime.flags = im->dim.flags;	// duplicate all IMAGE flags

	switch( im->data_type )
	{
		case T_FLOAT:   theDSR->dime.datatype = DT_FLOAT;         break;
		case T_SHORT:   theDSR->dime.datatype = DT_SIGNED_SHORT;  break;
		case T_USHORT:  theDSR->dime.datatype = DT_SIGNED_SHORT;  break; // may not support unsigned short!!!
		case T_UCHAR:   theDSR->dime.datatype = DT_UNSIGNED_CHAR; break;
		case T_INT:     theDSR->dime.datatype = DT_SIGNED_INT;    break;
		case T_DOUBLE:  theDSR->dime.datatype = DT_DOUBLE;        break;
		case T_COMPLEX: theDSR->dime.datatype = DT_COMPLEX;       break;
		default:        theDSR->dime.datatype = DT_UNKNOWN;       break;
	}

	if( im->data_type > 0 && im->data_type< T_NATIVE ) {
		theDSR->dime.bitpix = 8 * get_datasize( im->data_type );
	}
	theDSR->dime.pixdim[0] = 4;
	theDSR->dime.pixdim[1] = im->dim.dpixsize_X;
	theDSR->dime.pixdim[2] = im->dim.dpixsize_Y;
	theDSR->dime.pixdim[3] = im->dim.slthick;
	theDSR->dime.pixdim[4] = im->acq.tr / 1e6;

	theDSR->dime.glmax = min( (int)im->theMaxf, INT_MAX );
	theDSR->dime.glmin = max( (int)im->theMinf, INT_MIN + 1);

	strncpy( theDSR->hist.descrip, im->subj.descr, 80 );
	strncpy( theDSR->hist.aux_file, im->subj.aux_file, 24 );
	error = calc_orientationFromIMAGE( im, &theDSR->hist.orient );
 		RETURNONERROR;

	strncpy( theDSR->hist.originator, im->subj.hospname, 10 );
	strcpy( theDSR->hist.generated, "" );
	strcpy( theDSR->hist.scannum, "0" );
	strncpy( theDSR->hist.patient_id, im->subj.patid, 16 );

	if( im->acq.im_actual_dt == 0 ) {
		im->acq.im_actual_dt = time( NULL );
	}
	nowstruct = localtime( (time_t *)&im->acq.im_actual_dt );
	strftime(nowstr, 11, "%m/%d/%y", nowstruct); /* 12/23/1944 Y2k */
	strncpy(theDSR->hist.exp_date,nowstr,10);
	strftime(nowstr, 11, "%H:%M:%S", nowstruct);
	strncpy(theDSR->hist.exp_time,nowstr,10);
	
	return error;
}

/* --> *****************************************************************************
*	<B>VarianFromIMAGE</B>
*   Populate all fields of a varian header from the IMAGE struct
*
*   OSErr  VarianFromIMAGE( IMAGE *im, dsr *theDSR )
*
*	INPUTS:
*		a pointer to a populated IMAGE struct
*		a pointer to a Varian Header;
*	OUTPUTS:
*		error
*	CHANGES:
*		contents of Varian Header
<!-- ******************************************************************************/
OSErr  VarianFromIMAGE( IMAGE *im, dsr *theDSR )
{
	OSErr error = noErr;

	if( im==nil || theDSR==nil ) {
		return INVALID_ADDRESS;
	}	

	return error;
}

/* --> ********************************************************************************
*   <B>WriteMGHImage</B>
*   Mostly as a debugging utility, write one image to a named output
*   file and create the needed headers.
*
*   At least on initial release, performs no datatype conversions and
*   only supports output to MGH file types of bshort, buchar and bfloat.
*
*   CALLING SEQUENCE:
*   WriteMGHImage(
*      void *imData,          -- pointer to image data location
*      char *filename,        -- filename string (base name only)
*      short xs,              -- image dimensions
*      short ys,
*      short zs,
*      int data_type )        -- data type of image data. Five types supported.
*
*   OUTPUTS:
*   Creates an image file of appropriate type to represent data, and
*   a corresponding .hdr file.  Currently files are converted as follows:
*      data_type        file type
*      T_USHORT   --->  bshort	* this may cause trouble. *
*      T_SHORT    --->  bshort
*      T_CHAR     --->  buchar
*      T_UCHAR    --->  buchar
*      T_FLOAT    --->  bfloat
*
*   There is no data range checking and signed integer types may not be displayed
*   or managed properly (This is a limitation of the MGH header system).
<!-- *********************************************************************************/
OSErr WriteMGHImage( void *imData, char *filename, short xs, short ys,
                       short zs, int data_type )
{
   OSErr error = noErr;
   FILE     *fp;
   char     buffer[128];

/* prepare a header file */
   sprintf( buffer, "%s.hdr", filename );
   error = ck_fopen( &fp, buffer,"w");
   RETURNONERROR;

   fprintf( fp, "%d %d %d %d", ys, xs, zs, macByteOrder() );
   fclose( fp );

/* determine the file name based on data_type */
   switch( data_type ) {
      case T_CHAR:
      case T_UCHAR:
         sprintf( buffer, "%s.buchar", filename );
         break;
      case T_SHORT:
      case T_USHORT:
         sprintf( buffer, "%s.bshort", filename );
         break;
      case T_FLOAT:
         sprintf( buffer, "%s.bfloat", filename );
         break;
      default:
         return( UNKNOWNTYPE);
         break;
   }
   
   error = ck_fopen( &fp, buffer, "wb" );
   RETURNONERROR;
   fwrite( imData, get_datasize( data_type ), xs*ys*zs, fp );
   fclose( fp );

   return error;
}

/* --> ******************************************************************
*  <B>InitializeIm</B>
*
*  void InitializeIm( IMAGE *im )
*
*  Empty an IMAGE struct by clearing all fields then adding a few defaults
<!-- ******************************************************************/
void InitializeIm( IMAGE *im )
{
	vclr( im, sizeof( IMAGE ), T_UCHAR );
	
	strcpy( im->MagicNumber, "ScSt" );
	im->im_header = sizeof( IMAGE );
	im->fileData_type = T_SHORT;	/* 6-18-02 */
	
//	strcpy( im->subj.hospname, "UCLA Brain Mapping Division" );
	
	im->dim.ColorTable = kGray256; /* from above defined types */
	
	im->acq.numecho = 1;
	im->acq.echonum = 1;
	im->dim.n_slices = 1;
	im->dim.timePts = 1;
	im->acq.image_type = MAGNITUDE;

/* initialize the max and min to be very small and very large, respectively. This way, they can be
	re-scaled in UC_ReadImage */
	im->theMaxf = (float)-LONG_MAX;	/* this is not OK... I should have the actual max and min for floats */
	im->theMinf = (float)LONG_MAX;
}


/* --> ************************************************************************
*               <B>UC_Readheader and UC_Readimage</B> tools
*
*    Original:  Jianxin Wang  2/11/96
*    Modification date:  9/17/96
*    Bug Fix: 10/4/96 - fix errors on T_FLOAT imports
*    Major re-write and conversion for Macintosh  - M. Cohen 4/97
*    fix header bug & (nearly a) complete rewrite. wls 10-11-1997
*
*    UC_Readimage and UC_Readheader allow the calling routine to import images
*    from any of the five standard data formats used at UCLA:
*
*      XXX.MR  -- GE Signa Genesis format
*      XXX_YYY.img (with XXX.irp header files)  -- ANMR APD2 format
*      XXX.img (with XXX.hdr header files)      -- Analyze 3D or 4D
*      XXX.buchar (and XXX.hdr)                 -- blocks of unsigned char
*      XXX.bshort (and XXX.hdr)                 -- blocks of unsigned shorts
*      XXX.bfloat (and XXX.hdr)                 -- blocks of floats
*
*    The latter three, inherited from MGH, use simple ascii headers with the
*    format:
*                       ysize, xsize, zsize, swap
*    Where ysize is the number of points/row, xsize is the number of points/column
*    and zsize is the number of images in the file. The swap byte is set to 0 for
*    images created locally and 1 for images on a machine that is byteswapped 
*    relative to the local machine.
*
*    UC_Readheader is used to fill out an IMAGE struct with needed dimensional information
*    UC_Readimage is used to read in an image. The image is returned to the pointer
*    passed as *image_data with a data type specified by im->data_type. Note that the
*    memory for the image must be malloc'd by the calling routine. Usually this means
*    that UC_Readimage is called first, and the needed memory is calculated from the
*    x, y and timePts fields of the IMAGE struct.
*
*    The crop flag is used on ANMR images when the FOV is oblong. This is especially
*    relevant to head images on that system.
*
*    If you extend this routine to include other image types, please let us
*    know at UCLA so that we can keep it up to date.
*
*    #ifdef UNIX
*    OSErr UC_Readheader( char *fn, IMAGE *im )
*    #endif
*
*    #ifdef MAC
*    OSErr UC_Readheader( FSSpec *imFileFS, IMAGE *im, Boolean RT )
*    #endif
*
<!-- ************************************************************************
*	This is a complete re-write of UC_ReadHeader that eliminates
*	a lot of hacks and is intended to have more clarity.
*
*	The calling function must supply an FSSpec for Macintosh applications
*	(this eliminates an extern global, which was clumsy).
*
*	First:	Determine the file type based on its name
*	Second:	Attempt to read the associated header and
*	        to parse its contents into the IMAGE struct
*	NOTE: Assume ANALYZE: If we can't find a ".hdr" file for an analyze file
*	      we look for a ".irp" header for an apd2 file.
*	      If none is found, we abort
*
*****************************************************************************/
#ifdef UNIX
OSErr UC_Readheader( char *fn, IMAGE *im )
#endif
#ifdef MAC
OSErr UC_Readheader( FSSpec *imFileFS, IMAGE *im, Boolean RT )
#endif
{
	char	filename[255];
	char	*p;
	char	*headerData = nil;
/* im.file_type is determined by indexing into this array. That index
	will then be used as an offset into the pix_size, imgHeader and imgTrailer */	
	char	*file_suffix[]={ "",
	                        ".bshort",
	                        ".bfloat",
	                        ".buchar",
	                        ".img",      /* ANMR/apd2 */
	                        ".img",      /* analyze */
	                        ".mr",       /*  signa  */
                            ".BRIK",     /* AFNI zrinka 08/20/02 */
	                        ".fnf"       /*  Varian, as I recall  */
	};    /* image types are defined by file_suffix */
	short	Pix_size[] = { 0,                 /* err */
			               sizeof( short),    /* bshort */
			               sizeof( float ),   /* bfloat */
			               sizeof( char ),    /* buchar */
			               sizeof( short ),   /* ANMR (reset in parsing routine) */
			               sizeof( short ),   /* Analyze - this is variable. */
                           sizeof( short ),	  /* AFNI - this too is probably variable: zrinka 08/20/02 */
                           sizeof( float ),   /* Varian? */
			               sizeof( short ) }; /* SIGNA */
	long	ImgHeader[7]  = {0, 0, 0, 0, ANMRHDRSIZE, 0, SIGHDRSIZE};
	long	ImgTrailer[7] = {0, 0, 0, 0, ANMRTRLSIZE, 0, SIGTRLSIZE};
	OSErr	error = noErr;
	long	fileSize;
#ifdef MAC
	char    fn[255];
	Str255	bufferStr;
	
//	P2CStr( imFileFS->name );	//zrinka 03/18/02 - P2CStr not used in Carbon
	CopyPascalStringToC( imFileFS->name, (char *)bufferStr );
	strcpy( (char *)imFileFS->name, (char *)bufferStr );
	
	strcpy( fn, (char *)imFileFS->name );
	
	//C2PStr( (char *)imFileFS->name );	//zrinka 03/18/02 - C2PStr not supported in Carbon
	CopyCStringToPascal( (char *)imFileFS->name, bufferStr ); /*convert p to c string; c string in 'buffer'*/
	strcpy( (char *)imFileFS->name, (char *)bufferStr );	/* copy buffer back so that the original name can be used below*/
#endif

	InitializeIm( im );	/* zero out the contents of the IMAGE struct */

	strcpy( filename, fn );	/* make a local copy of the file name */
	strncpy( im->ImageName, fn, 127 );	/* attach the original file name to im */
	
/* convert to lowercase (in case the user had caps lock on) */
	lowerString( filename );
/* figure out image type based upon fn, return with error if no match */
	im->file_type = LAST_TYPE-1;	/* initialize the file type and count down */
	p = strrchr( filename, '.' );
	while( strcmp( p, file_suffix[im->file_type ]) && im->file_type > No_TYPE ) {
		im->file_type--;
	}
	
	if(im->file_type == No_TYPE) {
		return( UNKNOWNTYPE );
	}

/* Signa files have embedded headers. The other supported types have a hdr named '.hdr' */
	switch( im->file_type )
	{
		case SIGNA:
			im->fileData_type = T_SHORT;	/* 6-18-02 was T_USHORT */
			headerData = (char *)ck_malloc( SIGHDRSIZE, "Buffer for Signa Header" );
#ifdef UNIX
			error = ReadLocalFileBytes( fn, headerData, (long)SIGHDRSIZE );
#endif
#ifdef MAC
			error = ReadLocalFileBytes( fn, headerData, (long)SIGHDRSIZE, imFileFS );
#endif
				RETURNONERROR;
			error = ParseSignaHeader( headerData, im );
				RETURNONERROR;

			error = memFree( (void **)&headerData, "Readheader - temp data storage" );

			im->data_type = im->fileData_type;

			return error;		/* note: processing stops here for Signa files */
		break;

		case BSHORT:
			/* im->fileData_type = T_USHORT; // changed 6-18-02 */
			im->fileData_type = T_SHORT;
		break;

		case BFLOAT:
			im->fileData_type = T_FLOAT;
		break;

		case BUCHAR:
			im->fileData_type = T_UCHAR;
		break;

		case ANALYZE:
		case EPI:
		break;
		
		case VARIAN:
// Needs work
		break;
		default:
			;// return INVALID_HDR;
	}
	RETURNONERROR;
	
	strcpy( filename, fn );
	sprintf( strrchr(filename, '.')+1, "hdr" );	/* add the header extension */
	
#ifdef UNIX
	headerData = ReadLocalFile( filename, &fileSize, &error );           /* allocates memory for headerData */
#endif
#ifdef MAC
	headerData = ReadLocalFile( filename, &fileSize, imFileFS, RT, &error ); /* allocates memory for headerData */
#endif
	switch( error ) {	/* the typical error will be on opening an apd2 file */
		case noErr:
			break;
#ifdef MAC
		case bdNamErr:
		case fnfErr:
#endif
		case FILE_NOT_FOUND:
		case IMG_HEADER_ERROR:
		case READ_OPEN_FAILURE:
			if( headerData ) {
				error = memFree( (void **)&headerData, "UC_Readheader: headerData" );
					RETURNONERROR;
			}
			if( im->file_type != ANALYZE ) {
				return MISSING_IMG_HEADER;
			}
			
			im->file_type = EPI;
			strcpy( filename, fn );
			
/* The last 8 characters of the file name are the '_'+image_number+'.img' */
			p = strstr( filename, ".img" );
			if (p != nil) {
				sprintf( p-4, ".irp" );	
			} else {
				p = strstr( filename, ".IMG" );	/* check for all caps */
				if (p != nil) {
					sprintf( p-4, ".IRP" );
				} else {
					return NILIMGPTR;
				}
			}
#ifdef UNIX
			headerData = ReadLocalFile( filename, &fileSize, &error );
#endif
#ifdef MAC
			headerData = ReadLocalFile( filename, &fileSize, imFileFS, RT, &error );
#endif
			if( error ) {
				memFree( (void **)&headerData, "RealLocalFile: headerData" );
				return COULD_NOT_OPEN_HEADER;
			}
			break;
		default:
			return COULD_NOT_OPEN_HEADER;
	}

/* Fill in known values - may be overridden by parsing routines */
   strncpy( im->RawHdr, filename, 127 );
   im->all_header = im->im_header = im->all_trailer = im->im_trailer = 0L; /* init. header sizes */
   im->im_header = ImgHeader[im->file_type];   /* size of pre-pended header */
   im->im_trailer = ImgTrailer[im->file_type]; /* size of per image trailer */
	
	switch( im->file_type )
	{
		case BUCHAR:
		case BSHORT:
		case BFLOAT:
/* without the accompanying header data, we can only guess */
			im->acq.im_datetime = im->acq.im_actual_dt = time( NULL );
			im->dim.BytesPix = Pix_size[im->file_type];
			error = ParseMGHHeader( headerData, im );
		break;

		case ANALYZE:
			if( fileSize != sizeof( dsr )) {
				error = HEADER_SIZE_MISMATCH;
			}
			if( error != noErr ) {
				memFree( (void **)&headerData, "Readheader: headerData" );
				return error;
			}
			error = ParseDSR( headerData, im, fileSize );
		break;

		case EPI:
			error = ParseANMRHeader( headerData, im );
		break;

		case VARIAN:
			error = COULD_NOT_OPEN_HEADER;
//			error = ParseVarian( headerData, ... );
		break;

		default:
			error = COULD_NOT_OPEN_HEADER;
	}

	if( im->file_type == EPI ) {	// ANMR files are handled differently because of interpolation option
		if( im->dim.flags & kNeedXInterp ) {
			im->dim.isoX = im->dim.x * 2;
		} else {
			im->dim.isoX = im->dim.x;
		}
		if( im->dim.flags & kNeedYInterp ) {
			im->dim.isoY = im->dim.y * 2;
		} else {
			im->dim.isoY = im->dim.y;
		}
	} else {
		im->dim.isoX = im->dim.x;
		im->dim.isoY = im->dim.y;
	}

// If the slice spacing is not valid, make something up!
	if( im->dim.slthick == 0 || im->dim.scanspacing == 0 ) {
		im->dim.slthick = 4;
		im->dim.scanspacing = 5;
	}

	im->data_type = im->fileData_type;

#undef DEBUGHEADER
#ifdef DEBUGHEADER
printf( "Address of im: %ld\n", im );
printf( "Addresses:\n&im->dim.isoX: %ld\t&im->dim.isoY: %ld\t&im->dim.BytesPix: %ld\n",
	&im->dim.isoX, &im->dim.isoY, &im->dim.BytesPix );

printf( "isoX:%hd\tisoY:%hd\tBytesPix:%hd\n\n",
	im->dim.isoX, im->dim.isoX, im->dim.BytesPix );
#endif

	return error;
}	/* UC_Readheader */

/* --> *************************************************************
*	<B>ParseXXXHeader</B>
*	inputs:	headerData populated with raw header info
*   outputs: IMAGE struct with valid contents based on headerData
*   headerData must contain the complete contents of the corresponding
*   file type (MGH, Signa, etc...)
*
*   OSErr ParseSignaHeader( void *headerData, IMAGE *im )
*   OSErr ParseMGHHeader  ( void *headerData, IMAGE *im )
*   OSErr ParseANMRHeader ( void *headerData, IMAGE *im )
*   OSErr ParseDSR        ( char *headerData, IMAGE *im, short fileSize )
*
<!-- *************************************************************/
OSErr ParseSignaHeader( void *headerData, IMAGE *im )
{
	OSErr	error = noErr;
	char	myString[256];
	char	*Hdrp;	/* pointer to the image header base address */
	ImgHdr	*myHdr;
//	ImgHst	*Hist;
	int     TmpInt;
	short   TmpShort;
	char    *ImPtr, *HistPtr, *SuPtr, *ExPtr, *SerPtr;
	int     imageIdentOffset, SuiteOffset, ExamOffset, SeriesOffset;
	int     ImageOffset, HistOffset, UserDataOffset;
	char	*userdata;
	char	*imageIdent;
	Boolean swap = pcByteOrder();
		
	if( headerData == nil ) {
		return( NIL_PTR );
	}

/* The Signa format that we use is a single image per file with 7904 header. 
	Must not have both all_header && im_header */

/* check for valid magic label */
	strncpy( myString, (char *)headerData, 4 );
	myString[4] = '\0';
	if( strcmp( myString, "IMGF" )) {
		return INVALID_GENESIS_HDR;
	}

	myHdr = (ImgHdr *)headerData;
	Hdrp = (char *)myHdr;

/* in the genesis header, the l_XX variables indicate the length of a
	sub-header, and the p_XX variables are the offset (from the start
	of the header) to that sub-header. The offsets are 'long's
*/

	error = MoveSwap( &myHdr->p_ID,     &imageIdentOffset, swap, T_INT );
		ILError( error, "imageIdentOffset" );
	error = MoveSwap( &myHdr->p_histo,  &HistOffset, swap, T_INT );
		ILError( error, "HistOffset" );
	error = MoveSwap( &myHdr->p_suite,  &SuiteOffset, swap, T_INT );
		ILError( error, "SuiteOffset" );
	error = MoveSwap( &myHdr->p_exam,   &ExamOffset, swap, T_INT );
		ILError( error, "ExamOffset" );
	error = MoveSwap( &myHdr->p_series, &SeriesOffset, swap, T_INT );
		ILError( error, "SeriesOffset" );
	error = MoveSwap( &myHdr->p_image,  &ImageOffset, swap, T_INT );
		ILError( error, "ImageOffset" );
	error = MoveSwap( &myHdr->p_user,  &UserDataOffset, swap, T_INT );
		ILError( error, "UserDataOffset" );

	if( myHdr->l_ID ) {
		imageIdent = (char *)(Hdrp + (long)imageIdentOffset );
	}
	if( myHdr->l_histo ) {
		HistPtr = Hdrp + (long)HistOffset;
	}
	if( myHdr->l_suite ) {
		SuPtr = Hdrp + (long)SuiteOffset;
	}
	if( myHdr->l_exam ) {
		ExPtr = Hdrp + (long)ExamOffset;
	}
	if( myHdr->l_series ) {
		SerPtr = Hdrp + (long)SeriesOffset;
	}
	if( myHdr->l_image ) {
		ImPtr = Hdrp + (long)ImageOffset;
	}
	if( myHdr->l_user ) {
		userdata = Hdrp + (long)UserDataOffset;
	}

/* general header variables */
	MoveSwap( &myHdr->img_hdr_length,      &im->im_header,       swap, T_INT);
	MoveSwap( &myHdr->ImgWidth,            &TmpInt,              swap, T_INT);
		im->dim.x = (short)TmpInt;
	MoveSwap( &myHdr->ImgHeight,           &TmpInt,              swap, T_INT);
		im->dim.y = (short)TmpInt;

/* exam variables */
	im->subj.male = (Boolean) *( ExPtr + _EX_patsex );
	MoveSwap( ExPtr + _EX_patage,        &im->subj.patage,     swap, T_INT );	// assume age in days
	MoveSwap( ExPtr + _EX_patweight,     &im->subj.patweight,  swap, T_INT );
	MoveSwap( ExPtr + _EX_hospname,         &im->acq.im_exno,     swap, T_SHORT );
	if( strcmp( ExPtr + _EX_hospname, "UCLA Brain Mapping" )) {
		MoveSwap( ExPtr + _EX_magstrength,  &im->acq.fieldStrength,swap, T_INT );
	} else {
		im->acq.fieldStrength = 30000;	// our scanner does not identify itself properly
	}

/* series variables */
	MoveSwap( SerPtr + _SE_se_contrast, &TmpShort, swap, T_SHORT );
	if( TmpShort ) {
		im->acq.contrast = true;
	} else {
		im->acq.contrast = false;
	}
	MoveSwap( SerPtr + _SE_se_sortorder,&im->acq.se_sortorder, swap, T_SHORT );
	
/* image data variables */
	MoveSwap( ImPtr + _MR_im_exno,      &im->acq.im_exno,      swap, T_SHORT );
	MoveSwap( ImPtr + _MR_im_seno,      &im->acq.im_seno,      swap, T_SHORT );
	MoveSwap( ImPtr + _MR_im_no,        &im->acq.im_no,        swap, T_SHORT );
	MoveSwap( ImPtr + _MR_im_datetime,  &im->acq.im_datetime,  swap, T_INT );
	MoveSwap( ImPtr + _MR_im_actual_dt, &im->acq.im_actual_dt, swap, T_INT );
	MoveSwap( ImPtr + _MR_sctime,       &im->acq.sctime,       swap, T_FLOAT );
	MoveSwap( ImPtr + _MR_tr,           &im->acq.tr,           swap, T_INT );
	MoveSwap( ImPtr + _MR_ti,           &im->acq.ti,           swap, T_INT );
	MoveSwap( ImPtr + _MR_te,           &im->acq.te,           swap, T_INT );
	MoveSwap( ImPtr + _MR_mr_flip,      &im->acq.mr_flip,      swap, T_SHORT );
	MoveSwap( ImPtr + _MR_numecho,      &im->acq.numecho,      swap, T_SHORT );
	MoveSwap( ImPtr + _MR_echonum,      &im->acq.echonum,      swap, T_SHORT );
	MoveSwap( ImPtr + _MR_tbldlta,      &im->acq.tbldlta,      swap, T_FLOAT );
	MoveSwap( ImPtr + _MR_nex,          &im->acq.nex,          swap, T_FLOAT );
	MoveSwap( SerPtr + _SE_se_nacq,     &im->acq.se_nacq,      swap, T_SHORT );

	MoveSwap( ImPtr + _MR_contig, &TmpShort, swap, T_SHORT );
	if( TmpShort ) {
		im->acq.contig = true;
	} else {
		im->acq.contig = false;
	} 
	MoveSwap( ImPtr + _MR_hrtrate,      &im->acq.hrtrate,      swap, T_SHORT );
	MoveSwap( ImPtr + _MR_xmtgain,      &im->acq.xmtgain,      swap, T_SHORT );
	MoveSwap( ImPtr + _MR_prescan_r1,   &im->acq.rec1,         swap, T_SHORT );
	MoveSwap( ImPtr + _MR_prescan_r2,   &im->acq.rec2,         swap, T_SHORT );
	MoveSwap( ImPtr + _MR_vbw,          &im->acq.vbw,          swap, T_FLOAT );
	MoveSwap( ImPtr + _MR_slquant,      &im->dim.n_acquired_slices,     swap, T_SHORT );
	MoveSwap( ImPtr + _MR_echo_trn_len, &im->acq.echo_trn_len, swap, T_SHORT );
	
	MoveSwap( ImPtr + _MR_scanspacing,  &im->dim.scanspacing,  swap, T_FLOAT );
	MoveSwap( ImPtr + _MR_slthick,      &im->dim.slthick,      swap, T_FLOAT );
	MoveSwap( ImPtr + _MR_dfov,         &im->dim.dfov,         swap, T_FLOAT );
	MoveSwap( ImPtr + _MR_dfov_rect,    &im->dim.dfov_rect,    swap, T_FLOAT );
	MoveSwap( ImPtr + _MR_imatrix_X,    &im->acq.nReadPoints,  swap, T_SHORT );
	MoveSwap( ImPtr + _MR_imatrix_Y,    &im->acq.nPhaseLines,  swap, T_SHORT );
	MoveSwap( ImPtr + _MR_pixsize_X,    &im->dim.dpixsize_X,   swap, T_FLOAT );
	MoveSwap( ImPtr + _MR_pixsize_Y,    &im->dim.dpixsize_Y,   swap, T_FLOAT );

	/* RMD 1/7/02 adding RAS coords */
	MoveSwap( ImPtr + _MR_tlhc_R,       &im->loc.tlhc[0],       swap, T_FLOAT );
	MoveSwap( ImPtr + _MR_tlhc_A,       &im->loc.tlhc[1],       swap, T_FLOAT );
	MoveSwap( ImPtr + _MR_tlhc_S,       &im->loc.tlhc[2],       swap, T_FLOAT );
	MoveSwap( ImPtr + _MR_trhc_R,       &im->loc.trhc[0],       swap, T_FLOAT );
	MoveSwap( ImPtr + _MR_trhc_A,       &im->loc.trhc[1],       swap, T_FLOAT );
	MoveSwap( ImPtr + _MR_trhc_S,       &im->loc.trhc[2],       swap, T_FLOAT );
	MoveSwap( ImPtr + _MR_brhc_R,       &im->loc.brhc[0],       swap, T_FLOAT );
	MoveSwap( ImPtr + _MR_brhc_A,       &im->loc.brhc[1],       swap, T_FLOAT );
	MoveSwap( ImPtr + _MR_brhc_S,       &im->loc.brhc[2],       swap, T_FLOAT );
	MoveSwap( ImPtr + _MR_norm_R,       &im->loc.imgNorm[0],    swap, T_FLOAT );
	MoveSwap( ImPtr + _MR_norm_A,       &im->loc.imgNorm[1],    swap, T_FLOAT );
	MoveSwap( ImPtr + _MR_norm_S,       &im->loc.imgNorm[2],    swap, T_FLOAT );
    /* RMD end */

	im->dim.acqpixsize_X = im->dim.dpixsize_X;
	im->dim.acqpixsize_Y = im->dim.dpixsize_Y;
	
	MoveSwap( HistPtr + _HS_hs_min,     &TmpShort,             swap, T_SHORT );
		im->theMinf = 0.0; // could be TmpShort
	MoveSwap( HistPtr + _HS_hs_max,     &TmpShort,             swap, T_SHORT );
		im->theMaxf = (float)TmpShort;

/* strings */
	if( strlen( ExPtr + _EX_patname )) {
		strncpy( im->subj.patname,  ExPtr + _EX_patname, 24 );
	} else {
		sprintf( im->subj.patname,  "anonymous" );
	}
	strncpy( im->subj.patid,      ExPtr + _EX_patid, 13 );
	strncpy( im->subj.reqnum,     ExPtr + _EX_reqnum, 12 );
	strncpy( im->subj.descr,      ExPtr + _EX_ex_desc, 23 );
	strncpy( im->subj.refphy,     ExPtr + _EX_refphy, 32 );
	strncpy( im->subj.diagrad,    ExPtr + _EX_diagrad, 32 );
	strncpy( im->subj.ex_op,      ExPtr + _EX_op, 4 );
	strncpy( im->subj.hospname,   ExPtr + _EX_hospname, 32 );
	strncpy( im->subj.sysID,      ExPtr + _EX_ex_sysid, 9 );

	strncpy( im->acq.prtcl,       SerPtr + _SE_prtcl, 24 );
	
	strncpy( im->acq.contrastIV,  ImPtr + _MR_contrastIV, 16 );
	strncpy( im->acq.contrastOral,ImPtr + _MR_contrastOral, 16 );
	strncpy( im->acq.psdname,     ImPtr + _MR_psdname, 32 );
	strncpy( im->acq.psd_iname,   ImPtr + _MR_psd_iname, 12 );

/* stuff we knew already */
	im->dim.isoX = im->dim.x;	// Signa images are always isotropic
	im->dim.isoY = im->dim.y;
	im->fileData_type  = T_USHORT;
	im->dim.timePts    = 1L;
	im->dim.n_slices = 1; 	/* as far as we know, genesis files are all single slice */

	SET( im->dim.flags, kmacByteOrder );

	im->dim.BytesPix = sizeof( short );	/* GE images are always shorts (?) */

/* // Useful for debugging
printf( "the offset to imageIdent should be: %ld\n", (long)imageIdentOffset );
printf( "the offset to Hist should be: %ld\n", (long)HistOffset );
printf( "the offset to Su should be: %ld\n", (long)SuiteOffset );
printf( "the offset to Ex should be: %ld\n", (long)ExamOffset );
printf( "the offset to Ser should be: %ld\n", (long)SeriesOffset );
printf( "the offset to Img should be: %ld\n\n", (long)ImageOffset );
printf( "ImgWidth: %d\n", im->dim.x );
printf( "im_header: %d\n", im->im_header );
printf( "ImgHeight: %d\n", im->dim.y );
printf( "hosp: %s\n", im->subj.hospname );
printf( "field strength: %d\n\n", im->acq.fieldStrength);
printf( "Subject name: %s\n", im->subj.patname );
printf( "patage: %hd\n", im->subj.patage );
printf( "male: %s\n", YorN(im->subj.male) );
printf( "PatID: %s\n", ExPtr + patidOffset );
printf( "weight: %d\n", (int)(im->subj.patweight*.0022) );
printf( "description: %s\n", im->subj.descr );
printf( "im_exno: %hd\n", im->acq.im_exno );
printf( "seno: %hd\n", im->acq.im_seno);
printf( "im_no: %hd\n", im->acq.im_no );
printf( "tr: %d\n", im->acq.tr );
printf( "te: %d\n", im->acq.te );
printf( "flip: %d\n", im->acq.mr_flip );
printf( "slthick: %f\n", im->dim.slthick );
printf( "dfov: %f\tdfov_rect: %f\n", im->dim.dfov, im->dim.dfov_rect );
printf( "pixsize_X: %f\tpixsize_Y: %f\n", im->dim.dpixsize_X, im->dim.dpixsize_Y );
printf( "xmtgain: %hd\n", im->acq.xmtgain );
printf( "minF: %f\tmaxF: %f\n", im->theMinf, im->theMaxf );
*/
	return error;
}

OSErr ParseMGHHeader( char *headerData, IMAGE *im )
{
	OSErr	error = noErr;
	short	macByteOrder;
	short	count;
	int   timePts;
	int   x, y;
	
/* these dimensions are added to handle the degenerate MGH header type */
	im->dim.dfov = 200.0;        /* Display field of view - X (mm) */
	im->dim.dfov_rect = 200.0;   /* Display field of view - Y (if different) */
	im->dim.dpixsize_X = 3.125;  /* Display pixel size (mm) - X */
	im->dim.dpixsize_Y = 3.125;  /* Display pixel size (mm) - Y */
	im->dim.acqfov = 200.0;      /* Acquired FOV (mm) - X */
	im->dim.acqfov_rect = 200.0; /* Acquired FOV (mm) - Y (if different) */
	im->dim.acqpixsize_X = 3.125;    /* Acquired pixel size (mm) - X */
	im->dim.acqpixsize_Y = 3.125;    /* Acquired pixel size (mm) - Y */
	im->dim.slthick = 4.0;           /* slice thickness */
	im->acq.tr = 2500000;            /* random guess at tr */

	count = sscanf( headerData, "%d%d%d%hd", &y, &x, &timePts, &macByteOrder );

    if( count!=4 ) {
    	error = HEADER_SIZE_MISMATCH;
    }
    RETURNONERROR;
             
    im->dim.y = y;
    im->dim.x = x;
    im->dim.timePts = timePts;
    
	if( macByteOrder ) {
		SET( im->dim.flags, kmacByteOrder );
	}

	im->dim.n_slices=1;
	
	return error;
}

OSErr ParseANMRHeader( char *headerData, IMAGE *im )
{
	OSErr error = noErr;
	float   acqfovX, acqfovY;	/* acquired field of view in X and Y */
	short   pixMult;        /* change in number of pixels with cropping/interpolation */
	char	buffer[128];	/* just to store nothing */

	if( sscanf( headerData, "%s%hd%s%d%s%s%s%hd%s%hd%s%s%s%s%s%s%s%s%s%f%s%f",
	             buffer,                  /* skip unneeded info */
	             &im->dim.n_slices,
	             buffer,
	             &im->dim.timePts,
	             buffer, buffer, buffer,  /* skip unneeded info */
	             &im->acq.nReadPoints,
	             buffer,
	             &im->acq.nPhaseLines,
	             buffer, buffer, buffer, buffer,
	             buffer, buffer, buffer, buffer, buffer,
	             &acqfovX,
	             buffer,
	             &acqfovY ) != 22 ) {
		return( IMG_HEADER_ERROR );
	}

	im->dim.x            = im->dim.isoX      = im->acq.nReadPoints;
	im->dim.y            = im->dim.isoY      = im->acq.nPhaseLines;
	im->dim.acqfov       = im->dim.dfov      = acqfovX;
	im->dim.acqfov_rect  = im->dim.dfov_rect = acqfovY;
	im->dim.acqpixsize_X = acqfovX / im->acq.nReadPoints;
	im->dim.acqpixsize_Y = acqfovY / im->acq.nPhaseLines;

/* Calculate the need for interpolation */
	if( acqfovX == acqfovY ) {  /* square FOV ... */
		if( im->dim.x < im->dim.y ) {   /* ... but less x pixels than y pixels */
			im->dim.isoX = im->dim.x = im->dim.x * 2;
		} else if (im->dim.x > im->dim.y ) { /* ... but less y pixels than x pixels */
			im->dim.isoY = im->dim.y = im->dim.y * 2;
		}
	} else if( acqfovX > acqfovY ) { /* image wider than tall */
		if( !(im->dim.x > im->dim.y )) { /* ... but same number of x and y pixels */
/* do nothing */
		}
	} else if( acqfovX < acqfovY ) {	/* image taller than wide (default) */
		if( !(im->dim.x < im->dim.y )) {  /* ... but same number of x and y pixels */
/* do nothing */
		}
	}
		
/* Calculate need for cropping */
/* Note that im.dim.x and im.dim.y are the number of "valid" pixels */
	if( acqfovX > acqfovY )	/* Landscape style */
	{
		SET(im->dim.flags, kNeedXCrop);
		pixMult = (short)( acqfovX / acqfovY );
		im->dim.x    = im->dim.x / pixMult;
		im->dim.isoX = im->dim.isoX / pixMult;
		im->dim.dfov = acqfovX / pixMult;
	}
	else if( acqfovY > acqfovX )	/* Portrait style */
	{
		SET(im->dim.flags, kNeedYCrop);
		pixMult = (short)( acqfovY / acqfovX );
		im->dim.y         = im->dim.y / pixMult;
		im->dim.isoY      = im->dim.isoY / pixMult;
		im->dim.dfov_rect = acqfovY / pixMult;
	}

/* calculate pixel size based on matrix, fov and interpolation */
	im->dim.dpixsize_X = im->dim.dfov / im->dim.isoX;
	im->dim.dpixsize_Y = im->dim.dfov_rect / im->dim.isoY;

	SET( im->dim.flags, kmacByteOrder );
		
/* Fill in other known values. */
	im->dim.BytesPix = sizeof( short );	/* apd2 images are always short */
	return error;
}

OSErr ParseDSR( char *headerData, IMAGE *im, short fileSize )
{
	dsr	        *DSR;	/* pointer to analyze header struct */
	struct tm   now;
	OSErr       error = noErr;
	char        myString[5];

	DSR = (dsr *)headerData;	/* point to the header data */

// Make sure that this data is valid and in the correct byte order
// This assumes that the byte order of the header and data are consistent...
	if( fileSize != sizeof( dsr ) && (im->file_type == ANALYZE) ) {
		return INVALID_HDR;
	}
	
	if( DSR->hk.sizeof_hdr == sizeof( dsr ) ) {   /* Data and cpu are the same */
		if( macByteOrder() ) {      /* so set the flags appropriately */
			SET( im->dim.flags, kmacByteOrder);
		} else {
			CLEAR( im->dim.flags, kmacByteOrder);
		}
	
// data did not look right: must be less than 15 if the byte order is same in data and cpu
	} else {
		swapHdrBytes( headerData );
		if( pcByteOrder() ) {
			SET( im->dim.flags, kmacByteOrder);
		} else {
			CLEAR( im->dim.flags, kmacByteOrder);
		}
	}
	
	if( DSR->hk.sizeof_hdr != sizeof( dsr ) ) {	/* f*cked up header - get out of here */
		return INVALID_HDR;	/* Incorrect analyze header format */
	}

// handle dimensions
	switch( DSR->dime.datatype )
	{
		case DT_UNSIGNED_CHAR:
			im->fileData_type = T_UCHAR;
			break;
		case DT_SIGNED_SHORT:	// mysteriously does not support unsigned short
		case DT_UNKNOWN:
			im->fileData_type = T_SHORT;	/* changed from T_USHORT 6-18-02 */
			break;
		case DT_SIGNED_INT:
			im->fileData_type = T_INT;
			break;
		case DT_FLOAT:
			im->fileData_type = T_FLOAT;
			break;
		case DT_DOUBLE:
			im->fileData_type = T_DOUBLE;
			break;
		case DT_COMPLEX:
			im->fileData_type = T_COMPLEX;
			break;
		default: /* wls: avoid the memory leak! */
			memFree( (void **)&headerData, "UC_Readheader: Header Data" );
			return( UNKNOWNTYPE );
	}

	im->dim.BytesPix     = get_datasize( im->fileData_type );
	im->dim.x            = (short)DSR->dime.dim[1];
	im->dim.y            = (short)DSR->dime.dim[2];

// Analyze header does not distinghuish acquired vs. file number of slices
	im->dim.n_slices     = im->dim.n_acquired_slices = (short)DSR->dime.dim[3];
	im->dim.timePts      = (short)DSR->dime.dim[4];
	im->dim.dpixsize_X   = abs( DSR->dime.pixdim[1] );
	im->dim.dpixsize_Y   = abs( DSR->dime.pixdim[2] );

// Analyze header doesn't distinguish acquisition and display pix size.
	im->dim.acqpixsize_X = im->dim.dpixsize_X;
	im->dim.acqpixsize_Y = im->dim.dpixsize_Y;
	im->dim.acqfov       = im->dim.dfov      = im->dim.dpixsize_X * im->dim.x;
	im->dim.acqfov_rect  = im->dim.dfov_rect = im->dim.dpixsize_Y * im->dim.y;
	
// Analyze header does not distinguish slice thickness and gap.
	im->dim.slthick      = DSR->dime.pixdim[3];
	im->dim.scanspacing  = DSR->dime.pixdim[3];

// FSL and SPM? poke the scan tr into pixdim[4]
	if( im->acq.tr == 0 ) {
		im->acq.tr = 2.5;	// just to make a valid number
	} else {
		im->acq.tr           = 1e6 * DSR->dime.pixdim[4];	// convert to microseconds
	}

	im->dim.flags |= DSR->dime.flags;	/* Copy all flags that are set */
	if( im->dim.flags & kNeedXInterp ) {
		im->dim.isoX = im->dim.x * 2;
	} else {
		im->dim.isoX = im->dim.x;
	}
	if( im->dim.flags & kNeedYInterp ) {
		im->dim.isoY = im->dim.y * 2;
	} else {
		im->dim.isoY = im->dim.y;
	}

	im->theMaxf  = (float)DSR->dime.glmax;
	im->theMinf  = (float)DSR->dime.glmin;

	if( !im->dim.x  ||  !im->dim.y || !im->dim.n_slices || !im->dim.timePts ) {
		return INVALID_HDR;
	}

// Handle demographics
	strncpy( im->subj.descr, DSR->hist.descrip, 80 );
	strncpy( im->subj.patid, DSR->hist.patient_id,  16 );

//Analyze header does not include patient name and other patient data
	sprintf( im->subj.patname, "" );	
	sprintf( im->subj.reqnum, "" );
	sprintf( im->subj.HumanStudiesID, "" );
	sprintf( im->subj.ResearchProtocol, "" );
	sprintf( im->subj.refphy, "" );
	sprintf( im->subj.diagrad, "" );
	sprintf( im->subj.ex_op, "" );
	sprintf( im->subj.hospname, "" );
	sprintf( im->subj.sysID, "" );
	sprintf( im->subj.aux_file, "" );

// this is pretty unlikely to work with randomly imported images...
	now.tm_sec  = atoi( &DSR->hist.exp_time[6] );
	now.tm_min  = atoi( &DSR->hist.exp_time[3] );
	now.tm_hour = atoi( &DSR->hist.exp_time[0] );
	now.tm_mon  = atoi( &DSR->hist.exp_date[0] );
	now.tm_mday = atoi( &DSR->hist.exp_date[3] );
	strncpy( myString, &DSR->hist.exp_date[6], 4 );	// only the first four digits
	myString[4] = 0;	// terminate the string, just to be safe
	now.tm_year = atoi( &DSR->hist.exp_date[6] );

	im->acq.im_datetime = im->acq.im_actual_dt = mktime( &now );

// other stuff that we know
	im->file_type = ANALYZE;
	im->all_header = 0L;
	im->all_trailer = 0L;
	im->im_header = 0L;
	im->im_trailer = 0L;

	return error;
}

OSErr ParseVarian( char *headerData, IMAGE *im )
{
	OSErr error = noErr;

	if( headerData==nil || im==nil ) {
		return INVALID_ADDRESS;
	}	

// map each field of the Varian header to an associated field in im
	return error;
}

// ReadLocalFileBytes
/* --> *************************************************************
*	<B>ReadLocalFileBytes</B>
*	This routine must differ greatly for Mac and unix, because of the
*	differences in file handling for the two machines.
*
*	Mac version reads the named file located in the directory spec'd
*	by the accompanying FSSpec (refFS) which points to a DIFFERENT file.
*
*   #ifdef UNIX
*   OSErr ReadLocalFileBytes( char *fn, char *data, long numBytes )
*
*   #ifdef MAC
*   OSErr ReadLocalFileBytes( char *fn, char *data, long numBytes, FSSpec *refFS )
*
<!-- *************************************************************/
#ifdef UNIX
OSErr ReadLocalFileBytes( char *fn, char *data, long numBytes )
#endif
#ifdef MAC
OSErr ReadLocalFileBytes( char *fn, char *data, long numBytes, FSSpec *refFS )
#endif
{
	OSErr        error = noErr;
#ifdef UNIX
	FILE         *fp;
#endif
#ifdef MAC
	short        fileID;
	char         buffer[ 128 ];
	FSSpec       ThisFS;
#endif

#ifdef UNIX
	error = ck_fopen( &fp, fn, "rb" );
	if( error ) return IMG_HEADER_ERROR;

	if( fread( data, sizeof( char ), numBytes, fp ) != numBytes ) {
		return( IMG_HEADER_ERROR );
	}
	fclose( fp );
	return error;
#endif

#ifdef MAC
	Str255	bufferStr;
	
	strcpy( buffer, fn );	/* we will alter this with C2PStr */
	
	if (uc_dbg) {
		sprintf( gMessageStr,"ReadLocalFileBytes: read file %s\n", fn );
		LogMessage( gMessageStr );
	}

// Create new FSSpec using the same volume as the image file
	//error = FSMakeFSSpec( refFS->vRefNum, refFS->parID, C2PStr( fn ), &ThisFS );	//zrinka 03/18/02 - C2PStr not supported in Carbon
	CopyCStringToPascal( fn, bufferStr ); /*convert p to c string; c string in 'buffer'*/
	strcpy( fn, (char *)bufferStr );	/* copy buffer back so that the original name can be used below*/
	error = FSMakeFSSpec( refFS->vRefNum, refFS->parID, (ConstStr255Param)fn, &ThisFS );
	
	RETURNONERROR;

// open the file
	error = FSpOpenDF ( &ThisFS, fsRdPerm, &fileID );
	if( error ) {
		return COULD_NOT_OPEN_HEADER;
	}

	error = SetFPos(fileID, fsFromStart, 0);
	if (error) {
		return error;
	}

/* read the data */
	error = FSRead(fileID, &numBytes, data);

	FSClose ( fileID );
	
	return error;
#endif
}	/* ReadLocalFileBytes */

/* --> ****************************************************************************
*  <B>UC_Readimage</B>
*
*  This is the combined MAC/UNIX version of UC_Readimage.
*  Much thanks to Jianxin Wang for his inspiration.
*
*  UC_Readimage2D is just a calling shell for UC_Readimage
*
*  OSErr UC_Readimage(
*	                FILE *fp,          OPEN input image file
*                   IMAGE *im,         image struct declaration
*                                      im->data_type is the output data type needed
*                   void *image_data,  pointer to output image data, cast to char*
*                   int image_slice,   image slice an image file
*                   int TimePt,        image time point requested
*
<!-- ****************************************************************************/
#ifdef UNIX
OSErr UC_Readimage( FILE *fp, IMAGE *im, void *image_data, int image_slice, int TimePt )
#endif
#ifdef MAC
OSErr UC_Readimage( short fp, IMAGE *im, void *image_data, int image_slice, int TimePt )
#endif
{
	OSErr     error = noErr;
#ifdef no_log_file
	const short readIm_dbg = 0; /* must be quiet */
#else
	const short readIm_dbg = 0;                 /* dbg for UC_Read_Image */
#endif
	unsigned char swap;
	int     x_skip, y_skip;
	int     fileXS;                  /* size of a line in input file */
	int     fileYS;                  /* number of lines/image in input file */
	short     fileBytesPix;            /* bytes/pixel within the file */
	void      *Buffer=nil;             /* memory for cropping of images */
	long      imSize;
	long      uncropdImageSize;        /* image size before x crop */
	long      fileImSize;
	long      count;
	long      seek;
	float     imax, imin;              /* min and max signal intensity in this file */
	short     ConversionResult = kLossLess;

	if (readIm_dbg) {
		sprintf( gMessageStr,"UC Read Image %d,%d\n",image_slice,TimePt);
		LogMessage( gMessageStr );
	}

/* calculate cropping dimensions
Since im.dim.x and im.dim.y are the number of VALID pixels, the number of
pixels to crop is equal to the file dimensions in x and y less im.dim.x and
im.dim.y respectively. Here, though, we assume that if cropping is needed,
the images have a 2 by 1 aspect ratio. The x_skip and y_skip represent the
number of pixels to ignore on the outside eges of the images.
*/
	x_skip = y_skip = 0;

	fileBytesPix = get_datasize( im->fileData_type );
	if( im->file_type == EPI ) {	// this nonsense is needed only for ANMR type files
		if( im->dim.flags & kNeedXCrop ) {
			x_skip = im->dim.x/2;
		}
		if( im->dim.flags & kNeedYCrop ) {
			y_skip = im->dim.y/2;
		}
	}
	
/* check that computer and data have the same endian... */
	{
		Boolean	macOrderFile;
		
		if( im->dim.flags & kmacByteOrder ) {
			macOrderFile = true ;
		} else {
			macOrderFile = false;
		}
		
		if( macByteOrder() != macOrderFile ) {
			swap = true;
		} else {
			swap = false;
		}
	}

	fileXS = x_skip + im->dim.x + x_skip;
	fileYS = y_skip + im->dim.y + y_skip;

	imSize = fileXS * fileYS * fileBytesPix;
	fileImSize = im->im_trailer + im->im_header + imSize;
	uncropdImageSize = im->dim.y * (im->dim.x + x_skip*2) * fileBytesPix;

//printf( "im->dim.y: %d\tim->dim.x: %d\tBytesPix: %d\tuncropdImageSize: %ld\n", im->dim.y,im->dim.x,im->dim.BytesPix,uncropdImageSize );
// data are read into this buffer first
	Buffer = (char *)ck_malloc( uncropdImageSize, "UCReadimage--data buffer" );
	if( Buffer == nil ) {
		return MEMALLOCERROR;
	}

/* for each image type we calculate where the image starts and seek to it. */
	seek = im->all_header
	     + im->im_header + y_skip * fileXS * fileBytesPix;   // top of image

/* Seek to desired image, but depends on the file order */
	if( im->file_type == EPI ) {
		seek += (image_slice*im->dim.timePts + TimePt) * fileImSize;         /* images to skip */
	} else {
		seek += (im->dim.n_slices*TimePt + image_slice) * fileImSize;        /* images to skip */
	}

/* Read the image, plus the unwanted x_skip'd bytes (i.e. too much) and don't */
#ifdef UNIX
/* fseek() returns zero if it is successful and returns a nonzero value if it fails. */
	if( fseek( fp, seek, SEEK_SET ) ) {
		return IMG_FILE_ERROR;
	}
	
	count = fread( Buffer, sizeof( char ), uncropdImageSize, fp);
	if( count != uncropdImageSize ) {
		return BEYOND_EOF;
	}
#endif

#ifdef MAC
	error = SetFPos(fp, fsFromStart, seek);
		RETURNONERROR;
	error = FSRead (fp, &uncropdImageSize, Buffer);
		RETURNONERROR;
#endif	/* MAC */

	if( x_skip != 0 ) {
		CropImageSides( Buffer, fileXS, im->dim.y, x_skip, x_skip, fileBytesPix);
	}

/* swap data if necessary */
	if ( swap ) {
		switch( im->fileData_type ) {
			case T_FLOAT: 
			case T_USHORT:
			case T_SHORT:
				error = vbyteswap( Buffer, im->dim.x*im->dim.y, im->fileData_type );
					RETURNONERROR;
			break;

			case T_UCHAR:
			break;

			case T_COMPLEX:
				error = vbyteswap( Buffer, im->dim.x*im->dim.y * 2, im->fileData_type );
					RETURNONERROR;		
			break;

			break;
				default:
				return ( UNKNOWNTYPE ); /* we were not returning an error indication, may as well exit */
		}
	}

	/* perform type conversion, if needed, otherwise just copy to the output */
	if( im->data_type != im->fileData_type ) {

		type_convert( Buffer, im->fileData_type, image_data, im->data_type, im->dim.x*im->dim.y, 
		                  &imax, &imin, &ConversionResult);
	} else {
		error = vmov( Buffer, 1, image_data, 1, im->dim.x*im->dim.y, im->data_type );
			RETURNONERROR;
	}
	memFree( &Buffer, "Read image buffer" );

	error = vfminmax( image_data, im->dim.x * im->dim.y, &imax, &imin, im->data_type );
	RETURNONERROR;

	im->theMaxf = imax;
	im->theMinf = imin;

	return error;         /* initialized to noErr */
}	/* UC_Readimage */

/* --> ************************************************************************
*	<B>ReadLocalFile</B>
*	Creates memory for data and reads the complete contents of the file into it.
*
*	Mac version reads the named file located in the directory spec'd
*	by the accompanying FSSpec (refFS) which points to a DIFFERENT file.
*
*	Routine returns size of file actually read. If the calling routine doesn't
*	like it, error handling belongs to the calling routine.
*
*   #ifdef UNIX
*   char *ReadLocalFile( char *fn, long *fileSize, OSErr *ERR )
*
*   #ifdef MAC
*   char *ReadLocalFile( char *fn, long *fileSize, FSSpec *refFS, Boolean RT, OSErr *ERR )
<!-- *************************************************************************/
#ifdef UNIX
char *ReadLocalFile( char *fn, long *fileSize, OSErr *ERR )
#endif
#ifdef MAC
char *ReadLocalFile( char *fn, long *fileSize, FSSpec *refFS, Boolean RT, OSErr *ERR )
#endif
{
	char       buffer[128];
	char       *data;

#ifdef UNIX
	FILE       *fp;
#endif
#ifdef MAC
	short      fileID;
	FSSpec       ThisFS;
#endif
   
	*ERR = noErr;
	
	strcpy( buffer, fn );
	*fileSize = 0L;
	
#ifdef UNIX
/*
	fp = fopen( buffer, "r" );
	if( fp==NULL ) {
		if( errno==ENOENT ) {
			*ERR = FILE_NOT_FOUND;
		} else {
			*ERR = IMG_HEADER_ERROR:
		}
	}
*/
	*ERR = ck_fopen( &fp, buffer, "rb" );
	if( *ERR != noErr ) {
		return nil;
	}

	fseek( fp, 0, SEEK_END );	/* one way to get file length */
	*fileSize = ftell( fp );
	fseek( fp, 0, SEEK_SET );   /* rewind the file */
	
/* by nulling one point past the buffer, we terminate char strings. */
	data = (char *)ck_calloc( (*fileSize)+1, sizeof( char ), "ReadLocalFile contents" );
	
	if( fread( data, sizeof( char ), *fileSize, fp ) != *fileSize ) {
		*ERR = errno;
		return nil;
	}

	fclose( fp );
#endif

#ifdef MAC
	if (uc_dbg) {
		sprintf( gMessageStr,"ReadLocalFile: read file %s\n",buffer);
		( gMessageStr );
	}
	
	if( RT ) {
		
//zrinka 03/22/02 - disbling real-time mode until GUSI is carbonized
/*
#ifdef REMOTE
// open file, get size back
		error = OpenRemoteFile( buffer, fileSize );
		if( error != noErr ) {
			*ERR = error;
			return nil;
		}
	
// MUST ALLOCATE THIS MEMORY!!
		data = (char *)ck_malloc( (*fileSize)+1, "ReadLocalFile contents" );

// read the file
		error = ReadRemoteFile( fileSize, data );	// tcpsrc.c
		if( error != noErr ) {
			*ERR = error;
			return nil;
		}
		
		error = CloseRemoteFile( buffer );
		if( error != noErr ) {
			*ERR = error;
			return nil;
		}
#else
		*ERR = NO_REMOTE;
		return nil;
#endif
*/

	} else {
		Str255 bufferStr;

// Create new FSSpec using the same volume as the image file
		//error = FSMakeFSSpec( refFS->vRefNum, refFS->parID, C2PStr(buffer), &ThisFS );	//zrinka 03/18/02 - C2PStr not supported in Carbon
		CopyCStringToPascal( fn, bufferStr ); //convert p to c string; c string in 'buffer'
		strcpy( fn, (char *)bufferStr );	// copy buffer back so that the original name can be used below
		error = FSMakeFSSpec( refFS->vRefNum, refFS->parID, (ConstStr255Param)fn, &ThisFS );

		if( error != noErr ) {
			*ERR = error;
			return nil;
		}

// open the file
		error = FSpOpenDF ( &ThisFS, fsRdPerm, &fileID );
		if( error ) {
			*ERR = COULD_NOT_OPEN_HEADER;
			return nil;
		}

// get the file size
		error = GetEOF( fileID, fileSize );
		if( error != noErr ) {
			*ERR = error;
			return nil;
		}
		
		error = SetFPos(fileID, fsFromStart, 0);
		if( error != noErr ) {
			*ERR = error;
			return nil;
		}

/* allocate storage for these data */
		data = (char *)ck_calloc( (*fileSize)+1, sizeof( char ), "ReadLocalFile contents" );

/* read the data */
		error = FSRead(fileID, fileSize, data);
		if( error != noErr ) {
			*ERR = error;
			memFree( (void **)&data, "Allocated in ReadLocalFile" );
			return nil;
		}
		
		FSClose ( fileID );
	}
#endif
	return data;
}

# if __profile__
OSErr MySetFPos (short fp, long seek);
# endif /* __profile__ */

void VerifyTrailers( short *check_trailer, int TimePt, int slice );

void VerifyTrailers( short *check_trailer, int TimePt, int slice )
{
#ifdef MAC
  if (check_trailer[0] != slice+1)
    if (dbg) {
    	sprintf( gMessageStr, "Warning: wrong slice no. in header %04x, %04x expected\n", check_trailer[0], slice+1);
  		LogMessage( gMessageStr );
  	}
  if (check_trailer[1] != TimePt+1)
    if (dbg) {
    	sprintf( gMessageStr, "Warning: wrong img. no. in header %04x, %04x expected\n", check_trailer[1], TimePt+1);
  		LogMessage( gMessageStr );
  	}
  if (check_trailer[2] != 1)
    if (dbg) {
    	sprintf( gMessageStr, "Warning: wrong type in header %04x, %04x expected\n", check_trailer[2], 1);
  		LogMessage( gMessageStr );
  	}
  if (check_trailer[3] != 0)
    if (dbg) {
    	sprintf( gMessageStr, "Warning: wrong time in header %04x, %04x expected\n", check_trailer[3], 0);
  		LogMessage( gMessageStr );
  	}
  if (check_trailer[4] != 0)
    if (dbg) {
    	sprintf( gMessageStr, "Warning: wrong res1 in header %04x, %04x expected\n", check_trailer[4], 0);
  		LogMessage( gMessageStr );
  	}
  if (check_trailer[5] != 0)
    if (dbg) {
    	sprintf( gMessageStr, "Warning: wrong res2 in header %04x, %04x expected\n", check_trailer[5], 0);
  		LogMessage( gMessageStr );
  	}
  if (check_trailer[6] != 0)
    if (dbg) {
    	sprintf( gMessageStr, "Warning: wrong res3 in header %04x, %04x expected\n", check_trailer[6], 0);
  		LogMessage( gMessageStr );
  	}
  if (check_trailer[7] != 1)
    if (dbg) {
    	sprintf( gMessageStr, "Warning: wrong saved in header %04x, %04x expected\n", check_trailer[7], 1);
  		LogMessage( gMessageStr );
  	}
#else
	TimePt = slice * *check_trailer;
#endif
}

/* --> ******************************************************************************
*
*    <B>GetSelectedVolume</B>
*    Read a multislice multiple time point data set as a volume at a single time
*
*    This is a total rebuild (new call line) of GetSelectedVolume, which is hereby
*    antiquated. For backwards compatilibility, the new function is GetNextVoume
*
*    CALLING SEQUENCE:
*    void GetSelectedVolume(
*                        FILE *imFile  -- The file pointer to an OPEN data file
*                            IMAGE im  -- a UC image type for image dimensions
*                                           im->data_type is the output data type needed
*                     void *theVolume  -- the destination for the image data
*                          int TimePt  -- the volume (time point) to be accessed
*
*    NOTE: desired output file type must be passed in im.data_type
<!-- *****************************************************************************/
#ifdef UNIX
OSErr   GetSelectedVolume( FILE *imFile, IMAGE *im, void *theVolume, int TimePt )
#endif
#ifdef MAC
OSErr   GetSelectedVolume( short imFileID, IMAGE *im, void *theVolume, short TimePt )
#endif

{
	short      theSlice;
	long       imSize;
	char       *vp;
	long       sliceSize;
	float      fileMax, fileMin;		/* the max and min pixel intensities for the volume */
	OSErr      error = noErr;

	vp = (char *)theVolume;
	imSize = im->dim.x*im->dim.y;
	sliceSize = imSize*get_datasize( im->data_type );

	fileMax = im->theMaxf;
	fileMin = im->theMinf;

#ifdef UNIX
	for( theSlice=0; theSlice < im->dim.n_slices; theSlice++ ) {
		error = UC_Readimage( imFile, im, vp + sliceSize*theSlice, theSlice, TimePt );
		if( error != noErr && error != CONVERSION_ERROR ) {
			fclose( imFile );
			return error;
		}
	}
#endif
#ifdef MAC
	for( theSlice=0; theSlice < im->dim.n_slices; theSlice++ ) {
		error = UC_Readimage( imFileID, im, vp + sliceSize*theSlice, theSlice, TimePt );
		if (error != noErr && error != CONVERSION_ERROR) {
			FSClose( imFileID );
			return error;
		}
	}
#endif

	fileMax = fileMax > im->theMaxf ? fileMax : im->theMaxf;
	fileMin = fileMin < im->theMinf ? fileMin : im->theMinf;	  

	im->theMaxf = fileMax;
	im->theMinf = fileMin;
	return error;
}

/* --> *************************************************************************************/
#ifdef UNIX
OSErr UC_Readimage2D( FILE *fp, IMAGE *im, void *image_data, int imageIndex )
/* <!-- */
{
    return UC_Readimage(fp, im, image_data, 0, imageIndex );
}
#endif
/* --> */
#ifdef MAC
OSErr UC_Readimage2D( short imFileID, IMAGE *im, void *image_data, short imageIndex )
/* <!-- ***************************************************************************************/
{
    return UC_Readimage( imFileID, im, image_data, 0, imageIndex );
}
#endif

/* --> </pre></BODY></HTML> */


