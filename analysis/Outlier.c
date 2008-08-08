/* <html><head><title>Outlier source</title></head><body>
<table cellpadding="3" border="1"><tr><td>Outlier scans through a time series of images and finds those that
are likely to be artifacts, based on motion, signal intensity aberrations and the
like.
</td></tr></table>
<h2>How to make Outlier</h2>
<ol>
<li>Build ImgLib.o analyzeUtil.o ImgLibError.o VLib.o (from
<a
href="http://airto.loni.ucla.edu/BMCweb/SharedCode/SharedSoftware.html#Anchor-UCLA-35829"> www.brainmapping.org</a>)
<li>compile Averager, linking the objects above. You will need to have C++ extensions
and the Math Library.
A typical command might be:<br><font face=courier>
cc ImgLib.o analyzeUtil.o ImgLibError.o VLib.o MathLib.o Outlier.c -lm -o Outlier</font>
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
<a href="http://airto.loni.ucla.edu/BMCweb/HowTo/Outlier.html">
Brain Mapping Center support pages</a>.
</td></tr></table></center>
<pre>*/
/* $Author: mscohen $
*  $Date: 2002/12/18 17:36:42 $
*  $Log: Outlier.c,v $
*  Revision 1.7  2002/12/18 17:36:42  mscohen
*  Updated web links. Fixed copmilation instructions.
*
*
*  Revision 1.5  2002/09/09 23:07:40  nina
*  changed fopen mode from "r"/"w" to "rb"/"wb" where applicable
*
*  Revision 1.4  2002/06/06 10:43:35  mscohen
*  Modified call to OpenProcFile
*  Fixed unsupported 'R' option
*
 * Revision 1.3  2002/01/28  21:58:31  mscohen
 * It was too talky...
 *
 * Revision 1.2  2002/01/28  21:37:47  mscohen
 * Added summary information to the procfile
 *
*/

#include "getopt.h"
#include "ImgLib.h"

#define false 0
#define true 1
#ifndef noErr
#define noErr 0
#endif

#ifdef MAC
#include <console.h>
#include <sioux.h>
#endif

// types of artifacts

#define isSpike    1
#define isMotion   2
#define isArtifact 4
#define kMotionThresh    2.0	// 2% of the pixels in the volume
#define kSpikeThresh    30.0	// 30% of the pixels in an image
#define kArtifactThresh 30.0	// 40% of the pixels in an image

/* globals */
typedef struct {
	char    InFileName[255];
	IMAGE   inIm;
	Boolean Verbose;
	FILE    *inFile;
	FILE    *ProcFile;
	short   MaskTimePt;
	float   SpikeThresh;	/* Threshold for spike detection (fraction of pixels) */
	float   MotionThresh; 	/* Pixel change threshold for motion */
	float   ArtifactThresh; /* fraction of changed pixels to call an "artifact" */
	long    NUnknownArtifacts;
	long    NSpikes;
	long    NMotion;
} UtilGlobal;

UtilGlobal u;

/* PROTOTYPES */
OSErr FindOutliers( FILE *inFile );
void  print_usage( char *name );
OSErr ProcessCommandLine( int argc, char *argv[] );

