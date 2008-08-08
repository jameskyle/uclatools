/* <HTML><HEAD><TITLE>ImgLib routines</TITLE></HEAD><BODY><PRE><!--
/***************************************************************************
*	Part of ImgLib
*   $Id: analyzeUtil.h,v 1.8 2002/06/06 03:53:36 mscohen Exp $
*   A set of primitives for use in image processing.
*               c 1996-2002  Mark S. Cohen
*   This software is distributed as is with no guarantees.
*   Please report any errors or bug fixes to mscohen@ucla.edu
*
*   For academic use only. Commercial users should contact
*   Mark Cohen for licensing information.
*
***************************************************************************/
/******************************************************


 $Log: analyzeUtil.h,v $
 Revision 1.8  2002/06/06 03:53:36  mscohen
 Added AnalyzeTypeToString

 * Revision 1.7  2002/04/16  17:03:14  mscohen
 * copyright
 *
 * Revision 1.7  2002/04/16  17:03:14  mscohen
 * copyright
 *
 * Revision 1.6  2001/08/24  16:44:56  mscohen
 * added html commeting
 *
 * Revision 1.5  2000/07/06  22:23:04  mscohen
 * changes in byteswap
 *
 * Revision 1.5  2000/07/06  22:23:04  mscohen
 * changes in byteswap
 *
 * Revision 1.4  2000/02/15  15:55:04  mscohen
 * Rename BigEndian and LittleEndian functions
 *
 * Revision 1.4  2000/02/15  15:55:04  mscohen
 * Rename BigEndian and LittleEndian functions
 *
 * Revision 1.3  1999/09/20  16:34:25  mscohen
 * Reconstruction after crash of 9-1999
 *
 * Revision 1.4  1999/07/14  05:51:46  mscohen
 * ANSI C compatibility
 *
 * Revision 1.2  1998/04/15  04:06:20  mscohen
 * Include byte swap and datasize routines.
 *
 $Date: 2002/06/06 03:53:36 $

A set of primitives to create and read analyze headers
Be sure to initialize the machine type before compiling!

*******************************************************/

#pragma once

#ifndef __ANALYZEUTIL__
#define __ANALYZEUTIL__

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
   kNeedYCrop    = 16
};	/* used for UCLA specific flags */

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
    float pixdim[8];             /* 36 + 32   */
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
 *               char vox_units[4] * labels voxerl spatial unit
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
#define OSErr short
#endif
/* --> */
OSErr  EmptyAnaHdr( dsr* DSR, char *description, float sliceSpacing,
	 int xs, int ys, int n_slices, int time_pts, char orient ); /* analyzeUtil.c */
int    get_datasize(int data_type); /* analyzeUtil.c */
OSErr  vbyteswap(void *ivec, long vlen, int data_type); /* analyzeUtil.c */
void ShowAnalyzeHeader( dsr *DSR, FILE *fp, char *name ); /* analyzeUtil.c */
short swapHdrBytes( void *Hdr ); /* analyzeUtil.c */
char *AnalyzeTypeToString( short TYPE ); /* analyzeUtil.c */

/* </PRE></BODY></HTML> */
#endif /* conditional compilation */
