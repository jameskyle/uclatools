/*[ Compilation unit ********************************************************\
*
* Name        :   ds_head_constants.h
*
* Author      : A. Thumser (TH); Siemens AG UBMed CMS/SCE64; (09131)844797
*
* Language    : C
*
* Description : The header file defines the constants used by the internal
*				header definition.
*
\****************************************************************************/
/* Changed: 30-MAR-1993; Pracht; B1.1; CHARM: - *****************************\
*
* Description : Adapted to B1.1 enviroment
*
\****************************************************************************/
/* Changed: 24-JUN-1993; Chr. Schaefer; B1.1; CHARM: - **********************\
*
* Description : The value of 'DS_DUMMY_LENGTH' has been corrected to 0.
*		This macro defines the difference between the size of the
*		header and the next multiple of 1k bytes. It is only used
*		in those routines which write the header to the global
*		database (ESEGDAT) or read the header from there to
*		determine the size of the header (LENGTH_GROUP_0008 +
*		LENGTH_GROUP_0009 + ... + LENGTH_TO_FILL_K_BORDER).
*
\****************************************************************************/

/*] END: */


#ifndef DS_HEAD_CONSTANTS
#define DS_HEAD_CONSTANTS

#define DS_DUMMY_LENGTH                                                                    0L

/* PRECOMPILER: define length of group overlaying buffers */
#define LENGTH_GROUP_0008                                                                 384
#define LENGTH_GROUP_0009                                                                 384
#define LENGTH_GROUP_0010                                                                 256
#define LENGTH_GROUP_0011                                                                 128
#define LENGTH_GROUP_0013                                                                 384
#define LENGTH_GROUP_0018                                                                 384
#define LENGTH_GROUP_0019_PART1                                                           128
#define LENGTH_GROUP_0019_PART2                                                           512
#define LENGTH_GROUP_0019_PART3                                                           384
#define LENGTH_GROUP_0019_PART4                                                           256
#define LENGTH_GROUP_0020                                                                 512
#define LENGTH_GROUP_0021_PART1                                                           256
#define LENGTH_GROUP_0021_PART2                                                           256
#define LENGTH_GROUP_0021_PART3                                                           768
#define LENGTH_GROUP_0028                                                                 256
#define LENGTH_GROUP_0029                                                                 256
#define LENGTH_GROUP_0051                                                                 640
#define LENGTH_TO_FILL_K_BORDER                                               DS_DUMMY_LENGTH


/* PRECOMPILER: define length of common header strings */
#define LENGTH_LABEL                                                                      26L


/* PRECOMPILER: define length of special header stings */
#define LENGTH_AGE                                                                         4L
#define LENGTH_COMMENT                                                                    26L
#define LENGTH_DIAGNOSIS                                                                  40L
#define LENGTH_DIRECTION                                                                   4L
#define LENGTH_FILE_NAME                                                                  64L
#define LENGTH_FILTER_ID                                                                  12L
#define LENGTH_HEADER_VERSION                                                              8L
#define LENGTH_NUCLEUS                                                                     8L
#define LENGTH_MANUFACTURER                                                                8L
#define LENGTH_ORIENTATION                                                                 3L
#define LENGTH_PATIENT_ID                                                                 12L
#define LENGTH_SEQUENCE_INFO                                                               8L
#define LENGTH_SOFTWARE_VERSION                                                            8L


/* PRECOMPILER: define length of origianl headers */
#define LENGTH_NUM1_HEADER                                                              4096L
#define LENGTH_NUM2_HEADER                                                              8192L
#define LENGTH_SOM0_HEADER                                                              2048L
#define LENGTH_SOM0_IMAGE_TEXT                                                           960L
#define LENGTH_SOM1_HEADER                                                              4096L

#define LENGTH_SOM_HEADER                                                  LENGTH_SOM1_HEADER


/* PRECOMPILER: define Xxx_UNDEFINED constants */
#define Enum_UNDEFINED                                                               (-19222)
#define Integer_UNDEFINED                                                            (-19222)
#define String_UNDEFINED                                                                  '?'
#define Real_UNDEFINED                                                             (-19222.0)


#endif
