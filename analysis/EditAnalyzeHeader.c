/* <html><head><title>EditAnalyzeHeader source</title></head><body>
<h1>EditAnalyzeHeader.c</h1>
<i>Please note last saved date:</i>
 <font size=+1><b>$Date: 2003/08/08 17:39:57 $</b><p></font><p>

<center>
<table bgcolor="black" cellpadding=5 border="1">
	<tr><td><font color="white">
General purpose tool for interactively editing (and creating) Mayo "analyze" headers.
	</font></td></tr></table><p>
	&copy;2002-today  <a href="mailto:mscohen@ucla.edu">Mark S. Cohen, Ph.D.</a><br>
	<a href="http://www.brainmapping.org">UCLA Brain Mapping Division</a><br>
</center>
<h2>How to make EditAnalyzeHeader</h2>
<ol>
	<li>Build ImgLib.o analyzeUtil.o ImgLibError.o VLib.o (from
	<a href= "http://airto.loni.ucla.edu/BMCweb/SharedCode/SharedSoftware.html#Anchor-UCLA-35829">
www.brainmapping.org</a>)
	<li>compile EditAnalyzeHeader, linking the objects above. You will need to have C++ extensions and the Math Library.
	A typical command might be:<br><font face=courier><p>
	<center>
		cc ImgLib.o analyzeUtil.o ImgLibError.o VLib.o MathLib.o EditAnalyzeHeader.c -o EditAnalyzeHeader -lm</font>
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
<a href="http://airto.loni.ucla.edu/BMCweb/HowTo/EditAnalyzeHeader.html">
Brain Mapping Center support pages</a>.
</td></tr></table></center>
<pre>
<!--
$Author: mscohen $
$Log: EditAnalyzeHeader.c,v $
Revision 1.4  2003/08/08 17:39:57  mscohen
Modified ImgLib links

Revision 1.3  2002/12/28 17:56:29  mscohen
Modified html for posting on HowTo pages.

Revision 1.2  2002/09/09 22:07:59  nina
changed fopen mode from "r"/"w" to "rb"/"wb" where appropriate

Revision 1.1  2002/08/29 11:37:12  mscohen
Initial revision

-->
*/

#include "getopt.h"
#include "ImgLib.h"

#define false 0
#define true 1
#ifndef noErr
#define noErr 0
#endif

#undef MAC
#ifdef MAC
#include <console.h>
#include <sioux.h>
#endif

enum {
	kDescrip = 1,
	kPat_ID,
	kScannum,
    kMat_size,
    kSlices,     // 5
    kTimePts,
    kPixSize,
    kThick,
    kTR,
    kData_type,  // 10
    kBitPix,
    kOrient,
    kGlobalMaxMin,
    kSecondaryMenu,
    kFlags,      // 15
    kDate,
    kTime,
    kTextDataType,
    kDB_name,
    kViews,      // 20
    kVols_added,
    kOriginator,
    kAux_file,
    kVox_units,
    kVox_offset, // 25
    kCal_units,
    kCalMaxMin,
    kGenerated,
    kMainMenu,
    kLastMenuChoice // 30
};

/* globals */
typedef struct {
	char    OutHeaderName[255];
	char    InHeaderName[255];
	Boolean Verbose;
	FILE    *inFile;
	FILE    *outFile;
	FILE    *ProcFile;
	Boolean CreateNewHeader;
} UtilGlobal;

UtilGlobal u;

/* PROTOTYPES */
void  print_usage( char *name );
OSErr ProcessCommandLine( int argc, char *argv[] );
char  *ResolveDataType( short TheType );
char  *ResolveOrientation( char orient );
OSErr PrintMainMenu( dsr *d, int *MenuChoice );
OSErr PrintSecondaryMenu( dsr *d, int *MenuChoice );
OSErr HandleMenuChoice( dsr *d, int *MenuChoice );
OSErr EditAnalyzeHeader( dsr *d );
OSErr CreateDSRDefaults( dsr *myDSR );

