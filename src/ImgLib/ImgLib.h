/* <HTML><HEAD><TITLE>ImgLib routines</TITLE></HEAD><BODY><PRE>
/***************************************************************************
*	<font size=+2><b>ImgLib.h</b></font>
*	<i>Please note saving date below:</i>
*	<font size=+1><b>$Date: 2008/05/16 17:19:20 $</b></font><p>
*	Part of ImgLib
*   A set of primitives for use in image processing.
*               c 1996-2002  Mark S. Cohen
*   This software is distributed as is with no guarantees.
*   Please report any errors or bug fixes to mscohen@ucla.edu
*
*   For academic use only. Commercial users should contact
*   Mark Cohen for licensing information.
*
<table border="1" cellspacing="2" cellpadding="4">
<tr align="center">
<th><a href="#Preamble"><font size="+1" color="#cc0000">Preamble</font></a></th>
<th><a href="#UCHeader"><font size="+1" color="#cc0000">UCHeaders</font></a></th>
<th><a href="#GenesisHeader"><font size="+1" color="#cc0000">Genesis Headers</font></a></th>
<th><a href="#Analyze"><font size="+1" color="#cc0000">Analyze</font></a></th>
<th><a href="#Functions"><font size="+1" color="#cc0000">Functions</font></a></th>
</tr>
</table>
<!-- *************************** rcs logs ****************************************/
/**********************************************************************************
* $Author: mscohen $ $Log: ImgLib.h,v $
* $Author: mscohen $ Revision 2.55  2008/05/16 17:19:20  mscohen
* $Author: mscohen $ Added function CalcT2
* $Author: mscohen $
* $Author: mscohen $ Revision 2.49  2006/03/26 06:08:28  mscohen
* $Author: mscohen $ Added SetImDatatype accessor function
* $Author: mscohen $
* $Author: mscohen $ Revision 2.51  2006/03/25 08:50:42  mscohen
* $Author: mscohen $ Only formatting (I think)
* $Author: mscohen $
* $Author: mscohen $ Revision 2.49  2005/05/06 16:31:05  mscohen
* $Author: mscohen $ Changed convention for tr in analyze struct to seconds (was microseconds).
* $Author: mscohen $
* $Author: mscohen $ Revision 2.48  2003/01/22 20:42:22  mscohen
* $Author: mscohen $ Changed comments regarding scanspacing
* $Author: mscohen $
* $Author: mscohen $ Revision 2.47  2003/01/15 02:02:34  mscohen
* $Author: mscohen $ Updated comments regarding analyze file dimensions.
* $Author: mscohen $
* $Author: mscohen $ Revision 2.46  2002/09/10 20:36:55  mscohen
* $Author: mscohen $ added afni type, zrinka
* $Author: mscohen $
* $Author: mscohen $ Revision 2.45  2002/06/26 13:10:55  mscohen
* $Author: mscohen $ Better commenting on pixel dimension process.
* $Author: mscohen $
 * Revision 2.44  2002/06/06  03:53:13  mscohen
 * Added UCFlagToString
 *
 * Revision 2.43  2002/05/13  15:55:58  mscohen
 * Changed call to OpenProcFile to support standard error handler.
 *
 * Revision 2.42  2002/04/25  02:12:55  mscohen
 * Added hooks for Varian image types - no functions yet.
 *
 * Revision 2.40  2002/04/19  03:46:45  mscohen
 * Rebuilt offset table for genesis data to make it more closely resemble GE format
 * Added html to improve file searching.
 *
 * Revision 2.39  2002/04/16  17:02:02  mscohen
 * copyright
 *
 * Revision 2.38  2002/04/16  15:54:23  mscohen
 * Improved html.
 *
 * Revision 2.37  2002/04/16  15:10:03  mscohen
 * Added ImageType to UCHeader
 *
 * Revision 2.35  2001/12/04  06:19:10  mscohen
 * html cleaner
 *
 * Revision 2.34  2001/12/04  04:24:16  mscohen
 * Changed parameter data types in FindOutputType.
 *
 * Revision 2.32  2001/11/25  20:23:57  mscohen
 * Various changes to genesis header labels
 * Added se_nacq to IMAGE header.
 *
 * Revision 2.31  2001/10/18  04:21:15  mscohen
 * Renamed 'overlay' to 'Overlay' to eliminate Mach name conflict.
 *
 * Revision 2.30  2001/08/24  20:31:58  zrinka
 * changed calling sequence for GetSelectedVolume,
 * UC_ReadImage and UC_ReadImage2D - removed crop flag.
 *
 * Revision 2.29  2001/08/24  16:45:44  mscohen
 * added html commenting
 *
 * Revision 2.28  2001/08/23  20:01:34  mscohen
 * Revised several fields of uc_header
 * Added offset tables for genesis.h to support translations
 *
 * Revision 2.27  2001/03/17  04:06:38  mscohen
 * Added notes about BigEndian/LittleEndian
 *
 * Revision 2.32  2000/11/02  18:01:57  mscohen
 * Added ImageFlipY
 *
 * Revision 2.31  2000/10/19  15:36:09  mscohen
 * Removed FTP functions from ImgLib
 *
 * Revision 2.30  2000/08/28  21:54:50  mscohen
 * removed rows and cols from command line of mdisplay
 *
 * Revision 2.29  2000/08/24  15:30:00  mscohen
 *  new call line for unmdisplay
 *
 * Revision 2.28  2000/08/23  15:19:47  mscohen
 * added function ReportFileType
 * added function CreateHeader
 * added function FindOutputType
 * TimePt is now int in readimage
 * added FTPRead function (not completed)
 *
 * Revision 2.25  2000/05/08  17:31:02  mscohen
 * Modified call line for UC_Readheader (MAC)
 *
 * Revision 2.22  2000/03/26  11:51:04  mscohen
 * Compatibility with ImgLib.c v 2.28
 *
 * Revision 2.19  2000/02/15  15:55:04  mscohen
 * Rename BigEndian and LittleEndian functions
 *
 * Revision 2.18  1999/09/28  04:09:49  mscohen
 * Fixed set and clear flags for handing byte order
 *
 * Revision 2.17  1999/09/22  22:23:00  mscohen
 * Minor mods
 *
 * Revision 2.16  1999/09/22  15:25:14  mscohen
 * United various header files into a single ImgLib.h
 * Added OpenProcFile.
 *
 * consolidate UCHeader, genesis, analyzeUtil
 * Revision 2.15  1999/08/20  21:04:30  mscohen
 * Rev. synch.
 *
 * Revision 2.14  1999/08/09  22:17:22  mscohen
 * maintenance - keep revisions in synch.
 *
 * Revision 2.14  1999/08/09  22:17:22  mscohen
 * maintenance -  keep revisions in synch.
 *
 * Revision 2.12  1999/07/14  05:50:48  mscohen
 * Modified to make ANSI C compaitible
 *
 * Revision 2.10  1999/05/28  22:16:16  mscohen
 * Keep rev levels consistent with ImgLib.c
 *
 * Revision 2.10  1999/05/28  22:16:16  mscohen
 * Keep rev levels consistent with ImgLib.c
 *
 * Revision 2.91  1999/04/28  17:40:59  mscohen
 * Version consistency
 *
 * Revision 2.9  1999/04/28  17:40:59  mscohen
 * Version consistency
 *
 * Revision 2.8  1999/04/28  17:16:45  mscohen
 * Maintenance to keep rev levels consistent
 *
 * Revision 2.7  1999/04/20  00:05:10  mscohen
 * Big improvements to Readimage and type con. Modified global error handling
 *
 * Revision 2.6  1999/04/13  00:07:40  mscohen
 * Check in of modified Mac version
 *
 * Revision 2.4  1999/02/28  21:07:40  mscohen
 * extracted components of VLib
 *
* 
*    ImgLib.h
*
*************************************************************************-->********/
/* <a name="Preamble"> */
#pragma once
#ifndef __IMGLIBH__
#define __IMGLIBH__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <time.h>
#include <limits.h>
#include <errno.h>

#include "Target.h"
#include "VLib.h"

#ifdef SPLINE
#include "spline.h"
#endif

#include "ImgLibError.h"

#ifdef MAC
#include <files.h>
#endif

#ifndef OSErr
#define OSErr  short
#endif
#ifndef noErr
#define noErr 0
#endif

enum {
	No_TYPE  =   noErr,  /* at the present, 6 types of image files are used */
	BSHORT,	    /* 1 */
	BFLOAT,     /* 2 */
	BUCHAR,     /* 3 */
	EPI,        /* 4 */
	ANALYZE,    /* 5 */
	SIGNA,      /* 6 */
	VARIAN,     /* 7 Varian files are not yet supported; this is a placeholder */
    AFNI,       /* 8 added on 08-14-02, zrinka */	
    LAST_TYPE   /* last is last valid plus one */
};

enum	/* overlay Modes */
{
	kPosNeg = 1,
	kPosOnly,
	kNegOnly,
	kDual,
	kTriple
};

enum	/* conversion modes used in FloatToCImage */
{
	kAutoRange = 1,	/* automatically determine best window/level */
	kFileMinMax,        /* use the min and max values in the file */
	kManualRange,       /* use the min and max passed as arguments */
	kNoScale            /* don't convert the file values */
};

enum	/* return values from header conversions */
{
	IMAGE_MAX_OUT_OF_RANGE = 1,
	IMAGE_MIN_OUT_OF_RANGE = 2
};

/* TYPE DEFINITIONS */
/* 9/13/97 wls: added notion of image trailer & started fix of .img trailer */

/* MGH Headers (.bfloat, .buchar, .bshort ) are interpreted as:
	ysize xsize zsize macByteOrder
i.e., if the last value is non-zero, the data are macByteOrder, meaning foreign w.r.t. digital
*/
#define ANMRHDRSIZE  0
#define ANALYZEHDRSIZE 0
#define ANMRTRLSIZE  16
#define SIGHDRSIZE 7904
#define SIGTRLSIZE 0

#define POSMAX 239        /* magic numbers for 8 bit mac display */
#define POSMIN 224
#define NEGMAX 255
#define NEGMIN 240
#define BMAX   223        /* magic color table cutoff values for overlay images */
#define BMIN 0

#define kNoSmooth  0      /* these values are chosen so that a transition can be made to Fourier smoothing */
#define kSmooth9   3
#define kSmooth25  5

#define APD2HDRSIZE 0		/* BYTES (not pixels) in front of each ANMR image */
#define APD2TRLSIZE 16		/* BYTES (not pixels) behind of each ANMR image */

