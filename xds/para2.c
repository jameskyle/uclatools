#include <stdio.h>
#include <math.h>

main()

{
   int i, j, events, pts, count=0;
   float amp, sum=0;
   char outfile[127];
   FILE *fp, *fp2, *fopen();

/* read a standard file of pairs, return a prototype vector */

   printf ("             Paradigm File Creator\n\n");
   printf ("Enter the name of your new paradigm file: ");
   scanf ("%s",outfile);

/* test for legal file on open */
   if ((fp = fopen(outfile,"w")) == NULL) {
      printf ("\nError opening %s as output\n");
      exit (-1);
      }

   printf ("\nEnter the number of event pairs: ");
   scanf ("%d",&events);

/* check for legal input */
   if (events <1) {
      printf ("\nERROR:  There must be one or more events\n");
      exit (-1);
      }

   printf ("\nEnter the event pairs (number,amplitude):\n");
   for (i=0;i<events;i++)
   {
      scanf ("%d%f",&pts,&amp);
      if (pts < 1) {
         printf ("\nERROR:  Number of events must be 1 or more\n");
         break;
         }
      for (j=0;j<pts;j++) {
         fprintf(fp,"%g\n",amp);
         count++;
         sum=sum+amp;
         }
   }

fclose (fp);
printf ("Total is %g, in %d images\n", sum, count);
}

