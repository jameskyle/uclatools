/*<HTML><HEAD><TITLE>MathLib routines</TITLE></HEAD><BODY><PRE>
* <font size=+2><b>MathLib.c</b></font>
* <i>Please note last saved date below:</i>>
*  <font size=+1><b>$Date: 2008/05/16 16:04:19 $</b><p></font><p>
*****************************************************************************
*
*	Mark S. Cohen, Ph.D. c1999-2002
*	All Rights reserved, except as noted below
*
*	Unlimited academic use of this software is granted, except that any
*	publications that derive from its use, or any new software that
*   incorporates its underlying sources or algorithms must specifically
*   acknowledge the author.
*
*	Modification of the program to meet local needs is encouraged, but all
*   such changes are to be communicated to the author to support the continued
*   growth of these tools.
*
*	Commercial, for-profit, use of the sources or algorithms is expressly
*   prohibited without permission of the author.
*
<!-- ************************************************************************/
/*
 * $Log: MathLib.c,v $
 * Revision 1.20  2008/05/16 16:04:19  mscohen
 * Revised and repaired PadVolume to allow for non power of two data sizes
 * Embedded gaussSmooth functions
 * Added LinFit routine
 *
 * Revision 1.19  2007/06/27 17:37:57  mscohen
 * Changed argument list for vmag (now assumes that input data are float)
 *
 * Revision 1.17  2006/03/26 06:22:21  mscohen
 * *** empty log message ***
 *
 * Revision 1.15  2006/03/26 06:09:46  mscohen
 * gasdev
 *
 * Revision 1.16  2006/03/25 08:47:15  mscohen
 * formatting
 *
 * Revision 1.14  2004/02/03 20:48:37  mscohen
 * Added correlation and probability functions.
 *
 * Revision 1.13  2002/09/11 16:14:52  mscohen
 * RCS update for Log directive
 *
 * Revision 1.12  2002/09/04 17:41:09  mscohen
 * Created GetMathLibVer()
 *
 * Revision 1.11  2002/09/03 19:23:39  mscohen
 * Upgraded to local RCS file.
 * Added vconvolve1D
 *
 * Revision 1.11  2002/09/03 19:20:34  mscohen
 * Added vconvolve1D
 *
 * Revision 1.10  2002/04/16  17:24:09  mscohen
 * html
 *
 * Revision 1.9  2002/04/16  17:02:11  mscohen
 * copyright
 *
 * Revision 1.8  2002/04/16  16:57:18  mscohen
 * html
 *
 * Revision 1.8  2002/04/16  16:57:18  mscohen
 * html
 * */
#include "VLib.h"
#include "ImgLib.h"
#include "ImgLibError.h"

/* --> */
/***************************************************************************
*	MathLib contains a variety of routines that are largely for the
*	processing of complex data.
<!-- ************************************************************************/
#include "MathLib.h"
 
/* globals for these fuunctions */
static COMPLEX sc_lut2[2], sc_lut4[4],
               sc_lut8[8], sc_lut16[16],
               sc_lut32[32], sc_lut64[64],
               sc_lut128[128], sc_lut256[256],
               sc_lut512[512], sc_lut1024[1024];
static int  bitrev2[2], bitrev4[4],
            bitrev8[8], bitrev16[16],
            bitrev32[32], bitrev64[64],
            bitrev128[128], bitrev256[256],
            bitrev512[512], bitrev1024[1024];
static Boolean	FFTInit = false; // Indicates initialization of look up tables


/* --> ***********************************************************************
*       Keep control of MathLib revisions
*   <b>void GetVLibVer( char *buff )</b>
*
*   Returns the version number for MathLib in buff
*
<!-- ************************************************************************/
void GetMathLibVer( char *buff ) /* return a string describing MathLib revision */
{
   static char id[] = "$Revision: 1.20 $$Date: 2008/05/16 16:04:19 $";
   char versionStr[31];
   char buffer[63];

/* extract version and date */
   sscanf( id, "%s%s%s%s", buffer, versionStr, buffer, buffer );
   strcat( versionStr, " " );
   strcat( versionStr, buffer );
   strcpy( buff, versionStr );
}


/* does not seem worth re-writing here. Actual call will be:
	vmov( source, s_step, dest, d_step, count, T_COMPLEX );
--> ************************************************************************/
/* <b>cvmov_new</b>
* Complex vector move

void cvmov_new(float *source, int s_stride, float *dest, int d_stride, long count);
/* <!--  */
void cvmov_new(float *source, int s_stride, float *dest, int d_stride, long count)
{
  int i;
  
  d_stride-=1;
  s_stride-=1;
  for(i=0; i<count; i++, dest+=d_stride, source+=s_stride)
  {
     *dest++ = *source++;
     *dest   = *source;
  }
}

/* --> ********************************************************************
<b>TwoDGauss</b>
*	Create a 2D gaussian kernel as a matrix of floats
*	INPUTS
*		data	storage for output
*		xwidth, ywidth	half-width (in pixels) of gaussian for x and y
*		xs, ys	Matrix size. data must be large enough!
*	OUTPUTS
*		errorStatus
*	CHANGES
*		contents of data

OSErr TwoDGauss( float *data, float xwidth, float ywidth, short xs, short ys )
<!-- ************************************************************************/
OSErr TwoDGauss( float *data, float xwidth, float ywidth, short xs, short ys )
{
	int     i, j;
	float   xscale = xwidth*xwidth/(xs*xs);
	float   yscale = ywidth*ywidth/(ys*ys);
	float	value;
	float   EY;
	float   iLoc, jLoc;	// location w.r.t. center of image
	float   hxs, hys;	// half of the x and y dimensions
	long    offs;
	float   Tiny;	
	OSErr	error = noErr;

	if( xscale <0.0 || yscale <0.0 ) {
		error = DATA_OUT_OF_RANGE;
		return error;
	}
	
	Tiny = 0.01/(xs*ys);	// contribute << less than 1% of signal...
	hxs = xs/2;
	hys = ys/2;
	
	error = vclr( data, xs*ys, T_FLOAT );

	for( j=0; j<ys; j++ ) {
		jLoc = j - hys + 0.5;
		EY =   exp( -yscale * jLoc * jLoc );
		if( EY > Tiny ) {
			for( i=0; i<xs; i++ ) {
				iLoc = i - hxs + 0.5;
				value = exp( -xscale * iLoc * iLoc ) * EY;
				offs = i + j*xs;
				data[offs]     = value;
			}
		}
	}
	return error;
}


/* --> ***********************************************************************
<b>Complex2DGauss</b>
*	Create a 2D gaussian kernel as a matrix of floats
*	INPUTS
*		data	storage for output
*		xwidth, ywidth	half-width (in pixels) of gaussian for x and y
*		xs, ys	Matrix size. data must be large enough!
*	OUTPUTS
*		errorStatus
*	CHANGES
*		contents of data
*
OSErr Complex2DGauss( float *data, float xwidth, float ywidth, short xs, short ys )
<!-- ************************************************************************/
OSErr Complex2DGauss( float *data, float xwidth, float ywidth, short xs, short ys )
{
	int     i, j;
	float   xscale = xwidth*xwidth/(xs*xs);
	float   yscale = ywidth*ywidth/(ys*ys);
	float	value;
	float   EY;
	float   iLoc, jLoc;	// location w.r.t. center of image
	float   hxs, hys;	// half of the x and y dimensions
	long    offs;
	float   Tiny;	
	OSErr	error = noErr;

	if( xscale <0.0 || yscale <0.0 ) {
		error = DATA_OUT_OF_RANGE;
		return error;
	}
	
	Tiny = 0.01/(xs*ys);	// contribute << less than 1% of signal...
	hxs = xs/2;
	hys = ys/2;
	
	error = vclr( data, xs*ys, T_FLOAT );

	for( j=0; j<ys; j++ ) {
		jLoc = j - hys + 0.5;
		EY =   exp( -yscale * jLoc * jLoc );
		if( EY > Tiny ) {
			for( i=0; i<xs; i++ ) {
				iLoc = i - hxs + 0.5;
				value = exp( -xscale * iLoc * iLoc ) * EY;
				offs = i*2 + j*2*xs;
				
				data[offs]     = value;
				data[offs + 1] = value;
			}
		}
	}
	return error;
}