/***<a name="UCHeader">***  contents of UCHeader.h **********************
*	UCHeader.h
*	An attempt to create a "universal" cross platform and modality header
*
*	This proposed structure has four components:
*
*	subj   -	Patient/subject demographic data
*	dim    -	Image (pixelwise) dimensional information
*	loc    -	Image location data in instrument and patient coordinates
*	acq    -	Modality-specific information
 
ANMR images very typically are created with rectangular
FoV's containing data only in the center of the images. Generally, we assume
that cropping is done on the outside edges. For example, in an 8 by 4 image,
where 'O' is a valid pixel and 'x' is out of the useful field of view, the
images look like:

	xxOOOOxx
	xxOOOOxx
	xxOOOOxx
	xxOOOOxx

In our code, the image dimensions are in _valid_ (within the useful field of
view) pixels.

To support Real Time imaging, we also create images that have anisotropic
pixels, and perform the interpolation to isotropy later. This forces the need
for separate variables for the number of pixels/line (im.dim.x and im.dim.y)
and the number of _isotropic_ pixels (im.dim.isoX and im.dim.isoY)

**************************************************************************/
#ifdef ALPHA
#define LONG int
#else
#define LONG long
#endif

#ifndef Bool
#define Bool short	/* to accomodate modern struct alignments */
#endif

enum {	/* display modes */
	kX             = 1,	/* image has been interpolated in X */
	kY             = 2,	/* image has been interpolated in Y */
	kBilinear      = 4,
	kTrilinear     = 8,
	kSinc          = 16,
	kReplication   = 32,
	kMDisplay      = 64,
	kVolDisplay    = 128,
	kFlipTopBottom = 256,	/* flip needed to place image in standard view */
	kFlipLeftRight = 512 /* which is Superior or anterior on top, anterior or pt right on left */
};

enum {	/* color table types */
	kGray256 = 1,	/* 256 gray levels */
	kHotColdSS = 2,	/* scanSTAT hot-cold scale: 0-POSMIN gray; POSMIN-POSMAX hot; NEGMIN-255 cold */
	kHotCold256 = 4	/* PET coloring: 0-127 hot; 128-255 cold */
};

enum { 	/* imageType */
	MAGNITUDE = 0,
	PHASE,
	REAL,
	IMAGINARY
};

typedef struct
{
	char	patname[24];	/* Patient Name */
	char	patid[16];      /* Hospital-type Patient ID for this Exam */
	char	reqnum[12];     /* Requisition Number */
	char	HumanStudiesID[32];	/* tracking for human research subjects */
	char	ResearchProtocol[32];	/* name of research project */
	short	patage;         /* Patient Age (days) */
	Bool	male;           /* Patient Sex */
	LONG	patweight;      /* Patient Weight in grams */
	char	descr[128];     /* Scan description */
	char	refphy[32];     /* Referring Physician */
	char	diagrad[32];    /* Diagnostician/Radiologist */
	char	ex_op[32];      /* Operator */
	char	hospname[32];   /* Hospital Name */
	char	sysID[16];      /* identifying data for instrument */
	char	aux_file[32];   /* more data? */
	char    reserved[4];
} DEMOGRAPHICS;	// 432 bytes

typedef struct
{
	int   x;           /* image x dimension -- number of _valid_ pixels - see note above */
	int   y;           /* image y dimension -- number of _valid_ pixels */
	short	BytesPix;    /* bytes/pixel */
	int   isoX;        /* x pixels after interpolation to isotropy */
	int   isoY;        /* y pixels after interpolation to isotropy */
	short	x_skip;      /* no. of invalid pixels on left (and right) of image */
	short	y_skip;      /* no. of invalid lines on top of image */
	float	dfov;        /* Display field of view - X (mm) */
	float	dfov_rect;   /* Display field of view - Y (if different) */
	float	dpixsize_X;  /* Display pixel size (mm) - X */
	float	dpixsize_Y;  /* Display pixel size (mm) - Y */
	short	acq_Width;   /* Acquired width in pixels */
	short	acq_Height;  /* Acquired height in pixels */
	float	acqfov;      /* Acquired FOV (mm) - X */
	float	acqfov_rect; /* Acquired FOV (mm) - Y (if different) */
	float	acqpixsize_X;    /* Acquired pixel size (mm) - X */
	float	acqpixsize_Y;    /* Acquired pixel size (mm) - Y */
	float	slthick;     /* float - Slice Thickness (mm) */
	float	scanspacing; /* Spacing between scans (mm) slice center to center */
	short	n_slices;       /* Number of slices in this file */
	short   n_acquired_slices;  /* number of slices acquired in this run */
	int	timePts;     /* Number of timePts (volumes) */
	short	DispMode;	 /* defined above */
	short	ColorTable;  /* from above defined types */
	short	ImageFlip;
	short   ImageType;
	unsigned LONG	flags;
/*		enum
		{
		   kNeedXInterp  = 1,
		   kNeedYInterp  = 2,
		   kmacByteOrder = 4,
		   kNeedXCrop    = 8,
		   kNeedYCrop    = 16,
		   kTimeOrder    = 32
		} ucla_flags;
*/
	char    reserved[8];
} DIMENSIONS;   // 96 bytes

typedef struct
{
/* Idea borrowed from GE RAS is Right-Anterior-Superior location */
	char	loc_ras;	/* RAS letter of image location */
	float	loc;		/* float - Image location */
	float	imgCtr[3];	/* RAS coord of plane image */
	float	imgNorm[3];	/* Normal RAS coord */
	float	tlhc[3];	/* RAS Coord of Top Left Hand Corner of First Image */
	float	trhc[3];	/* RAS Coord of Top Right Hand Corner of First Image */
	float	brhc[3];	/* RAS Coord of Bottom Right Hand Corner of last image in volume */
	float	tlhc_tal[3];	/* Talairach Coord of Top Left Hand Corner of First Image  */
	float	trhc_tal[3];	/* Talairach Coord of Top Right Hand Corner of First Image */
	float	brhc_tal[3];	/* Talairach Coord of Bottom Right Hand Corner of last image in volume */
	float	AC[3];          /* RAS coord of the Anterior commisure */
	float	PC[3];          /* RAS coord of the Posterior commisure */
	float   SP[3];          /* RAS coord of the Superior pole */
	float   IP[3];          /* RAS coord of the Inferior pole */
	float   AP[3];          /* RAS coord of the Anterior pole */
	float   OP[3];          /* RAS coord of the Occipital pole */
	char    reserved[16];
} LOCATIONS;   // 192 bytes

typedef struct
{
	short   orig_datatype;  /* data type of stored image */
	short	im_exno;        /* Exam Number for this image */
	short	im_seno;        /* Series Number for this image */
	short	im_no;          /* Image Number */
	LONG	im_datetime;    /* Allocation Image date/time stamp */
	LONG	im_actual_dt;   /* Actual Image date/time stamp */
	float	sctime;         /* Duration of scan (secs) */
	short	contrast;       /* Non-zero if > 0 image used contrast */
	char	contrastIV[16];        /* IV Contrast Agent */
	char	contrastOral[16];      /* Oral Contrast Agent */
	short	se_sortorder;   /* Image Sort Order (L/S) */
	LONG	tr;             /* Pulse repetition time (usec) */
	LONG	ti;             /* Pulse inversion time(usec) */
	LONG	te;             /* Echo time(usec) */
	short	mr_flip;	    /* Flip Angle for GRASS scans (deg.) */
	short	numecho;        /* Number of echoes in this series */
	short	echonum;        /* Echo Number of this image */
	float	tbldlta;        /* Table Delta */
	float	nex;            /* Number of Excitations */
	short   nPhaseLines;    /* Number of phase encode acquired */
	short   nReadPoints;     /* Number of readout points acquired per line */
	short   interleave;     /* treat as Bool - images in interleaved order */
	short	contig;         /* Continuous Slices Flag */
	short	hrtrate;        /* Cardiac Heart Rate (bpm) */
	LONG	tdel;           /* Delay time after trigger (msec) */
	float	reptime;	    /* Cardiac repetition time */
	short	imgpcyc;	    /* Images per cardiac cycle */
	short	cphase;         /* Total Cardiac Phase prescribed */
	short	swappf;         /* Swap Phase/Frequency Axis */
	LONG	iopt;           /* Imaging Options */
	short	imode;          /* Imaging Mode */
	char	psdname[32];    /* Pulse Sequence Name */
	LONG	psd_datetime;   /* PSD Creation Date and Time */
	short	ctyp;           /* Coil Type */
	char	cname[16];      /* Coil Name */
	float	vbw;            /* Variable Bandwidth (Hz) */
	LONG	timeVol;        /* time between volumes, typically TR */
	LONG	intr_del;       /* Interimage/interloc delay (uSec) */
	short	image_type;     /* Magnitude, Phase, Imaginary, or Real */
	short	echo_trn_len;   /* Echo Train Length for Fast Spin Echo */
	short	frac_echo;      /* Fractional Echo - Effective TE Flag */
	short	prep_pulse;     /* Preporatory Pulse Option */
	short	cphasenum;      /* Cardiac Phase Number */
	short   xmtgain;        /* MRI transmit gain (dB) */
	short   rec1;           /* MRI receiver gain 1 and 2 */
	short   rec2;
	LONG	fphase;         /* Number Of Phases */
	short	freq_dir;       /* Frequency Direction */
	char	prtcl[24];      /* protocol name */
	char	psd_iname[12];  /* PSD name from inside PSD */
	int     fieldStrength;	/* magnetic field strength in gauss */
	short   se_nacq;        /* number of acquisitions (needed for slice coverage) */
	char    reserved[12];
} ACQDATA;	// 256 bytes

typedef struct
{
	char	MagicNumber[4];	/* Identifying type of header == ScST */
	LONG    im_header;      /* header size before each image */
	LONG    im_trailer;     /* trailer size after each image */
	LONG    all_header;     /* header size before all images */
	LONG    all_trailer;    /* trailer size after all images */
	short	file_type;      /* Signa, EPI, Analyze, etc... */
	short   fileData_type;  /* data type of data in file */
	short	data_type;      /* data type of data in memory (may differ from file) */
	float	theMinf;        /* minimum pixel intensity in the volume and time series */
	float	theMaxf;        /* maximum pixel intensity in the volume and time series */
	DIMENSIONS	dim;
	DEMOGRAPHICS	subj;	/* each struct padded to be an integer multiple of 8 for alignment */
	LOCATIONS	loc;
	ACQDATA	acq;
	char	RawHdr[127];	/* original header file name, if it exists */
	char	ImageName[127];	/* original file name */
	char    reserved[10];
#ifdef MAC
	Handle  imData;
#else
	void    *imData;
#endif
}	IMAGE;	
/***********************   ^^^  UCHeader.h ^^^   **************************/