OSErr CreateDSRDefaults( dsr *myDSR )
{
	OSErr error = noErr;
	time_t lclTime;
	struct tm *now;

	lclTime = time(NULL);
	now = localtime(&lclTime);

/* initialize contents to some rational values */
	myDSR->dime.dim[0]    = 4;
	myDSR->dime.dim[1]    = myDSR->dime.dim[2]    = 256;
	myDSR->dime.pixdim[1] = myDSR->dime.pixdim[2] = 240.0/256;
	myDSR->dime.pixdim[3] = 5.0;
	myDSR->dime.pixdim[4] = 2500.0;
	myDSR->hist.orient    = '0';	// axial
	myDSR->dime.datatype  = DT_SIGNED_SHORT;
	myDSR->dime.bitpix     = 8 * sizeof( short );
	strftime( myDSR->hist.exp_date, 10, "%D", now );
	strftime( myDSR->hist.exp_time, 10, "%T", now );

	return error;
}

void print_usage( char *name )
{
	printf( "USAGE: %s -i InputHeader.hdr -o OutputHeader.hdr\n\n", name );
	printf( "\tInteractive tool to edit or create analyze (Mayo) image headers\n" );
	printf( "\tCreates a file, 'outputheader.%s.proc' containing a\n", name );
	printf( "\trecord of all user edits.\n\n" );
	printf( "\tBoth arguments, and the '.hdr' extension, are optional.\n\n" );
	printf( "\tIf no OutputHeader name is provided, the user can choose to over-write\n" );
	printf( "\tthe InputHeader.\n\n" );
	printf( "\tIf no InputHeader name is specified, %s will\n", name );
	printf( "\tcreate and intialize a new analyze header with that name.\n\n" );
}

char *ResolveDataType( short TheType )
{
	char  *TypeString;
	
	TypeString = malloc( 16 );
	
	switch( (int)TheType ) {
		case DT_UNKNOWN:       strcpy( TypeString, "Unknown" ); break;
		case DT_BINARY:        strcpy( TypeString, "binary" ); break;
		case DT_UNSIGNED_CHAR: strcpy( TypeString, "unsigned char" ); break;
		case DT_SIGNED_SHORT:  strcpy( TypeString, "signed short" ); break;
		case DT_SIGNED_INT:    strcpy( TypeString, "signed int" ); break;
		case DT_FLOAT:         strcpy( TypeString, "float" ); break;
		case DT_COMPLEX:       strcpy( TypeString, "complex" ); break;
		case DT_DOUBLE:        strcpy( TypeString, "double" ); break;
		case DT_RGB:           strcpy( TypeString, "RGB" ); break;
		case DT_ALL:           strcpy( TypeString, "all" ); break;
		default:               strcpy( TypeString, "Unknown" ); break;
	}

	return TypeString;
}	
		
char *ResolveOrientation( char orient )
{
	char  *OrientString;
	
	OrientString = malloc( 18 );
	
	if( orient == '0' ) strcpy( OrientString, "axial" );
	if( orient == '1' ) strcpy( OrientString, "coronal" );
	if( orient == '2' ) strcpy( OrientString, "sagittal" );
	if( orient == '3' ) strcpy( OrientString, "axial flipped" );
	if( orient == '4' ) strcpy( OrientString, "coronal flipped" );
	if( orient == '5' ) strcpy( OrientString, "sagittal flipped" );
	
	return OrientString;
}

