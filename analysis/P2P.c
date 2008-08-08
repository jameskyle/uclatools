/* <html><head><title>P2P source</title></head><body>
<h2>P2P</h2>
P2P is used to measure the stability of the signal intensity in a time series
of MR images. The program is intended to be used on a feature-filled MR phantom.
<h2>How to make P2P</h2>
<ol>
<li>Build ImgLib.o analyzeUtil.o ImgLibError.o VLib.o (from
<a
href="http://porkpie.loni.ucla.edu/BMD_HTML/SharedCode/SharedSoftware.html#Anchor-UCLA-35829
">www.brainmapping.org</a>)
<li>compile P2P, linking the objects above. You will need to have C++ extensions
and the Math Library.
A typical command might be:<br><font face=courier>
cc ImgLib.o analyzeUtil.o ImgLibError.o VLib.o P2P.c -lm -o P2P</font>
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
<a href="http://porkpie.loni.ucla.edu/HowTo/P2P-proto.html">
Brain Mapping Center support pages</a>.
</td></tr></table></center>
<pre>*/
/* $Author: mscohen $
*  $Date: 2004/06/11 07:18:38 $
*  $Log: P2P.c,v $
*  Revision 1.25  2004/06/11 07:18:38  mscohen
*  Cleaned up labeling of num slices.
*
*  Revision 1.5  2004/06/11 07:19:52  mscohen
*  Changed text output for num slices, which was listed wrongly before.
*
*  Revision 1.23  2004/04/18 00:16:39  mscohen
*  Fixed html to allow preformatted text from Zrinka
*
*  Revision 1.22  2004/04/16 23:52:32  mscohen
*  added html formatting option.
*
*  Revision 1.19  2004/03/09 17:03:42  mscohen
*  Strip path information from default name for logfile.
*
*  Revision 1.18  2004/02/09 19:02:22  mscohen
*  Removed excess verbiage from proc file.
*
*  Revision 1.4  2004/02/03 20:54:00  mscohen
*  synch with server
*
*  Revision 1.17  2004/02/03 20:47:41  mscohen
*  Added support for confidence intervals. Improved UI.
*
*  Revision 1.16  2004/02/01 08:56:42  mscohen
*  Replaced cc with r-squared. Removed rms deviation (useless)
*
*  Revision 1.3  2004/01/31 01:43:41  mscohen
*  Added better regional analysis
*
*  Revision 1.2  2004/01/30 21:22:45  mscohen
*  Revised for multi-slice analysis
*
*  Revision 1.1  2004/01/29 17:39:20  mscohen
*  Initial revision
*
*  Revision 1.13  2003/01/10 23:17:15  mscohen
*  Fixed bug that caused the output byte order to be wrong on some machines (UtilityTemplate bug).
*  Upgraded to latest template code base.
*  Made subroutine for ROI dimensions.
*
*  Revision 1.13  2003/01/10 22:21:42  mscohen
*  Fixed bug that caused the output byte order to be wrong on some machines (UtilityTemplate bug).
*  Upgraded to latest template code base.
*  Made subroutine for ROI dimensions.
*
*  Revision 1.11  2002/09/10 22:27:12  mscohen
*  changed call to OpenProcFile
*
*  Revision 1.10  2002/09/09 23:11:49  nina
*  changed fopen mode from "r"/"w" to "rb"/"wb" where applicable
*
*  Revision 1.9  2002/01/13 20:10:52  mscohen
*  Augmented error handling.
*
 * Revision 1.8  2002/01/13  16:52:52  mscohen
 * Added std. dev. measurements on the ROI time series and within the ROI.
 * Linked source to HowTo pages.
 * Fixed an initialization bug which caused inaccurate statistics on alphastations.
 * Added and tested support for ROI location and size options.
 * Fixed a bug that would cause errors when the ROI was outside the image FOV.
 * Comments refer to version 1.8 - these changes did not appear in 1.7
 *
 * Revision 1.6  2002/01/12  08:05:29  mscohen
 * Cosmetic formatting changes.
 *
 * Revision 1.5  2002/01/12  08:01:14  mscohen
 * This is the upgraded version of P2P, using the latest framework for utilities.
 *
*/

#include "getopt.h"
#include "ImgLib.h"
#include "MathLib.h"

#define false 0
#define true 1
#ifndef noErr
#define noErr 0
#endif

/* Definition of location and size of region sampled for logfile */
#define kYOFFSET -4
#define kXOFFSET -4
#define kREGSIZE 9
#define kHILITE 1.25
#define kEXTRA 1.5

#undef MAC
#ifdef MAC
#include <console.h>
#include <sioux.h>
#endif

/* globals */
typedef struct {
	char    OutFileName[255];
	char    InFileName[255];
	char    OutBaseName[256];
	short   output_datatype;
	short   NumOutImages;
	IMAGE   inIm;
	IMAGE   outIm;
	Boolean RawMode;	// if raw mode is set, do not normalize outputs
	Boolean Verbose;
	FILE    *inFile;
	FILE    *outFile;
	FILE    *ProcFile;
	
	char    logfileName[256];
	int     skip;
	int     xoffs, yoffs;
	int     regSize;
	float   hilite;
	float   Threshold;
	Boolean WebIt;
} UtilGlobal;

UtilGlobal u;