/***<a name="GenesisHeader">****** contents of genesis.h ******************/
/* this is the structure definition for a Signa genesis header */

typedef struct VARTYP {
        unsigned long   length; /* length of the data */
        char *          data;   /* pointer to the data */
}  VARTYPE;

typedef struct {
	float	R;
	float   A;
	float   S;
} RAS;

typedef struct {
	float          Alpha;
	float          Beta;
	unsigned short Window;
	short          Level;
} ABWL;

/* Offsets into the image header. Some have been altered to point to arrays, RAS or ABWL structs
   in order to make the definitions more compact */

/*  ImageFile PixHdr definitions  */

#define _IH_img_magic      0x0     /* 0 - int - magic number */
#define _IH_img_hdr_length 0x4     /* 4 - int - a byte displacement to the <pixel data area> */
#define _IH_img_width      0x8     /* 8 - int - width (pixels) of image */
#define _IH_img_height     0xc     /* 12 - int - height (pixels) of image */
#define _IH_img_depth      0x10    /* 16 - int - depth (1, 8, 16, or 24 bits) of */
#define _IH_img_compress   0x14    /* 20 - int - type of compression; see IC_* below */

#define IC_AsIs         0       /* No image transformation */
#define IC_RECT         1       /* image is width*height*depth/8 bytes */
#define IC_PACKED       2       /* image is CT line mapped packed */
#define IC_COMPRESSED   3       /* image is compressed */
#define IC_COMPACK      4       /* image is compressed and packed */

/*  ImgHst definitions  */

#define _HS_hs_version   0x0     /* 0 - int - version of the histogram structure */
#define _HS_hs_sd        0x4     /* 4 - float - standard deviation of pixel data value */
#define _HS_hs_mean      0x8     /* 8 - short - rounded mean pixel data value */
#define _HS_hs_min       0xa     /* 10 - short - minimum pixel data value */
#define _HS_hs_max       0xc     /* 12 - short - maximum pixel data value */
#define _HS_hs_first     0xe     /* 14 - short - first histogram entry used for <hs_sd> */
#define _HS_hs_region    0x10    /* 16 - short - region entries used for <hs_sd> */
#define _HS_hs_length    0x12    /* 18 - short - number of bins in the histogram area */
#define _HS_hs_bins      0x814   /* 2068 - u_short[1024] - histogram bins */

/*  SUITEDATATYPE definitions  */

#define _SU_su_id        0x0     /* 0 - char[4] - Suite ID */
#define _SU_su_uniq      0x4     /* 4 - short - Make Unique Flag */
#define _SU_su_diskid    0x6     /* 6 - char[1] - Disk ID */
#define _SU_prodid       0x7     /* 7 - char[13] - Product ID */
#define _SU_su_verscre   0x14    /* 20 - char[2] - Original Version of Record */
#define _SU_su_verscur   0x16    /* 22 - char[2] - Current Version of Record */
#define _SU_su_checksum  0x18    /* 24 - u_int - Suite Record Checksum */
#define _SU_su_padding   0x1c    /* 28 - char[85] - Spare Space */
#define _SU_sizeof       0x72    /* 114 - sizeof SUITEDATATYPE */

/*  EXAMDATATYPE definitions  */

#define _EX_ex_suid      0x0     /* 0 - char[4] - Suite ID for this Exam */
#define _EX_ex_uniq      0x4     /* 4 - short - Make-Unique Flag */
#define _EX_ex_diskid    0x6     /* 6 - char - Disk ID for this Exam */
#define _EX_ex_no        0x8     /* 8 - u_short - Exam Number */
#define _EX_hospname     0xa     /* 10 - char[33] - Hospital Name */
#define _EX_detect       0x2c    /* 44 - short - Detector Type */
#define _EX_numcells     0x2e    /* 46 - int - Number of cells in det */
#define _EX_zerocell     0x32    /* 50 - float - Cell number at theta */
#define _EX_cellspace    0x36    /* 54 - float - Cell spacing */
#define _EX_srctodet     0x3a    /* 58 - float - Distance from source to detector */
#define _EX_srctoiso     0x3e    /* 62 - float - Distance from source to iso */
#define _EX_tubetyp      0x42    /* 66 - short - Tube type */
#define _EX_dastyp       0x44    /* 68 - int - DAS type */
#define _EX_num_dcnk     0x46    /* 70 - short - Number of Decon Kernals */
#define _EX_dcn_len      0x48    /* 72 - short - Number of elements in a Decon Kernal */
#define _EX_dcn_density  0x4a    /* 74 - short - Decon Kernal density */
#define _EX_dcn_stepsize 0x4c    /* 76 - short - Decon Kernal stepsize */
#define _EX_dcn_shiftcnt 0x4e    /* 78 - short - Decon Kernal Shift Count */
#define _EX_magstrength  0x50    /* 80 - int - Magnet strength (in gauss) */
#define _EX_patid        0x54    /* 84 - char[13] - Patient ID for this Exam */
#define _EX_patname      0x61    /* 97 - char[25] - Patient Name */
#define _EX_patage       0x7a    /* 122 - short - Patient Age (years, months or days) */
#define _EX_patian       0x7c    /* 124 - short - Patient Age Notation */
#define _EX_patsex       0x7e    /* 126 - short - Patient Sex */
#define _EX_patweight    0x80    /* 128 - int - Patient Weight */
#define _EX_trauma       0x84    /* 132 - short - Trauma Flag */
#define _EX_hist         0x86    /* 134 - char[61] - Patient History */
#define _EX_reqnum       0xc3    /* 195 - char[13] - Requisition Number */
#define _EX_ex_datetime  0xd0    /* 208 - int - Exam date/time stamp */
#define _EX_refphy       0xd4    /* 212 - char[33] - Referring Physician */
#define _EX_diagrad      0xf5    /* 245 - char[33] - Diagnostician/Radiologist */
#define _EX_op           0x116   /* 278 - char[4] - Operator */
#define _EX_ex_desc      0x11a   /* 282 - char[23] - Exam Description */
#define _EX_ex_typ       0x131   /* 305 - char[3] - Exam Type */
#define _EX_ex_format    0x134   /* 308 - short - Exam Format */
#define _EX_firstaxtime  0x136   /* 310 - double - Start time(secs) of first axial in exam */
#define _EX_ex_sysid     0x13e   /* 318 - char[9] - Creator Suite and Host */
#define _EX_ex_lastmod   0x148   /* 328 - int - Date/Time of Last Change */
#define _EX_protocolflag 0x14c   /* 332 - short - Non-Zero indicates Protocol Exam */
#define _EX_ex_alloc_key 0x14e   /* 334 - char[13] - Process that allocated this record */
#define _EX_ex_delta_cnt 0x15c   /* 348 - int - Indicates number of updates to header */
#define _EX_ex_verscre   0x160   /* 352 - char[2] - Version - Created */
#define _EX_ex_verscur   0x162   /* 354 - char[2] - Version - Now */
#define _EX_ex_checksum  0x164   /* 356 - u_int - Exam Record Checksum */
#define _EX_ex_complete  0x168   /* 360 - int - Exam Complete Flag */
#define _EX_ex_seriesct  0x16c   /* 364 - int - Last Series Number Used */
#define _EX_ex_numarch   0x170   /* 368 - int - Number of Series Archived */
#define _EX_ex_numseries 0x174   /* 372 - int - Number of Series Existing */
#define _EX_ex_series    0x178   /* 376 - VARTYPE - Series Keys for this Exam */
#define _EX_ex_numunser  0x180   /* 384 - int - Number of Unstored Series */
#define _EX_ex_unseries  0x184   /* 388 - VARTYPE - Unstored Series Keys for this Exam */
#define _EX_ex_toarchcnt 0x18c   /* 396 - int - Number of Unarchived Series */
#define _EX_ex_toarchive 0x190   /* 400 - VARTYPE - Unarchived Series Keys for this Exam */
#define _EX_ex_prospcnt  0x198   /* 408 - int - Number of Prospective/Scout Series */
#define _EX_ex_prosp     0x19c   /* 412 - VARTYPE - Prospective/Scout Series Keys for this Exam */
#define _EX_ex_modelnum  0x1a4   /* 420 - int - Last Model Number used */
#define _EX_ex_modelcnt  0x1a8   /* 424 - int - Number of ThreeD Models */
#define _EX_ex_models    0x1ac   /* 428 - VARTYPE - ThreeD Model Keys for Exam */
#define _EX_ex_stat      0x1b4   /* 436 - short - Patient Status */
#define _EX_uniq_sys_id  0x1b6   /* 438 - char[16] - Unique System ID */
#define _EX_service_id   0x1c6   /* 454 - char[16] - Unique Service ID */
#define _EX_mobile_loc   0x1d6   /* 470 - char[4] - Mobile Location Number */
#define _EX_study_uid    0x1da   /* 474 - char[32] - Study Entity Unique ID */
#define _EX_ex_padding   0x1fa   /* 506 - char[518] - Spare Space */
#define _EX_sizeof       0x400   /* 1024 - sizeof EXAMDATATYPE */

/*  SERIESDATATYPE definitions  */