/*****************************     ProcessArtifactData      **************************
*  ProcessArtifactData
*
*	INPUTS:
*		numCumLowerPoints, numCumGreaterPoints
*		numSeqLowerPoints, numSeqGreaterPoints		--  difference from earlier images
*		priorSpike         -- the previous image had a spike artifact
*		IgnorePt           -- this point is not used in data analysis
*	OUTPUTS
*		artClass           -- the type of artifact detected.
*		error
*
*	Use the changes in the number of points above and below
*	Threshold to estimate the presence of artifacts
*	If an artifact is detected, set the 'artclass' to the current type.
*
**************************************************************************************/
#define logIgnored 1
OSErr ProcessArtifactData( long numCumLowerPts, long numCumGreaterPts,
			long numSeqLowerPts, long numSeqGreaterPts,
			Boolean *priorSpike, short *artClass, Boolean IgnorePt )
{
	Boolean  erase = true;
	float	 cumInc, cumDec, seqInc, seqDec, seqRatio, seqChanged, cumChanged;
	float    invVolSize;
	float	 VolSize;
	char	 myString[255];
	OSErr    error = noErr;
	
/* initialize */
	VolSize = u.inIm.dim.isoX * u.inIm.dim.isoY * u.inIm.dim.n_slices;
	invVolSize = 100/VolSize;                   /* 100/volSize to normalize as percent of volume */
	cumInc     = numCumGreaterPts * invVolSize; /* percent increased since reference point */
	cumDec     = numCumLowerPts   * invVolSize; /* percent decreased since reference point */
	seqInc     = numSeqGreaterPts * invVolSize; /* percent increased since prior point */
	seqDec     = numSeqLowerPts   * invVolSize; /* percent decreased since prior point */
	seqChanged = (numSeqGreaterPts + numSeqLowerPts) * invVolSize; /* percent changed since prior point */
	cumChanged = (numCumGreaterPts + numCumLowerPts) * invVolSize; /* percent changed since reference point */

/* difference in % of volume increasing and decreasing
Expressed as fraction of total changed over difference in the number increased and decreased */
	if( numSeqGreaterPts != numSeqLowerPts ) {
		seqRatio = fabs( (numSeqGreaterPts + numSeqLowerPts)/(numSeqGreaterPts - numSeqLowerPts) * invVolSize );
	} else {
		seqRatio = (numSeqGreaterPts + numSeqLowerPts) * invVolSize;
	}
	
	if( !(*priorSpike) ) {		/* checks are not relevant following a spike */
/* Artifact detect looks for an unusual number of change points*/
		if( seqChanged > u.ArtifactThresh || cumChanged > u.ArtifactThresh ) {
			*artClass += isArtifact;
			u.NUnknownArtifacts++;
		}

/* motion check */
/* This works as a crude motion check, because if a large fraction of pixels change identity, but a near
	equal number increase and decrease, motion is the most parsimonious interpretation */
		if( seqRatio > u.MotionThresh ) {
			*artClass += isMotion;
			u.NMotion++;
		}
	}

/* spike check */
/* Spike check is performed by looking at the number of pixels that have exceeded nominal
	background threshold, compared to the reference image */	
	*priorSpike = false;	/* clear the priorSpike flag */
	if( cumInc > u.SpikeThresh ) {
		*priorSpike = true;
		*artClass += isSpike;
		u.NSpikes++;
	}
	
	fprintf( u.ProcFile, "%0.2f\t%0.2f\t%0.2f\t%0.2f\t%0.2f\t", seqInc, seqDec, cumInc, cumDec, seqRatio );
//	fprintf( u.ProcFile, "%s\t%s\t%s", YorN( *artClass & isSpike ), YorN( *artClass & isMotion ), YorN( *artClass & isArtifact ));
	fprintf( u.ProcFile, "%d\t%d\t%d", *artClass & isSpike, *artClass & isMotion, *artClass & isArtifact );

	return error;
}