/* --> ***********************************************************************
<b>Complex3DGauss</b>
*	Create a 3D gaussian kernel as a matrix of floats
*	INPUTS
*		data                    - storage for output
*		xwidth, ywidth, zwidth  - half-width (in pixels) of gaussian for x and y
*		xs, ys, zs              - Matrix size. data must be large enough!
*	OUTPUTS
*		errorStatus
*	CHANGES
*		contents of data
*
OSErr Complex3DGauss( float *data, float xwidth, float ywidth,
<!-- ************************************************************************/
OSErr Complex3DGauss( float *data, float xwidth, float ywidth,
                      float zwidth, short xs, short ys, short zs )
{
	int     i, j, k;
	float   xscale = xwidth*xwidth/(xs*xs);
	float   yscale = ywidth*ywidth/(ys*ys);
	float   zscale = zwidth*zwidth/(zs*zs);
	float	value;
	float   hxs, hys, hzs;	// half x, y and z sizes
	long    imSize;
	long    offs;
	float   iLoc, jLoc, kLoc;	// location w.r.t center
	float   EY, EZ;
	float	Tiny = 0.01/(xs*ys*zs);
	OSErr	error = noErr;
	
	hxs = xs/2;
	hys = ys/2;
	hzs = zs/2;
	imSize = xs * ys * 2;	// points (R&I) per image
	
	if( xscale < 0.0 || yscale < 0.0 || zscale < 0.0) {
		error = DATA_OUT_OF_RANGE;
		return error;
	}

	error = vclr( data, imSize*zs, T_FLOAT );
	if( error ) return error;
	
	for( k=0; k<zs; k++ ) {
		kLoc = k - hzs + 0.5;
		EZ = exp( -zscale * kLoc * kLoc );
		if( EZ > Tiny ) {
			for( j=0; j<ys; j++ ) {
				jLoc = j - hys + 0.5;
				EY =   exp( -yscale * jLoc * jLoc );
				if( EY > Tiny ) {
					for( i=0; i<xs; i++ ) {
						iLoc = i - hxs + 0.5;
						value = exp( -xscale * iLoc * iLoc ) * EY * EZ;
						
						offs = i*2 + j*2*xs + k*imSize;
						data[offs]     = value;
						data[offs + 1] = value;
					}
				}
			}
		}
	}
	return error;
}

/* --> ************************************************************************
<b>vconvolve1D</b>
Function: vconvolve1D( float *invec, int vlength, float *outvec, float *kernel,
	int kernelLength )
Description: forms the convolution of invec with kernel and places it into outvec
Parameters:
invec - input data (float only)
vlength - number of points in input data
outvec - storage for output
kernel - convolution kernel
kernelLength - points in convolution kernel
<!-- *************************************************************************/
OSErr vconvolve1D( float *invec, int vlength, float *outvec, float *kernel, int kernelLength )
{
	OSErr error = noErr;
	int   i, j;
	int   index;
	float mySum;
	
	if( kernelLength > vlength ) {
		return ARRAY_RANGE_ERR;
	}
	
	if( invec==NULL || outvec==NULL || kernel==NULL ) {
		return INVALID_ADDRESS;
	}

// convolve the input vector with the smoothing vector and place it into the output.
// The ends of the vector are handled by assuming constant amplitude where data are unknown
	for( i=0; i<vlength; i++ ) {
		mySum = 0;
		for( j=0; j<kernelLength; j++ ) {
			index = i + j - kernelLength/2;
			if( index < 0 || index>vlength ) {
				mySum += invec[i] * kernel[j];
			} else {				
				mySum += invec[index] * kernel[j];
			}
		}
		outvec[i] = mySum;
	}
	return error;
}

/* --> ***********************************************************************
<b>Boolean fft_bitrev_sincos_lut_init</b>
Function: fft_bitrev_sincos_lut_init() 
Description: Generate the lookup tables for fft functions 
Parameters:
    IN/OUT/IO <parameter name> [<range of values>] <description>
Globals Accessed: bitrev2, ..., bitrev1024, sc_lut2, ..., sc_lut1024 
Returns: completion flag.

Boolean fft_bitrev_sincos_lut_init()
<!-- ************************************************************************/
Boolean fft_bitrev_sincos_lut_init()
{
	int i;
	float phi, nphi;
	int nsize;
	COMPLEX *p_sc;
	int j, m, l;
	int *pbitrev;
	
	for(l=0, nsize=2; l<10; l++, nsize<<=1) {
		switch(nsize)
		{
			case 2:
				pbitrev=bitrev2;    p_sc=sc_lut2;    break;
			case 4:
				pbitrev=bitrev4;    p_sc=sc_lut4;    break;
			case 8:
				pbitrev=bitrev8;    p_sc=sc_lut8;    break;
			case 16:
				pbitrev=bitrev16;   p_sc=sc_lut16;   break;
			case 32:
				pbitrev=bitrev32;   p_sc=sc_lut32;   break;
			case 64:
				pbitrev=bitrev64;   p_sc=sc_lut64;   break;
			case 128:
				pbitrev=bitrev128;  p_sc=sc_lut128;  break;
			case 256:
				pbitrev=bitrev256;  p_sc=sc_lut256;  break;
			case 512:
				pbitrev=bitrev512;  p_sc=sc_lut512;  break;
			case 1024:
				pbitrev=bitrev1024; p_sc=sc_lut1024; break;
		}
	
		*pbitrev++=0;
		for(i=1, j=1; i<nsize; i++) {
			m = nsize>>1;
			while ( j>m ) {
				j -= m;
				m >>= 1;
			}
			j += m;
			*pbitrev ++ = j-1;
		}
		
		for(i=0, phi=PI/(float)nsize, nphi=0.0;
		           i<nsize; i++, nphi+=phi, p_sc++) {
			p_sc->real = cos (nphi);
			p_sc->imag = sin (nphi);
		}
	}
	return true;	// signifying successful completion
}

/* --> ***********************************************************************
<b>bitreverse</b>
Function: bitreverse(int nsize, COMPLEX_F *pptr_cf, int *ppbitrev)
Description: performs the bitreverse of the input vector
Parameters:
    IN/OUT/IO <parameter name> [<range of values>] <description>
	nsize     -  size of the input vector
	*pptr_cf  -  pointer of the input vector with the struct COMPLEX_F
	*ppbitrev -  pointer of the lookup table for the bitreverse
Returns: error status - currently unimplemented.

OSErr bitreverse(int nsize, COMPLEX *pptr_cf, int *ppbitrev)
<!-- ************************************************************************/
OSErr bitreverse(int nsize, COMPLEX *pptr_cf, int *ppbitrev)
{
	int i,j;
	COMPLEX u;
	OSErr error = noErr;
	
	for(i=0; i<nsize-1; i++) {
		j=*(ppbitrev+i);
		if( i<=j ) {
			u.real=(pptr_cf+i)->real;
			u.imag=(pptr_cf+i)->imag;
			(pptr_cf+i)->real=(pptr_cf+j)->real;
			(pptr_cf+i)->imag=(pptr_cf+j)->imag;
			(pptr_cf+j)->real=u.real;
			(pptr_cf+j)->imag=u.imag;
		}
	}
	return error;
}

