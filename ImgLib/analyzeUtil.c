/* <HTML><HEAD><TITLE>ImgLib routines</TITLE></HEAD><BODY><PRE><!--
/******************************************************************
-->
       A set of primitives to manipulate analyze headers
             c1999-2002 Mark S. Cohen

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
<!--
 $Id: analyzeUtil.c,v 1.10 2002/06/06 03:53:20 mscohen Exp $
 $Log: analyzeUtil.c,v $
 Revision 1.10  2002/06/06 03:53:20  mscohen
 Added AnalyzeTypeToString and modified ShowAnalyzeHeader

 * Revision 1.9  2002/04/16  17:03:10  mscohen
 * copyright
 *
 * Revision 1.9  2002/04/16  17:03:10  mscohen
 * copyright
 *
 * Revision 1.8  2001/02/28  14:38:46  mscohen
 * Update copyright notice
 *
 * Revision 1.8  2000/10/19  15:35:47  mscohen
 * *** empty log message ***
 *
 * Revision 1.7  2000/05/03  22:06:36  mscohen
 * Current version modest changes (not tracked)
 *
 * Revision 1.7  2000/05/03  22:06:36  mscohen
 * Current version modest changes (not tracked)
 *
 * Revision 1.6  2000/02/15  15:54:39  mscohen
 * Rename BigEndian and LittleEndian functions
 *
 * Revision 1.5  1999/10/07  16:05:46  mscohen
 * Found an unitialized memory error in swapHdrBytes (and fixed it)
 *
 * Revision 1.4  1999/09/22  15:29:43  mscohen
 * Consolidated header files.
 *
 * Revision 1.5  1999/07/14  05:52:12  mscohen
 * ANSI C compatibility
 *
 * Revision 1.2  1998/04/15  04:04:22  mscohen
 * Include data size and bytswapping routines here so that the code can stand alone
 *
 $Date: 2002/06/06 03:53:20 $
*******************************************************/
#include <time.h>
#include <string.h>
#include <stdio.h>
#include <limits.h>

#include "ImgLib.h"

#define SLENGTH 80

/* --> *******************************************************************************
*  <B> EmptyAnaHdr </B>
*     Return an analyze header with most basic fields populated with reasonable values
*
*     OSErr  EmptyAnaHdr( dsr *DSR,                       -- pre-allocated memory
*                         char *description,              -- text description
*                         float sliceSpacing,             -- center to center slice spacing
*       		          int xs, int ys, int n_slices, 
*       		          int time_pts,
*       		          char orient )
*       		 
*  Caller should consider adding content to:
*		
*    dsr.dime.pixdim[1]  -- the x pixel size
*    dsr.dime.pixdim[2]  -- the y pixel size
*    glmax and glmin     -- the global max and min
*    DSR.hist.aux_file   -- 24 characters of file name
<!--   *******************************************************************************/

