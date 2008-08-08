/* <html><head><title>ImageInfo.c</title></head><body>
* <font size=+2><b>ImageInfo.c</b></font>
* <i>Please note last saved date:</i>
*  <font size=+1><b>$Date: 2002/11/19 21:00:03 $</b><p></font><p>
*	Dumps the header contents for any image types recognized by UCLA Brain Mapping
*               &copy;1996-2002  Mark S. Cohen
<h2>How to make ImageInfo</h2>
<ol>
<li>Build ImgLib.o analyzeUtil.o ImgLibError.o VLib.o (from
<a
href="http://airto.loni.ucla.edu/BMCweb/SharedCode/SharedSoftware.html#Anchor-UCLA-35829
">www.brainmapping.org</a>)
<li>compile ImageInfo, linking the objects above. You will need to have C++ extensions
and the Math Library.
A typical command might be:<font face=courier><p>
cc ImgLib.o analyzeUtil.o ImgLibError.o VLib.o ImageInfo.c -o ImageInfo -lm</font>
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
<a href="http://airto.loni.ucla.edu/BMCweb/HowTo/ImageInfo.html">
Brain Mapping Center support pages</a>.
</td></tr></table></center>
<pre> 
<!--
* $Log: ImageInfo.c,v $
* Revision 1.12  2002/11/19 21:00:03  zrinka
* fixed some html
*
* Revision 1.11  2002/09/09 22:47:42  nina
* changed fopen mode from "r"/"w" to "rb"/"wb" where appropriate
*
* Revision 1.10  2002/06/01 02:21:58  mscohen
* Closed header file that was left open. Small cleanups in code.
*
* Revision 1.1  2002/04/24 20:59:40  mscohen
* Initial revision
*
 * Revision 1.9  2002/04/17  03:43:31  mscohen
 * mostly html
 *
 * Revision 1.8  2002/04/16  19:08:04  mscohen
 * modified html
 *
 * Revision 1.7  2002/04/16  19:04:58  mscohen
 * Revision 1.6  2002/04/16  18:38:09  mscohen
 * rcs upgrade
* -->
********************************************************************************/
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "ImgLib.h"

void usage( char *progname );
OSErr MoreAnalyzeHeader( char *name );

void usage( char *progname )
{
	printf( "Usage: %s ImageFileName" );
	printf( "Summarizes key header information for a variety of medical\
	image file types\n" );
}

OSErr MoreAnalyzeHeader( char *name )
{
	OSErr error = noErr;
	FILE  *hdrFile;
	dsr   theDSR;

	error = ck_fopen( &hdrFile, name, "rb" );
	RETURNONERROR;
	
	error = ck_fread( &theDSR, sizeof( dsr ), 1, hdrFile );
	RETURNONERROR;

	if( theDSR.dime.dim[0] != 4 ||	// check byte order consistency
	   theDSR.hk.sizeof_hdr != sizeof( dsr ) ) {
		swapHdrBytes( &theDSR );
	}

	ShowAnalyzeHeader( &theDSR, stdout, name );
	error = ck_fclose( hdrFile );
	
	return error;
}