/* --> ***********************************************************************
<b>bitrev_norm</b>
Function: bitrev_norm(int nsize, COMPLEX_F *pptr_cf, int *ppbitrev)
Description: performs the bitreverse and the normalization of the vector
Parameters:
    IN/OUT/IO <parameter name> [<range of values>] <description>
	nsize     -  size of the input vector
	*pptr_cf  -  pointer of the input vector with the struct COMPLEX_F
	*ppbitrev -  pointer of the lookup table for the bitreverse
Returns: error status - currently unimplemented.

OSErr bitrev_norm(int nsize, COMPLEX *pptr_cf, int *ppbitrev)
<!-- ************************************************************************/
OSErr bitrev_norm(int nsize, COMPLEX *pptr_cf, int *ppbitrev)
{
	int i,j;
	COMPLEX u;
	float norm_fac;
	OSErr error = noErr;
	
	norm_fac=(float)sqrt(1.0/(double)nsize);
	
	for(i=0; i<nsize-1; i++) {
		j=*(ppbitrev+i);
		if( i<=j ) {
			u.real=(pptr_cf+i)->real*norm_fac;
			u.imag=(pptr_cf+i)->imag*norm_fac;
			(pptr_cf+i)->real=(pptr_cf+j)->real*norm_fac;
			(pptr_cf+i)->imag=(pptr_cf+j)->imag*norm_fac;
			(pptr_cf+j)->real=u.real;
			(pptr_cf+j)->imag=u.imag;
		}
	}
	(pptr_cf+nsize-1)->real *= norm_fac;
	(pptr_cf+nsize-1)->imag *= norm_fac;
	return error;
}

/* --> ***********************************************************************
<b>cfft</b>
Function: cfft(float *buffer, int size, int direction)
Description: In-place complex fft.
Parameters:
    IN/OUT/IO <parameter name> [<range of values>] <description>
	IN
	buffer - pointer to complex array;
	size - input element count.
	direction: 1 for forward FFT;
              -1 for inverse FFT.
	OUT
	Places FFT output on top of input COMPLEX array.
	
Globals Accessed: local global vectors bitrev and sc_lut 
Returns: error status.

OSErr cfft(float *data, int size, char direction)
<!-- ************************************************************************/
OSErr cfft(float *data, int size, char direction)
{
	OSErr error = noErr;
	int      i, m;
	float    *ptr1, *ptr2;
	COMPLEX  *p_sc;
	int      *pbitrev;
	int      n, mmax, istep;
	float    wr, wi;
	float    tempr, tempi;
	int      inc, ind, do_norm_flag=0;
	
/* Make certain that the look up table has been initialized */
	if ( !FFTInit ) {
		FFTInit = fft_bitrev_sincos_lut_init();
	}

	switch(size) 
	{
		case 2:
			pbitrev=bitrev2;    p_sc=sc_lut2;    break;
		case 4:
			pbitrev=bitrev4;    p_sc=sc_lut4;    break;
		case 8:
			pbitrev=bitrev8;    p_sc=sc_lut8;    break;
		case 16:
			pbitrev=bitrev16;   p_sc=sc_lut16;   break;
		case 32:
			pbitrev=bitrev32;   p_sc=sc_lut32;   break;
		case 64:
			pbitrev=bitrev64;   p_sc=sc_lut64;   break;
		case 128:
			pbitrev=bitrev128;  p_sc=sc_lut128;  break;
		case 256:
			pbitrev=bitrev256;  p_sc=sc_lut256;  break;
		case 512:
			pbitrev=bitrev512;  p_sc=sc_lut512;  break;
		case 1024:
			pbitrev=bitrev1024; p_sc=sc_lut1024; break;
		default:
		/* handle error condition for unacceptable fft size */
			error = FFT_POWER_OF_TWO_NEEDED;
			return error;
			break;
	}
	
	if(do_norm_flag>0) {
		bitrev_norm(size, (COMPLEX*)data, pbitrev);
	} else {
		bitreverse (size, (COMPLEX*)data, pbitrev);
	}

	
	--data; n=size<<1;
	
	for(mmax=2, inc=size, istep=4; mmax<n; inc>>=1, mmax<<=1, istep<<=1) {
		for(m=1, ind=0; m<mmax; m+=2) {
			wr=(p_sc + ind)->real;
			if( direction > 0 ) {
				wi = (p_sc + ind)->imag;	/* forward FFT */
			} else {
				wi = -(p_sc + ind)->imag;	/* inverse FFT */
			}
			ptr1 = data+m;
			ptr2 = data+m+mmax;
			for( i=m; i<=n; i+=istep ) {
				tempr = (wr * *ptr2) - (wi * *(ptr2+1));
				tempi = (wr * *(ptr2+1)) + (wi * *ptr2);
				*ptr2 = *ptr1 - tempr;
				*(ptr2+1) = *(ptr1+1) - tempi;
				*ptr1 += tempr;
				*(ptr1+1) += tempi;
				
				ptr1+=istep;
				ptr2+=istep;
			}
			ind += inc;
		}
	}
	return error;
}

/* --> ***********************************************************************
<b>cfft2d</b>
Function: cfft2d(float *ptr_f, int nx, int ny, char direction)
Description: In-place 2D complex fft.
Parameters:
	IN
	ptr_f- pointer to complex array;
	nx - input columns of elements;
	ny - input rows of elements;
	direction -    1 for forward FFT;
		      -1 for inverse FFT.
	OUT
	Places 2D FFT output on top of input COMPLEX array.

OSErr cfft2d(float *data, int nx, int ny, char direction)
<!-- ************************************************************************/
OSErr cfft2d(float *data, int nx, int ny, char direction)
{
	int i;
	int nx2;
	float *fptr, *tmp_vec;
	OSErr error = noErr;
	
	for(i=0, fptr=data, nx2=nx*2; i<ny; i++, fptr+=nx2) {
		error = cfft(fptr, nx, direction);
		if( error != noErr ) {
			return error;
		}
	}
	
	tmp_vec = (float *)calloc(ny*2,sizeof(float));
	if( !tmp_vec ) {
		error = MEM_ALLOC_ERROR;
		return error;
	} else {
		for(i=0, fptr=data; i<nx; i++, fptr+=2) {
			cvmov_new( fptr, nx2, tmp_vec, 2, ny );
			error = cfft( tmp_vec, ny, direction );
			if( error != noErr ) {
				return error;
			}
			cvmov_new( tmp_vec, 2, fptr, nx2, ny );
		}
		free(tmp_vec);
	}
	return error;
}

