/* <HTML><HEAD><TITLE>MathLib routines</TITLE></HEAD><BODY><PRE>
/***************************************************************************
*	<font size=+2><b>MathLib.h - Part of ImgLib</b></font>
* <i>Please note last saved date below:</i>
*  <font size=+1><b>$Date: 2008/05/16 16:05:58 $</b></font><p>
*   A set of primitives for use in image processing.
*               c 1996-2002  Mark S. Cohen
*   This software is distributed as is with no guarantees.
*   Please report any errors or bug fixes to mscohen@ucla.edu
*
*   For academic use only. Commercial users should contact
*   Mark Cohen for licensing information.
*
<!--  *********************************************************************/
/* 
 * $Log: MathLib.h,v $
 * Revision 1.16  2008/05/16 16:05:58  mscohen
 * Embedded gaussSmooth functions
 * Added LinFit routine
 *
 * Revision 1.13  2006/03/26 06:10:32  mscohen
 * gasdev
 *
 * Revision 1.13  2006/03/25 08:48:03  mscohen
 * gasdev prototype
 *
 * Revision 1.12  2004/02/03 20:49:04  mscohen
 * Added correlation and probability functions.
 *
 * Revision 1.11  2002/09/11 16:16:52  mscohen
 * Modified $Log: MathLib.h,v $
 * Modified Revision 1.16  2008/05/16 16:05:58  mscohen
 * Modified Embedded gaussSmooth functions
 * Modified Added LinFit routine
 * Modified
 * Modified Revision 1.13  2006/03/26 06:10:32  mscohen
 * Modified gasdev
 * Modified
 * Modified Revision 1.13  2006/03/25 08:48:03  mscohen
 * Modified gasdev prototype
 * Modified
 * Modified Revision 1.12  2004/02/03 20:49:04  mscohen
 * Modified Added correlation and probability functions.
 * Modified directive for RCS
 *
 * Revision 1.10  2002/09/04 17:41:22  mscohen
 * Created GetMathLibVer()
 *
 * Revision 1.9  2002/09/03 19:24:08  mscohen
 * Upgraded to local RCS file.
 * Added vconvolve1D
 *
 * Revision 1.9  2002/09/03 19:20:50  mscohen
 * Added vconvolve1D
 *
 * Revision 1.8  2002/05/13  15:56:26  mscohen
 * Maintenance
 *
 * Revision 1.7  2002/04/16  16:58:00  mscohen
 * html
 *
 * Revision 1.7  2002/04/16  16:58:00  mscohen
 * html
 * */
/***************************************************************************
*	MathLib contains a variety of routines that are largely for the
*	processing of complex data.
***************************************************************************/
#ifndef __MATHLIBH__
#define __MATHLIBH__

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include  "VLib.h"
#include  "ImgLib.h"

#define PI 3.141592654

#ifndef Boolean
#define Boolean unsigned char
#define true 1
#define false 0
#endif
#ifndef OSErr
#define OSErr short
#define noErr 0
#endif

#define FORWARD 1
#define REVERSE -1

#define MEM_ALLOC_ERROR 1000
#define FFT_POWER_OF_TWO_NEEDED 1002
#define DIVIDE_BY_ZERO 1003

typedef struct
{
	float real; 
	float imag;
} COMPLEX;

/* --> function definitions */
/* N.B. call this once before using any of the Fourier library routines */
void       GetMathLibVer( char *buff );
OSErr      TwoDGauss( float *data, float xwidth, float ywidth, short xs, short ys );
OSErr      Complex2DGauss( float *data, float xwidth, float ywidth, short xs, short ys );
OSErr      Complex3DGauss( float *data, float xwidth, float ywidth, float zwidth, short xs, short ys, short zs );
OSErr      vconvolve1D( float *invec, int vlength, float *outvec, float *kernel, int kernelLength );

/* from Numerical recipes. May not be re-used without permission */
Boolean    fft_bitrev_sincos_lut_init();
OSErr      bitreverse(int nsize, COMPLEX *pptr_cf, int *ppbitrev);
OSErr      bitrev_norm(int nsize, COMPLEX *pptr_cf, int *ppbitrev);
OSErr      cfft(float *data, int size, char flag);
OSErr      cfft2d(float *data, int nx, int ny, char direction);
OSErr      cfft3d(float *data, int nx, int ny, int nz, char direction);

void       exch_quad( float *inbuf, short xsize, short ysize );
OSErr      exch_quad3d( float *inbuf, short xsize, short ysize, short zsize );
void       RealToComplex( float *vec, long npts );
OSErr      vreal( float *inbuf, float *outbuf, long nPairs );
OSErr      vimag( float *inbuf, float *outbuf, long nPairs );
OSErr      vmagphas( float *inbuf, float *outbuf, long nPairs );
OSErr      vmag( float *in, short inStride, float *out, short outStride, long nPairs );
OSErr      vphase( float *in, short inStride, float *out, short outStride, long nPairs );
OSErr      vdegree( float *in, short inStride, float *out, short outStride, long nPairs );
OSErr      PadVolume( void *inVol, void *outVol, IMAGE *im, int PadXS, int PadYS, int PadZS, int direction );

/* functions used by gaussSmooth */
typedef struct
{
	long    ImSize, VolSize;
	int     PaddedXS, PaddedYS, PaddedZS;			// sizes of the padded data volume
	long	DataVolSize;							// number of complex pairs
	float   ySmooth, zSmooth, xSmooth;				// in pixels!
	Boolean VolumeMode;
} SmoothGlobal;

OSErr      gaussSmooth( IMAGE *im, void *vin, void *vout, float *xSmooth, float *ySmooth, float *zSmooth );
OSErr      FirstFT( float *imData, SmoothGlobal g );
OSErr      CalcDimensions( IMAGE *im, SmoothGlobal g );
OSErr      gaussMultiply( float *imData, float *gaussKernel, IMAGE *im, SmoothGlobal g );
OSErr      Linfit( float *xvals, float *yvals, int nx, float *slope, float *intercept, float *chi2 );

/* Statistical functions, based on Numerical Recipes. May not be used without permission */
OSErr      correlation( float *X, float *Y, double *r, double *slope, long npairs );
OSErr      pcorr( double r, double slope, double *p, double *conf95, long npairs );
float      gasdev(long *idum);
float      gammln(float xx);
float      betacf(float a, float b, float x);
float      betai(float a, float b, float x);

#endif
/* </PRE></BODY></HTML> */
