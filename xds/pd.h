/************************************************************************
 * Copyright (C) 1992 Advanced NMR Systems, Inc. All Rights Reserved    *
 * 									*
 * This software contains information confidential to Advanced NMR      *
 * and its suppliers, and may not be used, copied or disclosed except   *
 * as agreed by Advanced NMR in writing.				*
 *								        *
 ************************************************************************/

/****************************************************************************
Filename: pd.h
Purpose : header file for processing definition.
Functions/Objects :
Date    : 11/02/91
Author  : Jim Wang
Modifications:
	<author> <date> <modification description>
	J.Wang	11/15/91 Changed the displayHdr structure's timedata field to
					 be interImage.
	J.Wang	04/02/92 DisplayHdr has a new field numInterImages.
	J.Wang	04/13/92 Removed the scale field in PDParaStruct.
****************************************************************************/

/*************
MSC:  I have altered all occurences of type long to be type int. While int and long are the
same length (4 bytes) on the Sun, longs are 8 bytes on the DEC, while ints are 4 bytes.

Bools turn out to be 4 bytes (on the Sun and DEC).
*************/

#ifndef PD_H
#define PD_H

#include <X11/Xlib.h>

/* APD Header definitions */

/*  header found at the beginning of every image file in APD System. Although
    certain fields of the header are meaningful outside the system, the header
    itself is only meaningful with the APD */

typedef struct displayHdr
    {
    ushort          style;          /* style sheet number                   */
    Bool            geSave;         /* flag indicating whether APD transfers
                                       the image file & whether the Signa will
                                       save it. interImage == 0 implies that
                                       geSave == OFF                        */
    Bool            anmrSave;       /* flag indicating whether APD saves the
                                       image file                           */
    Bool            rotation;       /* flag indicating 90deg clockwise rot'n*/
    Bool            reflectionX;    /* flag indicating left-right reflection*/
    Bool            reflectionY;    /* flag indicating top-bottom reflection*/
    float           scale;          /* */
    float           shift;          /* */
    ushort          displayX;       /* display X size                       */
    ushort          displayY;       /* display Y size                       */
    ushort          slice;          /* slice number starting with 0         */
    ushort          image;          /* image number starting with 0         */
    int          	interImage;     /* intermediate image number.			*/
             				/* interImage==0 means final image.		*/
    ushort          echo;           /* echo number                          */
    ushort          phase;          /* phase of scan                        */
    int             numInterImages; /* total number of intermediate images  */
    int             expand1;        /* expansion variable 1                 */
    int             expand2;        /* expansion variable 1                 */
    int             expand3;        /* expansion variable 1                 */
    int             expand4;        /* expansion variable 1                 */
    }   DisplayHdr;

typedef struct imageColumn                  /* structure for pdfSetup() calls */
    {
        int     dispImages;                 /* number of displayable images */
        int     dispColumns;                /* number of postagestamp columns */
        int     psWidth;                    /* pixel width of a postage stamp */
        int     psHeight;                   /* pixel height of postage stamp  */
    } ImageColumn;

/*
 *  the following structure contains sizes of various kinds of data
 *  that a PDF processes
 */
typedef struct dataSizes
    {
    int xsize;      /*# of pixels of the scan matrix in x direction*/
    int ysize;      /*# of pixels of the scan matrix in y direction*/
    int xtime;      /*# of pixels of the timedata in x direction*/
    int ytime;      /*# of pixels of the timedata in y direction*/
    int ximage;     /*# of pixels of the con'ed image in x direction*/
    int yimage;     /*# of pixels of the con'ed image in y direction*/
    int xdisp;      /*# of pixels of the displayable image in x direction*/
    int ydisp;      /*# of pixels of the displayable image in y direction*/

    int numAverages;    /*# of frames needed to generate a complete timedata*/
    int mosaic;     /*1 for one shot; 2 for two shots*/

    /*total # of frames =numAverages * mosaic * numSlices * imagesPerSlices*/
    } DataSizes;

/*defines a structure which contains parameters for spike correction*/
typedef struct
	{
	int	xoff; 			/*number of columns in the left half of k space*/
	int yoff;			/* number of lines in the top half of k space*/
	float sp_thresh;	/*Factor above the average of adjoining
						 *points that a given time point must be in order
						 *be considered a spike */
	float sp_fac;		/*This value is multiplied by Threshfac to determine 
						 *the threshold above which to look for spikes.*/
	float sp_ign;		/*Doesn't check for spikes within a box that
						 *is sp_ign pixels around the center of the echo.*/
	} SpikeParaStruct;

/*the following structure defines pdf parameters which are derived from
	pdf symbols*/
typedef struct
	{
	int xtime;			/*time data size*/
	int ytime;			/*time data size*/
	int xfft;			/*processing size*/
	int yfft;			/*processing size*/
	int ximage;			/*reconstructed image size*/
	int yimage;			/*reconstructed image size*/
	int xdisp;			/*displayable image size*/
	int ydisp;			/*displayable image size*/

	int yextra;			/*for part-phase*/
	int xover_use;		/*for mosaic*/
	int numAverages;	/*number of averages*/

	int xinterp;		/*flag for interpolation in x*/
	int yinterp;		/*flag for interpolation in y*/

	} PDParaStruct;

#endif