/* --> ***********************************************************************
<b>cfft3d</b>
Function: cfft3d(float *ptr_f, int nx, int ny, char direction)
Description: In-place 3D complex fft.
Parameters:
	IN
	ptr_f- pointer to complex array;
	nx - input columns of elements;
	ny - input rows    of elements;
	nz - input planes  of elements;
	direction -    1 for forward FFT;
		      -1 for inverse FFT.
	OUT
	Places 3D FFT output on top of input COMPLEX array.

How? Treat as a series of one-D transforms. Copy each line somewhere, do an FT
	then copy it back.
	
Not wildly efficient: copies the data back and forth prior to and after each
fft. Therefore, it is not a truly "in-place" routine. Fix someday?

OSErr cfft3d(float *data, int nx, int ny, int nz, char direction)
<!-- ************************************************************************/
OSErr cfft3d(float *data, int nx, int ny, int nz, char direction)
{
	int     i, k;
	int     nx2;
	long    imSizeX2;
	float *fptr, *tmp_vec;
	OSErr error = noErr;
	

/* fft of each row */
	fptr = data;
	nx2  = nx * 2;
	for( i=0; i<ny*nz; i++, fptr+=nx2 ) {
		error = cfft(fptr, nx, direction);
		if( error != noErr ) {
			return error;
		}
	}

	imSizeX2 = nx*ny*2;	// size of image, accounting for Real and Imaginary parts

/* fft of each column */	
	tmp_vec = (float *)calloc( ny*2, sizeof(float));
	if( !tmp_vec ) {
		return( MEM_ALLOC_ERROR );
	} else {
		for( k=0; k<nz; k++ ) {	// do this for each slice
			fptr = data + k*imSizeX2;
			for( i=0; i<nx; i++, fptr+=2 ) {
				cvmov_new( fptr, nx2, tmp_vec, 2, ny );
				error = cfft( tmp_vec, ny, direction );
				if( error != noErr ) {
					return error;
				}
				cvmov_new( tmp_vec, 2, fptr, nx2, ny );
			}
		}
		free(tmp_vec);
	}
	
	tmp_vec = (float *)calloc( nz*2, sizeof(float));

	if( !tmp_vec ) {
		return( MEM_ALLOC_ERROR );
	} else {
		for( i=0; i < nx*ny; i++ ) {
			fptr = data + i*2;
			cvmov_new( fptr, imSizeX2, tmp_vec, 2, nz );
			error = cfft( tmp_vec, nz, direction );
			if( error != noErr ) {
				return error;
			}
			cvmov_new( tmp_vec, 2, fptr, imSizeX2, nz );		
		}
		free( tmp_vec );
	}
	return error;
}


/* --> ***********************************************************************
<b>exch_quad</b>
Function        : exch_quad
Description     : This routine will exchange the first and third quadrants, 
				  and exchange the second and fourth quadrants. The exchanged 
				  data will be found in the same buffer as the input data.
Parameters      : input_data  - data to be quadrant swapped
				  ysize     - number of points in a full column
				  xsize     - number of points in a full row
Returns         : None

void exch_quad( float *inbuf, short xsize, short ysize)
<!-- ************************************************************************/
void exch_quad( float *inbuf, short xsize, short ysize)
{
	long i,j;
	long ny2;
	float *ptr_f1, *ptr_f2, *ptr_f3, *ptr_f4, tmp;
	
	ny2=ysize/2;
	
	ptr_f1=inbuf;
	ptr_f2=inbuf+xsize;
	ptr_f3=inbuf+xsize*ysize;
	ptr_f4=inbuf+xsize*ysize+xsize;
	
	for(i=0; i<ny2; i++,
	    ptr_f1+=xsize, ptr_f2+=xsize, ptr_f3+=xsize, ptr_f4+=xsize)
	{
		for(j=0; j<xsize; j++, ptr_f1++, ptr_f2++, ptr_f3++, ptr_f4++)
		{
			tmp=*ptr_f1;
			*ptr_f1=*ptr_f4;
			*ptr_f4=tmp;
			tmp=*ptr_f2;
			*ptr_f2=*ptr_f3;
			*ptr_f3=tmp;
		}
	}
}

/* --> ***********************************************************************
<b>exch_quad3d</b>
Function        : exch_quad3d
Description     : This routine will exchange the corners in a complex 3d Volume.
Parameters      : input_data  - data to be quadrant swapped
				  ysize     - number of points in a full column
				  xsize     - number of points in a full row
				  zsize     - number of planes
Returns         : None

OSErr exch_quad3d( float *inbuf, short xsize, short ysize, short zsize)
<!-- ************************************************************************/
OSErr exch_quad3d( float *inbuf, short xsize, short ysize, short zsize)
{
	long  k;
	long  imSize, cimSize;	// imaage size as points and char
	char  *tmp;
	char  *cin;
	
	imSize = xsize*ysize;
	cin    = (char *) inbuf;
	
// first perform a quadrant exchange on each slice
	for( k=0; k<zsize; k++ ) {
		exch_quad( inbuf + k*imSize*2, xsize, ysize );
	}

	cimSize = imSize * 2 * sizeof( float );
	tmp = (char *)malloc( cimSize );
	if( !tmp ) {
		return MEM_ALLOC_ERROR;
	}

// perform the final exchange one image at a time	
	for( k=0; k<zsize/2; k++ ) {
		memcpy( tmp, cin + k*cimSize, cimSize );
		memcpy( cin + k*cimSize, cin + (k + zsize/2)*cimSize, cimSize );
		memcpy( cin + (k + zsize/2)*cimSize, tmp, cimSize );
	}
	
	free( tmp );
	
	return noErr;
}
/* --> ***********************************************************************
<b>exch_vec</b>
void exch_vec( float *inbuf, long len );
/* <!-- */
void exch_vec( float *inbuf, long len );
void exch_vec( float *inbuf, long len )
{
	long	ctr;
	float	*pf1, *pf2;
	float	tmp;
	
	pf1 = inbuf;
	pf2 = inbuf + len/2;
	
	for( ctr=0; ctr<len/2; ctr++ ) {
		tmp = *pf1;
		*pf1++ = *pf2;
		*pf2++ = tmp;
	}
}
/* --> ***********************************************************************
<b>vreal and vimag</b>
Function        : vreal and vimag
Description     : Extract real or imaginary part of an input vector
Parameters      : inbuf     - complex input vector
                  outbuf    - storage for output
                  npairs    - number of complex pairs in the inputvector
Returns         : None

OSErr vreal( float *inbuf, float *outbuf, long nPairs )
OSErr vimag( float *inbuf, float *outbuf, long nPairs )
<!-- ************************************************************************/
OSErr vreal( float *inbuf, float *outbuf, long nPairs )
{
	OSErr error = noErr;
	long i;
	
	for( i=0; i<nPairs; i++ ) {
		outbuf[i] = inbuf[i*2];
	}
	
	return error;
}

OSErr vimag( float *inbuf, float *outbuf, long nPairs )
{
	OSErr error = noErr;
	long i;
	
	for( i=0; i<nPairs; i++ ) {
		outbuf[i] = inbuf[i*2+1];
	}
	
	return error;
}

/* --> ***********************************************************************
<b>RealToComplex</b>
*	void RealToComplex( float *vec, long npts );
*	Takes a vector of real values and expands it to a complex vector
*	INPUTS
*		vec          the input vector - must be large enough to handle complex size
*		npts         number of real points
*
*	OUTPUTS
*		none

void RealToComplex( float *vec, long npts )
<!-- ************************************************************************/
void RealToComplex( float *vec, long npts )
{
	long i;
	
	for( i=npts; i>0; ) {
		--i;
		vec[2 * i] = vec[i];	// real part
		vec[2 * i + 1] = 0.0;	// imaginary part
	}
}