OSErr PrintMainMenu( dsr *d, int *MenuChoice )
{
	OSErr  error = noErr;

	system( "clear" );

	printf( "Editing \"%s\" -- Select a field to change, by number\n\n", u.InHeaderName );
	printf( "\t%d - Description: [\"%s\"]\n",          kDescrip, d->hist.descrip );
	printf( "\t%d - Patient ID: [\"%s\"]\n",           kPat_ID, d->hist.patient_id );
	printf( "\t%d - Scan number: [\"%s\"]\n",          kScannum, d->hist.scannum );
	printf( "\t%d - xsize [%d] and ysize [%d]\n",      kMat_size, d->dime.dim[1], d->dime.dim[2] );
	printf( "\t%d - number of slices [%d]\n",          kSlices, d->dime.dim[3] );
	printf( "\t%d - number of time points [%d]\n",     kTimePts, d->dime.dim[4] );
	printf( "\t%d - pixel size  [%0.3f x %0.3f %s]\n", kPixSize,
		      d->dime.pixdim[1], d->dime.pixdim[2], d->dime.vox_units );
	printf( "\t%d - slice spacing [%0.3f %s]\n",       kThick, d->dime.pixdim[3], d->dime.vox_units );
	printf( "\t%d - tr [%0.3f] (for some programs)\n", kTR, d->dime.pixdim[4] );
	printf( "\t%d - binary data type [%s]\n",          kData_type, ResolveDataType( d->dime.datatype ));
	printf( "\t%d - bits/pixel [%hd]\n",               kBitPix, d->dime.bitpix );
	printf( "\t%d - orientation [%s]\n",               kOrient, ResolveOrientation( d->hist.orient ));
	printf( "\t%d - global maximum [%d] and minimum [%d] intensities\n",
	          kGlobalMaxMin, d->dime.glmax, d->dime.glmin );
	printf( "\t%d - More Analyze Header fields...\n",  kSecondaryMenu );
	printf( "\t%d - UC flag fields...\n",              kFlags );
	printf( "\t---------\n\t0. Done\n" );

	scanf( "%d", MenuChoice );
	return noErr;
}

OSErr PrintSecondaryMenu( dsr *d, int *MenuChoice )
{
	OSErr  error = noErr;

	system( "clear" );

	printf( "Editing \"%s\" -- Select a field to change, by number\n\n", u.InHeaderName );
	printf( "\t%d - experiment date  [\"%s\"]\n",          kDate, d->hist.exp_date );
	printf( "\t%d - experiment time  [\"%s\"]\n",          kTime, d->hist.exp_time );
	printf( "\t%d - descriptive data type [\"%s\"]\n",     kTextDataType, d->hk.data_type ); 
	printf( "\t%d - db_name [\"%s\"]\n",                   kDB_name, d->hk.db_name );
	printf( "\t%d - views [%d]\n",                         kViews, d->hist.views );
	printf( "\t%d - volumes added [%d]\n",                 kVols_added, d->hist.vols_added );
	printf( "\t%d - Originator: [\"%s\"]\n",               kOriginator, d->hist.originator );
	printf( "\t%d - Auxiliary file: [\"%s\"]\n",           kAux_file, d->hist.aux_file );
	printf( "\t%d - voxel units [\"%s\"]\n",               kVox_units, d->dime.vox_units );
	printf( "\t%d - vox_offset [%f]\n",                    kVox_offset, d->dime.vox_offset );
	printf( "\t%d - calibration units [\"%s\"]\n",         kCal_units, d->dime.cal_units );
	printf( "\t%d - calibration max [%f] and min [%f]\n",  kCalMaxMin, d->dime.cal_max, d->dime.cal_min );
	printf( "\t%d - generated [\"%s\"]\n",                 kGenerated, d->hist.generated );
	printf( "\t---------\n\t%d. Main menu\n",              kMainMenu );
	
	scanf( "%d", MenuChoice );
	return noErr;
}
	