OSErr  EmptyAnaHdr( dsr *DSR, char *description, float sliceSpacing,
		 int xs, int ys, int n_slices, int time_pts, char orient )
{
/*
Return an analyze header with most basic fields populated with reasonable values

Caller should consider adding content to:

dsr.dime.pixdim[1]  -- the x pixel size
dsr.dime.pixdim[2]  -- the y pixel size
glmax and glmin     -- the global max and min
DSR.hist.aux_file   -- 24 characters of file name

*/

	time_t  now;
	char    nowstr[SLENGTH];
	struct  tm *nowstruct;
	int	    i;
	OSErr		error = noErr;

/* header key data */
	DSR->hk.sizeof_hdr = sizeof( dsr );
	strcpy( DSR->hk.db_name, "none" );
	strcpy( DSR->hk.data_type, "" );
	DSR->hk.extents = 16384;
	DSR->hk.session_error = 0;	/* set this if the scanner dies */
	DSR->hk.regular = 'r';
	DSR->hk.hkey_un0 = 0;

/* dimension data */
	DSR->dime.dim[0] = 4;
	DSR->dime.dim[1] = xs;
	DSR->dime.dim[2] = ys;
	DSR->dime.dim[3] = n_slices;
	DSR->dime.dim[4] = time_pts;
	for( i=5; i<8; i++ )
	{
		DSR->dime.dim[i] = 0;
	}
	strcpy(	DSR->dime.vox_units, "mm" );
	strcpy( DSR->dime.cal_units, "" );
	DSR->dime.flags = 0;
	if( macByteOrder() ) {
		DSR->dime.flags |= kmacByteOrder;
	}
	DSR->dime.datatype = DT_SIGNED_SHORT;	/* we have trouble if the input data exceed 32767 */
	DSR->dime.bitpix = 16;
	DSR->dime.dim_un0 = 0;
	DSR->dime.pixdim[0] = 0.0; /* number of dimensions? */
	DSR->dime.pixdim[1] = 3.125; /* assume axial cuts */
	DSR->dime.pixdim[2] = 3.125;
	DSR->dime.pixdim[3] = sliceSpacing;
	for( i=4; i<8; i++ )
	{
		DSR->dime.pixdim[i] = 0.0;
	}
	DSR->dime.vox_offset = 0.00;
	DSR->dime.funused1 = DSR->dime.funused2 = DSR->dime.funused3 = 0.00;
	DSR->dime.cal_max = 0.00;	/* ought to be big enough */
	DSR->dime.cal_min = 0.00;
	DSR->dime.compressed = 0L;
	DSR->dime.verified = 0L;
	DSR->dime.glmax = INT_MAX;
	DSR->dime.glmin = -INT_MAX;

/* data_history */
	sprintf( DSR->hist.descrip, "%-80.80s", description );
	strncpy( DSR->hist.descrip, description, 80);
	strcpy( DSR->hist.aux_file, "" );
	switch( orient )  /* NOTE THAT 3 through 5 are "flipped" orientation */
	{
		case 'z':
		case 'Z':
			DSR->hist.orient = '3';
			break;
		case 'y':
		case 'Y':
			DSR->hist.orient = '4';
			break;
		case 'x':
		case 'X':
			DSR->hist.orient = '5';
			break;
		default:
			DSR->hist.orient = '3';
			break;
	}
	strcpy( DSR->hist.originator, "UCLA MRI" );
	strcpy( DSR->hist.generated, "" );
	strcpy( DSR->hist.scannum, "0" );
	strcpy( DSR->hist.patient_id, "" );

	time( &now );
	nowstruct = localtime( &now );
	strftime(nowstr, SLENGTH, "%m/%d/%Y", nowstruct); /* 12/23/1944 Y2k */
	strncpy(DSR->hist.exp_date,nowstr,10);
	strftime(nowstr, SLENGTH, "%H:%M:%S", nowstruct);
	strncpy(DSR->hist.exp_time,nowstr,10);
	strcpy(DSR->hist.hist_un0,"");
	DSR->hist.views = DSR->hist.vols_added = DSR->hist.start_field = 0L;
	DSR->hist.field_skip = DSR->hist.omax = DSR->hist.omin = DSR->hist.smax = DSR->hist.smin = 0L;

	return( error );
}

/* --> *******************************************************************************
*  <B> ImageToAnalyzeHeader </B>
*   OSErr  ImageToAnalyzeHeader( IMAGE *im,         -- populated IMAGE struct
*                                dsr *DSR,          -- allocated memory
*                                short *result )    -- data range result code
*
*   Create an analyze header (dsr) based on an IMAGE struct
*	populating any known fields
<!--   *******************************************************************************/
OSErr  ImageToAnalyzeHeader( IMAGE *im, dsr *DSR, short *result )
{
	char    descr[80];
	char    orient = 'z';
	struct tm  *theTime;
	char    timeString[11];
	
	*result = noErr;
	strncpy( descr, im->subj.descr, 79 );
	descr[79] = '\0';
	
// calculate image orientation based on RAS...

	EmptyAnaHdr( DSR, descr, im->dim.scanspacing, im->dim.x, im->dim.y,
	             im->dim.n_slices, im->dim.timePts, orient );

	DSR->dime.datatype     = im->data_type;
	strcpy( DSR->dime.vox_units, "mm" );
	
	if( DSR->dime.glmax > INT_MAX ) {
		DSR->dime.glmax = (int)im->theMaxf;
		result += IMAGE_MAX_OUT_OF_RANGE;
	} else {
		DSR->dime.glmax = INT_MAX;
	}
	
	if( DSR->dime.glmin > INT_MAX ) {
		DSR->dime.glmin = (int)im->theMinf;
		result += IMAGE_MIN_OUT_OF_RANGE;
	} else {
		DSR->dime.glmin = -INT_MAX;
	}
	
	sprintf( DSR->hist.scannum, "E%.3dS%.2dI%.2d", im->acq.im_exno,
	         im->acq.im_seno, im->acq.im_no );

	strncpy( DSR->hist.patient_id, im->subj.patid, 15 );
	DSR->hist.patient_id[15] = '\0';

	theTime = localtime( (time_t *)&im->acq.im_actual_dt );
	strftime( timeString, 10, "%m/%d/%Y", theTime );
	strncpy( DSR->hist.exp_date, timeString, 10 );
	
	strftime( timeString, 10, "%H:%M:%S", theTime );
	sprintf( DSR->hist.exp_time, "%s", timeString );

	DSR->dime.flags     = im->dim.flags;	// possible range error!
	DSR->dime.pixdim[1] = im->dim.dpixsize_X;
	DSR->dime.pixdim[2] = im->dim.dpixsize_Y;
	DSR->dime.pixdim[3] = im->dim.slthick;

	return noErr;
}