#define _SE_se_suid      0x0     /* 0 - char[4] - Suite ID for this Series */
#define _SE_se_uniq      0x4     /* 4 - short - The Make-Unique Flag */
#define _SE_se_diskid    0x6     /* 6 - char[1] - Disk ID for this Series */
#define _SE_se_exno      0x8     /* 8 - u_short - Exam Number */
#define _SE_se_no        0xa     /* 10 - short - Series Number */
#define _SE_se_datetime  0xc     /* 12 - int - Allocation Series Data/Time stamp */
#define _SE_se_actual_dt 0x10    /* 16 - int - Actual Series Data/Time stamp */
#define _SE_se_desc      0x14    /* 20 - char[30] - Series Description */
#define _SE_pr_sysid     0x32    /* 50 - char[9] - Primary Receiver Suite and Host */
#define _SE_pansysid     0x3b    /* 59 - char[9] - Archiver Suite and Host */
#define _SE_se_typ       0x44    /* 68 - short - Series Type */
#define _SE_se_source    0x46    /* 70 - short - Series from which prescribed */
#define _SE_se_plane     0x48    /* 72 - short - Most-like Plane (for L/S) */
#define _SE_scan_type    0x4a    /* 74 - short - Scout or Axial (for CT) */
#define _SE_position     0x4c    /* 76 - int - Patient Position */
#define _SE_entry        0x50    /* 80 - int - Patient Entry */
#define _SE_anref        0x54    /* 84 - char[3] - Anatomical reference */
#define _SE_lmhor        0x58    /* 88 - float - Horizontal Landmark */
#define _SE_prtcl        0x5c    /* 92 - char[25] - Scan Protocol Name */
#define _SE_se_contrast  0x76    /* 118 - short - Non-zero if > 0 image used contrast(L/S) */
#define _SE_start_ras    0x78    /* 120 - char[1] - RAS letter for first scan location (L/S) */
#define _SE_start_loc    0x7a    /* 122 - float - First scan location (L/S) */
#define _SE_end_ras      0x7e    /* 126 - char[1] - RAS letter for last scan location (L/S) */
#define _SE_end_loc      0x80    /* 128 - float - Last scan location (L/S) */
#define _SE_se_pseq      0x84    /* 132 - short - Last Pulse Sequence Used (L/S) */
#define _SE_se_sortorder 0x86    /* 134 - short - Image Sort Order (L/S) */
#define _SE_se_lndmrkcnt 0x88    /* 136 - int - Landmark Counter */
#define _SE_se_nacq      0x8c    /* 140 - short - Number of Acquisitions */
#define _SE_xbasest      0x8e    /* 142 - short - Starting number for baselines */
#define _SE_xbaseend     0x90    /* 144 - short - Ending number for baselines */
#define _SE_xenhst       0x92    /* 146 - short - Starting number for enhanced scans */
#define _SE_xenhend      0x94    /* 148 - short - Ending number for enhanced scans */
#define _SE_se_lastmod   0x96    /* 150 - int - Date/Time of Last Change */
#define _SE_se_alloc_key 0x9a    /* 154 - char[13] - Process that allocated this record */
#define _SE_se_delta_cnt 0xa8    /* 168 - int - Indicates number of updates to header */
#define _SE_se_verscre   0xac    /* 172 - char[2] - Genesis Version - Created */
#define _SE_se_verscur   0xae    /* 174 - char[2] - Genesis Version - Now */
#define _SE_se_pds_a     0xb0    /* 176 - float - PixelData size - as stored */
#define _SE_se_pds_c     0xb4    /* 180 - float - PixelData size - Compressed */
#define _SE_se_pds_u     0xb8    /* 184 - float - PixelData size - UnCompressed */
#define _SE_se_checksum  0xbc    /* 188 - u_int - Series Record checksum */
#define _SE_se_complete  0xc0    /* 192 - int - Series Complete Flag */
#define _SE_se_numarch   0xc4    /* 196 - int - Number of Images Archived */
#define _SE_se_imagect   0xc8    /* 200 - int - Last Image Number Used */
#define _SE_se_numimages 0xcc    /* 204 - int - Number of Images Existing */
#define _SE_se_images    0xd0    /* 208 - VARTYPE - Image Keys for this Series */
#define _SE_se_numunimg  0xd8    /* 216 - int - Number of Unstored Images */
#define _SE_se_unimages  0xdc    /* 220 - VARTYPE - Unstored Image Keys for this Series */
#define _SE_se_toarchcnt 0xe4    /* 228 - int - Number of Unarchived Images */
#define _SE_se_toarchive 0xe8    /* 232 - VARTYPE - Unarchived Image Keys for this Series */
#define _SE_echo_ABWL    0xf0    /* 240 - float - Echo 1 - 8 ABWL */
#define _SE_series_uid   0x150   /* 336 - char[32] - Series Entity Unique ID */
#define _SE_landmark_uid 0x170   /* 368 - char[32] - Landmark Unique ID */
#define _SE_equipmnt_uid 0x190   /* 400 - char[32] - Equipment Unique ID */
#define _SE_se_padding   0x1b0   /* 432 - char[588] - Spare Space */
#define _SE_sizeof       0x3fc   /* 1020 - sizeof SERIESDATATYPE */

/*  CTIMAGEDATATYPE definitions  */

