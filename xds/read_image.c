#if(0)
  FTANGLE v1.40, created with UNIX on "Saturday, October 30, 1993 at 13:15." 
  COMMAND LINE: "ftangle Webs/read_image.web"
  RUN TIME: "Friday, November 12, 1993 at 12:19."
  WEB FILE:    "Webs/read_image.web"
  CHANGE FILE: (none)
#endif
/* 3: */
//#include "read_otl.h"
#include "read_image.h"
void* malloc();

#define toupper(c) ((c)-'a'+'A')
#define tolower(c) ((c)-'A'+'a')

/* :3 *//* 4: */

#define T_CHAR   1
#define T_SHORT   2
#define T_LONG   3
#define T_FLOAT   4
#define T_DOUBLE  5
#define T_COMPLEX  6
#define T_UCHAR   7
#define T_USHORT  8
#define T_ULONG   9
#define T_NATIVE 10

/* :4 *//* 5: */

#define IDF  1
#define GED  2
#define OMEGA  3
#define ANMR  4
#define FLT  5
#define STR  6
#define BRUK  7
#define GEDNO  8
#define BLOCK_CHAR 9
#define BLOCK_UCHAR 10
#define BLOCK_SHORT 11
#define BLOCK_USHORT 12
#define BLOCK_LONG 13
#define BLOCK_ULONG 14
#define BLOCK_FLOAT 15
#define IMG  16
#define GENESIS  17
#define SISCO  18


/* :5 *//* 6: */

#define NO_SUCH_FILE -1
#define BAD_FILE -2
#define BAD_SCALE -3

/* :6 *//* 7: */

#define VERSION_NUMBER 3

/* :7 *//* 8: */