/* moved from ImgLib.c to be used by Make/Show Header */
/* --> *******************************************************************************
*  <B> ShowAnalyzeHeader </b>
*   void ShowAnalyzeHeader( dsr *DSR,     -- populated dsr (analyze struct)
*                           FILE *fp,     -- output file
*                           char *name )  -- Name to display
*   Display, using fprintf, the contents of an analyze header in a human readable form.
<!--   *******************************************************************************/
void ShowAnalyzeHeader( dsr *DSR, FILE *fp, char *name )
{
	int	i;
	char exp_date[10]; /* ten plus \0 */
	
	fprintf( fp, "Show Header, ver 2.01 Dec 23, 1998.\nName = %s\n",name);
	/* byte swap already doone. */
	fprintf( fp, "hk.size_of_hdr:   %d\nhk.data_type:    \"%-10s\"\nhk.db_name:      \"%-8s\"\n",
			DSR->hk.sizeof_hdr, DSR->hk.data_type, DSR->hk.db_name );
	fprintf( fp, "hk.extents:       %d\nhk.session_error: %d\nhk.regular:      \'%c\'\n",
			DSR->hk.extents, DSR->hk.session_error, DSR->hk.regular );
	fprintf( fp, "hk.hkey_unused0: \'%c\'\n", DSR->hk.hkey_un0);
	fprintf( fp, "\ndime.dim:         " );
	for( i=0; i<8; i++ ) {
		fprintf( fp, " [%d] | ", i );
	}
	fprintf( fp, "\n                  " );
	for( i=0; i<8; i++ ) {
		fprintf( fp, "%.4d | ", DSR->dime.dim[i] );
	}
	fprintf( fp, "\n [hex]            " );
	for( i=0; i<8; i++ ) {
		fprintf( fp, "%04x | ", DSR->dime.dim[i] );
	}
	fprintf( fp, "\ndime.vox_units:  \"%-4s\"\ndime.cal_units:  \"%-8s\"\n",
		DSR->dime.vox_units, DSR->dime.cal_units );
	
	fprintf( fp, "dime.flags:       %04x %s\n",
	    DSR->dime.flags, UCFlagString( DSR->dime.flags ));	

	fprintf( fp, "dime.datatype:    %d (%s)\n",
	              DSR->dime.datatype, AnalyzeTypeToString( DSR->dime.datatype ) );
	
	fprintf( fp, "dime.bitpix:      %d\ndime.dim_un0:     %d\n",
	          DSR->dime.bitpix, DSR->dime.dim_un0 );
	          
	for( i=0; i<8; i++ ) {
		fprintf( fp, "dime.pixdim[%d]:   %f\n", i, DSR->dime.pixdim[i] );
	}
	fprintf( fp, "dime.vox_offset:  %f\n", DSR->dime.vox_offset );
	fprintf( fp, "dime.funused1:    %f\n", DSR->dime.funused1 );
	fprintf( fp, "dime.funused2:    %f\n", DSR->dime.funused2 );
	fprintf( fp, "dime.funused3:    %f\n", DSR->dime.funused3 );
	fprintf( fp, "dime.cal_max:     %f\n", DSR->dime.cal_max );
	fprintf( fp, "dime.cal_min:     %f\n", DSR->dime.cal_min );
	fprintf( fp, "dime.compressed:  %d\n", DSR->dime.compressed );
	fprintf( fp, "dime.verified:    %d\n", DSR->dime.verified );
	fprintf( fp, "dime.glmax:       %d\n", DSR->dime.glmax );
	fprintf( fp, "dime.glmin:       %d\n", DSR->dime.glmin );
	fprintf( fp, "\n" );
	fprintf( fp, "hist.descrip:    \"%-40.40s\"\n", DSR->hist.descrip );
	fprintf( fp, "                 \"%c\"\n", DSR->hist.descrip[40]);
	fprintf( fp, "hist.aux_file:   \"%-24s\"\n", DSR->hist.aux_file );
	fprintf( fp, "hist.orient:     \"%c\"\n", DSR->hist.orient );
	fprintf( fp, "hist.originator: \"%-10s\"\n", DSR->hist.originator );
	fprintf( fp, "hist.generated:  \"%-10s\"\n", DSR->hist.generated );
	fprintf( fp, "hist.scannum:    \"%-10s\"\n", DSR->hist.scannum );
	fprintf( fp, "hist.patient_id: \"%-10s\"\n", DSR->hist.patient_id );
	strncpy(exp_date,DSR->hist.exp_date,10); /* we've written over the '\0' in [9]. this is to display it properly  */
	fprintf( fp, "hist.exp_date:   \"%-10s\"\n", exp_date );
	fprintf( fp, "hist.exp_time:   \"%-10s\"\n", DSR->hist.exp_time );
	fprintf( fp, "hist.hist_un0:   \"%-3s\"\n", DSR->hist.hist_un0 );
	fprintf( fp, "hist.views:       %d\n", DSR->hist.views );
	fprintf( fp, "hist.vols_added:  %d\n", DSR->hist.vols_added );
	fprintf( fp, "hist.start_field: %d\n", DSR->hist.start_field );
	fprintf( fp, "hist.field_skip:  %d\n", DSR->hist.field_skip );
	fprintf( fp, "hist.omax:        %d\n", DSR->hist.omax );
	fprintf( fp, "hist.omin:        %d\n", DSR->hist.omin );
	fprintf( fp, "hist.smax:        %d\n", DSR->hist.smax );
	fprintf( fp, "hist.smin:        %d\n", DSR->hist.smin );
	fprintf( fp, "\n\n");
	fflush (fp);
	return;
}