OSErr HandleMenuChoice( dsr *d, int *MenuChoice )
{
	OSErr error = noErr;
	char  myString[100];
	int   myInt;
	short myShort;
	short flags = d->dime.flags;
	char  orientString[16];
	int   stdType;	// for translation of bits/pixel
	
	if( *MenuChoice >= kLastMenuChoice ) {
		return noErr;
	}
	
	switch( *MenuChoice ) {
		case kMainMenu:
		case kSecondaryMenu:
		break;

		case kDescrip:
			printf( "Enter a new description (up to 79 characters):\n\t" );
			scanf( "\n" );
			gets( myString );
			strncpy( d->hist.descrip, myString, 79 );
			fprintf( u.ProcFile, "description changed to: %s\n", d->hist.descrip );
		break;
		
		case kPat_ID:
			printf( "Enter a patient ID (up to 9 characters):\n\t" );
			scanf( "\n" );
			gets( myString );
			strncpy( d->hist.patient_id, myString, 9 );
			fprintf( u.ProcFile, "Patient id changed to: %s\n", d->hist.patient_id );
		break;
		
		case kScannum:
			printf( "Enter a scan number (up to 9 characters):\n\t" );
			scanf( "\n" );
			gets( myString );
			strncpy( d->hist.scannum, myString, 9 );
			fprintf( u.ProcFile, "Scan number changed to: %s\n", d->hist.scannum );
		break;
		
		case kMat_size:
			printf( "Enter the number of pixels in x and y, separated by white space: " );
			scanf( "%hd%hd", &d->dime.dim[1], &d->dime.dim[2] );
			fprintf( u.ProcFile, "Matrix changed to: %hd x %hd\n", d->dime.dim[1], d->dime.dim[2] );
		break;
	
		case kSlices:
			printf( "Enter the number of slices: " );
			scanf( "%hd", &d->dime.dim[3] );
			fprintf( u.ProcFile, "Number of slices changed to: %hd\n", d->dime.dim[3] );
		break;
		
		case kTimePts:
			printf( "Enter the number of time points: " );
			scanf( "%hd", &d->dime.dim[4] );
			fprintf( u.ProcFile, "Number of time points changed to: %hd\n", d->dime.dim[4] );
		break;
		
		case kPixSize:
			printf( "Enter the pixel size in %s for x and y, separated by white space: ",
			       d->dime.vox_units );
			scanf( "%f%f", &d->dime.pixdim[1], &d->dime.pixdim[2] );
			fprintf( u.ProcFile, "Pixel size changed to: %f x %f\n", d->dime.pixdim[1], d->dime.pixdim[2] );
		break;
		
		case kThick:
			printf( "Enter the slice to slice spacing (center to center) in mm: " );
			scanf( "%f", &d->dime.pixdim[3] );
			fprintf( u.ProcFile, "Slice spacing changed to: %f\n", d->dime.pixdim[3] );
		break;
		
		case kTR:
			printf( "Enter the repetition time (msec): " );
			scanf( "%f", &d->dime.pixdim[4] );
			fprintf( u.ProcFile, "Repetition time changed to: %f\n", d->dime.pixdim[4] );
		break;
		
		case kData_type:
			printf( "Select the data type:\n" );
			printf( "\t1. Unknown\n" );
			printf( "\t2. Binary\n" );
			printf( "\t3. unsigned char\n" );
			printf( "\t4. signed short\n" );
			printf( "\t5. signed int\n" );
			printf( "\t6. float\n" );
			printf( "\t7. complex\n" );
			printf( "\t8. double\n" );
			printf( "\t9. RGB\n" );
			printf( "\t10. All\n\n" );
			
			scanf( "%d", &myInt );
			
			switch( myInt ) {
				case 1:  d->dime.datatype = DT_UNKNOWN;       stdType = T_USHORT; break;
				case 2:  d->dime.datatype = DT_BINARY;        stdType = T_USHORT; break;
				case 3:  d->dime.datatype = DT_UNSIGNED_CHAR; stdType = T_UCHAR;  break;
				case 4:  d->dime.datatype = DT_SIGNED_SHORT;  stdType = T_USHORT; break;
				case 5:  d->dime.datatype = DT_SIGNED_INT;    stdType = T_INT;    break;
				case 6:  d->dime.datatype = DT_FLOAT;         stdType = T_FLOAT;  break;
				case 7:  d->dime.datatype = DT_COMPLEX;       stdType = T_COMPLEX;break;
				case 8:  d->dime.datatype = DT_DOUBLE;        stdType = T_DOUBLE; break;
				case 9:  d->dime.datatype = DT_RGB;           stdType = T_USHORT; break;
				case 10: d->dime.datatype = DT_ALL;           stdType = T_USHORT; break;
				default: d->dime.datatype = DT_UNKNOWN;       stdType = T_USHORT; break;
			}
			d->dime.bitpix = 8 * get_datasize( stdType );
			fprintf( u.ProcFile, "Data type changed to: %s\n", ResolveDataType( d->dime.datatype) );
		break;

		case kBitPix:
			printf( "Enter the number of bits per pixel: " );
			scanf( "%hd", &d->dime.bitpix );
			fprintf( u.ProcFile, "Bits per pixel changed to: %hd\n", d->dime.bitpix );
		break;
		
		case kOrient:
			printf( "Select the Orientation:\n" );
			printf( "\t1. axial\n" );
			printf( "\t2. coronal\n" );
			printf( "\t3. sagittal\n" );
			printf( "\t4. axial flipped\n" );
			printf( "\t5. coronal flipped\n" );
			printf( "\t6. sagittal flipped\n" );
			
			scanf( "%d", &myInt );
			
			switch( myInt ) {
				case 1:  d->hist.orient = '0'; sprintf( orientString, "axial" );            break;
				case 2:  d->hist.orient = '1'; sprintf( orientString, "coronal" );          break;
				case 3:  d->hist.orient = '2'; sprintf( orientString, "sagittal" );         break;
				case 4:  d->hist.orient = '3'; sprintf( orientString, "axial flipped" );    break;
				case 5:  d->hist.orient = '4'; sprintf( orientString, "coronal flipped" );  break;
				case 6:  d->hist.orient = '5'; sprintf( orientString, "sagittal flipped" ); break;
				default: d->hist.orient = '0'; sprintf( orientString, "axial" );            break;
			}

			fprintf( u.ProcFile, "Orientation changed to: %s\n", orientString );
		break;
		
		case kGlobalMaxMin:
			printf( "Enter the global minimum and maximum, separated by white space: " );
			scanf( "%d%d", &d->dime.glmin, &d->dime.glmax );
			fprintf( u.ProcFile, "Global max changed to: %d\nGlobal min changed to: %d\n", 
			                      d->dime.glmax, d->dime.glmin );
		break;
		
		case kFlags:
			myShort = 0;
			printf( "Enter Y (Yes) or N (No) for each field:\n" );
			
			printf( "\tNeed interpolation in x [%s]: ", YorN( kNeedXInterp & flags ));
			scanf( "%s", &myString );
			if( myString[0]=='Y' || myString[0]=='y' ) {
				myShort += kNeedXInterp;
			}
			fprintf( u.ProcFile, "Interpolate in x:\t%s\n", YorN( kNeedXInterp & myShort ));
			
			printf( "\tNeed interpolation in y [%s]: ", YorN( kNeedYInterp & flags ));
			scanf( "%s", &myString );
			if( myString[0]=='Y' || myString[0]=='y' ) {
				myShort += kNeedYInterp;
			}
			fprintf( u.ProcFile, "Interpolate in y:\t%s\n", YorN( kNeedYInterp & myShort ));
			
			printf( "\tBig Endian byte order (like a Mac) [%s]: ", YorN( kmacByteOrder & flags ));
			scanf( "%s", &myString );
			if( myString[0]=='Y' || myString[0]=='y' ) {
				myShort += kmacByteOrder;
			}
			fprintf( u.ProcFile, "Big endian:\t%s\n", YorN( kmacByteOrder & myShort ));
			
			printf( "\tNeeds cropping in x [%s]: ", YorN( kNeedXCrop & flags ));
			scanf( "%s", &myString );
			if( myString[0]=='Y' || myString[0]=='y' ) {
				myShort += kNeedXCrop;
			}
			fprintf( u.ProcFile, "Crop in x:\t%s\n", YorN( kNeedXCrop & myShort ));
			
			printf( "\tNeed cropping in y [%s]: ", YorN( kNeedYCrop & flags ));
			scanf( "%s", &myString );
			if( myString[0]=='Y' || myString[0]=='y' ) {
				myShort += kNeedYCrop;
			}
			fprintf( u.ProcFile, "Crop in y:\t%s\n", YorN( kNeedYCrop & myShort ));
			
			printf( "\tSlices are in temporal order of acquisition [%s]: ", YorN( kTimeOrder & flags ));
			scanf( "%s", &myString );
			if( myString[0]=='Y' || myString[0]=='y' ) {
				myShort += kTimeOrder;
			}
			fprintf( u.ProcFile, "Slices are in time order:\t%s\n", YorN( kTimeOrder & myShort ));
			
			d->dime.flags = myShort;
		break;

		case kDate:
			printf( "Enter the experiment date as text [mm/dd/yyyy]: " );
			scanf( "%s", myString );
			strncpy( d->hist.exp_date, myString, 9 );
			fprintf( u.ProcFile, "Experiment date changed to: %s\n", d->hist.exp_date );
		break;
		
		case kTime:
			printf( "Enter the experiment time as text [hh:mm:ss]: " );
			scanf( "%s", myString );
			strncpy( d->hist.exp_time, myString, 9 );
			fprintf( u.ProcFile, "Experiment time changed to: %s\n", d->hist.exp_time );
		break;
		
		case kTextDataType:
			printf( "Enter the data type as text [9 characters]: " );
			scanf( "\n" );
			gets( myString );
			strncpy( d->hk.data_type, myString, 9 );
			fprintf( u.ProcFile, "Text data type changed to: %s\n", d->hk.data_type );
		break;
		
		case kDB_name:
			printf( "Enter the db name [17 characters]: " );
			scanf( "\n" );
			gets( myString );
			strncpy( d->hk.db_name, myString, 17 );
			fprintf( u.ProcFile, "db_name changed to: %s\n", d->hk.db_name );
		break;
		
		case kViews:
			printf( "Enter the number of views: " );
			scanf( "%d", &d->hist.views );
			fprintf( u.ProcFile, "Number of views changed to: %d\n", d->hist.views );
		break;
		
		case kVols_added:
			printf( "Enter the number of volumes added: " );
			scanf( "%d", &d->hist.vols_added );
			fprintf( u.ProcFile, "Number of volumes added changed to: %d\n", d->hist.vols_added );
		break;
		
		case kOriginator:
			printf( "Enter the originator [9 characters]: " );
			scanf( "\n" );
			gets( myString );
			strncpy( d->hist.originator, myString, 9 );
			fprintf( u.ProcFile, "originator changed to: %s\n", d->hist.originator );
		break;
		
		case kAux_file:
			printf( "Enter the name of the auxiliary file [23 characters]: " );
			scanf( "\n" );
			gets( myString );
			strncpy( d->hist.aux_file, myString, 23 );
			fprintf( u.ProcFile, "Auxiliary file changed to: %s\n", d->hist.aux_file );
		break;
		
		case kVox_units:
			printf( "Enter the name of the voxel units [3 characters]: " );
			scanf( "%s", myString );
			strncpy( d->dime.vox_units, myString, 3 );
			fprintf( u.ProcFile, "Voxel units changed to: %s\n", d->dime.vox_units );
		break;
		
		case kVox_offset:
			printf( "Enter the voxel offset: " );
			scanf( "%f", &d->dime.vox_offset );
			fprintf( u.ProcFile, "Voxel offset changed to: %f\n", d->dime.vox_offset );
		break;
		
		case kCal_units:
			printf( "Enter the name of the calibration units [7 characters]: " );
			scanf( "%s", myString );
			strncpy( d->dime.cal_units, myString, 7 );
			fprintf( u.ProcFile, "Calibration units changed to: %s\n", d->dime.cal_units );
		break;
		
		case kCalMaxMin:
			printf( "Enter the calibration min and max, separated by white space: " );
			scanf( "%f%f", &d->dime.cal_min, &d->dime.cal_max );
			strncpy( d->dime.cal_units, myString, 7 );
			fprintf( u.ProcFile, "Calibration min and max changed to:\t%f - %f\n",
			                      d->dime.cal_max, d->dime.cal_max );
		break;
		
		case kGenerated:
			printf( "Enter the text for 'generated'[9 characters]: " );
			scanf( "%s", myString );
			strncpy( d->hist.generated, myString, 9 );
			fprintf( u.ProcFile, "generated changed to: %s\n", d->hist.generated );
		break;

	}

	return error;
}