/* --> ***********************************************************************
*   <b>vmag</b>
*	Extract the magnitude from a complex vector - ok for in-place use
*	INPUTS
*		in           the input vector
*		inStride     number of floats separating adjacent complex pairs
*		outStride    number of floats by which to separate magnitudes
*		nPairs
*	OUTPUTS
*		out          the output vector
*
*	OSErr      vmag( float *in, short inStride, float *out, short outStride, long nPairs )
<!-- ************************************************************************/
OSErr vmag( float *in, short inStride, float *out, short outStride, long nPairs )
{
	long i, j;
	float tmp;
	OSErr error = noErr;
	
	for( i=0, j=0; i<nPairs*inStride; i+=inStride, j+=outStride ) {
		tmp = in[i]*in[i] + in[i+1]*in[i+1];
		if( (float)tmp == 0.0 ) {
			out[j] = 0.0;
		} else if( tmp<0 ) {
			return DIVIDE_BY_ZERO_ERR;
		} else {
			out[j] = sqrt( tmp );
		}
	}
	return error;
}

/* --> ***********************************************************************
<b>vphase</b>
*	OSErr vphase( float *in, short inStride, float *out, short outStride, long nPairs);
*	Extract the phase (in radians) from a complex vector - ok for in-place use
*	INPUTS
*		in           the input vector
*		inStride     number of floats separating adjacent complex pairs
*		outStride    number of floats by which to separate phases
*		nPairs
*	OUTPUTS
*		out          the output vector
*
OSErr      vphase( float *in, short inStride, float *out, short outStride, long nPairs )
<!-- ************************************************************************/
OSErr      vphase( float *in, short inStride, float *out, short outStride, long nPairs )
{
	long i, j;
	OSErr error = noErr;
	
	for( i=0, j=0; i<nPairs*inStride; i+=inStride, j+=outStride ) {
		if( in[i+1]==0 ) {
			out[j] = 0;
		} else {
			out[j] = atan2( in[i], in[i+1] );
		}
	}
	return error;
}

/* --> ***********************************************************************
<b>vdegree</b>
*   OSErr vdegree( float *in, short inStride, float *out, short outStride, long nPairs )
*	Convert a vector in radians to a vector in degrees - ok for in-place use
*	INPUTS
*		in           the input vector
*		inStride     number of floats separating adjacent phases in radians
*		outStride    number of floats by which to separate phases in degrees
*		nPairs
*	OUTPUTS
*		out          the output vector
<!-- ************************************************************************/
OSErr      vdegree( float *in, short inStride, float *out, short outStride, long nPairs )
{
	long i, j;
	OSErr error = noErr;
	
	for( i=0, j=0; i<nPairs*inStride; i+=inStride, j+=outStride ) {
		out[j] = 180*in[i]/PI;
	}
	return error;
}

/* --> ***********************************************************************
*	<b>PadVolume</b>
*
*	This function performs padding or unpadding of image data as an
*	in-place operation.
*
*	Space allocated for the padded image data must be
* 	larger than the image size, because Fourier transform requires
*	data to have power of 2 elements in each dimension. If PAD is
*	selected, this function will pad the data evenly with zeros in all 
*	three dimensions; if UNPAD is selected, it will move the data back
*	up to the beginning of the dataset.
*
*	IN:	theVolume   - pointer to the input image data
*		im		    - UC image type for image dimensions
*		volXS, volYS, volZS - size of data as processed - power of 2
*		
*		direction   - PAD(1) or UNPAD(0);
*	
*	OUT: outVolume   - desired location of output data
*
*
*                  |<------------- fullXS ------------>|
*                  |<- osX ->|                         |
*       padded --->.....................................-----------
*                  .....................................        ^
*                  ..................................... osY    |
*                  .....................................----    |
*                  ..........Soooooooooooooo............        |
*                  ..........ooooooooooooooo............        |
*                  ..........ooooooooooooooo............        |
*                  ..........ooooooooooooooo............     fullYS
*                  ..........ooooooooooooooo............
*                  ..........ooooooooooooooo............        |
*                  ..........ooooooooooooooo............        |
*                  ..........ooooooooooooooo............        |
*                  ..........ooooooooooooooo............        |
*                  ..........ooooooooooooooo............        |
*                  .....................................        |
*                  .....................................        |
*                  .....................................        |
*                  .....................................        V
*                  .....................................-----------
*                 
*      S = paddedStart
*
*	E.g.,
*		inXS = inYS = inZS = 4;
*		fullXS = fullYS = fullZS = 8;
*
*	3D padding	(2X each dimension)
*
*	Input:
*			Sxxxxxxx xxxxxxxx xxxxxxxx xxxxxxxx ........ ........ ........ ........
*			........ ........ ........ ........ ........ ........ ........ ........		
*			........ ........ ........ ........ ........ ........ ........ ........		
*			........ ........ ........ ........ ........ ........ ........ ........		
*			........ ........ ........ ........ ........ ........ ........ ........		
*			........ ........ ........ ........ ........ ........ ........ ........		
*			........ ........ ........ ........ ........ ........ ........ ........		
*			........ ........ ........ ........ ........ ........ ........ ........		
*
*
*	Output:
*			........ ........ ........ ........ ........ ........ ........ ........		
*			........ ........ ........ ........ ........ ........ ........ ........		
*			........ ........ ..Sxxx.. ..xxxx.. ..xxxx.. ..xxxx.. ........ ........
*			........ ........ ..xxxx.. ..xxxx.. ..xxxx.. ..xxxx.. ........ ........
*			........ ........ ..xxxx.. ..xxxx.. ..xxxx.. ..xxxx.. ........ ........
*			........ ........ ..xxxx.. ..xxxx.. ..xxxx.. ..xxxx.. ........ ........
*			........ ........ ........ ........ ........ ........ ........ ........		
*			........ ........ ........ ........ ........ ........ ........ ........		
*			
*
<!-- ************************************************************************/
OSErr PadVolume( void *inVol, void *outVol, IMAGE *im, int PadXS, int PadYS, int PadZS, int direction )
{
// pointers
	char	*padded, *paddedStart, *unpadded;

	long    noPadXSbytes, PadXSbytes, inImbytes, PadImbytes, osX, osY, osZ;
	long    unpadVolSize, padVolSize;
	long    src, dst;
	int     line, slice, noPadXS, noPadYS, noPadZS;
	short   data_size = get_datasize(im->data_type);
	
// flags
	Boolean pad = direction;

	OSErr error = noErr;

//	initialize 
	noPadXS      = im->dim.isoX;
	noPadYS      = im->dim.isoY;
	noPadZS      = im->dim.n_slices;
	noPadXSbytes = noPadXS * data_size;
	inImbytes    = noPadXSbytes * noPadYS;
	PadXSbytes   = PadXS * data_size;
	PadImbytes   = PadXSbytes * PadYS;
	padVolSize   = PadXS * PadYS * PadZS;
	
	osX          = (PadXS - noPadXS)/2;
	osY          = (PadYS - noPadYS)/2;
	osZ          = (PadZS - noPadZS)/2;

	unpadVolSize = noPadXS * noPadYS * noPadZS;
	
	if(im->dim.isoX==PadXS && im->dim.isoY==PadYS && im->dim.n_slices==PadZS) {
		error = vmov( inVol, 1, outVol, 1, unpadVolSize, im->data_type );
		return error;
	}

		
// Padding and unpadding are exactly symmetric with memcpy used this way.
	if( pad ) {
		padded   = (char *)outVol;
		unpadded = (char *)inVol; 
		paddedStart = padded + ( osZ * PadYS * PadXS + osY * PadXS + osX ) * data_size;
		for( slice=0; slice<noPadZS; slice++ )	{
			for( line=0; line<noPadYS; line++ ) {
				src = slice * inImbytes + line * noPadXSbytes;
				dst = slice * PadImbytes + line * PadXSbytes;
				memcpy( paddedStart + dst,
						unpadded + src,
						noPadXSbytes );
			}
		}
	} else {
		padded = (char *)inVol;
		unpadded = (char *)outVol; 
		paddedStart = padded + ( osZ * PadYS * PadXS + osY * PadXS + osX ) * data_size;
		for( slice = 0; slice < noPadZS; slice++ ) {
			for( line=0; line < noPadYS; line++ ) {
				dst = slice * inImbytes + line * noPadXSbytes;
				src = slice * PadImbytes + line * PadXSbytes;
				memcpy( unpadded + dst,
						paddedStart + src,
			    	    noPadXSbytes );
			}
		}
	}
	return error;
}


