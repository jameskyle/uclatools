/* <CODE> <PRE> */
/**************************    overlay      **************************
*
*	First complete re-write 9/1999
*
*  Reads in two images and forms a composite eight bit image with a two
*  range gray scale. The first image, occupies the upper values
*  from LMAX+1 to HMAX. The second occupies the range from 1 to LMAX.
*  the values 0 and 255 are reserved on the Macintosh.
*
*  Also allows the upper and lower window values to be adjusted.
*
*  2/1/96:  Change default to dual color overlay to give less biased data interpretation
*  9/21/96: Incorporate UC_Readimage and ImgLib routines
*  11/9/96: v 1.2 - Alter proc file to include base image range, default to pixel replication.
*  The logic is to read in the images as FLOATS, rescale each individually, and then merge them
*     using higher values as overlay, and lower values as base.  The data are then cast as char
*     in the final images.
*  Note that the code presently assumes that the absolute value of the positive data exceeds that
*     of the negative. The data are rescaled to the maximum positive levels.
*  3/12/97: v 1.3 better autoscaling. Darken the base image by setting effective LMAX to 100 (was 127)
*
*	Reproduction and use of this source code is permitted for academic purposes only
*	Commercial use s prohibited without prior consent of the author.

* (C) Copyright 1997-1999 by Mark S. Cohen
 * All Rights Reserved.
 *
 * Permission to use, copy, modify and distribute this software
 * and its documentation for academic purposes is hereby granted without
 * fee, provided that the above copyright notice appear in all copies
 * and that both that copyright notice and this permission notice
 * appear in supporting documentation, and that the name of Mark Cohen
 * not be used in advertising or publicity
 * pertaining to distribution of the software without specific,
 * written prior permission.  Mark Coheb makes no
 * representations about the suitability of this software for any
 * purpose.  It is provided "as is" without express or implied
 * warranty.
 *
 * THE AUTHOR DISCLAIMS ALL WARRANTIES WITH REGARD TO
 * THIS SOFTWARE, INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
 * AND FITNESS, IN NO EVENT SHALL THE AUTHOR BE LIABLE
 * FOR ANY SPECIAL, INDIRECT OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF USE, DATA OR PROFITS, WHETHER IN
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING
 * OUT OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS
 * SOFTWARE.
 */

/* $Log: overlayMac.c,v $
 * Revision 1.1  2004/07/29 00:38:09  mscohen
 * Initial revision
 *
 * Revision 1.1  2004/01/29 06:48:35  mscohen
 * Initial revision
 *
 * Revision 1.5  2000/04/27  16:39:20  mscohen
 * Added bilinear interpolation and automatic posting of revision level
 *
 * Revision 1.4  2000/04/27  16:31:08  mscohen
 * Consistent with new ImgLib.
 * Added feature for auto xds
 *
 * Revision 1.3  2000/01/21  17:04:52  mscohen
 * Changed handling of interpolation option. Still not debugged/supported
 *
 * Revision 1.2  1999/09/22  16:37:03  mscohen
 * Added support for new proc file
 * Added output of xds command line.
 *
 * Revision 1.1  1999/09/22  15:44:48  mscohen
 * Initial revision
 * */
 
static char id[] = "$Revision: 1.1 $$Date: 2004/07/29 00:38:09 $";

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "getopt.h"
#include "ImgLib.h"
#define  LMAX 127
#define  REDUCTION 0
#define  HMAX 254
#define  CROP 1

/* function prototypes */
void print_usage(char *name);
float absval(float aFloat);

void print_usage( char *name )
{
	char buffer[80];

	fprintf( stderr, "\n%s\n================\n", name ); 
   fprintf(stderr,
"Usage: %s -B baseimage -O overlayimage -o outputname -[u,l,U,L,d,#,N,A,V]\n\n\t%s\n", name, id );
	fprintf( stderr, "\tPlease report any errors to Mark Cohen\n\n\
*********************************************************************************\n\
\tPLEASE NOTE:  THE OLDER VERSION OF OVERLAY IS AVAILABLE AS: OVERLAY.OLD !!\n\
*********************************************************************************\n\
Takes two images, (base and functional) and forms a composite\n\
with the functional containing the upper gray scale values. The\n\
overlay image is interpolated, if needed, to the base image size.\n\
The output image is an eight bit Mac-compatible file\n\
\nThe current version shows positive and negative overlays by default\n\
and uses pixel replication (rather than interpolation) for overlays\n\
Note new options for slice and image.\n\
\nOPTIONS:\n\
B: baseimage (REQUIRED)\n   u,l: upper & lower range for baseimage\n\
     (default: automatic noise threshold detection)\n\
O: overlayimage (REQUIRED)\n   U,L: upper & lower range for overlay\n\
      (defaults: half of overlay max to max)\n\
      N.B.  -U and -L are REQUIRED for multi image sets\n\
o: Output file (REQUIRED). Basename only; type will be buchar\n\
m: Mode: 1-positive and negative overlay, 2-Positive only, 3-Negative only\n\
#: Image number to use from overlay set (counting from one)\n\
S: Slice number to use from multi-slice base set (counting from one)\n\
N: Image number to use from multi-image base set (counting from one)\n\
I: Bilinear Interpolation (SUPPORTED AS OF 4/27/00!).\n\
h: Prints out an extensive help file.\n\
A: automatically invoke the correct xds viewer and color table.\n\
V: verbose option\n\n\
Outputs a processing record: outputname.%s.proc\n", name );
}