/* PROTOTYPES */
void  InitializeUserVariables( void );
OSErr SetROIDimensions( short xs, short ys, int *ROI_N );
OSErr SetOutputType( char *outFileName, short *outFileType, short *outFileDataType );
OSErr P2P( FILE *inFile, FILE *outFile );
OSErr AdjustOutputIMAGE( IMAGE *outIm );
void  print_usage( char *name );
OSErr ProcessCommandLine( int argc, char *argv[] );
OSErr OutBaseNameFromOutName( char *OutFileName, char *OutBaseName );
OSErr RunMain( int argc, char **argv, char *id );

/*****************      SetOutputType   ***********************
* Output type MUST be float. Therefore, only bfloat and analyze
* are supported.
**************************************************************/
OSErr SetOutputType( char *outFileName, short *outFileType, short *outFileDataType )
{
	OSErr error = noErr;
	short itsType, data_type;
	
	error = FindOutputType( outFileName, &itsType, &data_type );
	RETURNONERROR;

	if( data_type != T_FLOAT ) {
		printf( "The output file type will be forced to Analyze float\n" );
		
		*outFileType = ANALYZE;
		*outFileDataType = T_FLOAT;
		sprintf( outFileName, "%s.img", u.OutBaseName );
	}
	
	return error;
}
/****************************   Initialize   *************************/
void InitializeUserVariables( void )
{
	u.skip       = 0;
	u.xoffs      = kXOFFSET;
	u.yoffs      = kYOFFSET;
	u.regSize    = kREGSIZE;
	u.hilite     = kHILITE;
	u.NumOutImages = 0;
	u.WebIt      = false;
}

/********************    SetROIDimensions   ****************************
* Establish the size and location of the ROI used for image quality
* assessment.
***********************************************************************/
OSErr  SetROIDimensions( short xs, short ys, int *ROI_N )
{
	OSErr error = noErr;
	short maxRegSize;
	
	maxRegSize = xs < ys ? xs : ys;

	if( u.regSize > maxRegSize ) {
		 u.regSize = maxRegSize;
	}
	
	if( u.regSize == xs ) {
		u.xoffs = xs/2;
	}
	if( u.regSize == ys ) {
		u.yoffs = ys/2;
	}

	while( xs/2 + u.xoffs + u.regSize > xs ) {
		u.regSize--;
	}
	while( ys/2 + u.yoffs + u.regSize > ys ) {
		u.regSize--;
	}
	*ROI_N = u.regSize * u.regSize;
	
	return error;
}