void xPadVolume( float *theVolume, IMAGE *im, short volXS, short volYS, short volZS, Boolean isComplex, int direction ) {
	
// offset values
	int		initialOffset;
	int		lineOffset;
	int		sliceOffset;
	int		offsetZ;
	int		offsetY;
	int		offsetX;
	int		offset;
	int		lineStart;
	
// image sizes
	short	imXS = im->dim.x;		// xsize of image before padding
	short	imYS = im->dim.y;		// ysize of image before padding
	short	imZS = im->dim.n_slices;		// zsize before padding
	long	volSize;				// size of data _after_ padding
	long	inVolSize;				// size of data _before_ padding
	long	imSize;
	
// counters	
	int		sliceCount;
	int		lineCount;
	
// flags
	Boolean pad = direction;
		
// if the dataset contains complex values, adjust number of elements in x dimension (real+imaginary)
	if ( isComplex ) {
		volXS = 2 * volXS;
		imXS = 2 * imXS;
	}
	
// calculate total volume and image sizes
	volSize   = volXS * volYS * volZS;
	imSize    = imXS * imYS;
	inVolSize = imSize * imZS;

// 	determine offsets (in number of elements) to the first valid element
	offsetX = ( volXS - imXS ) / 4;
	offsetY = ( volYS - imYS ) / 2;
	offsetZ = ceil(( volZS - imZS ) / 2); // this is only if there is odd number of slices;
	
	lineOffset = volXS - imXS;
	sliceOffset = ( volYS - imYS ) * volXS + ( volXS - imXS );

	
	if ( pad ) { // PADDING
		initialOffset = volSize - ( offsetZ * volXS * volYS + offsetY * volXS + offsetX + imXS );	// offset for the last data
																									// element; padding moves image
																									// data to the end of dataset
// printf( "VolSize: %ld\t XS:%hd\t YS:%hd\t ZS:%hd\t OffsetZ: %d\t initialOffset:%d\n",
//	volSize, volXS, volYS, volZS, offsetZ, initialOffset );

	} else {	// UNPADDING
		offsetZ = ( volZS - imZS ) * 0.5;
		initialOffset = offsetZ * volXS * volYS + offsetY * volXS + offsetX;
	}
	
// check if padding/unpadding necessary	
	if ( (imXS == volXS) && (imYS == volYS) && (imZS == volZS) ) {
// WriteMGHImage( theVolume, "Padded", (short)volXS, (short)volYS, (short)volZS, T_FLOAT );
		return; // no need to do the padding
	}

// for PADDING ONLY: clear all cells in theVolume behind image data (to make sure that, when moved, data
// will be padded with zeros)
	if ( pad ) {
		vclr( theVolume + inVolSize, volSize - inVolSize, T_FLOAT );
	}

// move data within theVolume with appropriate offsets
	if ( pad ) {	// PADDING
		offset = initialOffset;		// offset for the last image data element
		lineStart = inVolSize - imXS;	// start of the first line to be moved (complex data...)
		for ( sliceCount = 0; sliceCount < imZS; sliceCount++ ) {
			for ( lineCount = 0; lineCount < imYS; lineCount++ ) {
				memcpy( &theVolume[offset], &theVolume[lineStart], imXS * sizeof( float ) ); // move line at lineStart to offset
				vclr( &theVolume[lineStart], imXS, T_FLOAT );	// clear copied cells
				offset = offset - volXS;
				lineStart = lineStart - imXS;
			}
			offset = offset - ( volYS - imYS ) * volXS;	// move to the next slice
		}
		
		
	} else {	// UNPADDING
		offset = initialOffset;		// first element of the image data
		lineStart = 0;				// start of line where first line of image data will be moved to
		for ( sliceCount = 0; sliceCount < imZS; sliceCount++ ) {
			for ( lineCount = 0; lineCount < imYS; lineCount++ ) {
				memcpy( &theVolume[lineStart], &theVolume[offset], imXS * sizeof( float ) );	// move line at offset to lineStart
				offset = offset + volXS;
				lineStart = lineStart + imXS;
			}
			offset = offset + ( volYS - imYS ) * volXS;	// move to the next slice
		}
		// clear the remainder of the dataset
		vclr( &theVolume[inVolSize], volSize - inVolSize, T_FLOAT ); // only for unpadding
	}
// WriteMGHImage( theVolume, "Padded", (short)volXS, (short)volYS, (short)volZS, T_FLOAT );
}


/****************       GaussSmooth operations     *************************
*	Perform image smoothing by convolution with a gaussian function
*
*	OSErr gaussSmooth( IMAGE *im, float *in, float *out, float xSmooth, float ySmooth, float zSmooth )
*
*	Therefore *in and *out must be large enough, generally 2 * VolSize
*	Smoothing dimensions are in mm based on IMAGE parameters!
*	Data are returned in their original data type, as passed in *im.
*	
***************************************************************************/

/*******************************   FirstFT   *******************************
*
*	The first Fourier Transform is slightly more complicated, as it
*	involves a quadrant exchange and intensity scaling
**************************************************************************/     
OSErr FirstFT( float *imData, SmoothGlobal g )
{
	float FTscale;
	OSErr error = noErr;
				
	FTscale = 1.0/g.VolSize;

	error = vsmul( imData, 2, imData, 2, (void *)&FTscale, g.DataVolSize, T_FLOAT );	// Adjust intensity for FT effects
	RETURNONERROR;
	
	if( !g.VolumeMode ) {
		error = cfft2d( imData, g.PaddedXS, g.PaddedYS, FORWARD );
		RETURNONERROR;
	} else {
		error = cfft3d( imData, g.PaddedXS, g.PaddedYS, g.PaddedZS, FORWARD );
		RETURNONERROR;
	}
	
	if( !g.VolumeMode ) {
		exch_quad( imData, g.PaddedXS, g.PaddedYS );
	} else {
		error = exch_quad3d( imData, g.PaddedXS, g.PaddedYS, g.PaddedZS );
		RETURNONERROR;
	}
	return error;
}			

/******************************    CalcDimensions      ******************************
*
*	Given the IMAGE struct, define the dimensions used for local processing.
*	Mostly used to shorten variable names.
*************************************************************************************/
OSErr CalcDimensions( IMAGE *im, SmoothGlobal g )
{
	OSErr error = noErr;
	
	g.PaddedXS = 2;	// gPaddedXS is the size of the data volume, which may be larger than gXS
	while( g.PaddedXS < im->dim.x ) {	// the FFT needs a power of 2 foreach dimension
		g.PaddedXS *= 2;
	}

	g.PaddedYS = 2;	// gPaddedYS is the size of the data volume, which may be larger than gYS
	while( g.PaddedYS < im->dim.y ) {	// the FFT needs a power of 2 foreach dimension
		g.PaddedYS *= 2;
	}
	
	if( im->dim.n_slices > 1 ) {
		g.PaddedZS = 2;	// g.PaddedZS is the size of the data volume, which may be larger than gZS
		g.VolumeMode = true;
		while( g.PaddedZS < im->dim.n_slices ) {	// the FFT needs a power of 2 for each dimension
			g.PaddedZS *= 2;
		}
	} else {
		g.VolumeMode = false;
		g.PaddedZS = 1;
	}
	
	g.ImSize      = im->dim.x * im->dim.y;
	g.VolSize     = g.ImSize * im->dim.n_slices;
	g.DataVolSize = g.PaddedXS * g.PaddedYS * g.PaddedZS;

// printf( "g.PaddedXS: %d\tPaddedYS:%d\tPaddedZS:%d\n", g.PaddedXS, g.PaddedYS, g.PaddedZS );
	
	return error;
}


