/* <html><head><title>gen2bshort source</title></head><body>
<h2>How to make gen2bshort</h2>
<ol>
<li>Build ImgLib.o analyzeUtil.o ImgLibError.o VLib.o (from
<a
href="http://airto.loni.ucla.edu/BMCweb/SharedCode/SharedSoftware.html#Anchor-UCLA-35829">www.brainmapping.org</a>)
<li>compile gen2bshort, linking the objects above. You will need to have C++
extensions and the Math Library.
A typical command might be:<br><font face=courier>
cc ImgLib.o analyzeUtil.o ImgLibError.o VLib.o gen2bshort.c -lm -o
gen2bshort</font>
</ol>
<center>
<table border=2 cellpadding=8 width="90%"><tr><td>
This software is made available <b>AS IS</b> and no warranty is made as to its
accuracy
or performance.<p>
Unlimited academic use of this software is granted, except that the any
publications that derive from its use, or any new software that incorporates
its underlying sources or algorithms must specifically acknowledge the author.
Modification of the program to meet local needs is encouraged, but all such
changes are to be communicated to the author to support the continued growth
of these tools.
<p>
Commercial, for-profit, use of the sources or algorithms is expressly prohibited
without permission of the
<a href="mailto:mscohen@ucla.edu">author</a>.
<p>
Unless otherwise stated, I do not offer technical support beyond that found on the
<a href="http://airto.loni.ucla.edu/BMCweb/HowTo/SpecialFormats.html">
Brain Mapping Center support pages</a>.
</td></tr></table></center>
<pre> */
#define SIG_HD_SIZE 7904
#define XSIZE 256
#define YSIZE 256
#define NAME_SIZE 256

#include <stdio.h>

void print_usage(char *name)
{
   fprintf(stderr,
   "Usage: %s  outfile signafile(s)\n\
    Concatenates Genesis images (*.MR) to outfile.bshort\n\
    and creates an appropriate header file.\n\n\
    Strips signa header (i.e. the files must be 138976 bytes)\n\n\
    User must supply the base name of outfile.\n",name);
}

main (int argc, char *argv[])
{
       int     argcount;
       FILE    *inFile, *outFile;
       char    *signaBuffer;
       char    basename[NAME_SIZE], inputname[NAME_SIZE], outname[NAME_SIZE];

    if (argc<=2)
    {
           print_usage(argv[0]);
           exit(-1);
    }

    printf ("Processing %d files\n", argc-2);

                    /* Open the output file as Signa.bshort */
    strcpy(basename, argv[1]);
    sprintf(outname,"%s.bshort",basename);
    printf("Writing to %s\n", outname);
    if ((outFile = fopen(outname, "wb")) == NULL)
    {
        printf("Unable to open output file %s", outname);
        print_usage(argv[0]);
    }

    if ((signaBuffer = (char *) malloc(XSIZE*YSIZE*sizeof(short))) == NULL)
       printf("unable to allocate input buffer\n");
       
           /* Append contents of each file to output */
    for (argcount=2; argcount < argc; argcount++)
    {
       strcpy(inputname, argv[argcount]);

       if ((inFile = fopen(inputname, "rb")) == NULL)
          {
              printf("Unable to input file %s\n", inputname);
              print_usage(argv[0]);
          }
               /* Read past the header (future expansion - process it) */
          if ((fread(signaBuffer, 1, SIG_HD_SIZE, inFile)) == NULL)
             {
                 printf("Unable to read input file header: %s", inputname);
             }
       if (( fread(signaBuffer, 1, XSIZE*YSIZE*sizeof(short), inFile)) == NULL)
          {
              printf("Unable to read input file data: %s", inputname);
          }
       fclose(inFile);
       if ((fwrite(signaBuffer, 1, XSIZE*YSIZE*sizeof(short), outFile)) == NULL)
          {
              printf("Error writing output file");
          }
       
    }   /*  loop on input files using argcount */
    fclose(outFile);
    free(signaBuffer);
    
            /* Create and write out the new header. N.B. assume
                 that the signa files are all 256 x 256  */

    sprintf(outname,"%s.hdr",basename);
    if ((outFile = fopen(outname, "w")) == NULL)
       {
           printf("unable to open header file %s", outname);
       }
    fprintf(outFile, "%d %d %d 1",XSIZE,YSIZE,argc-2);
    fclose(outFile);
} /* main */
/* </PRE> </CODE> */
/* </body></html> */