/******************************    P2P     ***************************
*   Go through each time point and find:
*		- The maximum and minimum intensity at each location over time
*		- The average intensity over time
*		- The average intensity at each time point of a ROI
*		- The maximum and minimum of the average ROI intensity
**********************************************************************/
OSErr P2P( FILE *inFile, FILE *outFile )
{
	OSErr   error = noErr;
	long    theTimePt;
	short   theSlice;
	float   *inData;
	long    VolSize;
	long    ImSize;
	FILE    *logfile;
	long    i, j;
	long    n_timePts;
	short   n_slices;
	float   currPt;	/* the pixel we are working on */
	float   fillValue;

/* statistics for the target ROI */
	float   *ROImax, *ROImin;  /* max and min in the ROI */
	float   *ROIavg;           /* running sum (later average) of the ROI activity */
	double  *ROISy2, *ROISy;   /* sum of y2 and sum of y for std dev calculation */
	int     ROI_N;             /* number of pixels in ROI */
	float   HiCut;             /* place holder for ImgLib functions */
	long    numNotNoise;
	short   xsize, ysize;
	float   *ROILog;           /* ROI mean at each time point */
	float   *SliceCorrelation; /* Correlation of this slice with the average of the others */
	float   *ROITimeCorr;      /* Correlation of this slice with time */
	float   *ROISmooth;	       /* Smoothed representation of ROI time course */
	float   *ROISmoothAvg;     /* Average of all slices smoothed time course */
	float   *ROISlope;         /* Slope of the regression of the ROI against time */
	float   *ROIProb;          /* Probability that the slope is non-zero */
	float   *SlopeConfidence;          /* Confidence band of the slope */
	double  *ROI_SD;           /* ROI std dev for each slice */
	float   *minImage;         /* image of the minimum intensities at each location */
	float   *maxImage;         /* image of the maximum intensities at each location */
	float   *sumImage;         /* image of the summated intensities at each location */
	float   *Sx2Image;         /* for calculation of sd - sum of the squared pixel values */
	float   *regionImage;      /* image showing ROI hilited */
	float   *P2PImage;         /* Peak to peak intensity variations at each location */
	float   *sdImage;          /* the standard deviation as a map */
	long    N;                 /* number of time points used */
	double  SxTime, Sx2Time;   /* sums for regression against time */
	double  ssxxTime;          /* sum of squared deviations for time vector */
	float   diff;

	n_timePts = u.inIm.dim.timePts;
	N          = n_timePts - u.skip;
	if( N<2 ) {
		return DATATYPE_RANGE_ERROR;
	}
	
	n_slices    = u.inIm.dim.n_slices;
	xsize       = u.inIm.dim.isoX;
	ysize       = u.inIm.dim.isoY;
	ImSize      = xsize * ysize;
	VolSize     = ImSize * n_slices;
	
	ROILog           = (float *)ck_malloc( n_timePts * n_slices*sizeof( float ), "ROILog" );
	SliceCorrelation = (float *)ck_malloc( n_slices * sizeof( float ), "SliceCorrelation" );
	ROITimeCorr      = (float *)ck_malloc( n_slices * sizeof( float ), "ROITimeCorr" );
	ROISmooth        = (float *)ck_malloc( n_timePts * n_slices*sizeof( float ), "ROISmooth" );
	ROISmoothAvg     = (float *)ck_malloc( n_timePts * sizeof( float ), "ROISmoothAvg" );
	ROImax           = (float *)ck_calloc( n_slices, sizeof( float ), "ROImax" );
	ROIavg           = (float *)ck_calloc( n_slices, sizeof( float ), "ROIavg" );
	ROImin           = (float *)ck_malloc( n_slices * sizeof( float ), "ROImin" );
	for( i=0; i< n_slices; i++ ) {
		ROImin[i] = LONG_MAX;
	}

	ROISy       = (double *)ck_calloc( n_slices, sizeof( double ), "ROISy" );
	ROISy2      = (double *)ck_calloc( n_slices, sizeof( double ), "ROISy2" );
	ROISlope    = (float *)ck_calloc( n_slices, sizeof( float ), "ROISlope" );
	ROIProb     = (float *)ck_calloc( n_slices, sizeof( float ), "ROIProb" );
	SlopeConfidence      = (float *)ck_calloc( n_slices, sizeof( float ), "SlopeConfidence" );
	ROI_SD      = (double *)ck_calloc( n_slices, sizeof( double ), "ROI_SD" );
	inData      = (float *)ck_malloc( VolSize * sizeof( float ), "Storage for input data" );
	P2PImage    = (float *)ck_calloc( VolSize, sizeof( float ),  "Storage for P2P image" );
	sumImage    = (float *)ck_calloc( VolSize, sizeof( float ),  "Storage for sum image" );
	Sx2Image    = (float *)ck_calloc( VolSize, sizeof( float ),  "Storage for Sx2" );
	minImage    = (float *)ck_calloc( VolSize, sizeof( float ),  "Storage for min image" );
	maxImage    = (float *)ck_calloc( VolSize, sizeof( float ),  "Storage for max image" );
	sdImage     = (float *)ck_calloc( VolSize, sizeof( float ),  "Storage for sdtev image" );
	regionImage = (float *)ck_calloc( VolSize, sizeof( float ),  "Storage for region image" );

/* don't let the ROI be larger than the image */
	error = SetROIDimensions( xsize, ysize, &ROI_N );
	
/* initialize the intensities in minImage to a large constant */
	fillValue = LONG_MAX;
	error = vfill( minImage, &fillValue, VolSize, T_FLOAT );
	RETURNONERROR;

	u.inIm.data_type = T_FLOAT;	/* read the data in as float */

	printf( "Read " );
	fflush( stdout );
	for( theTimePt = u.skip; theTimePt < n_timePts; theTimePt++ ) {
		error = GetSelectedVolume( inFile, &u.inIm, inData, theTimePt );
		RETURNONERROR;

		if( theTimePt == u.skip )  /* first image special */
		{
			if( u.Threshold == 0 ) {  /* if the user didn't enter a threshold, find one */
				error = imageDataRange( inData, VolSize, &HiCut, &u.Threshold, &numNotNoise,  T_FLOAT );
				RETURNONERROR;

/* Set threshold a little higher than calculated */
				u.Threshold = u.Threshold * kEXTRA;
			}

			numNotNoise = vThreshold( inData, VolSize, &u.Threshold, T_FLOAT );
			if( u.Verbose ) {
				printf( "\nFirst Image Threshold:%0.4f\tNon-Noise Pts: %d\n",
				         u.Threshold, numNotNoise );
			}
		}

/* find the statistics for each pixel */
		for ( j=0; j< VolSize; j++ )  {
			currPt = inData[j];
			Sx2Image[j] += currPt * currPt;
			if( currPt > u.Threshold ) {
				minImage[j] = ( currPt < minImage[j] ? currPt : minImage[j] );
				maxImage[j] = ( currPt > maxImage[j] ? currPt : maxImage[j] );
				sumImage[j] += currPt;
			}
		}  /* min max each image */

/* Find the mean for an test region near the center of each image */
		for( theSlice = 0; theSlice < n_slices; theSlice++ ) {
			long SlcOff = ImSize * theSlice;
			float ROIMean;	// local copy for compactness of code

			ROIMean = 0;
			for (j = (ysize/2 + u.yoffs); j< (ysize/2 + u.yoffs + u.regSize); j++ ) {
				for (i = (xsize/2 + u.xoffs); i < (xsize/2 + u.xoffs + u.regSize); i++ ) {
					currPt = inData[SlcOff + j * xsize + i];
					ROIMean += currPt;
				}
			}

			ROIMean /= ROI_N;
	 		ROILog[theTimePt * n_slices + theSlice] = ROIMean;

/* calculate min, max and average for the target region */
			ROImax[theSlice]
				= ROImax[theSlice] > ROIMean ? ROImax[theSlice] : ROIMean;

			ROImin[theSlice]
				= ROImin[theSlice] < ROIMean ? ROImin[theSlice] : ROIMean;

			ROIavg[theSlice] += ROIMean;
			ROISy[theSlice]  += ROIMean;
			ROISy2[theSlice] += ROIMean * ROIMean;
		}	/* slice loop */

		SxTime  += theTimePt;
		Sx2Time += theTimePt * theTimePt;
	}   /* loop through time points */

	ssxxTime = Sx2Time - (SxTime * SxTime) / N;

	if( u.Verbose ) fprintf(stderr,"\n");

	printf( "Evaluate " );
	fflush( stdout );

/* Prepare the low frequency time course */
	error = vmov( ROILog, 1, ROISmooth, 1, n_slices * N, T_FLOAT );
	RETURNONERROR;
	
	for( theSlice=0; theSlice<n_slices; theSlice++ ) {
		error = vGaussSmooth( &ROISmooth[theSlice], n_timePts/20, n_slices, N, T_FLOAT );
		RETURNONERROR;
	}
	
/* Create the average of the smoothed time course for all slices */
	for( theTimePt=0; theTimePt<N; theTimePt++ ) {
		ROISmoothAvg[theTimePt] = (float)vsum( &ROISmooth[theTimePt*n_slices], 1, n_slices, T_FLOAT ) / n_slices;
	}



/* Summarize the time history */
#define TINY 1.0e-20
	for( theSlice=0; theSlice < n_slices; theSlice++ ) {
		float   xval, yval;
		float   den;
		double  Sx, Sx2, Sy, Sy2, Sxy;
		double  prob, slope;	// higher resolution local variables
		double  t;
		double  ssxy, ssxx, ssyy;
		double  Sb;   /* standard error of the slope */
		double  r; /* correlation coefficient */

	/* find correlations among slices */
		Sx = 0; Sx2 = 0; Sy = 0; Sy2 = 0; Sxy = 0;

		for( theTimePt=0; theTimePt<N; theTimePt++ ) {
			xval = (n_slices * ROISmoothAvg[theTimePt] - xval) / (n_slices -1);
			Sx  += xval;
			Sx2 += xval * xval;

			yval = ROISmooth[theTimePt*n_slices + theSlice];
			Sy  += yval;
			Sy2 += yval * yval;

			Sxy += xval * yval;
		}
	
		ssxy = Sxy - ( Sx * Sy ) / N;
		ssxx = Sx2 - ( Sx * Sx ) / N;
		ssyy = Sy2 - ( Sy * Sy ) / N;
	
		den = sqrt(ssxx * ssyy);
		if( den < TINY ) {
			ILError( DIVIDE_BY_ZERO_ERR, "Correlation to time" );
		}
		SliceCorrelation[theSlice] = (float)( ssxy / den );

	/* find the average intensity of our ROI through time */
		ROIavg[theSlice] /= N;
		if( ROIavg[theSlice] == 0 ) {
			return DIVIDE_BY_ZERO_ERR;
		}
	
	/* calculate ROI temporal Std Dev. */
		ROI_SD[theSlice] = sqrt( ( N*ROISy2[theSlice] - (ROISy[theSlice] * ROISy[theSlice]) ) / (N*N-N) );

	/* slope */
		Sy = Sy2 = Sxy = 0;
		for( theTimePt=0; theTimePt<N; theTimePt++ ) {
			yval = ROILog[theTimePt*n_slices + theSlice];
			Sy  += yval;
			Sy2 += yval * yval;

			Sxy += theTimePt * yval;
		}
		ssxy = Sxy - (SxTime * Sy) / N;
		ssyy = Sy2 - ( Sy * Sy ) / N;
		
		slope = ssxy / ssxxTime;
		ROISlope[theSlice] = slope;
		
		den = sqrt(ssxxTime * ssyy);
		if( den < TINY ) {
			return DIVIDE_BY_ZERO_ERR;
		}

		r = ssxy / den;
		ROITimeCorr[theSlice] = r;
		
		den = (1.0 - r*r );
		if( den < TINY ) {
			return DIVIDE_BY_ZERO_ERR;
		}		
		
		t = r * sqrt((N-2)/den);

		prob = betai(0.5*(N-2), 0.5, (N-2)/(N-2+t*t));
		if( prob > 1.0 ) {
			prob = 1.0;
		}
		if( prob <= 10E-9 ) {
			prob = 10e-9;
		}

/* Log probability */
//		ROIProb[theSlice] = (float)(-log(prob)/(float)ln_10)
		ROIProb[theSlice] = prob;
		
		Sb = slope / t;

		SlopeConfidence[theSlice] = 1.984 * Sb;
		
		if( !u.RawMode ) {
			ROISlope[theSlice]        = 100 * ROISlope[theSlice] / ROIavg[theSlice];
			SlopeConfidence[theSlice] = 100 * SlopeConfidence[theSlice] / ROIavg[theSlice];
			ROI_SD[theSlice] = 100 * ROI_SD[theSlice] / ROIavg[theSlice];
		}

/* highlight the region of interest */
		for (j = (ysize/2 + u.yoffs); j< (ysize/2 + u.yoffs + u.regSize); j++ ) {
			long SlcOff = ImSize * theSlice;
			for (i = (xsize/2 + u.xoffs); i < (xsize/2 + u.xoffs + u.regSize); i++ ) {
				regionImage[SlcOff + j*xsize + i] *= u.hilite;
			}
		}
	}

#undef TINY

	if( u.Verbose ) {
		for( theSlice = 0; theSlice<n_slices; theSlice ++ ) {
			fprintf(stderr,"Max in Slice %d: %f\nMin in ROI: %f\n", theSlice, ROImax[theSlice], ROImin[theSlice]);
		}
	}

/* Check for zero values to avoid divide by zero errors */
	for ( j=0; j < VolSize; j++ )  {
		if( sumImage[j] == 0 )  {
			P2PImage[j] = 0;
			minImage[j] = 0;
			regionImage[j] = 0;
		} else  {

/* Create the standard deviation map */
			sdImage[j] = sqrt( (( Sx2Image[j] - (sumImage[j] * sumImage[j])/N ) / (N-1) ));

/* Create P2PImage expressed as percent */
			sumImage[j] /= N;
			P2PImage[j] = 100 * (maxImage[j] - minImage[j])/sumImage[j];
			regionImage[j] = sumImage[j];
		}
	}


	printf( " Write\n\n" );
	fflush( stdout );
	error = ck_fwrite( P2PImage,    sizeof( float ), VolSize, outFile );
	RETURNONERROR;
	u.NumOutImages++;
	error = ck_fwrite( sdImage,     sizeof( float ), VolSize, outFile );
	RETURNONERROR;
	u.NumOutImages++;
	error = ck_fwrite( maxImage,    sizeof( float ), VolSize, outFile );
	RETURNONERROR;
	u.NumOutImages++;
	error = ck_fwrite( minImage,    sizeof( float ), VolSize, outFile );
	RETURNONERROR;
	u.NumOutImages++;
	error = ck_fwrite( sumImage,    sizeof( float ), VolSize, outFile );
	RETURNONERROR;
	u.NumOutImages++;
	error = ck_fwrite( regionImage, sizeof( float ), VolSize, outFile );
	RETURNONERROR;
	u.NumOutImages++;

	error = ck_fopen( &logfile, u.logfileName,  "w" );
	RETURNONERROR;
	
/* tabulate data into the log file */
	fprintf( logfile, "Image" );
	for( theSlice=0; theSlice<n_slices; theSlice++ ) {
		fprintf( logfile, "\tSlc_%0.2d", theSlice+1 );
	}	

	fprintf( logfile, "\tRawMean" );
	for( theSlice=0; theSlice<n_slices; theSlice++ ) {
		fprintf( logfile, "\tSm_%0.2d", theSlice+1 );
	}	

	fprintf( logfile, "\tSmoothMean" );
	for( theSlice=0; theSlice<n_slices; theSlice++ ) {
		fprintf( logfile, "\tDev_%0.2d", theSlice+1 );
	}	

	fprintf( logfile, "\n" );

	for (theTimePt=u.skip; theTimePt< n_timePts; theTimePt++ )  {
		float average;
	
		fprintf( logfile, "%d\t", theTimePt+1 );

/* Raw time Series */
		average = 0;
		for( theSlice=0; theSlice<n_slices; theSlice++ ) {
			if( u.RawMode ) {
				fprintf( logfile, "%0.4f\t", ROILog[theTimePt * n_slices + theSlice]);
			} else {
				fprintf( logfile, "%0.4f\t", 100 * ((ROILog[theTimePt * n_slices + theSlice]/ROIavg[theSlice]) - 1) );
			}
			average += ROILog[theTimePt * n_slices + theSlice];
		}
		fprintf( logfile, "%0.4f\t", average / n_slices );
		
/* Smoothed Time Series */
		average = 0;
		for( theSlice=0; theSlice<n_slices; theSlice++ ) {
			if( u.RawMode ) {
				fprintf( logfile, "%0.4f\t", ROISmooth[theTimePt * n_slices + theSlice]);
			} else {
				fprintf( logfile, "%0.4f\t", 100 * ((ROISmooth[theTimePt * n_slices + theSlice]/ROIavg[theSlice]) - 1) );			
			}
			average += ROISmooth[theTimePt * n_slices + theSlice];
		}
		fprintf( logfile, "%0.4f\t", average / n_slices );

/* Differentials */
		for( theSlice=0; theSlice<n_slices; theSlice++ ) {
			fprintf( logfile, "%0.4f\t",
			   100 * (ROILog[theTimePt*n_slices + theSlice] - ROISmooth[theTimePt*n_slices + theSlice])/ ROIavg[theSlice]);
		}

		fprintf( logfile, "\n" );
	}
	fclose( logfile );
	if( u.WebIt ) {
		fprintf( u.ProcFile, "</pre>\n<center>\n<h3>P2P Results Summary:</h3>\n" );	
		fprintf( u.ProcFile, "<table border=\"1\" cellspacing=\"1\" cellpadding=\"2\">\n\t<tr>\n\t\t<th nowrap>Slice</th>\n\t\t<th nowrap>Peak-Peak</th>\n" );
		fprintf( u.ProcFile, "\t\t<th nowrap>Average</th>\n\t\t<th nowrap>Std Dev</th>\n\t\t<th nowrap>r<sup>2</sup></th>\n\t\t<th nowrap>SD/r<sup>2</sup></th>\n" );
		fprintf( u.ProcFile, "\t\t<th nowrap>Slope +/- </th>\n\t\t<th nowrap>p of non-zero slope</th>\n\t</tr>\n" );
		for( theSlice = 0; theSlice < n_slices; theSlice++ ) {
			fprintf( u.ProcFile, "\t<tr align=\"center\">\n" );
			if( n_slices > 0 ) {
				fprintf( u.ProcFile, "\t\t<td nowrap>%d</td>\n", theSlice+1 );
			}
			fprintf( u.ProcFile, "\t\t<td nowrap>%0.4f %%</td>\n", (100 * (ROImax[theSlice] - ROImin[theSlice])/ROIavg[theSlice] ) );
			fprintf( u.ProcFile, "\t\t<td nowrap>%0.2f</td>\n", ROIavg[theSlice] );
			fprintf( u.ProcFile, "\t\t<td nowrap>%0.4f%%</td>\n", ROI_SD[theSlice] );
			fprintf( u.ProcFile, "\t\t<td nowrap>%0.4f</td>\n", SliceCorrelation[theSlice]*SliceCorrelation[theSlice] );
			fprintf( u.ProcFile, "\t\t<td nowrap>%0.4f</td>\n", ROI_SD[theSlice] / SliceCorrelation[theSlice]*SliceCorrelation[theSlice] );
			fprintf( u.ProcFile, "\t\t<td nowrap>%f ", ROISlope[theSlice] );
			if( u.RawMode ) {
				fprintf( u.ProcFile, "%% +/- " );
			} else {
				fprintf( u.ProcFile, "+/- " );
			}
			fprintf( u.ProcFile, "%f</td>\n", SlopeConfidence[theSlice] );
			fprintf( u.ProcFile, "\t\t<td nowrap>%f</td>\n", ROIProb[theSlice] );
			fprintf( u.ProcFile, "\t</tr>\n" );
		}
		fprintf( u.ProcFile, "</table>\n</center>\n<pre>" );
	} else {
		fprintf( u.ProcFile, "P2P Results Summary:\n" );
		fprintf( u.ProcFile,
		"Col 1: slice number\tCol 2: peak to peak change\tCol 4: Mean over time\tCol 5: Standard deviation over time\n"
		"Col 6: squared correlation between this slice and the denoised average of all slices\tCol 7: Col5/Col6\n"
		"Col 8: Slope (b) of the regression over time +/- 95%% confidence interval\tCol 9: Probability of non-zero slope\n\n" );
		
		for( theSlice = 0; theSlice < n_slices; theSlice++ ) {
			if( n_slices > 0 ) {
				fprintf( u.ProcFile, "Sl:\t%d\t", theSlice+1 );
			}
			fprintf( u.ProcFile, "P-P:\t%f\t", (100 * (ROImax[theSlice] - ROImin[theSlice])/ROIavg[theSlice] ) );
			fprintf( u.ProcFile, "Avg:\t%f\t", ROIavg[theSlice] );
			fprintf( u.ProcFile, "sd:\t%f\t", ROI_SD[theSlice] );
			fprintf( u.ProcFile, "r^2:\t%f\t", SliceCorrelation[theSlice]*SliceCorrelation[theSlice] );
			fprintf( u.ProcFile, "SD/r^2:\t%f\t", ROI_SD[theSlice] / (SliceCorrelation[theSlice]*SliceCorrelation[theSlice]) );
			fprintf( u.ProcFile, "b:\t%f", ROISlope[theSlice] );
			if( u.RawMode ) {
				fprintf( u.ProcFile, "%% +/- " );
			} else {
				fprintf( u.ProcFile, "+/- " );
			}
			fprintf( u.ProcFile, "%f\t", SlopeConfidence[theSlice] );
			fprintf( u.ProcFile, "p<\t%f\n", ROIProb[theSlice] );
		}
	}

	fprintf( u.ProcFile, "\n\nTIME PTS in FILE: ..... %d\n",n_timePts );
	fprintf( u.ProcFile, "NUMBER of POINTS USED:  %d\n",    u.skip );
	fprintf( u.ProcFile, "NUM SLICES:............ %d\n",    n_slices );
	fprintf( u.ProcFile, "LOG FILE: ............. %s\n",    u.logfileName );
	fprintf( u.ProcFile, "THRESHOLD: ............ %0.2f\n", u.Threshold );
	fprintf( u.ProcFile, "NON-NOISE PTS: ........ %d\n",    numNotNoise );
	fprintf( u.ProcFile, "\nROI LOCATION:\n\tleft: %d\ttop: %d\n\tright: %d\tbottom: %d\n\n",
			xsize/2+u.xoffs, ysize/2+u.yoffs, xsize/2+u.xoffs+u.regSize-1,
			ysize/2+u.yoffs+u.regSize-1);
	fprintf( u.ProcFile, "Image 0: P-P map\tImage 1: Std. Deviation Image\n" );
	fprintf( u.ProcFile, "Image 2: Maximum Image\tImage 3: Minimum Image\n" );
	fprintf( u.ProcFile, "Image 4: Average Image\tImage 5: ROI-highlighted Image\n\n" );
	fprintf( u.ProcFile, "The log file contains (for each slice):\n\t- the average signal in the RoI at each time point and its mean\n"
		"\t- the low pass filtered signal in the ROI and its mean\n"
		"\t- the deviation from the filtered time course, expressed as percent of the mean.\n" );

	fprintf( stderr, "\nP2P RESULTS SUMMARY:\n" );
	fprintf( stderr,
	"Col 1: slice number\tCol 2: peak to peak change\tCol 4: Mean over time\tCol 5: Standard deviation over time\n"
	"Col 6: squared correlation between this slice and the denoised average of all slices\tCol 7: Col5/Col6\n"
	"Col 8: Slope (b) of the regression over time +/- 95%% confidence interval\tCol 9: Probability of non-zero slope\n\n" );

	for( theSlice = 0; theSlice < n_slices; theSlice++ ) {
		double  ln_10 = log((double)(10) );

		fprintf( stderr, "Sl: %2.0d   ", theSlice+1 );
		fprintf( stderr, "P-P: %7.3f %%   ", (100 * (ROImax[theSlice] - ROImin[theSlice])/ROIavg[theSlice] ) );
		fprintf( stderr, "Avg: %7.2f   ", ROIavg[theSlice] );
		if( u.RawMode ) {
			fprintf( stderr, "sd: %4.3f   ", ROI_SD[theSlice] );
		} else {
			fprintf( stderr, "sd: %2.4f %%   ", ROI_SD[theSlice] );
		}
		fprintf( stderr, "r^2: %0.3f\t", SliceCorrelation[theSlice]*SliceCorrelation[theSlice] );
		fprintf( stderr, "sd/r^2: %2.3f\t", ROI_SD[theSlice]/(SliceCorrelation[theSlice]*SliceCorrelation[theSlice]) );
		if( u.RawMode ) {
			fprintf( stderr, "b: %7.5f +/- ", ROISlope[theSlice] );
		} else {
			fprintf( stderr, "b: %7.5f %% +/- ", ROISlope[theSlice] );
		}
		fprintf( stderr, "%f\t", SlopeConfidence[theSlice] );
		fprintf( stderr, " p < %0.5f\n", ROIProb[theSlice] );
	}

	fprintf( stderr, "\n\n\tTIME PTS in FILE: ..... %d\n",    n_timePts );
	fprintf( stderr, "\tNUMBER SKIPPED: ....... %d\n",    u.skip );
	fprintf( stderr, "\tLOG FILE: ............. %s\n",    u.logfileName );
	fprintf( stderr, "The log file contains (for each slice):\n\t- the average signal in the RoI at each time point and its mean\n"
		"\t- the low pass filtered signal in the ROI and its mean\n"
		"\t- the deviation from the filtered time course, expressed as percent of the mean.\n\n" );
	fprintf( stderr, "\tTHRESHOLD: ............ %0.2f\n", u.Threshold );
	fprintf( stderr, "\tNON-NOISE PTS: ........ %d\n",    numNotNoise );
	fprintf( stderr, "\nROI LOCATION:\n\tleft: %d\ttop: %d\n\tright: %d\tbottom: %d\n\n",
	        xsize/2+u.xoffs, ysize/2+u.yoffs, xsize/2+u.xoffs+u.regSize-1,
	        ysize/2+u.yoffs+u.regSize-1);
	fprintf( stderr, "Image 0: P-P map\tImage 1: Std. Deviation Image\n" );
	fprintf( stderr, "Image 2: Maximum Image\tImage 3: Minimum Image\n" );
	fprintf( stderr, "Image 4: Average Image\tImage 5: ROI-highlighted Image\n\n" );

	fprintf( stderr, "The log file contains (for each slice):\n\t- the average signal in the RoI at each time point and their means\n"
			"\t- the low pass filtered signal in the ROI and their mean\n"
			"\t- the deviation from the filtered time course, expressed as percent of the mean.\n\n" );

	free( Sx2Image );
	free( minImage );
	free( sumImage );
	free( regionImage );
	free( maxImage );
	free( sdImage );
	free( P2PImage );
	free( inData );
	free( SlopeConfidence );
	free( ROIProb );
	free( ROISlope );
	free( ROI_SD );
	free( ROISy2 );
	free( ROISy );
	free( ROImin );
	free( ROIavg );
	free( ROImax );
	free( ROISmoothAvg );
	free( ROISmooth );
	free( SliceCorrelation );
	free( ROILog );

	return error;
}