OSErr gaussMultiply( float *imData, float *gaussKernel, IMAGE *im, SmoothGlobal g )
{
	float Tiny;
	long  i;
	float kLoc;
	float EZ;
	float zscale;
	
	long  padImSize = g.PaddedXS * g.PaddedYS;	// padded image size
	
	OSErr error = noErr;

// Calculate a scaling level below which the data might as well be zero.
	Tiny = 0.01/(im->dim.x * im->dim.y * im->dim.n_slices);
	zscale = g.zSmooth * g.zSmooth / (im->dim.n_slices * im->dim.n_slices);
	
	for( i=0; i < g.PaddedZS; i++ ) {
		
// Here we do one dimension of the k-smoothing by multiplying each transformed plane
		kLoc = i - g.PaddedZS/2.0 + 0.5;
		EZ = exp( -zscale * kLoc * kLoc );
		
		if( EZ > Tiny ) {
			error = vfsmul( imData + i*padImSize*2, 1, imData + i*padImSize*2, 1, EZ, padImSize*2, T_FLOAT );
			RETURNONERROR;
			
// In-plane smoothing - real part then imaginary part	
			error = vmul( &imData[i*padImSize*2],     2, gaussKernel, 1, &imData[i*padImSize*2],     2, padImSize, T_FLOAT );
			RETURNONERROR;

			error = vmul( &imData[i*padImSize*2 + 1], 2, gaussKernel, 1, &imData[i*padImSize*2 + 1], 2, padImSize, T_FLOAT );
			RETURNONERROR;
		} else {
// There is no point in a trivial multiplication
			vclr( &imData[i*padImSize*2], padImSize*2, T_FLOAT );
		}
	}
	return error;
}

OSErr gaussSmooth( IMAGE *im, void *vin, void *vout, float *xSmooth, float *ySmooth, float *zSmooth )
/*************************************************************************************************
*
* This is the core routine:
*	Allocate space for padded volume
*	Create a 2D gaussian
*	Pad the image to be a power of two size (if needed)
*	FT of the input data
*	Multiply the FT'd data, slice by slice, with the gaussian image
*	Reverse FT
*	Unpad (if needed)
*
*	im --------- contains the image dimension info
*	in --------- contains the image data as complex pairs
*	
**************************************************************************************************/
{
// processing structures & buf1ers
	float	*buf1, *buf2, *gaussKernel;		// buf2 is storage for the data including padding
	short	initialDatatype;
	short	rules;		// dummy variables used by type_convert
	float	fmax, fmin;
	IMAGE   GIm;
	SmoothGlobal g;
	
// control parameters
	Boolean    pad = true;
	Boolean    unpad = false;
	OSErr      error = noErr;

	initialDatatype = im->data_type;
	
// while the user has entered smoothing in real units, the smoothing is performed on pixels. Therefore
// convert the user input to pixel values.
	g.xSmooth = *xSmooth / im->dim.dpixsize_X;
	g.ySmooth = *ySmooth / im->dim.dpixsize_Y;
	g.zSmooth = *zSmooth / im->dim.slthick;

	error = CalcDimensions( im, g );			// Populates global variable with dimensions of padded volume
		RETURNONERROR;

	memcpy( &GIm, im, sizeof(IMAGE) );
	GIm.data_type = T_COMPLEX;
	buf1 = (float *)ck_calloc( 2 * g.DataVolSize, sizeof(float), "buf1" );	// complex pairs
	buf2 = (float *)ck_calloc( 2 * g.DataVolSize, sizeof(float), "buf2" );	// complex pairs

// printf( "In gaussSmooth.\nim.dim.x:%hd, im.dim.n_slices:%hd in:%ld, out: %ld, g.xSmooth: %f g.ySsmooth: %f g.zSmooth: %f\n",
//	im->dim.x, im->dim.n_slices, in, out, g.xSmooth, g.ySmooth, g.zSmooth );

	gaussKernel = (float *)ck_malloc( g.PaddedXS * g.PaddedYS * g.PaddedZS * sizeof(float), "gaussKernel" );

// pre-calculate the smoothing kernel in 2D
	error = TwoDGauss( gaussKernel, g.xSmooth, g.ySmooth, g.PaddedXS, g.PaddedYS );
	RETURNONERROR;

// error = WriteMGHImage( gaussKernel, "gsGaussKernel", g.PaddedXS, g.PaddedYS, 1, T_FLOAT );
//	RETURNONERROR;
	
// Copy the volume into buf2, converting to complex, prior to padding
	error = type_convert( vin, initialDatatype, buf1, T_COMPLEX, g.VolSize, &fmax, &fmin, &rules );
	if( error && error != CONVERSION_ERROR ) {
	  	return error;
	}

	error = PadVolume( buf1, buf2, &GIm, g.PaddedXS, g.PaddedYS, g.PaddedZS, pad );
		RETURNONERROR;
// error=WriteMGHImage( buf2, "gsPAD", g.PaddedXS*2, g.PaddedYS, g.PaddedZS, T_FLOAT );	

// This is the meat of the algorithm: Fourier transform the image
// Multiply the transformed image by a gaussian
// Back transform. This convolves the image with the desired gaussian
	error = FirstFT( buf2, g );
		RETURNONERROR;

	error = gaussMultiply( buf2, gaussKernel, im, g );
		RETURNONERROR;

	if( !g.VolumeMode ) {
		error = cfft2d( buf2, g.PaddedXS, g.PaddedYS, REVERSE );
		RETURNONERROR;
	} else {
		error = cfft3d( buf2, g.PaddedXS, g.PaddedYS, g.PaddedZS, REVERSE );
		RETURNONERROR;
	}

// unpad the data in all three dimensions (data still complex)
	PadVolume( buf2, buf1, &GIm, g.PaddedXS, g.PaddedYS, g.PaddedZS, unpad );
 
 // error=WriteMGHImage( buf2, "gsUNPAD",im->dim.x*2, im->dim.y, im->dim.n_slices, T_FLOAT );	
	error = type_convert( buf1, T_COMPLEX, vout, initialDatatype, g.VolSize, &fmax, &fmin, &rules );
	if( error && error != CONVERSION_ERROR ) {
	  	return error;
	}
	free( gaussKernel );
	free( buf1 );
	free( buf2 );

	return error;
}




/* statistical functions */

OSErr Linfit( float *xvals, float *yvals, int nx, float *slope, float *intercept, float *chi2 )
{
	int i;
	float  sx=0.0;
	float  sx2=0.0;
	float  sy;
	float  M=0;	// the slope
	float  num, den;
	float  O_E;	// Observed - expected yvals
	OSErr  error = noErr;

	for (i=0; i<nx; i++) {
		sx2 += xvals[i] * xvals[i];
		sx  += xvals[i];
	}
	den = sx2 - sx*sx/nx;	// sum of squared x values - the average of the squares

	sy = 0;
	for (i=0;i<nx;i++) {
		sy += yvals[i];
		M  += xvals[i] * yvals[i];	// here just the cross product
	}

	num = M - sy*sx/nx;
	
	if( den == 0 ) {
		M = 0;
		*chi2  = 0;
		*intercept = 0;
	} else {
		M = num / den;
		*intercept = (sy - sx*M) / nx;
		*chi2 = 0;
		for (i=0;i<nx;i++) {
			O_E = (yvals[i] - *intercept - M*xvals[i]);
			*chi2 += O_E * O_E / ( *intercept + M*xvals[i] );
		}
	}

	*slope = M;
	return error;
}