/******************************    FindOutliers     ***************************
*   This is where the work will be done
*************************************************************************************/
OSErr FindOutliers( FILE *inFile )
{
	OSErr   error = noErr;
	short   theTimePt;
	unsigned short  *inData;
	unsigned short  noiseThreshold, dataMax;	/* dataMax is just a place holder */
	long    VolSize;
	long    inVolSize;
	long    numCumLowerPts, numCumGreaterPts, numSeqLowerPts, numSeqGreaterPts;	/* no. of changed pixels */
	long    numNotNoise;
	int     i;
	float   fThresh;
	unsigned char    *theMask, *priorMask;
	short   ArtifactClass;
	Boolean PriorSpike = false;
	Boolean IgnorePt = false;

	VolSize  = u.inIm.dim.isoX * u.inIm.dim.isoY * u.inIm.dim.n_slices;
	inVolSize = VolSize * get_datasize( u.inIm.data_type );
	inData    = (unsigned short *)ck_malloc( inVolSize,  "Storage for input data" );
	theMask   = (unsigned char  *)ck_malloc( VolSize,  "Storage for threshold mask data" );
	priorMask = (unsigned char  *)ck_malloc( VolSize,  "Storage for threshold mask data" );

/* create the image mask based on the first time point */
	u.inIm.data_type = T_USHORT;
	error = GetSelectedVolume( inFile, &u.inIm, inData, u.MaskTimePt );
	RETURNONERROR;

	numCumLowerPts = numCumGreaterPts = numSeqLowerPts = numSeqGreaterPts = 0;
	
	error = autoMask( inData, theMask, VolSize, &noiseThreshold, &numNotNoise, T_USHORT );
	RETURNONERROR;

	memcpy( priorMask, theMask, VolSize );

	fThresh = noiseThreshold;
	
	fprintf( u.ProcFile,"\nImg\tSeq+\tSeq-\tCum+\tCum-\tRatio\tSpk\tMov\tOther\n" );
	for( i=0; i<60; i++ ) { fprintf( u.ProcFile, "=" ); }
	fprintf( u.ProcFile, "\n" );

	for( theTimePt = 0; theTimePt < u.inIm.dim.timePts; theTimePt++ ) {

		error = GetSelectedVolume( inFile, &u.inIm, inData, theTimePt );
		RETURNONERROR;

/*
	Image Change detect compares the first argument to the second.
	Here we compare the current image to the first in the series
	(the 'cumulative' signal change)
*/
		error = ImageChangeDetect( inData, theMask, fThresh, VolSize,
		                      &numCumLowerPts, &numCumGreaterPts );
		RETURNONERROR;

/*
	'sequential' signal change is derived by comparing the current
	image to the prior image
*/
		error = ImageChangeDetect( inData, priorMask, fThresh, VolSize,
			                      &numSeqLowerPts, &numSeqGreaterPts );
		RETURNONERROR;

/* calculate priorMask from the current data */
/* long   ThreshMask( void *theImage,  unsigned char *theMask, long imageSize, void *threshold, int data_type ); */

		ThreshMask( (void *)inData, priorMask, VolSize, (void *)&noiseThreshold, T_USHORT );
	
/* log everything to the proc file */
		fprintf( u.ProcFile, "%d\t", theTimePt+1 );

/*	If an artifact is detected, set the 'ArtifactClass' to the current type */
		ArtifactClass = 0;
		error = ProcessArtifactData( numCumLowerPts, numCumGreaterPts, numSeqLowerPts,
		     numSeqGreaterPts, &PriorSpike, &ArtifactClass, IgnorePt );
		RETURNONERROR;

		fprintf( u.ProcFile, "\n" );

/* Advise the user if artifacts detected. */
		if(ArtifactClass)  {
			printf( "%d: ", theTimePt );
		}
		if(ArtifactClass & isSpike ) {
			printf( "Spike - " );
		}
		if(ArtifactClass & isMotion ) {
			printf( "Motion? - " );
		}
		if(ArtifactClass & isArtifact ) {
			printf( "Excess Changed pixels" );
		}
		if(ArtifactClass) {
			printf( "\n" );
		}
	}

	if( u.NSpikes > 0 ) {
		printf( "Spikes detected:   %ld\n", u.NSpikes );
		fprintf( u.ProcFile, "Spikes detected:   %ld\n", u.NSpikes );
	}
	if( u.NMotion > 0 ) {
		printf( "Motions detected:   %ld\n", u.NMotion );
		fprintf( u.ProcFile, "Motions detected:   %ld\n", u.NMotion );
	}
	if( u.NUnknownArtifacts > 0 )  {
		printf( "Other artifacts detected:   %ld\n", u.NUnknownArtifacts );
		fprintf( u.ProcFile, "Other artifacts detected:   %ld\n", u.NUnknownArtifacts );
	}
	return error;
}