/******************************  EditAnalyzeHeader  ***********************************/
OSErr EditAnalyzeHeader( dsr *d )
{
	OSErr error = noErr;
	int   MenuChoice=1;
	
	while( MenuChoice ) {
		if(   (MenuChoice < kDate && MenuChoice != kSecondaryMenu)
		   || MenuChoice == kMainMenu ) {			
			error = PrintMainMenu( d, &MenuChoice );
		} else {
			error = PrintSecondaryMenu( d, &MenuChoice );
		}
		RETURNONERROR;

		error = HandleMenuChoice( d, &MenuChoice );
		RETURNONERROR;
	}

	return noErr;
}

/******************************    ProcessCommandLine     ***************************
*   capture the user input, mostly globals
*************************************************************************************/
OSErr ProcessCommandLine( int argc, char *argv[] )
{
	OSErr error = noErr;
	int argp = EOF;
	short argsProcessed = 0;
	char  response[4];

	u.Verbose = false;
	
	while ( (argp=getopt(argc,argv,"i:o:Vh?OPTIONS")) != EOF )
	switch( argp )   {
		case 'o':
			strcpy( u.OutHeaderName, optarg );
		break;

		case 'i':
			strcpy( u.InHeaderName, optarg );
		break;

		case 'V':
			u.Verbose = true;
		break;

		case 'h':
		case '?':
		default:
			print_usage( argv[0] );
			exit( -1 );
	}
	
/* check for required arguments */
	if( argc<2 ) {
		print_usage( argv[0] );
		exit( -1 );
	}

	if( !strlen( u.InHeaderName )) {
		sprintf( u.InHeaderName, u.OutHeaderName );
		u.CreateNewHeader = true;
	} else {	// no output header specified. Make one
		u.CreateNewHeader = false;
	}

/* make sure that the input ends in .hdr */
	if( strrchr( u.InHeaderName, '.' ) == NULL  ||
		strcmp( strrchr( u.InHeaderName, '.' ), ".hdr" )) {
		strcat( u.InHeaderName, ".hdr" );
	}

/* make sure we have a valid header name for output */
	if( !strlen( u.OutHeaderName )) {
		printf( "No output header specified. Over-write %s?: ", u.InHeaderName );
		scanf( "%s", response );
		if( response[0]=='Y' || response[0]=='y' ) {
			sprintf( u.OutHeaderName, u.InHeaderName );
		} else {
			exit( -1 );
		}
	} else {
		if( strrchr( u.OutHeaderName, '.' ) == NULL  ||
			strcmp( strrchr( u.OutHeaderName, '.' ), ".hdr" )) {
			strcat( u.OutHeaderName, ".hdr" );
		}
	}

	return error;
}