float gammln(float xx)
{
	double x,y,tmp,ser;
	static double cof[6]={76.18009172947146,-86.50532032941677,
		24.01409824083091,-1.231739572450155,
		0.1208650973866179e-2,-0.5395239384953e-5};
	int j;

	y=x=xx;
	tmp=x+5.5;
	tmp -= (x+0.5)*log(tmp);
	ser=1.000000000190015;
	for (j=0;j<=5;j++) ser += cof[j]/++y;
	return -tmp+log(2.5066282746310005*ser/x);
}

#define MAXIT 100
#define EPS 3.0e-7
#define FPMIN 1.0e-30

float betacf(float a, float b, float x)
{
	int m,m2;
	float aa,c,d,del,h,qab,qam,qap;

	qab=a+b;
	qap=a+1.0;
	qam=a-1.0;
	c=1.0;
	d=1.0-qab*x/qap;
	if (fabs(d) < FPMIN) d=FPMIN;
	d=1.0/d;
	h=d;
	for (m=1;m<=MAXIT;m++) {
		m2=2*m;
		aa=m*(b-m)*x/((qam+m2)*(a+m2));
		d=1.0+aa*d;
		if (fabs(d) < FPMIN) d=FPMIN;
		c=1.0+aa/c;
		if (fabs(c) < FPMIN) c=FPMIN;
		d=1.0/d;
		h *= d*c;
		aa = -(a+m)*(qab+m)*x/((a+m2)*(qap+m2));
		d=1.0+aa*d;
		if (fabs(d) < FPMIN) d=FPMIN;
		c=1.0+aa/c;
		if (fabs(c) < FPMIN) c=FPMIN;
		d=1.0/d;
		del=d*c;
		h *= del;
		if (fabs(del-1.0) < EPS) break;
	}
	if (m > MAXIT) ILError( DATA_OUT_OF_RANGE, "betacf");
	return h;
}
#undef MAXIT
#undef EPS
#undef FPMIN

float betai(float a, float b, float x)
{
	float bt;
	float result;

	if (x < 0.0 || x > 1.0) {
		ILError ( DATA_OUT_OF_RANGE, "betai" );
	}

	if (x == 0.0 || x == 1.0) {
		bt=0.0;
	} else {
		bt = exp( gammln(a+b) - gammln(a) - gammln(b) + a*log(x) + b*log(1.0-x) );
	}
	if (x < (a+1.0)/(a+b+2.0)) {
		result = bt*betacf(a,b,x)/a;
	} else {
		result = 1.0-bt*betacf(b,a,1.0-x)/b;
	}

	return result;
}

/* --> ***********************************************************************
<b>correlation</b>
*	OSErr correlation( float *X, float *Y, double *r, double *slope, long npairs )
*	Calculates the correlation between vector X and Y. Returns correlation, significance
*   and slope,b: v1 = b*v2.
*	INPUTS
*		X, Y      input vectors
*		npairs    Number of points in X and Y
*	OUTPUTS
*		r         correlation (r, not r^2) of X and Y
*       slope     slope of the regression of X and onto Y
<!-- ************************************************************************/
#define TINY 1.0e-20

OSErr correlation( float *X, float *Y, double *r, double *slope, long npairs )
{
 	OSErr  error = noErr;
 	double Sx, Sx2, Sy, Sy2, Sxy, ssxx, ssyy, ssxy, cc, den;
 	long   i;
	Sx = Sx2 = Sy = Sy2 = 0;

	for( i=0; i<npairs; i++ ) {
		Sx  += X[i];
		Sx2 += X[i] * X[i];
		Sy  += Y[i];
		Sy2 += Y[i] * Y[i];
		Sxy += X[i] * Y[i];
	}

	ssxx = Sx2 - (Sx*Sx)/npairs;
	ssyy = Sy2 - (Sy*Sy)/npairs;
	ssxy = Sxy - (Sx*Sy)/npairs;
	
	den = ssxx * ssyy;
	if( den<TINY ) {
		return DIVIDE_BY_ZERO_ERR;
	}

	*r     = cc = ssxy/den;
	*slope = ssxy/ssxx;
	
	return error;
}

/* --> ***********************************************************************
<b>pcorr</b>
*	OSErr pcorr( double r, double slope, double *p, double *conf95, long npairs )
*	Calculates the probability of non-zero correlation.
*	INPUTS
*		r,        calculated correlation
*		npairs    Number of support points for r
*	OUTPUTS
*       p         probability that v1 and v2 are correlated significantly
*       conf95    95% confidence interval for slope (can be omitted)
* Assumes large n ( > 100 )
<!-- ************************************************************************/
OSErr pcorr( double r, double slope, double *p, double *conf95, long npairs )
{
	OSErr error = noErr;
	double t, den;

	den = 1.0 - r*r;
	if( den<TINY ) {
		return DIVIDE_BY_ZERO_ERR;
	}

	t = r * sqrt( (npairs-2) / den );
	
	*p      = betai( 0.5*(npairs-2), 0.5, (npairs-2)/(npairs-2 + t*t) );
	*conf95 = 1.984 * slope / t;

	return error;
}

/* gasdev and ran1 are from numerical recipes */
float gasdev(long *idum)
{
	float ran1(long *idum);
	static int iset=0;
	static float gset;
	float fac,rsq,v1,v2;

	if  (iset == 0) {
		do {
			v1=2.0*ran1(idum)-1.0;
			v2=2.0*ran1(idum)-1.0;
			rsq=v1*v1+v2*v2;
		} while (rsq >= 1.0 || rsq == 0.0);
		fac=sqrt(-2.0*log(rsq)/rsq);
		gset=v1*fac;
		iset=1;
		return v2*fac;
	} else {
		iset=0;
		return gset;
	}
}

#define IA 16807
#define IM 2147483647
#define AM (1.0/IM)
#define IQ 127773
#define IR 2836
#define NTAB 32
#define NDIV (1+(IM-1)/NTAB)
#define EPS 1.2e-7
#define RNMX (1.0-EPS)

float ran1(long *idum)
{
	int j;
	long k;
	static long iy=0;
	static long iv[NTAB];
	float temp;

	if (*idum <= 0 || !iy) {
		if (-(*idum) < 1) *idum=1;
		else *idum = -(*idum);
		for (j=NTAB+7;j>=0;j--) {
			k=(*idum)/IQ;
			*idum=IA*(*idum-k*IQ)-IR*k;
			if (*idum < 0) *idum += IM;
			if (j < NTAB) iv[j] = *idum;
		}
		iy=iv[0];
	}
	k=(*idum)/IQ;
	*idum=IA*(*idum-k*IQ)-IR*k;
	if (*idum < 0) *idum += IM;
	j=iy/NDIV;
	iy=iv[j];
	iv[j] = *idum;
	if ((temp=AM*iy) > RNMX) return RNMX;
	else return temp;
}
#undef IA
#undef IM
#undef AM
#undef IQ
#undef IR
#undef NTAB
#undef NDIV
#undef EPS
#undef RNMX