float absval(float aFloat)
{
   return (aFloat<0.0 ? -aFloat : aFloat);
}

int main( int argc, char **argv )
{
	char   dual=1;		/* dual color scale by default */
	int    bImage=0;	/* image number to use from base set */
	int    bSlice=0;	/* slice number to use from base set */
	int    fImage=0;	/* image number to use from overlay set */
	int    fSlice=0;    /* slice number to use from overlay set */
	int    j, argp=EOF;
	int    fxsize, fysize, fsize, zs;
	int    ovlMode = kPosNeg;
	long   bxsize, bysize, bsize;
	long   nonNoise;	// not used explicitly
	char   bilinear = false;
	char   AutoMask=true; /* flag for automatic noise threshold */
	char   autoView = false;
	char   command[256];
	char   debug = true;
	char   verbose = false;
	char   base=false, over=false, out=false;
	char   *base_im, *overlay_im, *out_im, fname[256], buffer[80];
	unsigned char	*cvec;
	char   *isAuto="N";
	char   *isDual="Dual  ";
	float  bscale, bmin, bmax, fmin, fmax;	/* intensity scaling */
	float  OutBaseMax, OutBaseMin;	/* min and max of base image in composite */
	float  inFmax=0, inFmin=0, ubmin=0, ubmax=0, ufmin=0, ufmax=0;
	float  *bdata, *fdata, *vec;
	float  *buf1, *buf2, *buf3;
	FILE   *myFile, *bFile, *fFile;
	IMAGE  fim, bim;
	OSErr  error = noErr;
	
	while ( (argp=getopt(argc,argv,"B:O:o:u:l:U:L:#:S:N:m:IAVh?")) != EOF ) {
		switch( argp )   {
			case 'B': base_im = optarg; base=true; break;
			case 'O': overlay_im = optarg; over=true; break;
			case 'o': out_im = optarg; out=true; break;
			case 'u': ubmax = atof( optarg ); break;
			case 'l': ubmin = atof( optarg ); break;
			case 'U': ufmax = atof( optarg ); break;
			case 'L': ufmin = atof( optarg ); break;
			case 'm': ovlMode = atoi( optarg );
				if( ovlMode < 1 || ovlMode > 3 ) {
					printf( "unrecognized overlay mode\nAborting\n" );
					return 0;
				}
				break;
			case '#': fImage = atof( optarg );
			    fprintf( stderr,"Using overlay image: %d\n",fImage ); break;
			case 'N': bImage = (atof( optarg ) - 1);
			    fprintf ( stderr,"Using base image: %d\n",bImage+1 ); break;
			case 'S': bSlice = (atof( optarg ) - 1);
			    fprintf( stderr,"Using base slice: %d\n",bSlice+1 ); break;
			case 'I': bilinear = true; break;
			case 'A': autoView = true; break;
			case 'V': verbose = 1; break;
			case 'h':
			case '?':
				exit( -1 );
				break;
			default:
				print_usage(argv[0]);
				exit(-1);
				break;
		}
	}

	if (!out || !base || !over) {
		fprintf(stderr,"One of the images wasn't specified\n");
		print_usage(argv[0]); exit(-1);
	}

	if( verbose ) fprintf(stderr,"...reading base image\n");

	error = UC_Readheader( base_im, &bim);
	if( error ) {
		ILError( error, "Reading base image" );
	}
	
	bxsize = bim.dim.x; bysize = bim.dim.y;
	bsize = bxsize*bysize;

	bdata = buf1 = (float *)ck_malloc( bsize*sizeof(float),"bdata");
	fdata = buf2 = (float *)ck_malloc( bsize*sizeof(float),"fdata");
	vec   = buf3 = (float *)ck_malloc( bsize*sizeof(float),"vec");
	cvec  = (unsigned char *)ck_malloc( bsize*sizeof(unsigned char),"cvec");

	if( !bdata || !fdata || !vec || !cvec ) {
		ILError( MEMALLOCERROR, "Could not allocated image storage" );
	}

/* Prepare the base image by finding the dimensions and data range */
	bFile = errfopen( base_im, "r" );
	if( !bFile ) {
		ILError( COULD_NOT_OPEN_IMAGE, "Base Image" );
	}
	
	bim.data_type = T_FLOAT;	// set desired input file type
	error = UC_Readimage( bFile, &bim, bdata, bSlice, bImage );
	if( error ) {
		ILError( IMG_FILE_ERROR, "Reading base image" );
	}
	
	fclose( bFile );

/* Find the base min and max */
	if(verbose) fprintf( stderr, "Detecting data range and thresholding\n" );

	error = vminmax( bdata, bsize, &bmax, &bmin, T_FLOAT );
	if( error ) {
		ILError( error, "" );
	}

	if(verbose) fprintf(stderr,"Max in base image: %0.4f, Min in base image: %0.4f\n",bmax, bmin);

	if ( !ubmax || !ubmin ) {
		if(verbose) fprintf(stderr,"Automatic windowing...\n");

		imageDataRange( bdata, bsize, &bmax, &bmin, &nonNoise, T_FLOAT );

		fprintf( stderr,"Calculated noise level in base image: %3.2f\n",bmin );
		fprintf( stderr,"Calculated hi cutoff level in base image: %3.2f\n",bmax );
	}

	if (ubmax) {
		AutoMask=0;
		bmax = ubmax;
	}
	if (ubmin) {
		AutoMask=0;
		bmin = ubmin;	/* user input range */
	}

	if(verbose) {
		fprintf(stderr,"Rescaling...\n");
	}

	OutBaseMin = BMIN;
	OutBaseMax = BMAX;
	
	vRescale( bdata, bsize, &bmax, &bmin, &OutBaseMax, &OutBaseMin, T_FLOAT );

	if (verbose) fprintf (stderr,"Base max:%0.4f, min:%0.4f, scale: %0.4f\n", bmax, bmin, bscale);

/* Find dimensions of overlay (stats) image */
	UC_Readheader( overlay_im, &fim );
	fxsize = fim.dim.x;
	fysize = fim.dim.y;
	zs = fim.dim.timePts;

	fsize = fxsize * fysize;

	if (fImage) zs = 1;  /* User selected a single image - process one only */
	fmax=fmin=0;
	if (zs != 1) {
		if ((!ufmax) || (!ufmin)) {
			fprintf(stderr,"You must supply values for the overlay min and max for multi image series\n");
			exit (-1);
		}
		fmax = ufmax;
		fmin = ufmin;
	}

	fFile = errfopen( overlay_im, "rb" );
	if( !fFile ) {
		ILError( COULD_NOT_OPEN_IMAGE, "Overlay Image");
	}
	
/* overlay all images in the overlay set  */
	fim.data_type = T_FLOAT;
	for (j=0; j< zs; j++) { 
		if (verbose) { fprintf(stderr,"Overlay image %0.3d\n",j); };
	   
		error = UC_Readimage( fFile, &fim, fdata, fSlice, j );
		if( error ) {
			ILError( IMG_FILE_ERROR, "overlay image" );
		}
		
		if (verbose) fprintf(stderr,"Calculating min and max\n");

		error = vminmax( fdata, fsize, &inFmax, &inFmin, T_FLOAT );
		if( error ) {
			ILError( error, "" );
		}
		
		if (zs == 1) {    /* single overlay image */
/* Calculate and display the min and max */
			fmax = inFmax;
			fmin = 0.5*inFmax;     /* There would be no point in accepting the full range */
			if (ufmax) {
				fmax = ufmax;
			}
			if (ufmin) {
				fmin = ufmin;
			}
			
			fprintf(stderr,"Overlay min used: %f. Overlay max used: %f\n",fmin, fmax);

			if (debug) printf ("User Entered: %f, fmax in program: %f\n",ufmax, fmax);
		}     /* single image */

/* Resize the images, so that the functional and base images are the same size */
		if (bsize != fsize) {
			if (bsize%fsize != 0) {
				fprintf(stderr,"The image sizes must be integer multiples, sorry\n");
				exit (-1);
			}
			if (bsize > fsize) {
				if( bilinear ) {     /* bilinear interp if requested */
					error = interp2D( fdata, fxsize, fysize, (int)(bxsize/fxsize), vec, T_FLOAT );
					if( error ) {
						ILError( OVERLAY_FAILURE, "interpolation" );
					}
					fdata = vec;       /* reassign the fdata pointer */
				} else {                  /* pixel replication by default */
					error = pixreplicate( fdata, fxsize, fysize, (int)(bxsize/fxsize), vec, T_FLOAT );
					if( error ) {
						ILError( OVERLAY_FAILURE, "interpolation" );
					}
					fdata = vec;       /* reassign the fdata pointer */
				}
			} else {
				fprintf(stderr,"\nIn this version, only the base image may be larger\n");
				exit (-1);
			}
		}  /*  bsize not equal to fsize */
	   
/* perform the actual merging. */
		switch( ovlMode ) {
			case kPosNeg:
				fprintf( stderr, "\nProcessing with positive and negative overlay.\n");
				break;
			case kPosOnly:
				fprintf( stderr, "\nOverlaying POSITIVE values only.\n");
				break;
			case kNegOnly:
				fprintf( stderr, "\nOverlaying NEGATIVE values only.\n");
				break;
			default:
				fprintf( stderr, "Unrecognized overlay mode" );
				return 0;
		}		

		error = Overlay( bdata, fdata, cvec, bsize, fmax, fmin, ovlMode );	

		if( error ) {
			ILError( OVERLAY_FAILURE, "" );
		}	   
	   
		if (zs == 1) {
			sprintf(fname,"%s.buchar",out_im);
			myFile = fopen(fname,"w");
			if( !myFile ) {
				ILError( IMG_FILE_ERROR, fname );
			}
			
			fprintf( stderr, "View with\n\txds -m /usr/local/bin/UCLATools/PosNeg16 %s &\n", fname );
			if( autoView ) {
				sprintf( command, "xds -m /usr/local/bin/UCLATools/PosNeg16 %s &\n", fname );
				system( command );
			}

			fwrite(cvec, sizeof(char), bsize, myFile);
			fclose(myFile);

			sprintf(fname,"%s.hdr",out_im);
			myFile = fopen(fname,"w");
			if( !myFile ) {
				ILError( IMG_FILE_ERROR, fname );
			}

/* byte swap irrelevant for char images */
			fprintf(myFile, "%d %d %d %d", bysize, bxsize, 1, 0);
			fclose(myFile);
		} else {
			sprintf(fname,"%s.%0.3d.buchar",out_im,j);

			myFile = errfopen(fname,"w");
			if( !myFile ) {
				ILError( IMG_FILE_ERROR, fname );
			}

			fwrite(cvec,sizeof(char),bsize,myFile);
			fclose(myFile);

			sprintf(fname,"%s.%0.3d.hdr",out_im,j);
			myFile = errfopen(fname,"w");
			if( !myFile ) {
				ILError( IMG_FILE_ERROR, fname );
			}

			fprintf(myFile, "%d %d %d %d", bysize, bxsize, 1, 0);
			fclose(myFile);
		}   /* multiple images */
	}    /* else for all overlay images */

	fclose (fFile);

	fprintf ( stderr,"\nMaximum in overlay set was: %0.4f, minimum was: %0.4f\n",inFmax,inFmin );
	isAuto = ( AutoMask ? "Y" : "N" );
	isDual = ( dual ? "Dual  " : "Single" );

	error = OpenProcFile( argc, argv, out_im, argv[0], &myFile );
	ILError( error, out_im );
	
	fprintf( myFile,"Report errors or questions to mscohen@ucla.edu\n" );
	fprintf( myFile,"BASE IMAGE FILE:..%s\n", base_im );
	fprintf( myFile,"BASE IMAGE #:.....%d\n", bImage+1 );
	fprintf( myFile,"BASE SLICE #:.....%d\n", bSlice+1 );
	fprintf( myFile,"OVERLAY FILE:.....%s\n", overlay_im );
	fprintf( myFile,"OVERLAY IMAGE #:..%d\n", fImage );
	fprintf( myFile,"NUM IMAGES:.......%d\n", zs );
	fprintf( myFile,"MAX IN OVERLAY....%0.4f\n", inFmax );
	fprintf( myFile,"MIN IN OVERLAY....%0.4f\n", inFmin );
	fprintf( myFile,"OVERLAY MODE:.....%s\n", isDual );
	fprintf( myFile,"AUTO-THRESHOLD:...%s\n", isAuto );
	fprintf( myFile,"OVERLAY RANGE:....%0.4f to %0.4f\n", fmin, fmax );
	fprintf( myFile,"BASE IMAGE RANGE:.%0.4f to %0.4f\n", bmin, bmax );
	fprintf( myFile,"INTERP FACTOR:....%d\n",(int)(bxsize/fxsize) );
	fprintf( myFile,"INTERP MODE:......");
	if(bilinear) {
		fprintf( myFile,"Bilinear\n");
	} else {
		fprintf( myFile,"Pixel Replication\n");
	}
	fclose( myFile );

	free(buf3);
	free(buf2);
	free(buf1);
	free(cvec);

	return 0;
}
/* </PRE> </CODE> */