/* functions */
void print_usage( char *name )
{
	printf( "USAGE: %s -i infile.ext1 [r | R | A | S]\n\n", name );
	printf( "\tr is the image number (from 1) to detect background threshold [default is image 1]\n" );
	printf( "\tR is the maximum acceptable ratio of decreasing to increasing pixel intensities [%0.1f%%]\n", kMotionThresh );
	printf( "\tA is the maximum fraction of pixels whose signal changed between signal & noise [%0.1f%%]\n", kArtifactThresh );
	printf( "\tS is the maximum fraction of background pixels that can exceed background threshold [%0.1f%%]\n\n", kSpikeThresh );
	printf( "Note that the spike and general artifact thresholds are computed on a per slice basis,\n" );
	printf( "and that 'mdisplay' formatted images are treat the entire volume as a single slice\n" );
	printf( "- thus the thresholds may need to be lowered accordingly\n\n" );
	printf( "More information can be found on the Brain Mapping lab HowTo pages\n" );
}

/******************************    ProcessCommandLine     ***************************
*   capture the user input, mostly globals
*************************************************************************************/
OSErr ProcessCommandLine( int argc, char *argv[] )
{
	OSErr error = noErr;

	int argp = EOF;
	short argsProcessed = 0;

	while ( (argp=getopt(argc,argv,"i:r:S:R:A:Vh?OPTIONS")) != EOF )
	switch( argp )   {
		case 'i':
			strcpy( u.InFileName, optarg );
		break;

		case 'V':
			u.Verbose = true;
		break;

		case 'r':
			u.MaskTimePt = atoi( optarg ) - 1;
			if( u.MaskTimePt < 0 ) {
				u.MaskTimePt = 0;
			}
		break;

		case 'S':
			u.SpikeThresh = atof( optarg );
		break;

		case 'M':
			u.MotionThresh = atof( optarg );
		break;

		case 'A':
			u.ArtifactThresh = atof( optarg );
		break;

		case 'h':
		case '?':
		default:
			print_usage( argv[0] );
			exit( -1 );
	}
	
/* check for required arguments */
	if( !strlen( u.InFileName )) {
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

/******************   Initialize   ************************/
OSErr Initialize( void )
{
	u.MaskTimePt     = 0;	/* unless the user over-rides this, the first time pt is used for a mask */

/* thresholds are defined as percent changes. */
	u.SpikeThresh    = kSpikeThresh;
	u.MotionThresh   = kMotionThresh;
	u.ArtifactThresh = kArtifactThresh;
	u.NMotion = u.NUnknownArtifacts = u.NSpikes = 0;
	
	return noErr;
}
	
/******************************************/

int main( int argc, char **argv)
{
/*  sample command lines
-i Visual.bshort -m 3 -M 0.1 -A 2 -S 5
*/
	OSErr   error = noErr;
	static  char id[] = "$Revision: 1.7 $";

#ifdef mactest
	SIOUXSettings.autocloseonquit = false;
	SIOUXSettings.asktosaveonclose = false;
	argc = ccommand(&argv);
#endif
	Initialize();

	error = ProcessCommandLine( argc, argv );
	ILError( error, "error in command line" );

	error = OpenProcFile( argc, argv, u.InFileName, id, &u.ProcFile );
	ILError( error, "Opening proc file" );

	fprintf( u.ProcFile, "Reference time point ...... %d\n", u.MaskTimePt + 1 );
	fprintf( u.ProcFile, "\nSpike Threshold: ....... %0.3f%%\n", u.SpikeThresh );
	fprintf( u.ProcFile, "Motion Threshold: ...... %0.3f%%\n", u.MotionThresh );
	fprintf( u.ProcFile, "Artifact Threshold: .... %0.3f%%\n", u.ArtifactThresh );
/* read in the input image header */
	error = UC_Readheader( u.InFileName, &u.inIm );
	ILError( error, "Input image header" );
	
	u.SpikeThresh    /= u.inIm.dim.n_slices; /* divide by n_slices, because these can occur on single slices */
	u.ArtifactThresh /= u.inIm.dim.n_slices;

	error = ck_fopen( &u.inFile, u.InFileName,  "rb" );
	ILError( error, "Opening infile" );

/* do something */
	error = FindOutliers( u.inFile );
	ILError( error, argv[0] );

	error = ck_fclose( u.inFile );
	ILError( error, "infile" );

/* dump processing file contents */
	ck_fclose( u.ProcFile );

	return 0;
}

/* </pre></body></html> */

