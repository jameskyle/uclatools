#if(0)
  FTANGLE v1.23a, created with SunOS/UNIX on "Tuesday, April 14, 1992 at 13:46." 
  COMMAND LINE: "ftangle read_otl.web"
  RUN TIME: "Tuesday, October 12, 1993 at 18:30."
  WEB FILE:    "read_otl.web"
  CHANGE FILE: (none)
#endif
/* 2: */
#line 120 "read_otl.web"
#include "read_otl.h"

/* :2 *//* 3: */
#line 165 "read_otl.web"

int summarize_otl(fname,regions)
char*fname;
char**regions;
{
/* 4: */
#line 225 "read_otl.web"

int fd= -1;
int hdr_len,hdr_len_read;
int hdr_type;
int number_points= -1;
int regions_used= 0,regions_avail= 0,region_number= 0;
int points_in_region;
int rows_big,cols_big;
char*raster_internal;
union hdr hdr;
struct pt seed;
struct pt*pts= (struct pt*)0;
char label[MYLABLENGTH];
struct pt*list,*list_new;

/* :4 */
#line 170 "read_otl.web"

/* 6: */
#line 253 "read_otl.web"

{
int errval;
if((fd= open(fname,O_RDONLY,0))<0)
/* 32: */
#line 672 "read_otl.web"

{
perror(fname);
if(fd>=0)close(fd);
return(-1);
}

/* :32 */
#line 257 "read_otl.web"


if((errval= read(fd,(char*)(&hdr),sizeof(struct outl_data_old)))!=
sizeof(struct outl_data_old))
/* 32: */
#line 672 "read_otl.web"

{
perror(fname);
if(fd>=0)close(fd);
return(-1);
}

/* :32 */
#line 261 "read_otl.web"


if(hdr.new.version<0){
hdr_len= sizeof(struct outl_data_new);
hdr_type= NEW_HDR;
}else{
hdr_len= sizeof(struct outl_data_old);
hdr_type= OLD_HDR;
}

if(lseek(fd,0,SEEK_SET)!=(off_t)0)
/* 32: */
#line 672 "read_otl.web"

{
perror(fname);
if(fd>=0)close(fd);
return(-1);
}

/* :32 */
#line 272 "read_otl.web"

}

/* :6 */
#line 171 "read_otl.web"

/* 9: */
#line 329 "read_otl.web"

{
for(;;)/* 10: */
#line 339 "read_otl.web"

{
/* 11: */
#line 355 "read_otl.web"

{
if((hdr_len_read= read(fd,&hdr,hdr_len))!=hdr_len)
/* 13: */
#line 391 "read_otl.web"

{
*((*regions)+regions_used)= '\0';
if(fd>=0)close(fd);
return(region_number);
}

/* :13 */
#line 358 "read_otl.web"


if(hdr_type==OLD_HDR)/* 15: */
#line 415 "read_otl.web"

{
points_in_region= hdr.old.num;
seed.x= hdr.old.seed.x;
seed.y= hdr.old.seed.y;
strncpy(label,hdr.old.label,LABLENGTH);
label[LABLENGTH]= '\0';
}

/* :15 */
#line 360 "read_otl.web"

else/* 16: */
#line 430 "read_otl.web"

{
points_in_region= hdr.new.num;
seed.x= hdr.new.seed.x;
seed.y= hdr.new.seed.y;
strncpy(label,hdr.new.label,LABLENGTH);
label[LABLENGTH]= '\0';
rows_big= hdr.new.rows;
cols_big= hdr.new.cols;
}

/* :16 */
#line 361 "read_otl.web"


region_number++;
}

/* :11 */
#line 341 "read_otl.web"

/* 17: */
#line 444 "read_otl.web"

{
/* 19: */
#line 460 "read_otl.web"

{
if(regions_avail<=0){
regions_avail= 128*MYLABLENGTH;
if(((*regions)= malloc((unsigned int)regions_avail))==
(char*)0)/* 31: */
#line 663 "read_otl.web"

{
perror(fname);
exit(-1);
}

/* :31 */
#line 465 "read_otl.web"

}else if(regions_used>=regions_avail){
regions_avail+= 128*MYLABLENGTH;
if(((*regions)= realloc(regions,(unsigned int)regions_avail))==
(char*)0)/* 31: */
#line 663 "read_otl.web"

{
perror(fname);
exit(-1);
}

/* :31 */
#line 469 "read_otl.web"

}
}

/* :19 */
#line 446 "read_otl.web"

memcpy((*regions)+regions_used,label,LABLENGTH);
*((*regions)+regions_used+LABLENGTH)= '\0';
regions_used+= MYLABLENGTH;
}

/* :17 */
#line 342 "read_otl.web"

/* 18: */
#line 453 "read_otl.web"

{
if(lseek(fd,points_in_region*sizeof(struct pt),SEEK_CUR)<0)
/* 32: */
#line 672 "read_otl.web"

{
perror(fname);
if(fd>=0)close(fd);
return(-1);
}

/* :32 */
#line 456 "read_otl.web"

}

/* :18 */
#line 343 "read_otl.web"

}

/* :10 */
#line 331 "read_otl.web"

}

/* :9 */
#line 172 "read_otl.web"

}