read_image(fname,type,image,image_number,rescale_ok)
char*fname;
int type;
struct image*image;
int image_number;
int rescale_ok;
{
/* 9: */

int fd= -1;
char*real_name= 0,*extension= 0,*basename= 0,*rawext= 0,*cp1;
float*tmpdat= 0;
int rows,cols;
int file_type;
int which_image= 0;
int ged_bias;

short*sp1= 0;
unsigned short*tmpushort= 0;
char*hdrname= 0;
float*fptmp= 0;
char*dirname= 0;
char*mainname= 0;
char*paramname= 0;
unsigned char*tmpspace= 0;

/* :9 */
;
/* 19: */

{
image->hdr= (unsigned char*)0;
image->image.c= (char*)0;
image->row_ptr.c= (char**)0;
image->version= VERSION_NUMBER;
image->scale= 1.0;
image->dim_x= image->dim_y= image->dim_z= -1;
}

/* :19 */
;
/* 11: */

{
/* 17: */

{
int basenamelen,extlen;
char*extptr;

if((real_name= malloc((unsigned)strlen(fname)+1))==(char*)0){
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;
}

strcpy(real_name,fname);

extptr= strrchr(fname,'.');
if(extptr==(char*)0){
basenamelen= strlen(fname)+1;
extlen= 0;
}else{
basenamelen= extptr-fname;
extlen= strlen(extptr)+1;
}

if((basename= malloc((unsigned)basenamelen+1))==(char*)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

strncpy(basename,fname,basenamelen);
*(basename+basenamelen)= '\0';

if(extlen>0){
if((extension= malloc((unsigned)extlen))==(char*)0){
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;
}

if((rawext= malloc((unsigned)extlen))==(char*)0){
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;
}

strcpy(extension,extptr+1);
}else{
extension= (char*)0;
}
}


/* :17 */
;

fd= open(real_name,O_RDONLY,0);
if(fd<0)/* 12: */

{
register char*cp;

if(extension!=(char*)0){
for(cp= extension;*cp;cp++)
if(isascii(*cp)&&islower(*cp))*cp= toupper(*cp);
sprintf(real_name,"%s.%s",basename,extension);
fd= open(real_name,O_RDONLY,0);
}
}

/* :12 */
;
if(fd<0)/* 13: */

{
register char*cp;

if(extension!=(char*)0){
for(cp= extension;*cp;cp++)
if(isascii(*cp)&&isupper(*cp))*cp= tolower(*cp);
sprintf(real_name,"%s.%s",basename,extension);
fd= open(real_name,O_RDONLY,0);
}
}

/* :13 */
;
if(fd<0)/* 14: */

{
register char*cp;

for(cp= basename;*cp;cp++)
if(isascii(*cp)&&islower(*cp))*cp= toupper(*cp);

if(extension!=(char*)0){
for(cp= extension;*cp;cp++)
if(isascii(*cp)&&islower(*cp))*cp= toupper(*cp);
sprintf(real_name,"%s.%s",basename,extension);
}else{
strcpy(real_name,basename);
}

fd= open(real_name,O_RDONLY,0);
}

/* :14 */
;
if(fd<0)/* 15: */

{
register char*cp;

for(cp= basename;*cp;cp++)
if(isascii(*cp)&&isupper(*cp))*cp= tolower(*cp);

if(extension!=(char*)0){
for(cp= extension;*cp;cp++)
if(isascii(*cp)&&isupper(*cp))*cp= tolower(*cp);
sprintf(real_name,"%s.%s",basename,extension);
}else{
strcpy(real_name,basename);
}

fd= open(real_name,O_RDONLY,0);
}

/* :15 */
;

if(fd<0)
/* 138: */

{
perror(real_name);
/* 139: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(NO_SUCH_FILE);
}

/* :139 */
;
}

/* :138 */
;

if(extension!=(char*)0){
strcpy(rawext,extension);
}
/* 16: */

{
register char*cp;

if(extension!=(char*)0){
for(cp= extension;*cp;cp++)
if(isascii(*cp)&&isupper(*cp))*cp= tolower(*cp);
}
}

/* :16 */
;
}

/* :11 */
;
/* 20: */

{
if(extension==(char*)0)
/* 147: */

{
fprintf(stderr,"I don't know how to read `%s'\n",real_name);
fprintf(stderr,"(I can't handle files with no extension)\n");
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :147 */
;

if(strcmp(extension,"idf")==0)file_type= IDF;
else if(strcmp(extension,"ged")==0)file_type= GED,ged_bias= 1024;
else if(strcmp(extension,"gedno")==0)file_type= GED,ged_bias= 0;
else if(strcmp(extension,"ome")==0)file_type= OMEGA;
else if(strcmp(extension,"anmr")==0)file_type= ANMR;
else if(strcmp(extension,"flt")==0)file_type= FLT;
else if(strcmp(extension,"str")==0)file_type= STR;
else if(strcmp(extension,"imag")==0)file_type= BRUK;
else if(strcmp(extension,"bchar")==0)file_type= BLOCK_CHAR;
else if(strcmp(extension,"buchar")==0)file_type= BLOCK_UCHAR;
else if(strcmp(extension,"bshort")==0)file_type= BLOCK_SHORT;
else if(strcmp(extension,"bushort")==0)file_type= BLOCK_USHORT;
else if(strcmp(extension,"blong")==0)file_type= BLOCK_LONG;
else if(strcmp(extension,"bulong")==0)file_type= BLOCK_ULONG;
else if(strcmp(extension,"bfloat")==0)file_type= BLOCK_FLOAT;
else if(strcmp(extension,"img")==0)file_type= IMG;
else if(strcmp(extension,"gen")==0)file_type= GENESIS;
else if(strcmp(extension,"sis")==0)file_type= SISCO;
else/* 142: */

{
fprintf(stderr,"unknown file type:  %s  (%s files not supported)\n",
real_name,extension);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :142 */
;

image->type= file_type;
}

/* :20 */
;
/* 21: */

{
switch(file_type){
case IDF:/* 40: */

{
float idf_scale;

/* 41: */

{
union{
float scl;
unsigned char byt[4];
}scl;

if((image->hdr= (unsigned char*)malloc(2048))==(unsigned char*)0){
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;
}

if(read(fd,image->hdr,2048)!=2048){
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;
}

image->rows= 256;
image->cols= 256;
image->images= 1;

if(127&image->hdr[908]){
scl.byt[0]= image->hdr[909]-1;
scl.byt[1]= image->hdr[908];
scl.byt[2]= image->hdr[911];
scl.byt[3]= image->hdr[910];
}else{
scl.scl= 1.;
}
idf_scale= 1./scl.scl;


}

/* :41 */
;
/* 42: */

{
int size;
float*fp;
unsigned char*ucp;

/* 43: */

{
size= image->rows*image->cols;

if((tmpdat= (float*)malloc((unsigned)size*sizeof(float)))==
(float*)0){
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;
}

if((tmpushort= (unsigned short*)malloc((unsigned)size*
sizeof(unsigned short)))==(unsigned short*)NULL){
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;
}
}

/* :43 */
;
/* 44: */

{
if(read(fd,tmpushort,size*sizeof(unsigned short))!=
size*sizeof(unsigned short))
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;
}

/* :44 */
;
/* 45: */

{
union{
unsigned char uc[2];
short s;
}un;
register int i;

ucp= (unsigned char*)tmpushort;
fp= tmpdat;

for(i= 0;i<size;i++,fp++){
un.uc[1]= *(ucp++);
un.uc[0]= *(ucp++);
*fp= (float)un.s*idf_scale;
}
}

/* :45 */
;
}

/* :42 */
;
}

/* :40 */
;break;
case GED:/* 22: */

{
/* 23: */

{
struct ged_hdr{
unsigned char hdr[14336];
};

if((image->hdr= (unsigned char*)
malloc((unsigned)sizeof(struct ged_hdr)))==(unsigned char*)0){
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;
}

if(read(fd,image->hdr,sizeof(struct ged_hdr))!=sizeof(struct ged_hdr)){
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;
}

image->rows= 256;
image->cols= 256;
image->images= 1;
}

/* :23 */
;
/* 24: */

{
int size;
register int i;
register float*fp;
register short*sp;

size= image->cols*image->rows;
if((sp1= (short*)malloc(size*sizeof(short)))==(short*)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

if((tmpdat= (float*)malloc(size*sizeof(float)))==(float*)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

if(read(fd,sp1,size*sizeof(short))!=size*sizeof(short))
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;

for(i= 0,fp= tmpdat,sp= sp1;i<size;i++,fp++,sp++)
*fp= (float)(*sp-ged_bias);
}

/* :24 */
;
}

/* :22 */
;break;
case OMEGA:/* 25: */

{
int echoes= -1,slices= -1,dimension= -1,orient;

image->hdr= (unsigned char*)0;

/* 26: */

{
FILE*fp;
char line[1024];

/* 27: */

{
if((hdrname= (char*)malloc((unsigned)(strlen(basename)+5)))==
(char*)0){
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;
}

sprintf(hdrname,"%s.hdr",basename);
if((fp= fopen(hdrname,"r"))==(FILE*)0){
/* 138: */

{
perror(real_name);
/* 139: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(NO_SUCH_FILE);
}

/* :139 */
;
}

/* :138 */
;
}
}

/* :27 */
;
/* 28: */

{
image->rows= image->cols= image->images= -1;

while(fgets(line,1024,fp)!=NULL){
if(strncmp(line,"    dim0",8)==0)
sscanf(&line[9],"%d",&image->cols);
else if(strncmp(line,"    dim1",8)==0)
sscanf(&line[9],"%d",&image->rows);
else if(strncmp(line,"    dim2",8)==0)
sscanf(&line[9],"%d",&image->images);
else if(strncmp(line,"    echoes",10)==0)
sscanf(&line[11],"%d",&echoes);
else if(strncmp(line,"    slices",10)==0)
sscanf(&line[11],"%d",&slices);
else if(strncmp(line,"    dimension",13)==0)
sscanf(&line[14],"%d",&dimension);
}

fclose(fp);
}

/* :28 */
;
/* 29: */

{
if((dimension<2)||(dimension>3)){
fprintf(stderr,"Error...I can only handle 2 or 3 dimensional data\n");
fprintf(stderr,
"Take your data set to mikeV, so he can extend read_image\n");
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

if((image->rows<1)||(image->cols<1)){
fprintf(stderr,"Error...impossible image size:  %d x %d\n",
image->rows,image->cols);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

if(image->images<1){
fprintf(stderr,"Error...less than 1 image in OMEGA file?\n");
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

if(echoes<1)echoes= 1;

if(slices<1)slices= 1;
}

/* :29 */
;
}

/* :26 */
;
/* 33: */

{
int size;

/* 30: */

{
size= image->rows*image->cols*sizeof(float);

if((tmpdat= (float*)malloc((unsigned)size))==(float*)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

if((fptmp= (float*)malloc((unsigned)size*2))==(float*)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;
}

/* :30 */
;
/* 35: */

{
int skip;

/* 36: */

{
which_image= image_number;
while(which_image<0){
printf("Reading %s...which of the %d images should I read?\n",
real_name,image->images);
printf("(Remember to count from zero!)\n");
scanf("%d",&which_image);
if((which_image<0)||(which_image>=image->images)){
fprintf(stderr,"\a\a\acome on...a number between 0 and %d\n",
image->images-1);
which_image= -1;
}
}
}

/* :36 */
;

skip= which_image*image->rows*image->cols*sizeof(float)*2;

if(lseek(fd,skip,SEEK_SET)!=skip)
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;
}

/* :35 */
;
/* 34: */

{
register int i;
register float*fp1,*fp2;

if(read(fd,fptmp,size*2)!=size*2){
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;
}

if((echoes==2)&&((image_number&1)==1)){
/* 31: */

{
register float*fp1,*fp2;
int i,j;

fp1= fptmp;
fp2= tmpdat;
for(i= 0;i<image->rows;i++){
for(j= 0;j<image->cols;j++,fp2++,fp1+= 2)
*fp2= *fp1;
}
}

/* :31 */
;
}else{
/* 32: */

{
register float*fp1,*fp2;
int i,j;

for(i= 0;i<image->rows;i++){
fp1= fptmp+image->cols*i*2;
fp2= tmpdat+(image->rows-i-1)*image->cols;
for(j= 0;j<image->cols;j++,fp2++,fp1+= 2){
*fp2= *fp1;
}
}
}

/* :32 */
;
}
}

/* :34 */
;
}

/* :33 */
;
}

/* :25 */
;break;
case ANMR:/* 46: */

{
int byteswap;

/* 47: */

{
FILE*fp;

if((hdrname= malloc((unsigned)(strlen(basename)+5)))==(char*)0){
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;
}

sprintf(hdrname,"%s.hdr",basename);

if((fp= fopen(hdrname,"r"))==(FILE*)0){
perror(hdrname);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

fscanf(fp,"%d %d %d %d",
&image->rows,&image->cols,&image->images,&byteswap);
fclose(fp);
}

/* :47 */
;
/* 48: */

{
/* 36: */

{
which_image= image_number;
while(which_image<0){
printf("Reading %s...which of the %d images should I read?\n",
real_name,image->images);
printf("(Remember to count from zero!)\n");
scanf("%d",&which_image);
if((which_image<0)||(which_image>=image->images)){
fprintf(stderr,"\a\a\acome on...a number between 0 and %d\n",
image->images-1);
which_image= -1;
}
}
}

/* :36 */
;
/* 49: */

{
int skip;

skip= which_image*image->rows*image->cols*sizeof(short);

if(lseek(fd,skip,SEEK_SET)!=skip)
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;
}

/* :49 */
;
/* 50: */

{
int size;

size= image->rows*image->cols*sizeof(unsigned short);

if((tmpushort= (unsigned short*)malloc((unsigned)size))==
(unsigned short*)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

if(read(fd,tmpushort,size)!=size){
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;
}
}

/* :50 */
;
/* 51: */

{
register int i,size;
register unsigned char*ucp,uc;

if(byteswap!=0){
size= image->rows*image->cols;
for(i= 0,ucp= (unsigned char*)tmpushort;i<size;i++,ucp+= 2){
uc= *ucp;
*ucp= *(ucp+1);
*(ucp+1)= uc;
}
}
}

/* :51 */
;
/* 52: */

{
int size;
register int i;
register float*fp;
register unsigned short*usp;

size= image->rows*image->cols*sizeof(float);

if((tmpdat= (float*)malloc((unsigned)size))==(float*)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

size/= sizeof(float);
for(i= 0,fp= tmpdat,usp= tmpushort;i<size;i++,fp++,usp++)
*fp= (float)*usp;
}

/* :52 */
;
}

/* :48 */
;
}

/* :46 */
;break;
case FLT:/* 37: */

{
/* 38: */

{
struct flt_hdr{
long rows;
long space1[31];
long cols;
long space2[95];
}*flt_hdr;

if((image->hdr= (unsigned char*)
malloc((unsigned)sizeof(struct flt_hdr)))==(unsigned char*)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

flt_hdr= (struct flt_hdr*)image->hdr;

if(read(fd,image->hdr,sizeof(struct flt_hdr))!=sizeof(struct flt_hdr))
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;

image->rows= flt_hdr->rows;
image->cols= flt_hdr->cols;
image->images= 1;
}

/* :38 */
;
/* 39: */

{
int size;

size= image->rows*image->cols*sizeof(float);

if((tmpdat= (float*)malloc(size))==(float*)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

if(read(fd,tmpdat,size)!=size){
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;
}
}

/* :39 */
;
}

/* :37 */
;break;
case STR:/* 53: */

{
fprintf(stderr,"Stream files not yet supported...sorry\n");
exit(-1);
}

/* :53 */
;break;
case BRUK:/* 54: */

{
int offset;

/* 55: */

{
/* 56: */

{
/* 57: */

{
char*cp,*cp1;

if(dirname!=(char*)0)free(dirname);
if((dirname= malloc((unsigned)strlen(basename)))==(char*)0){
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;
}

if(mainname!=(char*)0)free(mainname);
if((mainname= malloc((unsigned)strlen(basename)))==(char*)0){
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;
}

if(paramname!=(char*)0)free(paramname);
if((paramname= malloc((unsigned)strlen(basename)+strlen(extension)
+3))==(char*)0){
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;
}

if((cp= strrchr(basename,'/'))==(char*)0){
dirname[0]= '\0';
strcpy(mainname,basename);
}else{
strncpy(dirname,basename,cp-basename+1);
dirname[cp-basename+1]= '\0';
strcpy(mainname,++cp);
}
sprintf(paramname,"%sp_%s.%s",dirname,mainname,rawext);
}

/* :57 */
;
/* 58: */

{
#define MINUS_ONE_SIZE 768
#define BRUK_HDR_SIZE 3 * 256 * 16
#define THREE_TO_FOUR(x) ((((*((x)+2)) & 0x80) == 0x80) ? \
     *(x) | ((*((x)+1)) << 8) | \
     ((*((x)+2)) << 16) | 0xff000000 \
        : \
     *(x) | ((*((x)+1)) << 8) | \
     ((*((x)+2)) << 16))

int fd1,errval;

if((image->hdr= (unsigned char*)malloc((unsigned)
BRUK_HDR_SIZE+MINUS_ONE_SIZE))==(unsigned char*)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

if((fd1= open(paramname,O_RDONLY,0))<0){
if(read(fd,image->hdr,MINUS_ONE_SIZE)!=MINUS_ONE_SIZE){
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;
}
offset= MINUS_ONE_SIZE+BRUK_HDR_SIZE;
}else{
errval= read(fd1,image->hdr,MINUS_ONE_SIZE);
close(fd1);
if(errval!=MINUS_ONE_SIZE){
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;
}

offset= BRUK_HDR_SIZE;
}

errval= read(fd,image->hdr+MINUS_ONE_SIZE,BRUK_HDR_SIZE);
if(errval!=BRUK_HDR_SIZE)
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;
}

/* :58 */
;
}

/* :56 */
;
/* 59: */

{
static int my_images= 0,my_rows= 0,my_cols= 0;

image->images= THREE_TO_FOUR(image->hdr+774);
image->rows= THREE_TO_FOUR(image->hdr+786);
image->cols= THREE_TO_FOUR(image->hdr+783);

if((image->images==0)||(image->rows==0)||(image->cols==0)){
if((my_images==0)||(my_rows==0)||(my_cols==0)){
printf("The Bruker claims that your file has %d images\n",
image->images);
printf("and that each is %d rows x %d columns\n",
image->rows,image->cols);
printf("That doesn't make much sense, so I'll make you tell me");
printf(" what YOU think is there!\n");
printf("How many images should be in the file?  ");
scanf("%d",&my_images);
printf("How many rows should be in the file?  ");
scanf("%d",&my_rows);
printf("How many cols should be in the file?  ");
scanf("%d",&my_cols);
printf("I hope you're right...\n");
}
image->images= my_images;
image->rows= my_rows;
image->cols= my_cols;
}
}

/* :59 */
;
}

/* :55 */
;
/* 60: */

{
if(image->images<image_number)
/* 145: */

{
fprintf(stderr,
"I can't read image %d from `%s'...it only has %d images\n",
image_number,real_name,image->images);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :145 */
;

if((image_number<0)&&(image->images==1))image_number= 0;

if(image_number<0)
/* 36: */

{
which_image= image_number;
while(which_image<0){
printf("Reading %s...which of the %d images should I read?\n",
real_name,image->images);
printf("(Remember to count from zero!)\n");
scanf("%d",&which_image);
if((which_image<0)||(which_image>=image->images)){
fprintf(stderr,"\a\a\acome on...a number between 0 and %d\n",
image->images-1);
which_image= -1;
}
}
}

/* :36 */
;

which_image= image_number;

/* 61: */

{
#define BYTES_BRUK_BLOCK 768
int distance;
int blocks_per_image;

/* POSSIBLE PROBLEM! ...didn't have -1; added, should be OK*/
blocks_per_image= 
(image->rows*image->cols*3+BYTES_BRUK_BLOCK-1)/BYTES_BRUK_BLOCK;

distance= offset+blocks_per_image*BYTES_BRUK_BLOCK*image_number;

if(lseek(fd,distance,SEEK_SET)!=distance)
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;
}

/* :61 */
;
/* 62: */

{
int size;

size= image->rows*image->cols*3;

if((tmpspace= (unsigned char*)malloc((unsigned int)size))==
(unsigned char*)0){
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;
}

if(read(fd,tmpspace,size)!=size){
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;
}

size= image->rows*image->cols*sizeof(float);

if((tmpdat= (float*)malloc(size))==(float*)NULL){
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;
}

/* 63: */

{
register int i,j;
register unsigned char*cp;
register float*fp;

i= image->rows*image->cols;

for(j= 0,cp= tmpspace,fp= tmpdat;j<i;j++,cp+= 3,fp++)
*fp= (float)THREE_TO_FOUR(cp);
}

/* :63 */
;
}

/* :62 */
;
}

/* :60 */
;
}

/* :54 */
;break;
case BLOCK_CHAR:/* 64: */

{
int byteswap;

/* 47: */

{
FILE*fp;

if((hdrname= malloc((unsigned)(strlen(basename)+5)))==(char*)0){
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;
}

sprintf(hdrname,"%s.hdr",basename);

if((fp= fopen(hdrname,"r"))==(FILE*)0){
perror(hdrname);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

fscanf(fp,"%d %d %d %d",
&image->rows,&image->cols,&image->images,&byteswap);
fclose(fp);
}

/* :47 */
;
/* 65: */

{
char*tmpchar;

/* 36: */

{
which_image= image_number;
while(which_image<0){
printf("Reading %s...which of the %d images should I read?\n",
real_name,image->images);
printf("(Remember to count from zero!)\n");
scanf("%d",&which_image);
if((which_image<0)||(which_image>=image->images)){
fprintf(stderr,"\a\a\acome on...a number between 0 and %d\n",
image->images-1);
which_image= -1;
}
}
}

/* :36 */

/* 66: */

{
int skip;

skip= which_image*image->rows*image->cols*sizeof(unsigned char);

if(lseek(fd,skip,SEEK_SET)!=skip)
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;
}

/* :66 */

/* 67: */

{
unsigned int size;

size= image->rows*image->cols*sizeof(char);

if((tmpchar= (char*)malloc(size))==(char*)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

if(read(fd,tmpchar,(int)size)!=(int)size)
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;
}

/* :67 */

/* 68: */

{
int size,i;
char*cp;
float*fp;

size= image->rows*image->cols*sizeof(float);

if((tmpdat= (float*)malloc((unsigned)size))==(float*)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

size/= sizeof(float);

for(i= 0,fp= tmpdat,cp= tmpchar;i<size;i++,fp++,cp++)
*fp= (float)*cp;

free(tmpchar);
}

/* :68 */

}

/* :65 */
;
}

/* :64 */
;break;
case BLOCK_UCHAR:/* 69: */

{
int byteswap;

/* 47: */

{
FILE*fp;

if((hdrname= malloc((unsigned)(strlen(basename)+5)))==(char*)0){
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;
}

sprintf(hdrname,"%s.hdr",basename);

if((fp= fopen(hdrname,"r"))==(FILE*)0){
perror(hdrname);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

fscanf(fp,"%d %d %d %d",
&image->rows,&image->cols,&image->images,&byteswap);
fclose(fp);
}

/* :47 */
;
/* 70: */

{
unsigned char*tmpuchar;

/* 36: */

{
which_image= image_number;
while(which_image<0){
printf("Reading %s...which of the %d images should I read?\n",
real_name,image->images);
printf("(Remember to count from zero!)\n");
scanf("%d",&which_image);
if((which_image<0)||(which_image>=image->images)){
fprintf(stderr,"\a\a\acome on...a number between 0 and %d\n",
image->images-1);
which_image= -1;
}
}
}

/* :36 */

/* 71: */

{
int skip;

skip= which_image*image->rows*image->cols*sizeof(unsigned char);

if(lseek(fd,skip,SEEK_SET)!=skip)
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;
}

/* :71 */

/* 72: */

{
unsigned int size;

size= image->rows*image->cols*sizeof(unsigned char);

if((tmpuchar= (unsigned char*)malloc(size))==(unsigned char*)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

if(read(fd,tmpuchar,(int)size)!=(int)size)
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;
}

/* :72 */

/* 73: */

{
int size,i;
unsigned char*ucp;
float*fp;

size= image->rows*image->cols*sizeof(float);

if((tmpdat= (float*)malloc((unsigned)size))==(float*)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

size/= sizeof(float);

for(i= 0,fp= tmpdat,ucp= tmpuchar;i<size;i++,fp++,ucp++)
*fp= (float)*ucp;

free(tmpuchar);
}

/* :73 */

}

/* :70 */
;
}

/* :69 */
;break;
case BLOCK_SHORT:/* 74: */

{
int byteswap;

/* 47: */

{
FILE*fp;

if((hdrname= malloc((unsigned)(strlen(basename)+5)))==(char*)0){
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;
}

sprintf(hdrname,"%s.hdr",basename);

if((fp= fopen(hdrname,"r"))==(FILE*)0){
perror(hdrname);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

fscanf(fp,"%d %d %d %d",
&image->rows,&image->cols,&image->images,&byteswap);
fclose(fp);
}

/* :47 */
;
/* 75: */

{
short*tmpshort;

/* 36: */

{
which_image= image_number;
while(which_image<0){
printf("Reading %s...which of the %d images should I read?\n",
real_name,image->images);
printf("(Remember to count from zero!)\n");
scanf("%d",&which_image);
if((which_image<0)||(which_image>=image->images)){
fprintf(stderr,"\a\a\acome on...a number between 0 and %d\n",
image->images-1);
which_image= -1;
}
}
}

/* :36 */

/* 76: */

{
int skip;

skip= which_image*image->rows*image->cols*sizeof(short);

if(lseek(fd,skip,SEEK_SET)!=skip)
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;
}

/* :76 */

/* 77: */

{
unsigned int size;

size= image->rows*image->cols*sizeof(short);

if((tmpshort= (short*)malloc(size))==(short*)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

if(read(fd,tmpshort,(int)size)!=(int)size)
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;
}

/* :77 */

/* 78: */

{
register int i,size;
register unsigned char*ucp,uc;

if(byteswap!=0){
size= image->rows*image->cols;
for(i= 0,ucp= (unsigned char*)tmpshort;i<size;i++,ucp+= 2){
uc= *ucp;
*ucp= *(ucp+1);
*(ucp+1)= uc;
}
}
}

/* :78 */

/* 79: */

{
int size,i;
short*sp;
float*fp;

size= image->rows*image->cols*sizeof(float);

if((tmpdat= (float*)malloc((unsigned)size))==(float*)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

size/= sizeof(float);

for(i= 0,fp= tmpdat,sp= tmpshort;i<size;i++,fp++,sp++)
*fp= (float)*sp;

free(tmpshort);
}

/* :79 */

}

/* :75 */
;
}

/* :74 */
;break;
case BLOCK_USHORT:/* 80: */

{
int byteswap;

/* 47: */

{
FILE*fp;

if((hdrname= malloc((unsigned)(strlen(basename)+5)))==(char*)0){
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;
}

sprintf(hdrname,"%s.hdr",basename);

if((fp= fopen(hdrname,"r"))==(FILE*)0){
perror(hdrname);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

fscanf(fp,"%d %d %d %d",
&image->rows,&image->cols,&image->images,&byteswap);
fclose(fp);
}

/* :47 */
;
/* 81: */

{
unsigned short*tmpushort;

/* 36: */

{
which_image= image_number;
while(which_image<0){
printf("Reading %s...which of the %d images should I read?\n",
real_name,image->images);
printf("(Remember to count from zero!)\n");
scanf("%d",&which_image);
if((which_image<0)||(which_image>=image->images)){
fprintf(stderr,"\a\a\acome on...a number between 0 and %d\n",
image->images-1);
which_image= -1;
}
}
}

/* :36 */

/* 82: */

{
int skip;

skip= which_image*image->rows*image->cols*sizeof(unsigned short);

if(lseek(fd,skip,SEEK_SET)!=skip)
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;
}

/* :82 */

/* 83: */

{
unsigned int size;

size= image->rows*image->cols*sizeof(unsigned short);

if((tmpushort= (unsigned short*)malloc(size))==(unsigned short*)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

if(read(fd,tmpushort,(int)size)!=(int)size)
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;
}

/* :83 */

/* 84: */

{
register int i,size;
register unsigned char*ucp,uc;

if(byteswap!=0){
size= image->rows*image->cols;
for(i= 0,ucp= (unsigned char*)tmpushort;i<size;i++,ucp+= 2){
uc= *ucp;
*ucp= *(ucp+1);
*(ucp+1)= uc;
}
}
}

/* :84 */

/* 85: */

{
int size,i;
unsigned short*usp;
float*fp;

size= image->rows*image->cols*sizeof(float);

if((tmpdat= (float*)malloc((unsigned)size))==(float*)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

size/= sizeof(float);

for(i= 0,fp= tmpdat,usp= tmpushort;i<size;i++,fp++,usp++)
*fp= (float)*usp;

free(tmpushort);
}

/* :85 */

}

/* :81 */
;
}

/* :80 */
;break;
case BLOCK_LONG:/* 86: */

{
int byteswap;

/* 47: */

{
FILE*fp;

if((hdrname= malloc((unsigned)(strlen(basename)+5)))==(char*)0){
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;
}

sprintf(hdrname,"%s.hdr",basename);

if((fp= fopen(hdrname,"r"))==(FILE*)0){
perror(hdrname);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

fscanf(fp,"%d %d %d %d",
&image->rows,&image->cols,&image->images,&byteswap);
fclose(fp);
}

/* :47 */
;
/* 87: */

{
long*tmplong;

/* 36: */

{
which_image= image_number;
while(which_image<0){
printf("Reading %s...which of the %d images should I read?\n",
real_name,image->images);
printf("(Remember to count from zero!)\n");
scanf("%d",&which_image);
if((which_image<0)||(which_image>=image->images)){
fprintf(stderr,"\a\a\acome on...a number between 0 and %d\n",
image->images-1);
which_image= -1;
}
}
}

/* :36 */

/* 88: */

{
int skip;

skip= which_image*image->rows*image->cols*sizeof(long);

if(lseek(fd,skip,SEEK_SET)!=skip)
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;
}

/* :88 */

/* 89: */

{
unsigned int size;

size= image->rows*image->cols*sizeof(long);

if((tmplong= (long*)malloc(size))==(long*)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

if(read(fd,tmplong,(int)size)!=(int)size)
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;
}

/* :89 */

/* 90: */

{
register int i,size;
register unsigned char*ucp,uc;

if(byteswap!=0){
size= image->rows*image->cols;
for(i= 0,ucp= (unsigned char*)tmpushort;i<size;i++,ucp+= 4){
uc= *ucp;
*ucp= *(ucp+3);
*(ucp+3)= uc;
uc= *(ucp+1);
*ucp= *(ucp+2);
*(ucp+2)= uc;
}
}
}

/* :90 */

/* 91: */

{
int size,i;
long*lp;
float*fp;

size= image->rows*image->cols*sizeof(float);

if((tmpdat= (float*)malloc((unsigned)size))==(float*)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

size/= sizeof(float);

for(i= 0,fp= tmpdat,lp= tmplong;i<size;i++,fp++,lp++)
*fp= (float)*lp;

free(tmplong);
}

/* :91 */

}

/* :87 */
;
}

/* :86 */
;break;
case BLOCK_ULONG:/* 92: */

{
int byteswap;

/* 47: */

{
FILE*fp;

if((hdrname= malloc((unsigned)(strlen(basename)+5)))==(char*)0){
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;
}

sprintf(hdrname,"%s.hdr",basename);

if((fp= fopen(hdrname,"r"))==(FILE*)0){
perror(hdrname);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

fscanf(fp,"%d %d %d %d",
&image->rows,&image->cols,&image->images,&byteswap);
fclose(fp);
}

/* :47 */
;
/* 93: */

{
unsigned long*tmpulong;

/* 36: */

{
which_image= image_number;
while(which_image<0){
printf("Reading %s...which of the %d images should I read?\n",
real_name,image->images);
printf("(Remember to count from zero!)\n");
scanf("%d",&which_image);
if((which_image<0)||(which_image>=image->images)){
fprintf(stderr,"\a\a\acome on...a number between 0 and %d\n",
image->images-1);
which_image= -1;
}
}
}

/* :36 */

/* 94: */

{
int skip;

skip= which_image*image->rows*image->cols*sizeof(unsigned long);

if(lseek(fd,skip,SEEK_SET)!=skip)
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;
}

/* :94 */

/* 95: */

{
unsigned int size;

size= image->rows*image->cols*sizeof(unsigned long);

if((tmpulong= (unsigned long*)malloc(size))==(unsigned long*)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

if(read(fd,tmpulong,(int)size)!=(int)size)
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;
}

/* :95 */

/* 96: */

{
register int i,size;
register unsigned char*ucp,uc;

if(byteswap!=0){
size= image->rows*image->cols;
for(i= 0,ucp= (unsigned char*)tmpushort;i<size;i++,ucp+= 4){
uc= *ucp;
*ucp= *(ucp+3);
*(ucp+3)= uc;
uc= *(ucp+1);
*ucp= *(ucp+2);
*(ucp+2)= uc;
}
}
}

/* :96 */

/* 97: */

{
int size,i;
unsigned long*ulp;
float*fp;

size= image->rows*image->cols*sizeof(float);

if((tmpdat= (float*)malloc((unsigned)size))==(float*)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

size/= sizeof(float);

for(i= 0,fp= tmpdat,ulp= tmpulong;i<size;i++,fp++,ulp++)
*fp= (float)*ulp;

free(tmpulong);
}

/* :97 */

}

/* :93 */
;
}

/* :92 */
;break;
case BLOCK_FLOAT:/* 98: */

{
int byteswap;

/* 47: */

{
FILE*fp;

if((hdrname= malloc((unsigned)(strlen(basename)+5)))==(char*)0){
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;
}

sprintf(hdrname,"%s.hdr",basename);

if((fp= fopen(hdrname,"r"))==(FILE*)0){
perror(hdrname);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

fscanf(fp,"%d %d %d %d",
&image->rows,&image->cols,&image->images,&byteswap);
fclose(fp);
}

/* :47 */
;
/* 99: */

{

/* 36: */

{
which_image= image_number;
while(which_image<0){
printf("Reading %s...which of the %d images should I read?\n",
real_name,image->images);
printf("(Remember to count from zero!)\n");
scanf("%d",&which_image);
if((which_image<0)||(which_image>=image->images)){
fprintf(stderr,"\a\a\acome on...a number between 0 and %d\n",
image->images-1);
which_image= -1;
}
}
}

/* :36 */

/* 100: */

{
int skip;

skip= which_image*image->rows*image->cols*sizeof(float);

if(lseek(fd,skip,SEEK_SET)!=skip)
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;
}

/* :100 */

/* 101: */

{
unsigned int size;

size= image->rows*image->cols*sizeof(float);

if((tmpdat= (float*)malloc(size))==(float*)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

if(read(fd,tmpdat,(int)size)!=(int)size)
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;
}

/* :101 */

/* 102: */

{
if(byteswap!=0){
fprintf(stderr,"I don't know how to byteswap floats yet\n");
fprintf(stderr,"Go ask mikeV for help.\n");
fprintf(stderr,"He'll need the name of the file.\n");
return(BAD_FILE);
}
}

/* :102 */

}

/* :99 */
;
}

/* :98 */
;break;
case IMG:/* 103: */

{
int i;
unsigned int size;
short*tmpshort;
short*sp;
float*fp;

image->rows= 256;
image->cols= 256;
image->images= 1;
size= 256*256*sizeof(short);

if((tmpshort= (short*)malloc(image->rows*image->cols*sizeof(short)))
==(short*)0)/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

if((tmpdat= (float*)malloc(image->rows*image->cols*sizeof(float)))
==(float*)0)/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

if(read(fd,tmpshort,(int)size)!=(int)size)
/* 140: */

{
perror(real_name);
/* 141: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_FILE);
}

/* :141 */
;
}

/* :140 */
;

size= image->rows*image->cols;

for(i= 0,fp= tmpdat,sp= tmpshort;i<size;i++,fp++,sp++)
*fp= (float)*sp;

free(tmpshort);
}

/* :103 */
;break;
case GENESIS:/* 104: */

{
/* hack for now...just read the analyze header.  should read rawhdr */
struct analyze_header_key{
int sizeof_hdr;
char data_type[10];
char db_name[18];
int extents;
short session_error;
char regular;
char hkey_un0;
}analyze_header_key;
struct analyze_image_dimension{
short dim[8];
short unuxed[7];
short datatype;
short bitpix;
short dim_un0;
float pixdim[8];
float funused[6];
float compressed;
float verified;
int glmax,glmin;
}analyze_image_dimension;

/* 105: */

{
int f;

if((hdrname= malloc((unsigned)(strlen(basename)+5)))==(char*)0){
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;
}

sprintf(hdrname,"%s.hdr",basename);
if((f= open(hdrname,O_RDONLY,0))<0){
/* 138: */

{
perror(real_name);
/* 139: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(NO_SUCH_FILE);
}

/* :139 */
;
}

/* :138 */

}

if(read(f,&analyze_header_key,sizeof(struct analyze_header_key))!=
sizeof(struct analyze_header_key)){
/* 138: */

{
perror(real_name);
/* 139: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(NO_SUCH_FILE);
}

/* :139 */
;
}

/* :138 */

}
if(read(f,&analyze_image_dimension,
sizeof(struct analyze_image_dimension))!=
sizeof(struct analyze_image_dimension)){
/* 138: */

{
perror(real_name);
/* 139: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(NO_SUCH_FILE);
}

/* :139 */
;
}

/* :138 */

}
/* should deal with multiple images? */
image->rows= analyze_image_dimension.dim[1];
image->cols= analyze_image_dimension.dim[2];

close(f);
}

/* :105 */

/* 106: */

{
short*sp,*sploc;
float*fp;
int size,i;

size= image->rows*image->cols*sizeof(short);
if((sp= (short*)malloc(size))==(short*)0)/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

if(read(fd,sp,size)!=size){
/* 138: */

{
perror(real_name);
/* 139: */

{
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(NO_SUCH_FILE);
}

/* :139 */
;
}

/* :138 */

}

size= image->rows*image->cols*sizeof(float);
if((tmpdat= (float*)malloc(size))==(float*)0)/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

size= image->rows*image->cols;
for(i= 0,fp= tmpdat,sploc= sp;i<size;i++,fp++,sp1++)
*fp= (float)*sploc;

free(sp);
}


/* :106 */

}
/* :104 */
;break;
case SISCO:/* 107: */

{
}

/* :107 */
;break;

default:/* 143: */

{
fprintf(stderr,"``This can't happen...get help!''\n");
fprintf(stderr,"(Tell a programmer that: `read_image hit an ");
fprintf(stderr,"undefined case in `try to read file')\n");
exit(-1);
}

/* :143 */
;

}
}

/* :21 */
;
/* 108: */

{
image->union_type= type;

switch(type){
case T_CHAR:/* 110: */

{
register char*cp;
register float*fp;
register int i,size;
register float bottom,scale;

/* 109: */

{
register int i;
register float max,min,*fp;

max= min= *tmpdat;
for(i= 0,fp= tmpdat;i<image->rows*image->cols;i++,fp++){
if(*fp>max)max= *fp;
if(*fp<min)min= *fp;
}

image->max= max;
image->min= min;
}

/* :109 */
;
bottom= image->min;
scale= 256./(image->max-image->min);

size= image->rows*image->cols*sizeof(char);

if((image->image.c= (char*)malloc((unsigned)size))==(char*)0){
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;
}

size= image->rows*image->cols;

/* 111: */

{
if(rescale_ok!=0){
if((image->max>127.)||(image->min<-128.))
/* 146: */

{
fprintf(stderr,"Scaling problem...`%s' won't fit into your type,");
fprintf(stderr," and you specified no scaling\n");
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_SCALE);
}

/* :146 */
;

for(i= 0,cp= image->image.c,fp= tmpdat;i<size;i++,cp++,fp++)
*cp= *fp;
}else{
image->max= image->min= (*tmpdat-bottom)*scale+.5;

for(i= 0,cp= image->image.c,fp= tmpdat;i<size;i++,cp++,fp++){
*cp= (*fp-bottom)*scale+.5;
if(*cp>image->max)image->max= *cp;
if(*cp<image->min)image->min= *cp;
}
}
}

/* :111 */
;
/* 112: */

{
int i;

if((image->row_ptr.c= (char**)malloc((unsigned)
image->rows*sizeof(char*)))==(char**)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

for(i= 0;i<image->rows;i++)
*(image->row_ptr.c+i)= (image->image.c+i*image->cols);
}

/* :112 */
;
}

/* :110 */
;break;
case T_SHORT:/* 113: */

{
register short*sp;
register float*fp;
register int i,size;
register float bottom,scale;

/* 109: */

{
register int i;
register float max,min,*fp;

max= min= *tmpdat;
for(i= 0,fp= tmpdat;i<image->rows*image->cols;i++,fp++){
if(*fp>max)max= *fp;
if(*fp<min)min= *fp;
}

image->max= max;
image->min= min;
}

/* :109 */
;
bottom= image->min;
scale= 32767./(image->max-image->min);

size= image->rows*image->cols*sizeof(short);

if((image->image.s= (short*)malloc((unsigned)size))==(short*)0){
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;
}

size= image->rows*image->cols;

/* 114: */

{
if(rescale_ok!=0){
if((image->max>32767.)||(image->min<-32768.))
/* 146: */

{
fprintf(stderr,"Scaling problem...`%s' won't fit into your type,");
fprintf(stderr," and you specified no scaling\n");
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_SCALE);
}

/* :146 */
;

for(i= 0,sp= image->image.s,fp= tmpdat;i<size;i++,sp++,fp++)
*sp= *fp;
}else{
image->min= image->max= (*tmpdat-bottom)*scale+.5;

for(i= 0,sp= image->image.s,fp= tmpdat;i<size;i++,sp++,fp++){
*sp= (*fp-bottom)*scale+.5;
if(*sp<image->max)image->max= *sp;
if(*sp>image->min)image->min= *sp;
}
}
}

/* :114 */
;
/* 115: */

{
int i;

if((image->row_ptr.s= (short**)malloc((unsigned)
image->rows*sizeof(short*)))==(short**)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

for(i= 0;i<image->rows;i++)
*(image->row_ptr.s+i)= (image->image.s+i*image->cols);
}

/* :115 */
;
}

/* :113 */
;break;
case T_LONG:/* 116: */

{
register long*lp;
register float*fp;
register int i,size;
register double bottom,scale;

/* 109: */

{
register int i;
register float max,min,*fp;

max= min= *tmpdat;
for(i= 0,fp= tmpdat;i<image->rows*image->cols;i++,fp++){
if(*fp>max)max= *fp;
if(*fp<min)min= *fp;
}

image->max= max;
image->min= min;
}

/* :109 */
;
bottom= image->min;
scale= 2147473648./(image->max-image->min);

size= image->rows*image->cols*sizeof(long);

if((image->image.l= (long*)malloc((unsigned)size))==(long*)0){
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;
}

size= image->rows*image->cols;

/* 117: */

{
if(rescale_ok!=0){
if((image->max>2147483647.)||(image->min<-2147483648.))
/* 146: */

{
fprintf(stderr,"Scaling problem...`%s' won't fit into your type,");
fprintf(stderr," and you specified no scaling\n");
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_SCALE);
}

/* :146 */
;

for(i= 0,lp= image->image.l,fp= tmpdat;i<size;i++,lp++,fp++)
*lp= *fp;
}else{
image->max= image->min= (*tmpdat-bottom)*scale+.5;

for(i= 0,lp= image->image.l,fp= tmpdat;i<size;i++,lp++,fp++){
*lp= (*fp-bottom)*scale+.5;
if(*lp>image->max)image->max= *lp;
if(*lp<image->min)image->min= *lp;
}
}
}

/* :117 */
;
/* 118: */

{
int i;

if((image->row_ptr.l= (long**)malloc((unsigned)
image->rows*sizeof(long*)))==(long**)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

for(i= 0;i<image->rows;i++)
*(image->row_ptr.l+i)= (image->image.l+i*image->cols);
}

/* :118 */
;
}

/* :116 */
;break;
case T_FLOAT:/* 119: */

{
register int i,size;

/* 109: */

{
register int i;
register float max,min,*fp;

max= min= *tmpdat;
for(i= 0,fp= tmpdat;i<image->rows*image->cols;i++,fp++){
if(*fp>max)max= *fp;
if(*fp<min)min= *fp;
}

image->max= max;
image->min= min;
}

/* :109 */
;
image->image.f= tmpdat;
tmpdat= (float*)0;

/* 120: */

{
int i;

if((image->row_ptr.f= (float**)malloc((unsigned)
image->rows*sizeof(float*)))==(float**)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

for(i= 0;i<image->rows;i++)
*(image->row_ptr.f+i)= (image->image.f+i*image->cols);
}

/* :120 */
;
}

/* :119 */
;break;
case T_DOUBLE:/* 121: */

{
register double*dp;
register float*fp;
register int i,size;
register double bottom,scale;

/* 109: */

{
register int i;
register float max,min,*fp;

max= min= *tmpdat;
for(i= 0,fp= tmpdat;i<image->rows*image->cols;i++,fp++){
if(*fp>max)max= *fp;
if(*fp<min)min= *fp;
}

image->max= max;
image->min= min;
}

/* :109 */
;
bottom= image->min;
scale= 1./(image->max-image->min);

size= image->rows*image->cols*sizeof(double);

if((image->image.d= (double*)malloc((unsigned)size))==(double*)0){
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;
}

size= image->rows*image->cols;

for(i= 0,dp= image->image.d,fp= tmpdat;i<size;i++,dp++,fp++)
*dp= *fp;

/* 122: */

{
int i;

if((image->row_ptr.d= (double**)malloc((unsigned)
image->rows*sizeof(double*)))==(double**)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

for(i= 0;i<image->rows;i++)
*(image->row_ptr.d+i)= (image->image.d+i*image->cols);
}

/* :122 */
;
}

/* :121 */
;break;
case T_COMPLEX:/* 123: */

{
register struct complex*cop;
register float*fp;
register int i,size;
register double bottom,scale;

/* 109: */

{
register int i;
register float max,min,*fp;

max= min= *tmpdat;
for(i= 0,fp= tmpdat;i<image->rows*image->cols;i++,fp++){
if(*fp>max)max= *fp;
if(*fp<min)min= *fp;
}

image->max= max;
image->min= min;
}

/* :109 */
;
bottom= image->min;
scale= 1./(image->max-image->min);

size= image->rows*image->cols*sizeof(struct complex);

if((image->image.co= (struct complex*)malloc((unsigned)size))==
(struct complex*)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

size= image->rows*image->cols;

for(i= 0,cop= image->image.co,fp= tmpdat;i<size;i++,cop++,fp++){
cop->r= *fp;
cop->i= 0.;
}

/* 124: */

{
int i;

if((image->row_ptr.co = 
    malloc((unsigned) image->rows*sizeof(struct complex*))) == 
    (struct complex**)0 )
{
  perror("couldn't allocate memory in read_image");
  fprintf(stderr,"That is serious enough that I'm aborting\n");
  exit(-1);
}

/* :144 */
;

for(i= 0;i<image->rows;i++)
*(image->row_ptr.co+i)= (image->image.co+i*image->cols);
}

/* :124 */
;
}

/* :123 */
;break;
case T_UCHAR:/* 125: */

{
register unsigned char*ucp;
register float*fp;
register int i,size;
register float bottom,scale;

/* 109: */

{
register int i;
register float max,min,*fp;

max= min= *tmpdat;
for(i= 0,fp= tmpdat;i<image->rows*image->cols;i++,fp++){
if(*fp>max)max= *fp;
if(*fp<min)min= *fp;
}

image->max= max;
image->min= min;
}

/* :109 */
;
bottom= image->min;
scale= 256./(image->max-image->min);

size= image->rows*image->cols*sizeof(unsigned char);

if((image->image.uc= (unsigned char*)malloc((unsigned)size))==
(unsigned char*)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

size= image->rows*image->cols;

/* 126: */

{
if(rescale_ok!=0){
if((image->max>255.)||(image->min<0.)){
/* 146: */

{
fprintf(stderr,"Scaling problem...`%s' won't fit into your type,");
fprintf(stderr," and you specified no scaling\n");
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_SCALE);
}

/* :146 */
;
}

for(i= 0,ucp= image->image.uc,fp= tmpdat;i<size;i++,ucp++,fp++)
*ucp= *fp;
}else{
image->max= image->min= (*tmpdat-bottom)*scale+.5;
for(i= 0,ucp= image->image.uc,fp= tmpdat;i<size;i++,ucp++,fp++){
*ucp= (*fp-bottom)*scale+.5;
if((float)*ucp>image->max)image->max= (float)*ucp;
if((float)*ucp<image->min)image->min= (float)*ucp;
}
}
}

/* :126 */
;
/* 127: */

{
int i;

if((image->row_ptr.uc= (unsigned char**)malloc((unsigned)
image->rows*sizeof(unsigned char*)))==(unsigned char**)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

for(i= 0;i<image->rows;i++)
*(image->row_ptr.uc+i)= (image->image.uc+i*image->cols);
}

/* :127 */
;
}

/* :125 */
;break;
case T_USHORT:/* 128: */

{
register unsigned short*usp;
register float*fp;
register int i,size;
register float bottom,scale;

/* 109: */

{
register int i;
register float max,min,*fp;

max= min= *tmpdat;
for(i= 0,fp= tmpdat;i<image->rows*image->cols;i++,fp++){
if(*fp>max)max= *fp;
if(*fp<min)min= *fp;
}

image->max= max;
image->min= min;
}

/* :109 */
;
bottom= image->min;
scale= 65535./(image->max-image->min);

size= image->rows*image->cols*sizeof(unsigned short);

if((image->image.us= (unsigned short*)malloc((unsigned)size))==
(unsigned short*)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

size= image->rows*image->cols;

/* 129: */

{
if(rescale_ok!=0){
if((image->max>65535.)||(image->min<0.)){
/* 146: */

{
fprintf(stderr,"Scaling problem...`%s' won't fit into your type,");
fprintf(stderr," and you specified no scaling\n");
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_SCALE);
}

/* :146 */
;
}

for(i= 0,usp= image->image.us,fp= tmpdat;i<size;i++,usp++,fp++)
*usp= *fp;
}else{
image->max= image->min= (*tmpdat-bottom)*scale+.5;
for(i= 0,usp= image->image.us,fp= tmpdat;i<size;i++,usp++,fp++){
*usp= (*fp-bottom)*scale+.5;
if((float)*usp>image->max)image->max= (float)*usp;
if((float)*usp<image->min)image->min= (float)*usp;
}
}
}

/* :129 */
;
/* 130: */

{
int i;

if((image->row_ptr.us= (unsigned short**)malloc((unsigned)
image->rows*sizeof(unsigned short*)))==(unsigned short**)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

for(i= 0;i<image->rows;i++)
*(image->row_ptr.us+i)= (image->image.us+i*image->cols);
}

/* :130 */
;
}

/* :128 */
;break;
case T_ULONG:/* 131: */

{
register unsigned long*ulp;
register float*fp;
register int i,size;
register double bottom,scale;

/* 109: */

{
register int i;
register float max,min,*fp;

max= min= *tmpdat;
for(i= 0,fp= tmpdat;i<image->rows*image->cols;i++,fp++){
if(*fp>max)max= *fp;
if(*fp<min)min= *fp;
}

image->max= max;
image->min= min;
}

/* :109 */
;
bottom= image->min;
scale= 4294967295./(image->max-image->min);

size= image->rows*image->cols*sizeof(unsigned short);

if((image->image.ul= (unsigned long*)malloc((unsigned)size))==
(unsigned long*)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

size= image->rows*image->cols;

/* 132: */

{
if(rescale_ok!=0){
if((image->max>4294967295.)||(image->min<0.)){
/* 146: */

{
fprintf(stderr,"Scaling problem...`%s' won't fit into your type,");
fprintf(stderr," and you specified no scaling\n");
/* 136: */

{
/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;

if(image->hdr!=(unsigned char*)0)free(image->hdr);
if(image->image.c!=(char*)0)free(image->image.c);
if(image->row_ptr.c!=(char**)0)free(image->row_ptr.c);
}

/* :136 */
;
return(BAD_SCALE);
}

/* :146 */
;
}

for(i= 0,ulp= image->image.ul,fp= tmpdat;i<size;i++,ulp++,fp++){
*ulp= (long)*fp;
}
}else{
image->max= image->min= ((*tmpdat-bottom)*scale+.5);
for(i= 0,ulp= image->image.ul,fp= tmpdat;i<size;i++,ulp++,fp++){
*ulp= (long)((*fp-bottom)*scale+.5);
if(*ulp>image->max)image->max= *ulp;
if(*ulp<image->min)image->min= *ulp;
}
}
}

/* :132 */
;
/* 133: */

{
int i;

if((image->row_ptr.ul= (unsigned long**)malloc((unsigned)
image->rows*sizeof(unsigned long*)))==(unsigned long**)0)
/* 144: */

{
perror("couldn't allocate memory in read_image");
fprintf(stderr,"That is serious enough that I'm aborting\n");
exit(-1);
}

/* :144 */
;

for(i= 0;i<image->rows;i++)
*(image->row_ptr.ul+i)= (image->image.ul+i*image->cols);
}

/* :133 */
;
}

/* :131 */
;break;
default:fprintf(stderr,"Bad `type' in read_image...\n");break;
}
}

/* :108 */
;

/* 135: */

{
if(real_name!=(char*)0){
free(real_name);
real_name= (char*)0;
}
if(basename!=(char*)0){
free(basename);
basename= (char*)0;
}
if(extension!=(char*)0){
free(extension);
extension= (char*)0;
}
if(rawext!=(char*)0){
free(rawext);
rawext= (char*)0;
}
if(tmpdat!=(float*)0){
free(tmpdat);
tmpdat= (float*)0;
}
if(sp1!=(short*)0){
free(sp1);
sp1= (short*)0;
}
if(tmpushort!=(unsigned short*)0){
free(tmpushort);
tmpushort= (unsigned short*)0;
}
if(hdrname!=(char*)0){
free(hdrname);
hdrname= (char*)0;
}
if(fptmp!=(float*)0){
free(fptmp);
fptmp= (float*)0;
}
if(dirname!=(char*)0){
free(dirname);
dirname= (char*)0;
}
if(mainname!=(char*)0){
free(mainname);
mainname= (char*)0;
}
if(paramname!=(char*)0){
free(paramname);
paramname= (char*)0;
}
if(tmpspace!=(unsigned char*)0){
free(tmpspace);
tmpspace= (unsigned char*)0;
}

if(fd>=0)close(fd);
}

/* :135 */
;
return(which_image);
}

/* :8 */