/*******************************   AdjustOutputIMAGE    *****************************
*	Alter contents of IMAGE struct for output to reflect differences from u.inImg
************************************************************************************/
OSErr AdjustOutputIMAGE( IMAGE *outIm )
{
	outIm->dim.timePts = u.NumOutImages;	// make sure that the output file is dimensioned.

	if( macByteOrder() ) {
		SET( outIm->dim.flags, kmacByteOrder );
	} else {
		CLEAR( outIm->dim.flags, kmacByteOrder );
	}

	return noErr;
}

/* standard functions */

/********************************   print_usage   ************************************/
void print_usage( char *name )
{
	printf(
		"\nUsage: %s -i infile.ext -o outfile.ext [ -N noise_threshold -s images to skip\n"
		"\t-X x offset of ROI -Y y offset of ROI -S ROI size -H hilite_factor -r (raw mode) -W (html)]\n\n"
		"Find the minimum and maximum signal in each pixel in a stack of images\n"
		"determines the peak to peak variation each pixel and computes the\n"
		"peak to peak signal variation in an %d by %d (default) about the center.\n\n"
		"x and y offset are relative to the center pixel (negative numbers allowed).\n"
		"The hilite factor is the amount be which the ROI will be increased for review (default: 1.25)\n"
		"If RawMode is not set, the data will be normalized to the first time point.\n"
		"\tFirst Image:   Peak to peak maximum %%\n"
		"\tSecond Image:  Standard deviation map\n"
		"\tThird Image:   Maximum in each pixel\n"
		"\tFourth Image:  Minimum in each pixel\n"
		"\tFifth Image:   Average of stack\n"
		"\tSixth image:   Highlighted region of interest\n\n", name, kREGSIZE, kREGSIZE );
	printf( "Outputs a processing record: outfile.%s.proc\n\n", name );
	printf( "and a log file of the ROI statistics (for graphing): outfile.%s.log\n\n", name );
	printf( "The log file contains (for each slice):\n\t- the average signal in the RoI at each time point and its mean\n"
			"\t- the low pass filtered signal in the ROI and its mean\n"
			"\t- the deviation from the filtered time course, expressed as percent of the mean.\n" );
}