#define _CT_im_suid      0x0     /* 0 - char[4] - Suite id for this image */
#define _CT_im_uniq      0x4     /* 4 - short - The Make-Unique Flag */
#define _CT_im_diskid    0x6     /* 6 - char[1] - Disk ID for this Image */
#define _CT_im_exno      0x8     /* 8 - u_short - Exam number for this image */
#define _CT_im_seno      0xa     /* 10 - short - Series Number for this image */
#define _CT_im_no        0xc     /* 12 - short - Image Number */
#define _CT_im_datetime  0xe     /* 14 - int - Allocation Image date/time stamp */
#define _CT_im_actual_dt 0x12    /* 18 - int - Actual Image date/time stamp */
#define _CT_sctime       0x16    /* 22 - float - Duration of scan (secs) */
#define _CT_slthick      0x1a    /* 26 - float - Slice Thickness (mm) */
#define _CT_imatrix_X    0x1e    /* 30 - short - Image matrix size - X */
#define _CT_imatrix_Y    0x20    /* 32 - short - Image matrix size - Y */
#define _CT_dfov         0x22    /* 34 - float - Display field of view - X */
#define _CT_dfov_rect    0x26    /* 38 - float - Display field of view - Y */
#define _CT_dim_X        0x2a    /* 42 - float - Image dimension - X */
#define _CT_dim_Y        0x2e    /* 46 - float - Image dimension - Y */
#define _CT_pixsize_X    0x32    /* 50 - float - Image pixel size - X */
#define _CT_pixsize_Y    0x36    /* 54 - float - Image pixel size - Y */
#define _CT_pdid         0x3a    /* 58 - char[14] - Pixel Data ID */
#define _CT_contrastIV   0x48    /* 72 - char[17] - IV Contrast Agent */
#define _CT_contrastOral 0x59    /* 89 - char[17] - Oral Contrast Agent */
#define _CT_contmode     0x6a    /* 106 - short - Image Contrast Mode */
#define _CT_serrx        0x6c    /* 108 - short - Series from which prescribed */
#define _CT_imgrx        0x6e    /* 110 - short - Image from which prescribed */
#define _CT_screenformat 0x70    /* 112 - short - Screen Format(8/16 bit) */
#define _CT_plane        0x72    /* 114 - short - Plane Type */
#define _CT_scanspacing  0x74    /* 116 - float - Spacing between scans (mm?) */
#define _CT_im_compress  0x78    /* 120 - short - Image compression type for allocation */
#define _CT_im_scouttype 0x7a    /* 122 - short - Scout Type (AP or lateral) */
#define _CT_loc_ras      0x7c    /* 124 - char[1] - RAS letter of image location */
#define _CT_loc          0x7e    /* 126 - float - Image location */
#define _CT_ctr_RAS      0x82    /* 130 - float - Center RAS coord of plane image */
#define _CT_norm_RAS     0x8e    /* 142 - float - Normal RAS coord */
#define _CT_tlhc_RAS     0x9a    /* 154 - float - RAS Coord of Top Left Hand */
#define _CT_trhc_RAS     0xa6    /* 166 - float - RAS Coord of Top Right Hand */
#define _CT_brhc_RAS     0xb2    /* 178 - float - RAS Coord of Bottom Right Hand */
#define _CT_forimgrev    0xbe    /* 190 - char[4] - Foreign Image Revision */
#define _CT_sctstr       0xc2    /* 194 - float - Table Start Location */
#define _CT_sctend       0xc6    /* 198 - float - Table End Location */
#define _CT_tblspd       0xca    /* 202 - float - Table Speed (mm/sec) */
#define _CT_tblht        0xce    /* 206 - float - Table Height */
#define _CT_midstime     0xd2    /* 210 - float - Mid Scan Time */
#define _CT_midsflag     0xd6    /* 214 - short - MidScan flag - does midstime apply */
#define _CT_kvolt        0xd8    /* 216 - int - KVolt generator setting */
#define _CT_mamp         0xdc    /* 220 - int - MAmp generator setting */
#define _CT_gantilt      0xe0    /* 224 - float - Gantry Tilt (degrees) */
#define _CT_azimuth      0xe4    /* 228 - int - Degrees of Azimuth */
#define _CT_ganvel       0xe8    /* 232 - float - Gantry Velocity */
#define _CT_ganfilt      0xec    /* 236 - int - Gantry Filter Position */
#define _CT_trigon       0xf0    /* 240 - float - Trigger on Position */
#define _CT_degrot       0xf4    /* 244 - float - Degrees of rotation */
#define _CT_xrayon       0xf8    /* 248 - float - X-Ray On Position */
#define _CT_xrayoff      0xfc    /* 252 - float - X-Ray Off Position */
#define _CT_numtrig      0x100   /* 256 - int - Number of Triggers */
#define _CT_inviews      0x104   /* 260 - short - Total input views */
#define _CT_view1ang     0x106   /* 262 - float - Angle of first view */
#define _CT_trigfreq     0x10a   /* 266 - float - Trigger frequency */
#define _CT_trigsrc      0x10e   /* 270 - int - DAS trigger source */
#define _CT_fpagain      0x112   /* 274 - int - DAS fpa gain */
#define _CT_scanopmode   0x116   /* 278 - int - Scan Type */
#define _CT_outsrc       0x11a   /* 282 - int - DAS output source */
#define _CT_adin         0x11e   /* 286 - int - DAS ad input */
#define _CT_calmode      0x122   /* 290 - int - DAS cal mode */
#define _CT_calfreq      0x126   /* 294 - int - DAS cal frequency */
#define _CT_regxm        0x12a   /* 298 - int - DAS reg xm */
#define _CT_autozero     0x12e   /* 302 - int - DAS auto zero */
#define _CT_sfovtyp      0x132   /* 306 - short - Axial Type */
#define _CT_phantsize    0x134   /* 308 - short - Calibration phantom size */
#define _CT_phanttyp     0x136   /* 310 - short - Calibration phantom type */
#define _CT_filttyp      0x138   /* 312 - short - Calibration filter type */
#define _CT_reconalg     0x13a   /* 314 - short - Recon Algorithm */
#define _CT_perisflag    0x13c   /* 316 - short - Perstaltic flag */
#define _CT_iboneflag    0x13e   /* 318 - short - IterBone flag */
#define _CT_statflag     0x140   /* 320 - short - Stat Recon flag */
#define _CT_computetyp   0x142   /* 322 - short - Compute Type */
#define _CT_segnum       0x144   /* 324 - short - Segment Number */
#define _CT_segstotal    0x146   /* 326 - short - Total Number of Segments Requested */
#define _CT_isd          0x148   /* 328 - float - Inter scan delay (secs) */
#define _CT_sfovmm       0x14c   /* 332 - float - Scan field of view (mm) */
#define _CT_scannum      0x150   /* 336 - short - Scan Number */
#define _CT_viewstrtchan 0x152   /* 338 - short - Starting Channel of View */
#define _CT_viewcompfctr 0x154   /* 340 - short - View Compression Factor */
#define _CT_outviews     0x156   /* 342 - short - Total Output Views */
#define _CT_overranges   0x158   /* 344 - short - Number of Overranges */
#define _CT_totrefchan   0x15a   /* 346 - short - Total Number of Ref Channels */
#define _CT_scdatasize   0x15c   /* 348 - int - data size for scan data */
#define _CT_refchan      0x160   /* 352 - short - Reference channel 1 -6 */
#define _CT_postproc     0x16c   /* 364 - short - Recon post processing flag */
#define _CT_xmpat        0x16e   /* 366 - int - DAS xm pattern */
#define _CT_rottyp       0x172   /* 370 - short - Prescribed rotation type */
#define _CT_rawdataflag  0x174   /* 372 - short - Save Raw Data Flag */
#define _CT_ct_scalefact 0x176   /* 374 - float - IBH Image scale factors */
#define _CT_ct_water_num 0x17a   /* 378 - short - CT Water Number */
#define _CT_ct_bone_num  0x17c   /* 380 - short - CT Bone Number */
#define _CT_bbh_coef1    0x17e   /* 382 - float - BBH coefficient 1 */
#define _CT_bbh_coef2    0x182   /* 386 - float - BBH coefficient 2 */
#define _CT_bbh_coef3    0x186   /* 390 - float - BBH coefficient 3 */
#define _CT_bbh_numblend 0x18a   /* 394 - short - Num of BBH channels to blend */
#define _CT_firstchan    0x18c   /* 396 - int - Starting channel */
#define _CT_numchan      0x190   /* 400 - int - Number of channels (1..512) */
#define _CT_chaninc      0x194   /* 404 - int - Increment between channels */
#define _CT_firstview    0x198   /* 408 - int - Starting view */
#define _CT_numview      0x19c   /* 412 - int - Number of views */
#define _CT_viewinc      0x1a0   /* 416 - int - Increment between views */
#define _CT_windowrange  0x1a4   /* 420 - int - Window Range (0..4095) */
#define _CT_scalemin     0x1a8   /* 424 - float - Scaling value of the image data */
#define _CT_scalemax     0x1ac   /* 428 - float - Scaling value of the image data */
#define _CT_datamod      0x1b0   /* 432 - int - Amount of processing that will be */
#define _CT_qcalfile     0x1b4   /* 436 - char[13] - Source of the qcal vectors */
#define _CT_calmodfile   0x1c1   /* 449 - char[13] - Source of the cal vectors */
#define _CT_wordsperview 0x1ce   /* 462 - short - Number of words per view */
#define _CT_rl_ras       0x1d0   /* 464 - char[1] - RAS letter for side of image */
#define _CT_ap_ras       0x1d1   /* 465 - char[1] - RAS letter for anterior/posterior */
#define _CT_sctstr_ras   0x1d2   /* 466 - char[1] - RAS letter for scout start loc */
#define _CT_sctend_ras   0x1d3   /* 467 - char[1] - RAS letter for scout end loc */
#define _CT_sct_anref    0x1d4   /* 468 - char[3] - Anatomical reference for scout */
#define _CT_pps_scalwin  0x1d8   /* 472 - short - PpScan window range for output Scaling */
#define _CT_pps_qcalflag 0x1da   /* 474 - short - PpScan Qcal modification flag */
#define _CT_pps_pcalflag 0x1dc   /* 476 - short - PpScan Pcal modification flag */
#define _CT_pps_thetafix 0x1de   /* 478 - short - PpScan Theta Fix (Angle Correction) */
#define _CT_pps_bhflag   0x1e0   /* 480 - short - PpScan Beam Hardening Flag */
#define _CT_spot_size    0x1e2   /* 482 - short - tube focal spot size */
#define _CT_spot_pos     0x1e4   /* 484 - short - tube focal spot position */
#define _CT_recondataset 0x1e6   /* 486 - short - Dependent on #views processed */
#define _CT_ndetcellsfov 0x1e8   /* 488 - short - Field of view in detector cells */
#define _CT_strtscantime 0x1ea   /* 490 - double - Start time(secs) of this scan */
#define _CT_gandir       0x1f2   /* 498 - short - Gantry Rotation Direction */
#define _CT_rotorspeed   0x1f4   /* 500 - short - Tube Rotor Speed */
#define _CT_trigmode     0x1f6   /* 502 - short - TGGC Trigger Mode */
#define _CT_sitilt       0x1f8   /* 504 - float - Rxd gantry tilt - not annotated */
#define _CT_targcen_R    0x1fc   /* 508 - float - R/L coordinate for target recon center */
#define _CT_targcen_A    0x200   /* 512 - float - A/P coordinate for target recon center */
#define _CT_backprojflag 0x204   /* 516 - short - Value of Back Projection button */
#define _CT_fatqestflag  0x206   /* 518 - short - Set if fatq estimates were used */
#define _CT_zavg         0x208   /* 520 - float - Z chan avg over views */
#define _CT_leftrefavg   0x20c   /* 524 - float - avg of left ref chans over */
#define _CT_leftrefmax   0x210   /* 528 - float - max left chan value over views */
#define _CT_rightrefavg  0x214   /* 532 - float - avg of right ref chans over */
#define _CT_rightrefmax  0x218   /* 536 - float - max right chan value over views */
#define _CT_im_alloc_key 0x21c   /* 540 - char[13] -  */
#define _CT_im_lastmod   0x22a   /* 554 - int - Date/Time of Last Change */
#define _CT_im_verscre   0x22e   /* 558 - char[2] - Genesis Version - Created */
#define _CT_im_verscur   0x230   /* 560 - char[2] - Genesis Version - Now */
#define _CT_im_pds_a     0x232   /* 562 - int - PixelData size - as stored */
#define _CT_im_pds_c     0x236   /* 566 - int - PixelData size - Compressed */
#define _CT_im_pds_u     0x23a   /* 570 - int - PixelData size - UnCompressed */
#define _CT_im_checksum  0x23e   /* 574 - u_int - AcqRecon record checksum */
#define _CT_im_archived  0x242   /* 578 - int - Image Archive Flag */
#define _CT_im_complete  0x246   /* 582 - int - Image Complete Flag */
#define _CT_biop_pos     0x24a   /* 586 - short - Biopsy Position */
#define _CT_biop_tloc    0x24c   /* 588 - float - Biopsy T Location */
#define _CT_biop_refloc  0x250   /* 592 - float - Biopsy Ref Location */
#define _CT_ref_chan     0x254   /* 596 - short - Reference Channel Used */
#define _CT_bp_coef      0x256   /* 598 - float - Back Projector Coefficient */
#define _CT_psc          0x25a   /* 602 - short - Primary Speed Correction Used */
#define _CT_overrng_corr 0x25c   /* 604 - short - Overrange Correction Used */
#define _CT_dyn_z_alpha  0x25e   /* 606 - float - Dynamic Z Alpha Value */
#define _CT_ref_img      0x262   /* 610 - char[1] - Reference Image Field */
#define _CT_sum_img      0x263   /* 611 - char[1] - Summary Image Field */
#define _CT_img_window   0x264   /* 612 - u_short - Window Value */
#define _CT_img_level    0x266   /* 614 - short - Level Value */
#define _CT_slop_int     0x268   /* 616 - int - Integer Slop Field 1 - 5 */
#define _CT_slop_float_1 0x27c   /* 636 - float - Float Slop Field 1 - 5 */
#define _CT_slop_str_1   0x290   /* 656 - char[16] - String Slop Field 1 */
#define _CT_slop_str_2   0x2a0   /* 672 - char[16] - String Slop Field 2 */
#define _CT_deltastart   0x2b0   /* 688 - float - Delta Start Time */
#define _CT_maxOverrange 0x2b4   /* 692 - int - Max Overranges In A View */
#define _CT_avgOverrange 0x2b8   /* 696 - float  - Avg Overranges All Views */
#define _CT_afterglow    0x2bc   /* 700 - short - Corrected Afterglow Terms */
#define _CT_ref_blocked  0x2be   /* 702 - Unsign short - No Views Ref[1-6] Channel Blocked */
#define _CT_integrity    0x2ca   /* 714 - short - GE Image Integrity */
#define _CT_pitchRatio   0x2cc   /* 716 - char[8] - Scan Pitch Ratio */
#define _CT_image_uid    0x2d4   /* 724 - char[32] - Image Unique ID */
#define _CT_sop_uid      0x2f4   /* 756 - char[32] - Service Obj Class Unique ID */
#define _CT_xraydelay    0x314   /* 788 - float - Start Scan To Xray On Delay */
#define _CT_xrayduration 0x318   /* 792 - float - Duration Of Xray On */
#define _CT_ct_padding   0x31c   /* 796 - char[223] - Spare Space */
#define _CT_CTIMAGEDATATYPE      0x3fc   /* 1020 - sizeof - sizeof CTIMAGEDATATYPE */

/*  MRIMAGEDATATYPE definitions  */