/******************************************/

int main( int argc, char **argv)
{
	OSErr   error = noErr;
	static  char id[] = "$Revision: 1.4 $";
	dsr     myDSR;
	char    response[4];

#ifdef MAC
	SIOUXSettings.autocloseonquit = false;
	SIOUXSettings.asktosaveonclose = false;
	argc = ccommand(&argv);
#endif

	error = ProcessCommandLine( argc, argv );
	ILError( error, "error in command line" );

	error = OpenProcFile( argc, argv, u.OutHeaderName, id, &u.ProcFile );
	ILError( error, "Opening proc file" );

/* create, or read in, the input image header */
	if( u.CreateNewHeader ) {
		IMAGE im;

		InitializeIm( &im );
		error = dsrFromIMAGE( &im, &myDSR );
		ILError( error, "Creating new analyze header" );

		fprintf( u.ProcFile, "No input header specified - %s created\n", u.OutHeaderName );

		printf( "Populate %s with default header contents (y/n)?: ", u.OutHeaderName );
		scanf( "%s", response );
		if( response[0] == 'y' || response[0] == 'Y' ) {
			error = CreateDSRDefaults( &myDSR );
			ILError( error, "Entering default header contents" );
			fprintf( u.ProcFile, "%s filled with default contents\n", u.OutHeaderName );
		}
	} else {
		error = ck_fopen( &u.inFile, u.InHeaderName,  "rb" );
		ILError( error, "Opening infile" );

		error = ck_fread( &myDSR, sizeof( dsr ), 1, u.inFile );
		ILError( error, "Reading input header" );
	
		error = ck_fclose( u.inFile );
		ILError( error, "Closing input file" );
	}
	
	error = EditAnalyzeHeader( &myDSR );
	ILError( error, "Editing header" );

	error = ck_fopen( &u.outFile, u.OutHeaderName,  "wb" );
	ILError( error, "Opening outfile" );
	
	error = ck_fwrite( &myDSR, sizeof( dsr ), 1, u.outFile );
	ILError( error, "Writing output file" );
	
	error = ck_fclose( u.outFile );
	ILError( error, "Closing output file" );
	
	fprintf( u.ProcFile, "\n" );
	ck_fclose( u.ProcFile );

	system( "clear" );
	return 0;
}

/* </pre></body></html> */