/******************************    ProcessCommandLine     ***************************
*   capture the user input, mostly globals
*************************************************************************************/
OSErr ProcessCommandLine( int argc, char *argv[] )
{
	OSErr error = noErr;

	int argp = EOF;
	short argsProcessed = 0;

	while ( (argp=getopt(argc,argv,"i:o:s:N:X:Y:S:H:rWVh?OPTIONS")) != EOF )
	switch( argp )   {
		case 'o':
			strcpy( u.OutFileName, optarg );
		break;

		case 'i':
			strcpy( u.InFileName, optarg );
		break;

		case 's':
			u.skip = atoi( optarg );
		break;

		case 'N':
			u.Threshold = atof( optarg );
		break;

		case 'X':
			u.xoffs = atoi( optarg );
		break;

		case 'Y':
			u.yoffs = atoi( optarg );
		break;

		case 'S':
			u.regSize = atoi( optarg );
		break;

		case 'H':
			u.hilite = atof( optarg );
		break;

		case 'r':
			u.RawMode = true;
		break;

		case 'V':
			u.Verbose = true;
		break;

		case 'W':
			u.WebIt = true;
		break;

		case 'h':
		case '?':
		default:
			print_usage( argv[0] );
			exit( -1 );
	}
	
/* check for required arguments */
	if( !strlen( u.InFileName ) || !strlen( u.OutFileName )) {
		print_usage( argv[0] );
		exit( -1 );
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

/******************************************/

OSErr RunMain( int argc, char **argv, char *id )
{
	OSErr error = noErr;
	char  progName[256];
	char  *last, *p;
	
	InitializeUserVariables();

	error = ProcessCommandLine( argc, argv );
	RETURNONERROR;

	// fix illegal names
	p = argv[0];
        last = strrchr( p, '/' ); //zrinka 08/20/02 - added to handle program
        if ( last != NULL ) 		//names such as /usr/local/bin/imconvert
            p = last;
	while( *progName == '.' || *p == '/' ) {
		p++;
	}
	strcpy( progName, p );

	error = OpenProcFile( argc, argv, u.OutFileName, id, &u.ProcFile );
	RETURNONERROR;

/* read in the input image header */
	error = UC_Readheader( u.InFileName, &u.inIm );
	RETURNONERROR;

	memcpy( &u.outIm, &u.inIm, sizeof( IMAGE ));	// copy the input header to the output header, or?

	u.NumOutImages = 0;

	error = OutBaseNameFromOutName( u.OutFileName, u.OutBaseName );
	RETURNONERROR;

	sprintf( u.logfileName, "%s.P2PLog.txt", u.OutBaseName );

	error = SetOutputType( u.OutFileName, &u.outIm.file_type, &u.outIm.data_type );
	RETURNONERROR;

	error = ck_fopen( &u.inFile, u.InFileName,  "rb" );
	RETURNONERROR;

	error = ck_fopen( &u.outFile, u.OutFileName,  "wb" );
	RETURNONERROR;

/* do something */
	error = P2P( u.inFile, u.outFile );
	RETURNONERROR;

	error = ck_fclose( u.inFile );
	RETURNONERROR;

	error = ck_fclose( u.outFile );
	RETURNONERROR;

/* Change elements of output IMAGE (e.g., number of images/slices/timePts ) */
	error = AdjustOutputIMAGE( &u.outIm );
	RETURNONERROR;
	
	error = CreateHeader( u.outIm.file_type, &u.outIm, u.OutBaseName );
	RETURNONERROR;

	ck_fclose( u.ProcFile );

	return error;
}

/******************************************/

int main( int argc, char **argv)
{
	OSErr   error = noErr;
	static  char id[] = "$Revision: 1.25 $$Date: 2004/06/11 07:18:38 $";

#ifdef MAC
	SIOUXSettings.autocloseonquit = false;
	SIOUXSettings.asktosaveonclose = false;
	argc = ccommand(&argv);
#endif

	error = RunMain( argc, argv, id );
	ILError( error, "Running main program" );

}
/* </pre></body></html> */