#define _MR_im_suid      0x0     /* 0 - char[4] - Suite id for this image */
#define _MR_im_uniq      0x4     /* 4 - short - The Make-Unique Flag */
#define _MR_im_diskid    0x6     /* 6 - char[1] - Disk ID for this Image */
#define _MR_im_exno      0x8     /* 8 - u_short - Exam number for this image */
#define _MR_im_seno      0xa     /* 10 - short - Series Number for this image */
#define _MR_im_no        0xc     /* 12 - short - Image Number */
#define _MR_im_datetime  0xe     /* 14 - int - Allocation Image date/time stamp */
#define _MR_im_actual_dt 0x12    /* 18 - int - Actual Image date/time stamp */
#define _MR_sctime       0x16    /* 22 - float - Duration of scan (secs) */
#define _MR_slthick      0x1a    /* 26 - float - Slice Thickness (mm) */
#define _MR_imatrix_X    0x1e    /* 30 - short - Image matrix size - X */
#define _MR_imatrix_Y    0x20    /* 32 - short - Image matrix size - Y */
#define _MR_dfov         0x22    /* 34 - float - Display field of view - X (mm) */
#define _MR_dfov_rect    0x26    /* 38 - float - Display field of view - Y (if different) */
#define _MR_dim_X        0x2a    /* 42 - float - Image dimension - X */
#define _MR_dim_Y        0x2e    /* 46 - float - Image dimension - Y */
#define _MR_pixsize_X    0x32    /* 50 - float - Image pixel size - X */
#define _MR_pixsize_Y    0x36    /* 54 - float - Image pixel size - Y */
#define _MR_pdid         0x3a    /* 58 - char[14] - Pixel Data ID */
#define _MR_contrastIV   0x48    /* 72 - char[17] - IV Contrast Agent */
#define _MR_contrastOral 0x59    /* 89 - char[17] - Oral Contrast Agent */
#define _MR_contmode     0x6a    /* 106 - short - Image Contrast Mode */
#define _MR_serrx        0x6c    /* 108 - short - Series from which prescribed */
#define _MR_imgrx        0x6e    /* 110 - short - Image from which prescribed */
#define _MR_screenformat 0x70    /* 112 - short - Screen Format(8/16 bit) */
#define _MR_plane        0x72    /* 114 - short - Plane Type */
#define _MR_scanspacing  0x74    /* 116 - float - Spacing between scans (mm?) */
#define _MR_im_compress  0x78    /* 120 - short - Image compression type for allocation */
#define _MR_im_scouttype 0x7a    /* 122 - short - Scout Type (AP or lateral) */
#define _MR_loc_ras      0x7c    /* 124 - char[] - RAS letter of image location */
#define _MR_loc          0x7e    /* 126 - float - Image location */
#define _MR_ctr_R        0x82    /* 130 - float - Center R coord of plane image */
#define _MR_ctr_A        0x86    /* 134 - float - Center A coord of plane image */
#define _MR_ctr_S        0x8a    /* 138 - float - Center S coord of plane image */
#define _MR_norm_R       0x8e    /* 142 - float - Normal R coord */
#define _MR_norm_A       0x92    /* 146 - float - Normal A coord */
#define _MR_norm_S       0x96    /* 150 - float - Normal S coord */
#define _MR_tlhc_R       0x9a    /* 154 - float - R Coord of Top Left Hand Corner */
#define _MR_tlhc_A       0x9e    /* 158 - float - A Coord of Top Left Hand Corner */
#define _MR_tlhc_S       0xa2    /* 162 - float - S Coord of Top Left Hand Corner */
#define _MR_trhc_R       0xa6    /* 166 - float - R Coord of Top Right Hand Corner */
#define _MR_trhc_A       0xaa    /* 170 - float - A Coord of Top Right Hand Corner */
#define _MR_trhc_S       0xae    /* 174 - float - S Coord of Top Right Hand Corner */
#define _MR_brhc_R       0xb2    /* 178 - float - R Coord of Bottom Right Hand Corner */
#define _MR_brhc_A       0xb6    /* 182 - float - A Coord of Bottom Right Hand Corner */
#define _MR_brhc_S       0xba    /* 186 - float - S Coord of Bottom Right Hand Corner */
#define _MR_forimgrev    0xbe    /* 190 - char[4] - Foreign Image Revision */
#define _MR_tr           0xc2    /* 194 - int - Pulse repetition time(usec) */
#define _MR_ti           0xc6    /* 198 - int - Pulse inversion time(usec) */
#define _MR_te           0xca    /* 202 - int - Pulse echo time(usec) */
#define _MR_te2          0xce    /* 206 - int - Second echo echo (usec) */
#define _MR_numecho      0xd2    /* 210 - short - Number of echoes */
#define _MR_echonum      0xd4    /* 212 - short - Echo Number */
#define _MR_tbldlta      0xd6    /* 214 - float - Table Delta */
#define _MR_nex          0xda    /* 218 - float - Number of Excitations */
#define _MR_contig       0xde    /* 222 - short - Continuous Slices Flag */
#define _MR_hrtrate      0xe0    /* 224 - short - Cardiac Heart Rate (bpm) */
#define _MR_tdel         0xe2    /* 226 - int - Delay time after trigger (msec) */
#define _MR_saravg       0xe6    /* 230 - float - Average SAR */
#define _MR_sarpeak      0xea    /* 234 - float - Peak SAR */
#define _MR_monsar       0xee    /* 238 - short - Monitor SAR flag */
#define _MR_trgwindow    0xf0    /* 240 - short - Trigger window (% of R-R interval) */
#define _MR_reptime      0xf2    /* 242 - float - Cardiac repetition time */
#define _MR_imgpcyc      0xf6    /* 246 - short - Images per cardiac cycle */
#define _MR_xmtgain      0xf8    /* 248 - short - Actual Transmit Gain (.1 db) */
#define _MR_rcvgain1     0xfa    /* 250 - short - Actual Receive Gain Analog (.1 db) */
#define _MR_rcvgain2     0xfc    /* 252 - short - Actual Receive Gain Digital (.1 db) */
#define _MR_mr_flip      0xfe    /* 254 - short - Flip Angle for GRASS scans (deg.) */
#define _MR_mindat       0x100   /* 256 - int - Minimum Delay after Trigger (uSec) */
#define _MR_cphase       0x104   /* 260 - short - Total Cardiac Phase prescribed */
#define _MR_swappf       0x106   /* 262 - short - Swap Phase/Frequency Axis */
#define _MR_pauseint     0x108   /* 264 - short - Pause Interval (slices) */
#define _MR_pausetime    0x10a   /* 266 - float - Pause Time */
#define _MR_obplane      0x10e   /* 270 - int - Oblique Plane */
#define _MR_slocfov      0x112   /* 274 - int - Slice Offsets on Freq axis */
#define _MR_xmtfreq      0x116   /* 278 - int - Center Frequency (0.1 Hz) */
#define _MR_autoxmtfreq  0x11a   /* 282 - int - Auto Center Frequency (0.1 Hz) */
#define _MR_autoxmtgain  0x11e   /* 286 - short - Auto Transmit Gain (0.1 dB) */
#define _MR_prescan_r1   0x120   /* 288 - short - PreScan R1 - Analog */
#define _MR_prescan_r2   0x122   /* 290 - short - PreScan R2 - Digital */
#define _MR_user_bitmap  0x124   /* 292 - int - Bitmap defining user CVs */
#define _MR_cenfreq      0x128   /* 296 - short - Center Frequency Method */
#define _MR_imode        0x12a   /* 298 - short - Imaging Mode */
#define _MR_iopt         0x12c   /* 300 - int - Imaging Options */
#define _MR_pseq         0x130   /* 304 - short - Pulse Sequence */
#define _MR_pseqmode     0x132   /* 306 - short - Pulse Sequence Mode */
#define _MR_psdname      0x134   /* 308 - char[33] - Pulse Sequence Name */
#define _MR_psd_datetime 0x156   /* 342 - int - PSD Creation Date and Time */
#define _MR_psd_iname    0x15a   /* 346 - char[13] - PSD name from inside PSD */
#define _MR_ctyp         0x168   /* 360 - short - Coil Type */
#define _MR_cname        0x16a   /* 362 - char[17] - Coil Name */
#define _MR_surfctyp     0x17c   /* 380 - short - Surface Coil Type */
#define _MR_surfcext     0x17e   /* 382 - short - Extremity Coil Flag */
#define _MR_rawrunnum    0x180   /* 384 - int - RawData Run Number */
#define _MR_cal_fldstr   0x184   /* 388 - u_int - Calibrated Field Strength (x10 uGauss) */
#define _MR_supp_tech    0x188   /* 392 - short - SAT fat/water/none */
#define _MR_vbw          0x18a   /* 394 - float - Variable Bandwidth (Hz) */
#define _MR_slquant      0x18e   /* 398 - short - Number of slices in this scan group */
#define _MR_gpre         0x190   /* 400 - short - Graphically prescribed */
#define _MR_intr_del     0x192   /* 402 - int - Interimage/interloc delay (uSec) */
#define _MR_userVars     0x196   /* 406 - float - User Variables 0 - 22 */
#define _MR_user23       0x1f2   /* 498 - float - Projection Angle */
#define _MR_user24       0x1f6   /* 502 - float - Concat Sat Type Flag */
#define _MR_im_alloc_key 0x1fa   /* 506 - char[13] - */
#define _MR_im_lastmod   0x208   /* 520 - int - Date/Time of Last Change */
#define _MR_im_verscre   0x20c   /* 524 - char[2] - Genesis Version - Created */
#define _MR_im_verscur   0x20e   /* 526 - char[2] - Genesis Version - Now */
#define _MR_im_pds_a     0x210   /* 528 - int - PixelData size - as stored */
#define _MR_im_pds_c     0x214   /* 532 - int - PixelData size - Compressed */
#define _MR_im_pds_u     0x218   /* 536 - int - PixelData size - UnCompressed */
#define _MR_im_checksum  0x21c   /* 540 - u_int - AcqRecon record checksum */
#define _MR_im_archived  0x220   /* 544 - int - Image Archive Flag */
#define _MR_im_complete  0x224   /* 548 - int - Image Complete Flag */
#define _MR_satbits      0x228   /* 552 - short - Bitmap of SAT selections */
#define _MR_scic         0x22a   /* 554 - short - Surface Coil Intensity Correction Flag */
#define _MR_satxloc1     0x22c   /* 556 - short - R-side SAT pulse loc rel to lndmrk */
#define _MR_satxloc2     0x22e   /* 558 - short - L-side SAT pulse loc rel to lndmrk */
#define _MR_satyloc1     0x230   /* 560 - short - A-side SAT pulse loc rel to lndmrk */
#define _MR_satyloc2     0x232   /* 562 - short - P-side SAT pulse loc rel to lndmrk */
#define _MR_satzloc1     0x234   /* 564 - short - S-side SAT pulse loc rel to lndmrk */
#define _MR_satzloc2     0x236   /* 566 - short - I-side SAT pulse loc rel to lndmrk */
#define _MR_satxthick    0x238   /* 568 - short - Thickness of X-axis SAT pulse */
#define _MR_satythick    0x23a   /* 570 - short - Thickness of Y-axis SAT pulse */
#define _MR_satzthick    0x23c   /* 572 - short - Thickness of Z-axis SAT pulse */
#define _MR_flax         0x23e   /* 574 - short - Phase contrast flow axis */
#define _MR_venc         0x240   /* 576 - short - Phase contrast velocity encoding */
#define _MR_thk_disclmr  0x242   /* 578 - short - Slice Thickness */
#define _MR_ps_flag      0x244   /* 580 - short - Auto/Manual Prescan flag */
#define _MR_ps_status    0x246   /* 582 - short - Bitmap of changed values */
#define _MR_image_type   0x248   /* 584 - short - Magnitude, Phase, Imaginary, or Real */
#define _MR_vas_collapse 0x24a   /* 586 - short - Collapse Image */
#define _MR_user23n      0x24c   /* 588 - float - User Variable 23 */
#define _MR_user24n      0x250   /* 592 - float - User Variable 24 */
#define _MR_proj_alg     0x254   /* 596 - short - Projection Algorithm */
#define _MR_proj_name    0x256   /* 598 - char[13] - Projection Algorithm Name */
#define _MR_x_axis_rot   0x264   /* 612 - float - X Axis Rotation */
#define _MR_y_axis_rot   0x268   /* 616 - float - Y Axis Rotation */
#define _MR_z_axis_rot   0x26c   /* 620 - float - Z Axis Rotation */
#define _MR_thresh_min1  0x270   /* 624 - int - Lower Range of Pixels 1 */
#define _MR_thresh_max1  0x274   /* 628 - int - Upper Range of Pixels 1 */
#define _MR_thresh_min2  0x278   /* 632 - int - Lower Range of Pixels 2 */
#define _MR_thresh_max2  0x27c   /* 636 - int - Upper Range of Pixels 2 */
#define _MR_echo_trn_len 0x280   /* 640 - short - Echo Train Length for Fast Spin Echo */
#define _MR_frac_echo    0x282   /* 642 - short - Fractional Echo - Effective TE Flag */
#define _MR_prep_pulse   0x284   /* 644 - short - Preporatory Pulse Option */
#define _MR_cphasenum    0x286   /* 646 - short - Cardiac Phase Number */
#define _MR_var_echo     0x288   /* 648 - short - Variable Echo Flag */
#define _MR_ref_img      0x28a   /* 650 - char[1] - Reference Image Field */
#define _MR_sum_img      0x28b   /* 651 - char[1] - Summary Image Field */
#define _MR_img_window   0x28c   /* 652 - u_short - Window Value */
#define _MR_img_level    0x28e   /* 654 - short - Level Value */
#define _MR_slop_int     0x290   /* 656 - int - Integer Slop Field 1 - 5 */
#define _MR_slop_float   0x2a4   /* 676 - float - Float Slop Field 1 - 5 */
#define _MR_slop_str_1   0x2b8   /* 696 - char[16] - String Slop Field 1 */
#define _MR_slop_str_2   0x2c8   /* 712 - char[16] - String Slop Field 2 */
#define _MR_scanactno    0x2d8   /* 728 - short - Scan Acquisition Number */
#define _MR_vasflags     0x2da   /* 730 - short - Magnitude Weighting Flag */
#define _MR_vencscale    0x2dc   /* 732 - float - Scale Weighted Venc */
#define _MR_integrity    0x2e0   /* 736 - short - GE Image Integrity */
#define _MR_fphase       0x2e2   /* 738 - int - Number Of Phases */
#define _MR_freq_dir     0x2e6   /* 742 - short - Frequency Direction */
#define _MR_vas_mode     0x2e8   /* 744 - short - Vascular Mode */
#define _MR_image_uid    0x2ea   /* 746 - char[32] - Image Unique ID */
#define _MR_sop_uid      0x30a   /* 778 - char[32] - Service Obj Class Unique ID */
#define _MR_mr_padding   0x32a   /* 810 - char[212] - Spare Space */
#define _MR_MRIMAGEDATATYPE      0x3fe   /* 1022 - sizeof - sizeof MRIMAGEDATATYPE */
 