int read_otl(fname,region,nrows,ncols,raster,factor)
char*fname;
char*region;
int nrows;
int ncols;
unsigned char**raster;
int*factor;
{
/* 4: */
#line 225 "read_otl.web"

int fd= -1;
int hdr_len,hdr_len_read;
int hdr_type;
int number_points= -1;
int regions_used= 0,regions_avail= 0,region_number= 0;
int points_in_region;
int rows_big,cols_big;
char*raster_internal;
union hdr hdr;
struct pt seed;
struct pt*pts= (struct pt*)0;
char label[MYLABLENGTH];
struct pt*list,*list_new;

/* :4 */
#line 183 "read_otl.web"

/* 6: */
#line 253 "read_otl.web"

{
int errval;
if((fd= open(fname,O_RDONLY,0))<0)
/* 32: */
#line 672 "read_otl.web"

{
perror(fname);
if(fd>=0)close(fd);
return(-1);
}

/* :32 */
#line 257 "read_otl.web"


if((errval= read(fd,(char*)(&hdr),sizeof(struct outl_data_old)))!=
sizeof(struct outl_data_old))
/* 32: */
#line 672 "read_otl.web"

{
perror(fname);
if(fd>=0)close(fd);
return(-1);
}

/* :32 */
#line 261 "read_otl.web"


if(hdr.new.version<0){
hdr_len= sizeof(struct outl_data_new);
hdr_type= NEW_HDR;
}else{
hdr_len= sizeof(struct outl_data_old);
hdr_type= OLD_HDR;
}

if(lseek(fd,0,SEEK_SET)!=(off_t)0)
/* 32: */
#line 672 "read_otl.web"

{
perror(fname);
if(fd>=0)close(fd);
return(-1);
}

/* :32 */
#line 272 "read_otl.web"

}

/* :6 */
#line 184 "read_otl.web"

/* 7: */
#line 279 "read_otl.web"

{
int tmpfactor1,tmpfactor2;

if(hdr_type==OLD_HDR)/* 8: */
#line 312 "read_otl.web"

{
if((nrows>256)||(ncols>256)){
rows_big= nrows;
cols_big= ncols;
}else if((nrows>128)||(ncols>128)){
rows_big= nrows*2;
cols_big= ncols*2;
}else{
rows_big= nrows*4;
cols_big= ncols*4;
}
}

/* :8 */
#line 283 "read_otl.web"


tmpfactor1= rows_big/nrows;
tmpfactor2= cols_big/ncols;

if(tmpfactor1*nrows!=rows_big)/* 33: */
#line 680 "read_otl.web"

{
float tmp;

tmp= (float)rows_big/(float)nrows;
fprintf(stderr,"Error, you requested %d rows, which results \n",nrows);
fprintf(stderr,"in a scale factor of %f\n",tmp);
fprintf(stderr,"But I only know how to do integer scale factors > 1\n");
return(-1);
}

/* :33 */
#line 288 "read_otl.web"

if(tmpfactor2*ncols!=cols_big)/* 34: */
#line 692 "read_otl.web"

{
float tmp;

tmp= (float)cols_big/(float)ncols;
fprintf(stderr,"Error, you requested %d cols, which results \n",ncols);
fprintf(stderr,"in a scale factor of %f\n",tmp);
fprintf(stderr,"But I only know how to do integer scale factors > 1\n");
if(fd>=0)close(fd);
return(-1);
}

/* :34 */
#line 289 "read_otl.web"

if(tmpfactor1!=tmpfactor2)/* 35: */
#line 705 "read_otl.web"

{
fprintf(stderr,"Error, you requested %d rows by %d cols\n",nrows,ncols);
fprintf(stderr,"The original data were obtained as %d rows by %d cols\n",
rows_big,cols_big);
fprintf(stderr,
"This results in different scale factors for each direction\n");
fprintf(stderr,"but I don't know how to do that!\n");
if(fd>=0)close(fd);
return(-1);
}

/* :35 */
#line 290 "read_otl.web"

if(tmpfactor1>7)/* 36: */
#line 718 "read_otl.web"

{
fprintf(stderr,"Your scale factor was %d, but that's too big!\n",
tmpfactor1);
fprintf(stderr,
"I'm returning unsigned chars, but a scale factor of %d \n",
tmpfactor1);
fprintf(stderr,"is likely to produce counts as high as %d\n",
tmpfactor1*tmpfactor1);
fprintf(stderr,"(an unsigned char can't hold numbers > 255)\n");
fprintf(stderr,"I won't take a chance...sorry\n");
if(fd>=0)close(fd);
return(-1);
}

/* :36 */
#line 291 "read_otl.web"


*factor= tmpfactor1;

if((raster_internal= malloc((unsigned int)rows_big*cols_big))==
(char*)0)/* 31: */
#line 663 "read_otl.web"

{
perror(fname);
exit(-1);
}

/* :31 */
#line 296 "read_otl.web"

memset(raster_internal,0,rows_big*cols_big);
if((list= (struct pt*)malloc((unsigned int)
rows_big*cols_big*sizeof(struct pt)))==
(struct pt*)0)/* 31: */
#line 663 "read_otl.web"

{
perror(fname);
exit(-1);
}

/* :31 */
#line 300 "read_otl.web"

if((list_new= (struct pt*)malloc((unsigned int)
rows_big*cols_big*sizeof(struct pt)))==
(struct pt*)0)/* 31: */
#line 663 "read_otl.web"

{
perror(fname);
exit(-1);
}

/* :31 */
#line 303 "read_otl.web"

}

/* :7 */
#line 185 "read_otl.web"

/* 21: */
#line 505 "read_otl.web"

{
for(;;){
/* 12: */
#line 376 "read_otl.web"

{
if((hdr_len_read= read(fd,&hdr,hdr_len))!=hdr_len)
/* 14: */
#line 402 "read_otl.web"

{
/* 20: */
#line 474 "read_otl.web"

{
int r_int,c_int,r_call,c_call,rep_row,rep_col;
char*cp_int,*cp_tmp_int;
unsigned char*cp_call,*cp_tmp_call;
int locfactor= *factor;

if((*raster= (unsigned char*)malloc((unsigned int)nrows*ncols))
==(unsigned char*)0)/* 31: */
#line 663 "read_otl.web"

{
perror(fname);
exit(-1);
}

/* :31 */
#line 482 "read_otl.web"


memset(*raster,0,nrows*ncols);
cp_int= raster_internal;
cp_call= *raster;
for(r_call= 0;r_call<nrows;r_call++){
cp_tmp_call= cp_call;
for(rep_row= 0;rep_row<locfactor;rep_row++){
cp_call= cp_tmp_call;
for(c_call= 0;c_call<ncols;c_call++,cp_call++){
for(rep_col= 0;rep_col<locfactor;rep_col++,cp_int++){
if(*cp_int==1)(*cp_call)++;
}
}
}
}
}

/* :20 */
#line 404 "read_otl.web"

if(fd>=0)close(fd);
return(region_number);
}

/* :14 */
#line 379 "read_otl.web"


if(hdr_type==OLD_HDR)/* 15: */
#line 415 "read_otl.web"

{
points_in_region= hdr.old.num;
seed.x= hdr.old.seed.x;
seed.y= hdr.old.seed.y;
strncpy(label,hdr.old.label,LABLENGTH);
label[LABLENGTH]= '\0';
}

/* :15 */
#line 381 "read_otl.web"

else/* 16: */
#line 430 "read_otl.web"

{
points_in_region= hdr.new.num;
seed.x= hdr.new.seed.x;
seed.y= hdr.new.seed.y;
strncpy(label,hdr.new.label,LABLENGTH);
label[LABLENGTH]= '\0';
rows_big= hdr.new.rows;
cols_big= hdr.new.cols;
}

/* :16 */
#line 382 "read_otl.web"


region_number++;
}

/* :12 */
#line 508 "read_otl.web"

/* 22: */
#line 514 "read_otl.web"

{
if((*region=='\0')||(strcmp(region,label)==0))
/* 23: */
#line 526 "read_otl.web"

{
/* 24: */
#line 533 "read_otl.web"

{
/* 25: */
#line 542 "read_otl.web"

{
if((number_points<=0)||(pts==(struct pt*)0)){
if((pts= (struct pt*)malloc((unsigned int)
points_in_region*sizeof(struct pt)))==(struct pt*)0)
/* 31: */
#line 663 "read_otl.web"

{
perror(fname);
exit(-1);
}

/* :31 */
#line 547 "read_otl.web"

}else if(number_points<points_in_region){
if((pts= (struct pt*)realloc(pts,
points_in_region*sizeof(struct pt)))==(struct pt*)0)
/* 31: */
#line 663 "read_otl.web"

{
perror(fname);
exit(-1);
}

/* :31 */
#line 551 "read_otl.web"

}
number_points= points_in_region;
}

/* :25 */
#line 535 "read_otl.web"

/* 26: */
#line 559 "read_otl.web"

{
int size= points_in_region*sizeof(struct pt);
int i;
struct pt*pptr;

if(read(fd,pts,size)!=size)
/* 32: */
#line 672 "read_otl.web"

{
perror(fname);
if(fd>=0)close(fd);
return(-1);
}

/* :32 */
#line 566 "read_otl.web"

for(i= 0,pptr= pts;i<points_in_region;i++,pptr++)
*(raster_internal+pptr->y*cols_big+pptr->x)= 2;
}

/* :26 */
#line 536 "read_otl.web"

}

/* :24 */
#line 528 "read_otl.web"

/* 27: */
#line 572 "read_otl.web"

{
int n,iter;

list->x= seed.x;
list->y= seed.y;
*(raster_internal+seed.y*cols_big+seed.x)= 1;

n= 1;
iter= 0;
while(n>0)/* 28: */
#line 586 "read_otl.web"

{
int ic;
int m,i;
struct pt*pt1,*pt2;

iter++;
m= 0;

for(ic= 0;ic<n;ic++)/* 29: */
#line 607 "read_otl.web"

{
int i= (list+ic)->x,j= (list+ic)->y;

if((i>=cols_big)||(i<0)||(j>=rows_big)||(j<0))
/* 30: */
#line 646 "read_otl.web"

{
fprintf(stderr,"`This can't happen!'\n");
fprintf(stderr,"I hit an impossible location filling region %s\n",
label);
fprintf(stderr,"It was row %d, col %d; the raster is only %d x %d\n",
j,i,rows_big,cols_big);
fprintf(stderr,
"Go yell at mikeV (But tell him exactly what this message said.)\n");
exit(-1);
}

/* :30 */
#line 612 "read_otl.web"


if(*(raster_internal+i+1+(j*cols_big))==0){
(list_new+m)->x= i+1;
(list_new+m)->y= j;
m++;
*(raster_internal+i+1+(j*cols_big))= 1;
}

if(*(raster_internal+i-1+(j*cols_big))==0){
(list_new+m)->x= i-1;
(list_new+m)->y= j;
m++;
*(raster_internal+i-1+(j*cols_big))= 1;
}

if(*(raster_internal+i+((j+1)*cols_big))==0){
(list_new+m)->x= i;
(list_new+m)->y= j+1;
m++;
*(raster_internal+i+((j+1)*cols_big))= 1;
}

if(*(raster_internal+i+((j-1)*cols_big))==0){
(list_new+m)->x= i;
(list_new+m)->y= j-1;
m++;
*(raster_internal+i+((j-1)*cols_big))= 1;
}
}

/* :29 */
#line 595 "read_otl.web"


if(m>0){
for(i= 0,pt1= list,pt2= list_new;i<m;i++,pt1++,pt2++){
pt1->x= pt2->x;
pt1->y= pt2->y;
}
}
n= m;
}

/* :28 */
#line 582 "read_otl.web"

}

/* :27 */
#line 529 "read_otl.web"

}

/* :23 */
#line 517 "read_otl.web"

else
/* 18: */
#line 453 "read_otl.web"

{
if(lseek(fd,points_in_region*sizeof(struct pt),SEEK_CUR)<0)
/* 32: */
#line 672 "read_otl.web"

{
perror(fname);
if(fd>=0)close(fd);
return(-1);
}

/* :32 */
#line 456 "read_otl.web"

}

/* :18 */
#line 519 "read_otl.web"

}

/* :22 */
#line 509 "read_otl.web"

}
}

/* :21 */
#line 186 "read_otl.web"


return(0);
}

/* :3 */