/* --> *******************************************************************************
*  <B> swapHdrBytes </B>
*   short swapHdrBytes( void *Hdr )
*   byte swap the contents of the analyze header.
*   
*   Note that longs on the DEC are 8 bytes, and ints are 4
*   whereas on the Sun, longs are 4 and ints are 2
<!--   *******************************************************************************/
short swapHdrBytes( void *Hdr )
{
   char   *p;
   short  retValue = noErr;
   
   p = (char *)Hdr;
   
   vbyteswap( p, 1, T_INT );	/* header size */
   p += get_datasize( T_INT ) + 28;     /* data_type and db_name */
   vbyteswap( p, 1, T_INT );		/* extents */
   p += get_datasize( T_INT );
   vbyteswap( p, 1, T_SHORT );	/* session error */
   p += get_datasize( T_SHORT ) + 2;
   vbyteswap( p, 8, T_SHORT );	/* dimensions */
   p += 8*get_datasize( T_SHORT ) + 12; /* vox and cal_units */
   vbyteswap( p, 4, T_SHORT );	/* unused, datatype, bitpix, dim_un0 */
   p += 4*get_datasize( T_SHORT );
   vbyteswap( p, 14, T_FLOAT ); /* pixdim[8], vox_offset, funused[3], cal_max, cal_min,  */
   p += 14*get_datasize( T_FLOAT );
   vbyteswap( p, 4, T_LONG );   /* compressed, verified, glmax, glmin */
   p += 4*get_datasize( T_LONG ) + 168;
   vbyteswap( p, 8, T_LONG );   /* views, vols_added, start_field, field_skip, omax, omin, smax, smin */
   
   return retValue;
}

char *AnalyzeTypeToString( short int TYPE )
{
	switch( TYPE ) {
		case DT_UNKNOWN:
			return( "Unknown or none" );
		break;
		
		case DT_BINARY:
			return( "Binary" );
		break;
		
		case DT_UNSIGNED_CHAR:
			return( "unsigned char" );
		break;
		
		case DT_SIGNED_SHORT:
			return( "signed short" );
		break;
		
		case DT_SIGNED_INT:
			return( "signed int" );
		break;
		
		case DT_FLOAT:
			return( "float" );
		break;
		
		case DT_COMPLEX:
			return( "complex" );
		break;
		
		case DT_DOUBLE:
			return( "double" );
		break;
		
		case DT_RGB:
			return( "RGB" );
		break;
		
		case DT_ALL:
			return( "\"ALL\"" );
		break;
		
		default:
			return( "unrecognized" );
	}
}