/* There are four headers associated with each image. For MR images, there
is a suite, exam, series and MR image header.
	Suite Header	114 bytes
	Exam Header	1024 bytes
	Series Header	1020 bytes
	Image header	1022 bytes
*/

#define IMG_HDR_VERSION 3

/* overall img header */
/* This struct contains the offets, in bytes, from its start to the
	beginning of the suite, exam, series and image structs */
typedef struct {
	char  MagicNumber[4];     /* IMGF maintained by Sun and GE */
	int	  img_hdr_length;
	int   ImgWidth;           /* image width in pixels */
	int   ImgHeight;          /* image height in pixels */
	int   ImgDepth;           /* bits/pixel? */
	int   CompressType;       /* 20 see above */
	int   DefaultWindow;      /* 24 */
	int   DefaultLevel;       /* 28 */
	int   DefaultBGShade;     /* 32 background shade to use for non-image */
	int   OverflowValue;      /* 36 */
	int   UnderflowValue;     /* 40 */
	int   ImageTopOffset;     /* number of blank lines at image top */
	int   ImagebottomOffset;  /* number of blank lines at image bottom */
	short HeaderVersion;      /* 52 */
	unsigned short	CheckSum; /* 54 16 bit end_around_carry sum of pixels */

/* These displacements are bytes into the file to find the suggested structure. If the length is
	0, then the table is empty */
	int	p_ID;        /* 56 - byte disp to unique image identifier text table*/
	int	l_ID;        /* 60 - byte length of unique image identifier text table*/
	int	p_unpack;    /* a byte disp disp to unpack control table */
	int	I_unpack;    /* byte length of unpack control table */
	int	p_compress;  /* 72 - int - a byte disp to compression seed table */
	int	l_compress;  /* 76 - int - byte length of compression seed table */
	int	p_histo;     /* 80 - int - a byte disp to histogram control table */
	int	l_histo;     /* 84 - int - byte length of histogram control table */
	int	p_text;      /* 88 - int - a byte disp to text plane data */
	int	l_text;      /* 92 - int - byte length of text plane data */
	int	p_graphics;  /* 96 - int - a byte disp to graphics plane data */
	int	l_graphics;  /* 100 - int - byte length of graphics plane data */
	int	p_dbHdr;     /* 104 - int - a byte disp to data base header data */
	int	l_dbHdr;     /* 108 - int - byte length of data base header data */
	int	levelOffset; /* 112 - int - value to add to stored Pixel Data values
						to get the correct annotation value. */
	int	p_user;      /* 116 - int - byte displacement to user defined data */
	int	l_user;      /* 120 - int - byte length of user defined data */
	int	p_suite;     /* 124 - int - byte displacement to <suite> header data */
	int	l_suite;     /* 128 - int - byte length of <suite> header data */
	int	p_exam;      /* 132 - int - byte displacement to <exam> header data */
	int	l_exam;      /* 136 - int - byte length of <exam> header data */
	/* the listed offset for the series header does not appear to be correct in the files. */
	/* use p_exam + l_exam as p_series */
	int	p_series;    /* 140 - int - byte displacement to <series> header data */
	int	l_series;    /* 144 - int - byte length of <series> header data */
	int	p_image;     /* 148 - int - byte displacement to <image> header data */
	int	l_image;     /* 152 - int - byte length of <image> header data */
} ImgHdr;

/* histogram data */
typedef struct {
	int	hs_version;     /* version of the histogram structure */
	float	hs_sd;      /* standard deviation of pixel data value */
	short	hs_mean;    /* rounded mean pixel data value */
	short	hs_min;     /* minimum pixel data value */
	short	hs_max;     /* maximum pixel data value */
	short	hs_first;   /* first histogram entry used for <hs_sd> */
	short	hs_region;  /* region entries used for <hs_sd> */
	short	length;     /* number of bins in the histogram area */
	unsigned short	hs_bins[1024];	/* 2068 - u_short[1024] - histogram bins */
#define hs_binsOffset 20L
} ImgHst;

/* functions to access parameter interpretations */
OSErr  Eval_contig( short val, char *ret );
OSErr  Eval_contrast( short val, char *ret );
OSErr  Eval_coil( short val, char *ret );
OSErr  Eval_entry( short val, char *ret );
OSErr  Eval_frac_echo( short val, char *ret );
OSErr  Eval_gpre( short val, char *ret );
OSErr  Eval_imode( short val, char *ret );
OSErr  Eval_image_type( short val, char *ret );
void   Eval_iopt( int val, char *ret );
void   Eval_plane( short val, char *ret );
OSErr  Eval_patsex( short val, char *ret );

/************************* ^^^ genesis.h ^^^ ******************************/

/****<a name="Analyze">****   contents of analyzeUtil.h   ******************
A set of primitives to create and read analyze headers
Be sure to initialize the machine type before compiling!
***************************************************************************/
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

enum
{
   kNeedXInterp  = 1,
   kNeedYInterp  = 2,
   kmacByteOrder = 4,
   kNeedXCrop    = 8,
   kNeedYCrop    = 16,
   kTimeOrder    = 32,   // slices are in the temporal order of acquisition
   kScanning     = 64
};	/* used for UCLA specific flags */

#define kBigEndian 4

typedef struct /*      header_key       */
{                                /* off + size*/
    int sizeof_hdr;              /* 0 + 4     */
    char data_type[10];          /* 4 + 10    */
    char db_name[18];            /* 14 + 18   */
    int extents;                 /* 32 + 4    */
    short int session_error;     /* 36 + 2    */
    char regular;                /* 38 + 1    */
    char hkey_un0;               /* 39 + 1    */
} header_key;                               /* total=40  */

typedef struct /*      image_dimension  */
{                                /* off + size*/
/* The analyze spec requires dim to be 4, but modern programs (e.g. FSL) set this
to the number of dimensions actually used. Single time points have 3 dimensions... */
    short int dim[8];            /* 0 + 16    */
    char vox_units[4];           /* 16 + 4    */
    char cal_units[8];           /* 20 + 8    */
/* officially unused, UCLA will utilize this for various data flags */
/*  short int unused1;      */   /* 28 + 2    */
/* could #define ucla_flags unused1 */
    unsigned short int flags;						 /* 28 + 2 UCLA */
    short int datatype;          /* 30 + 2    */
    short int bitpix;            /* 32 + 2    */
    short int dim_un0;           /* 34 + 2    */
    float pixdim[8];             /* 36 + 32   */	// pixdim[4] is tr in seconds
    float vox_offset;            /* 68 + 4    */
    float funused1;              /* 72 + 4    */
    float funused2;              /* 76 + 4    */
    float funused3;              /* 80 + 4    */
    float cal_max;               /* 84 + 4    */
    float cal_min;               /* 88 + 4    */
    int   compressed;            /* 92 + 4    */
    int	verified;              /* 96 + 4    */
    int   glmax, glmin;          /* 100 + 8   */
} image_dimension;                          /* total=108 */

typedef struct /*      data_history     */
{                                /* off + size*/
    char descrip[80];            /* 0 + 80    */
    char aux_file[24];           /* 80 + 24   */
    char orient;                 /* 104 + 1   */
    char originator[10];         /* 105 + 10  */
    char generated[10];          /* 115 + 10  */
    char scannum[10];            /* 125 + 10  */
    char patient_id[10];         /* 135 + 10  */
    char exp_date[10];           /* 145 + 10  */
    char exp_time[10];           /* 155 + 10  */
    char hist_un0[3];            /* 165 + 3   */
    int  views;                  /* 168 + 4   */
    int  vols_added;             /* 172 + 4   */
    int  start_field;            /* 176 + 4   */
    int  field_skip;             /* 180 + 4   */
    int  omax,omin;              /* 184 + 8   */
    int  smax,smin;              /* 192 + 8   */
} data_history;                             /* total=200 */