int main( int argc, char **argv )
{
	IMAGE  im;
	dsr    myDSR;
	char   inName[256];
	int    i;
	time_t correctedTime;
	unsigned long   TimeOffset = 28800;	// 8 hrs from GMT...?
	short  minutes;
	OSErr  error = noErr;

	if( argc == 1 ) {
		printf( "Enter the name of an image file ( xxx.img ): " );
		scanf( "%s", inName );
	} else {
		strcpy( inName, argv[1] );
	}
	error = UC_Readheader( inName, &im );
	ILError( error, "reading input header" );

	printf( "\nImage file type: %s\n", ReportFileType( im.file_type ));
	printf( "Byte Order:  " );
	if( im.dim.flags & kmacByteOrder ) {
		printf( "Big Endian (like a Macintosh)\n" );
	} else {
		printf( "Little Endian (like a pc)\n" );
	}
	for( i=0; i<40; i++ ) printf( "-" );
	printf( "\n" );

	if( strlen( im.RawHdr ))
		printf( "Header name .............. %s\n", im.RawHdr );
	if( im.acq.im_exno )
		printf( "Exam number .............. %d\n", im.acq.im_exno );
	if( im.acq.im_seno )
		printf( "Series number ............ %d\n", im.acq.im_seno );
	if( im.acq.im_no )
		printf( "Image number ............. %d\n", im.acq.im_no );

	if( im.acq.im_datetime ) {
		correctedTime = (time_t)(im.acq.im_datetime + TimeOffset);
		printf( "Date/time (?)............. %s", ctime( &correctedTime ) );
	}

	if( im.acq.sctime > 0.0 ) {
		minutes = im.acq.sctime/60e6;
		printf( "Scan duration ............ %d:%0.2f min\n",
		               minutes, ( im.acq.sctime - minutes * 60e6 )/1e6 );
	}

// Demographics
	if( strlen( im.subj.patname ))
		printf( "Subject Name ............. %s\n", im.subj.patname );
	if( strlen( im.subj.patid ))
		printf( "Patient ID ............... %s\n", im.subj.patid );
	if( strlen( im.subj.reqnum ))
		printf( "Requisition No ........... %s\n", im.subj.reqnum );
	if( strlen( im.subj.HumanStudiesID  ))
		printf( "Human Subjects ID ........ %s\n", im.subj.HumanStudiesID );
	if( strlen( im.subj.ResearchProtocol ))
		printf( "Research Protocol ........ %s\n", im.subj.ResearchProtocol );
	if( im.subj.patage )
		printf( "Age ...................... %d years\n", im.subj.patage );
	if( im.file_type == SIGNA ) {
		if( im.subj.male ) {
			printf( "Sex ...................... Male\n" );
		} else {
			printf( "Sex ...................... Female or not specified\n" );
		}
	}
	if( im.subj.patweight)
		printf( "Weight ................... %0.1f pounds\n", im.subj.patweight * .0022 );
	if( strlen( im.subj.descr ))
		printf( "Scan description ......... %s\n", im.subj.descr );
	if( strlen( im.subj.refphy ))
		printf( "Referring physicion....... %s\n", im.subj.refphy );
	if( strlen( im.subj.diagrad ))
		printf( "Radiologist .............. %s\n", im.subj.diagrad );
	if( strlen( im.subj.ex_op ))
		printf( "Operator ................. %s\n", im.subj.ex_op );
	if( strlen( im.subj.hospname ))
		printf( "Hospital Name ............ %s\n", im.subj.hospname );
	if( im.acq.fieldStrength > 0.0 ) {
		if( strcmp( im.subj.hospname, "UCLA Brain Mapping" )) {
			printf( "Field strength ........... 3.0 Tesla\n" );
		} else {
			printf( "Field strength ........... %0.2f Tesla\n", im.acq.fieldStrength/10000.0 );
		}
	}
	for( i=0; i<40; i++ ) printf( "-" );
	printf( "\n" );	// section separator
		
// Image Data
	if( strlen( im.acq.psdname ))
		printf( "pulse sequence name ...... %s\n", im.acq.psdname );
	if( im.acq.nReadPoints && im.acq.nPhaseLines )
		printf( "acq matrix ............... %hd x %hd\n",
		         im.acq.nReadPoints, im.acq.nPhaseLines );
	if( im.dim.acqpixsize_X > 0.0 && im.dim.acqpixsize_Y > 0.0 )
		printf( "Resolution (x / y) ....... %0.3f x %0.3f mm\n",
		         im.dim.acqpixsize_X, im.dim.acqpixsize_Y );
	if( im.dim.dfov > 0.0 && im.dim.dfov_rect > 0.0 )
		printf( "Display FoV (x / y) ...... %0.0f / %0.0f mm\n",
		         im.dim.dfov, im.dim.dfov_rect );
	if( im.dim.dpixsize_X > 0.0 && im.dim.dpixsize_Y > 0.0 )
		printf( "Display Pixels (x / y) ... %0.3f / %0.3f mm\n",
		         im.dim.dpixsize_X, im.dim.dpixsize_Y );
	if( im.dim.x && im.dim.y )
		printf( "Display matrix ........... %hd x %hd pixels\n", im.dim.x, im.dim.y );
	if( im.dim.slthick > 0 ) {
		printf( "Slice thickness .......... %0.2f\n", im.dim.slthick );
		printf( "Slice spacing ............ %0.2f\n", im.dim.scanspacing );
	}
	/* RMD 1/7/02 */
	if( im.loc.tlhc[0] )
		printf( "Top left RAS coords ...... %0.2f %0.2f %0.2f\n", 
		        im.loc.tlhc[0],
		        im.loc.tlhc[1],
		        im.loc.tlhc[2] );
	if( im.loc.trhc[0] )
		printf( "Top right RAS coords ..... %0.2f %0.2f %0.2f\n",
				im.loc.trhc[0],
				im.loc.trhc[1],
				im.loc.trhc[2] );
	if( im.loc.brhc[0] )
		printf( "Bottom right RAS coords .. %0.2f %0.2f %0.2f\n",
				im.loc.brhc[0],
				im.loc.brhc[1],
				im.loc.brhc[2] );
	if( im.loc.imgNorm[0] )
		printf( "Normal RAS coords ........ %0.2f %0.2f %0.2f\n",
				im.loc.imgNorm[0],
				im.loc.imgNorm[1],
				im.loc.imgNorm[2] );
	/* RMD end */
	if( im.dim.n_slices )
		printf( "Number of slices ......... %d\n", im.dim.n_slices );
	if( im.dim.timePts )
		printf( "Time Points .............. %d\n", im.dim.timePts );
	if( im.fileData_type )
		printf( "File data type ........... %s\n", ReportDataType( im.fileData_type ));
	if( im.acq.tr )
		printf( "TR ....................... %0.1f msec\n", im.acq.tr/1000.0 );
	if( im.acq.te ) {
		printf( "TE ....................... %0.2f msec", im.acq.te/1000.0 );
		if( im.acq.frac_echo ) {
			printf( "(fractional)\n" );
		} else {
			printf( "\n" );
		}
	}
	if( im.acq.ti )
		printf( "TI ....................... %0.2f msec\n", im.acq.ti/1000.0 );
	if( im.acq.nex > 0.0 )
		printf( "nex ...................... %0.1f\n", im.acq.nex );
	if( im.acq.se_nacq )
		printf( "acquisitions ............. %hd\n", im.acq.se_nacq );
	if( im.acq.mr_flip )
		printf( "Flip Angle ............... %d degrees\n", im.acq.mr_flip );
	if( im.acq.vbw > 0.0 )
		printf( "Acquisition bandwidth .... %0.3f kiloHertz\n", im.acq.vbw );
	if( im.acq.numecho )
		printf( "Echo ..................... %d of %d\n", im.acq.echonum, im.acq.numecho );
	if( im.acq.echo_trn_len )
		printf( "Echo train length ........ %d\n", im.acq.echo_trn_len );
	if( im.acq.xmtgain )
		printf( "Transmitter gain ......... %d\n", im.acq.xmtgain );
	if( im.acq.rec1 )
		printf( "Receiver gain (1) ........ %d\n", im.acq.rec1 );
	if( im.acq.rec2 )
		printf( "Receiver gain (2) ........ %d\n", im.acq.rec2 );
	if( strlen( im.acq.prtcl ))
		printf( "Protocol name ............ %s\n", im.acq.prtcl );
	if( im.theMinf < im.theMaxf )
		printf( "Pixel min/max ............ %0.2f / %0.2f\n", im.theMinf, im.theMaxf );

	if( im.file_type == ANALYZE ) {
		char response[2];

		printf( "\n\nShow all Analyze header fields (y/n)?: " );
		scanf( "%s", response );
		lowerString( response );

		if( !strcmp( response, "y" )) {
			error = MoreAnalyzeHeader( im.RawHdr );
			ILError( error, "Showing extra analyze contents" );
		}
	}
	return 0;
}
/* --> </PRE></BODY></HTML> */
