#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>

#include "getopt.h"
#include "ImgLib.h"
#include "ImgLibError.h"

#undef mactest
#ifdef mactest
#include <console.h>
#endif
#define OSErr short

/***** APPLICATION-SPECIFIC CONSTANTS ******/
#define IGNORE 1234.5
#define N_OUTVOLUMES 4	/* number of statistical images in output file. */
#define TINY 1.0e-20
#define LS_MAX -50
#define STAT_MAX 1.0
#define STAT_CUTOFF 0.4
#define SLOPE_CUTOFF 0.1
#define SLOPE_MAX 15
#define true 1
#define false 0
#define LIMIT 60

#undef DEBUG

/* Prototypes */
void print_usage( char *name );

float FindGlobalRatio( float *volOneSum, unsigned short int *theVolume,
                         unsigned char *mask,long volSize, int npts );

void  MakeCCandSMap( float *Sy, float *Sy2, float *Sxy, float Sx2,
                     float Sx, float SSxx,
                     unsigned char *mask, float *CMap, float *SMap,
                     int npts, long volSize );

void  InterpCropMap( float *theMap, float *theDest, int bilinear, int xs, int ys, int Slices, int Ratio );

void  WriteStatOverlay( float *baseMult, float *StatVol, unsigned char *ColMap,
                        float *buf1, float *buf2, int xs, int ys, int Slices, 
                        float stat_cutoff, int interp, int bilinear, FILE *OVLFile );
//                        float stat_cutoff, int interp, int bilinear, char *name );

void  MakeProbabilityMap( float *CMap, float *PMap, float df, long volSize );

//-----------------------------------------------------