typedef struct                              /*      dsr              */
{                                /* off + size*/
    header_key hk;               /* 0 + 40    */
    image_dimension dime;        /* 40 + 108  */
    data_history hist;           /* 148 + 200 */
} dsr ;                                     /* total=348 */

#define DT_NONE                         0
#define DT_UNKNOWN                      0
#define DT_BINARY                       1
#define DT_UNSIGNED_CHAR                2
#define DT_SIGNED_SHORT                 4
#define DT_SIGNED_INT                   8
#define DT_FLOAT                        16
#define DT_COMPLEX                      32
#define DT_DOUBLE                       64
#define DT_RGB                          128
#define DT_ALL                          255

/* Comments:
 *
 * struct header_key
 *               long sizeof_header   * must indicate size of header file *
 *               long extants;        * should be 16384 *
 *               char regular;       * 'r' *
 *
 *       struct image_dimension struct decribes the organization and
 *       side of images. These elements enable IO routines to reference
 *       images by volume and slice number.
 *
 *               short int dim[]  * array of image dimensions *
 *                       dim[0]        * number of dimensions; usually 4 *
 *                       dim[1]        * image width *
 *                       dim[2]        * image height *
 *                       dim[3]        * volume depth *
 *                       dim[4]        * volumes in file *
 *
 *               char vox_units[4] * labels voxel spatial unit
 *               char cal_units[4] * labels voxel calibration unit *
 *               short int datatype * Acceptable values are *
 *               short int bitpix     * bits per pixel *
 *               float pixdim[]  * parallel array to dim giving voxel
 *               dimensions
 *                                  in each dimension *
 *                        pixdim[1] * voxel width *
 *                        pixdim[2]  * voxel height *
 *                        pixdim[3]  * voxel depth or slice thickness *
 *
 *               float vox_offset; * byte offset in the .img file at which
 *                                    voxels start. If value is negative
 *                                    specifies that the absolute value
 *                                    is applied for every image in the file. *
 *
 *               float calibrated Max & Min * specify range of calibration
 *               values *
 *               long glmax, glmin    * the max and min values for entire data
 *               set *
 *
 *The data_history substructure is not required, but the 'orient' element
 *is used to indicate individual slice orientation and determines whether
 *the ANALYZE 'Movie' program will attempt to flip the images before
 *displaying a movie sequence.
 *       orient:
 *                       0 - transverse unflipped
 *                       1 - coronal unflipped
 *                       2 - sagittal unflipped
 *                       3 - transverse flipped
 *                       4 - coronal flipped
 *                       5 - sagittal flipped *
 *
 */

#define transverse 0
#define coronal 1
#define sagittal 2
#define transverseFlipped 3
#define coronalFlipped 4
#define sagittalFlipped 5
#define axial 0

#ifndef OSErr
#define OSErr  short
#endif
/* <a name="Functions"> */
OSErr  EmptyAnaHdr( dsr* DSR, char *description, float sliceSpacing,
	     int xs, int ys, int n_slices, int time_pts, char orient ); /* analyzeUtil.c */
OSErr  ImageToAnalyzeHeader( IMAGE *im, dsr *DSR, short *result );
int    get_datasize(int data_type); /* analyzeUtil.c */
OSErr  vbyteswap(void *ivec, long vlen, int data_type); /* analyzeUtil.c */
void   ShowAnalyzeHeader( dsr *DSR, FILE *fp, char *name ); /* analyzeUtil.c */
char   *AnalyzeTypeToString( short int TYPE );
short  swapHdrBytes( void *Hdr ); /* analyzeUtil.c */

/*************************  ^^^   analyzeUtil.h   ^^^  ***************************/

/* UTLITIES */
void   GetImgLibVer( char *buff );
OSErr  OpenProcFile( int argc, char **argv, char *name, char *vers, FILE **f );
char   *ReportFileType( int file_type );
char   *UCFlagString( unsigned short int flags );

/* IMAGE ROUTINES */
OSErr  xinterp (void *invec, int inxs, int inys, int Ratio, void *outvec, int data_type);
OSErr  yinterp (void *invec, int inxs, int inys, int Ratio, void *outvec, int data_type);
OSErr  finterpX(void *invec, int xs, int ys, int outxs, void *outvec, int data_type);
OSErr  finterpY(void *invec, int xs, int ys, int outys, void *outvec, int data_type);
OSErr  finterp2D(void *invec, int xs, int ys, int outxs, int outys, void *outvec, int data_type);
OSErr  interp2D(void *invec, int inxsize, int inysize, int Ratio, void *outvec, int data_type);
OSErr  pixreplicate(void *invec, int inxsize, int inysize, int Ratio, void *outvec, int data_type);
OSErr  pixrepX(void *invec, int inxsize, int inysize, int Ratio, void *outvec, int data_type);
OSErr  pixrepY(void *invec, int inxsize, int inysize, int Ratio, void *outvec, int data_type);
OSErr  DownSample(void *invec, int xsize, int ysize, int xFactor, int yFactor, void *outvec, int data_type);
float  *ScaleImageSize( float *theImage, IMAGE *im, int interpFactor,
                        int displayMode, OSErr  *error, int data_type );
void   xcrop( void *InImg, int xs, int ys, void *CropImg, int data_type );
OSErr  ImageFlipY( void *invec, int xsize, int ysize, void *outvec, int data_type );
OSErr  Overlay( float *baseImage, float *OverlayImage, unsigned char *composite,
                 long imSize, float overlay_max, float overlay_cutoff, short OvlMode );
OSErr  vGaussSmooth( void *invec, int width, int step, long npts, int data_type );
OSErr  HanningSmooth( short Kernel, unsigned short int *image, unsigned short int *holder, int xsize, int ysize, int data_type );
OSErr  CalcT2( IMAGE *im, float *data, unsigned char *Mask, float *TEs, float *T2, float *Rho, float *chi2 );
OSErr  imageDataRange( void *vec, long npts, void *hicutoff, void *noiseLevel, long *nonNoise, int data_type );
long   ThreshMask( void *theImage,  unsigned char *theMask, long imageSize, void *threshold, int data_type );
long   vThreshold( void *theImage, long vlen, void *threshold, int data_type );
OSErr  autoMask( void *invec, unsigned char *theMask, long npts, void *threshold, long *nonNoise, int data_type );
OSErr  autoThresh( void *invec, long npts, void *threshold, long *nonNoise, int data_type );
OSErr  FloatToCImage( float *inImage, void *cimage, long len, float *fmax, float *fmin,
                      float newMax, float newMin, short rescale );
OSErr  ImageChangeDetect( unsigned short *theImage, unsigned char *theMask, float threshold, long npts,
                    long *numLowerPts, long *numGreaterPts );
long   MaskChangeDetect( unsigned char *oldImage, unsigned char *newMask, long npts );
OSErr  noiseCheck( unsigned short *theImage, float threshold, float noiseLevel, long npts, float *increase );
double vRescale( void *invec, long vlen, void *oldMax, void *oldMin,
                 void *newMax, void *newMin, int data_type );
OSErr  getRowsCols( short slices, short *ROWS, short *COLS );
OSErr  mdisplay( void *OutImg, void *TheImg, int xs, int ys, int slices, int data_type );
OSErr  unmdisplay( void *InImg, void *OutImg, int xs, int ys, int slices, int data_type );
void   CropImageSides( void *ImageData, short UnCropdXSize, short ysize,
                       short LeftSkip, short RightSkip, short bytesPix );

/* fill in an IMAGE struct based on headerData contents */
OSErr  ParseSignaHeader( void *headerData, IMAGE *im );
OSErr  ParseMGHHeader( char *headerData, IMAGE *im );
OSErr  ParseDSR( char *headerData, IMAGE *im, short fileSize );
OSErr  ParseANMRHeader( char *headerData, IMAGE *im );
OSErr  ParseVarian( char *headerData, IMAGE *im );
void   SetImDatatype( IMAGE *im, short data_type );

/* FILE ROUTINES - MOST REQUIRE LOCALIZATION TO UNIX OR MAC */

OSErr  ReadNextFileString( short fileID, char *myString );
void   InitializeIm( IMAGE *im );
char   *irpStringFromIMAGE( IMAGE *im );
OSErr  CreateHeader( int FileType, IMAGE *im, char *baseName );
OSErr  FindOutputType( char *fname, short *itsType, short *data_type );
OSErr  dsrFromIMAGE( IMAGE *im, dsr *theDSR );
OSErr  VarianFromIMAGE( IMAGE *im, dsr *theDSR );
OSErr  WriteMGHImage( void *imData, char *filename, short xs, short ys, short zs, int data_type ); /* ImgLib.c */

#ifdef MAC
OSErr  get_paradigm( void );
OSErr  WaitNextItemFromList( char *Item, short fileID, int WaitSec );
char   *ReadLocalFile( char *fn, long *fileSize, FSSpec *refFS, Boolean RT, OSErr  *ERR );
OSErr  ReadLocalFileBytes( char *fn, char *data, long numBytes, FSSpec *refFS );
OSErr  UC_Readheader( FSSpec *imFileFS, IMAGE *im, Boolean RT );
OSErr  UC_Readimage2D( short imFileID, IMAGE *im, void *image_data, short imageIndex );
OSErr  UC_Readimage( short fp, IMAGE *im, void *image_data, int image_slice, int TimePt );
OSErr  GetSelectedVolume( short imFileID, IMAGE *im, void *theVolume, short TimePt );
#endif

#ifdef UNIX
OSErr  UC_Readheader( char *fn, IMAGE *im );
OSErr  WaitNextItemFromList( char *Item, FILE *fp, char *fname, int WaitTime );
OSErr  get_para_file_size( char *fname );
OSErr  get_paradigm( char *fname, float *pv, int npts );
char   *ReadLocalFile( char *fn, long *fileSize, OSErr  *ERR );
OSErr  ReadLocalFileBytes( char *fn, char *data, long numBytes );
OSErr  UC_Readimage2D( FILE *fp, IMAGE *im, void *image_data, int imageIndex );
OSErr  UC_Readimage  ( FILE *fp, IMAGE *im, void *image_data, int image_slice, int image_t );
OSErr  NextVolume( FILE *imFile, IMAGE *im, void *theVolume, int TimePt, int crop );
OSErr  GetSelectedVolume( FILE *imFile, IMAGE *im, void *theVolume, int TimePt );
FILE  *waitfopen( char *name, char *mode, int WaitTime );
#endif

OSErr  open_ImgLib ( short dbg );
OSErr  close_ImgLib ( short dbg );
#endif
/* </PRE></BODY></HTML> */
