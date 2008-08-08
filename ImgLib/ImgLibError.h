/*<HTML><HEAD><TITLE>ImgLib error routines</TITLE></HEAD><BODY><PRE>
*  <font size=+2><b>ImgLibError.h</b></font>
*	<i>Please note last saved date below:</i>
*	<font size=+1><b>$Date: 2002/09/11 16:18:50 $</b></font>
*    A set of primitives for use in image processing.
*               c 1996-2002  Mark S. Cohen
*    This software is distributed as is with no guarantees.
*    Please report any errors or bug fixes to mscohen@ucla.edu
*
*    For academic use only. Commercial users should contact
*    Mark Cohen for licensing information.
*
*	Secret decoder between name and value of ImgLib errors
*************************************************************/
/*
 * $Log: ImgLibError.h,v $
 * Revision 2.16  2002/09/11 16:18:50  mscohen
 * Modified $Log$ directive in RCS
 *
 * Revision 2.15  2002/09/10 20:42:17  mscohen
 * added NOAFNITYPE definition, zrinka
 *
 * Revision 2.14  2002/04/16 17:02:08  mscohen
 * copyright
 *
 * Revision 2.13  2002/04/16  16:09:29  mscohen
 * html
 * */
#pragma once

#define READIM_SUCCESS     0
#define NOERROR            noErr

/* ImgLib errors start at -1000 */
/* this file contains the error codes for ImgLib errors, all of which are
   negative numbers starting at -1000 fix scanSTATerrors.c and scanSTATerrors.h too */
/* these are the defined ImgLib errors */

#define UNKNOWN_ERROR            -1000
#define FIRSTImgLibERROR UNKNOWN_ERROR
#define UNKNOWNTYPE              -1001
#define NOTAFNITYPE		         -1011

/* math and memory */
#define DIVIDE_BY_ZERO_ERR       -1002
#define INVALID_ADDRESS          -1003
#define ARRAY_RANGE_ERR          -1004
#define NIL_PTR                  -1005
#define NILIMGPTR                -1006
#define OUT_OF_MEMORY            -1007
#define MEMALLOCERROR            -1008
#define NEG_DATA_NOT_ALLOWED     -1009
#define MEM_ALREADY_ALLOCATED    -1010

/* functions */
#define OVERLAY_FAILURE          -1020
#define	MDISPLAY_ERROR           -1021
#define NEG_DATA_IN_AUTORANGE    -1022
#define CANT_HAVE_NEG_THRESHOLD  -1023
#define SCALE_FACTOR_ZERO        -1024
#define VOLUME_DISPLAY_INVALID   -1025
#define	DATA_OUT_OF_RANGE        -1026
#define	TIMEOUTERROR             -1027
#define CONVERSION_ERROR         -1028
#define SPARSE_DATA_VALUES       -1029
#define RESCALE_NEEDED           -1030
#define UNKNOWN_DISPLAY_MODE     -1031

/* headers */
#define IMG_HEADER_ERROR         -1050
#define COULD_NOT_OPEN_HEADER    -1051
#define	INVALID_GENESIS_HDR      -1052
#define HEADER_SIZE_MISMATCH     -1053
#define INVALID_HDR              -1054
#define MISSING_IMG_HEADER       -1055
#define HEADER_INTERP_FLAG       -1056

/* files */
#define COULD_NOT_OPEN_IMAGE     -1080
#define IMG_FILE_ERROR           -1081
#define PARADIGM_FILE_ERROR      -1082
#define	BEYOND_EOF               -1083
#define	FILE_NOT_FOUND           -1084
#define UNABLE_TO_OPEN_LOGFILE   -1085
#define	NO_REMOTE                -1086
#define UNKNOWNFILETYPE          -1087
#define WRITE_ERROR              -1088
#define READ_ERROR               -1089
#define FOPEN_ERROR              -1090
#define FCLOSE_ERROR             -1091
#define FILE_ERROR               -1092
#define DATATYPE_RANGE_ERROR     -1093
#define INVALID_FILENAME         -1094
#define LAST_IMGLIB_ERROR        -1095
#define READ_OPEN_FAILURE        -1096
#define WRITE_OPEN_FAILURE       -1097
#define APPEND_OPEN_FAILURE      -1098
#define SILENT_ERR               -1099

#ifndef OSErr
#define OSErr short
#endif
void ILError( OSErr theError, char *errString );

#ifdef MAC
enum {
	iUNKNOWN_ERROR = 1,
	iUNKNOWNTYPE,
	iDIVIDE_BY_ZERO_ERR,
	iINVALID_ADDRESS,
	iARRAY_RANGE_ERR,		/* 5 */
	iNIL_PTR,
	iNILIMGPTR,
	iMEMORY_ERROR,
	iMEMALLOCERROR,
	iNEG_DATA_NOT_ALLOWED,		/* 10 */
	iMEM_ALREADY_ALLOCATED,

/* Functions */
	iOVERLAY_FAILURE,
	iMDISPLAY_ERROR,
	iNEG_DATA_IN_AUTORANGE,
	iCANT_HAVE_NEG_THRESHOLD,		/* 15 */
	iSCALE_FACTOR_ZERO,
	iVOLUME_DISPLAY_INVALID,
	iDATA_OUT_OF_RANGE,
	iTIMEOUTERROR,
	iCONVERSION_ERROR, 	/* 20 */
	iSPARSE_DATA_VALUES,
	iRESCALE_NEEDED,
	iUNKNOWN_DISPLAY_MODE,

/* headers */
	iIMG_HEADER_ERROR,
	iCOULD_NOT_OPEN_HEADER, /* 25 */
	iINVALID_GENESIS_HDR,
	iHEADER_SIZE_MISMATCH,
	iINVALID_HDR,
	iMISSING_IMG_HEADER,
	iHEADER_INTERP_FLAG,         /* 30 */

/* files */
	iCOULD_NOT_OPEN_IMAGE,
	iIMG_FILE_ERROR,
	iPARADIGM_FILE_ERROR,
	iBEYOND_EOF,
	iFILE_NOT_FOUND,           /* 35 */
	iUNABLE_TO_OPEN_LOGFILE,
	iNO_REMOTE,
	iUNKNOWNFILETYPE,
	iWRITE_ERROR,
	iREAD_ERROR,   /* 40 */
	iFOPEN_ERROR,
	iFCLOSE_ERROR,
	iFILE_ERROR,
	iDATATYPE_RANGE_ERROR,
	iINVALID_FILENAME,	/* 45 */
	iREAD_OPEN_ERROR,
	iWRITE_OPEN_ERROR,
	iAPPEND_OPEN_ERROR,
	iSILENT_ERR
};
#endif

/* </PRE></BODY></HTML> */
