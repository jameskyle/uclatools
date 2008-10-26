/* <CODE> <PRE> */

/* $Id: Cproto.c,v 1.3 1998/07/11 04:54:35 mscohen Exp mscohen $ */
/* $Revision: 1.3 $ */
/* $Date: 1998/07/11 04:54:35 $ */
/* $Log: Cproto.c,v $
 * Revision 1.3  1998/07/11  04:54:35  mscohen
 * Mac/unix version. Controlled by 'MAC'
 * */

# include "CC_gr.h"
# include <stdio.h>
# include <stdlib.h>
# include <math.h>
# include <string.h>
#define MAXIMP 2048

#define	false 0
#define true 1

#undef MAC

#ifdef MAC
#include <sioux.h>

/* Establish reasonable console window behavior on the Macintosh */
void SetupSIOUX( void );

void SetupSIOUX( void )
{
   SIOUXSettings.showstatusline = false;
   SIOUXSettings.standalone = true;
   SIOUXSettings.asktosaveonclose = false;
   SIOUXSettings.autocloseonquit = true;
}
#endif

/************************************************************
*	Revised paradigm file creator, allows
*	convolution with an estimate of the brain
*	impulse response.
*************************************************************/
/* prototypes */
int convolve(float *iv);

/* GLOBALS */
   int      c, para_size;
   char     outfile[255];
   float    *invec;
   FILE     *fp, *fopen();

/* FUNCTIONS */

int	convolve(float *iv)
/*****************************************************************************
*	If the user elects, convolve the paradigm vector (iv[]) with an impulse
*	response estimate (Imp[]) to produce a response estimate (ov)
*   to the actual input (makes the naive assumption of linear time invariance).
*
*	Based on the data of Savoy, the impulse response can be estimated as:
*
*		k*t^8.59921*Exp(-t/0.5474691)
*****************************************************************************/
{

	float	Imp[MAXIMP];	/* impulse response */
	float	tr;				/* repetition time used in data collection */
	float	*ov;		/* output vector */
	int	i,j,ImpLen;		/* number of points in impulse response */
	char	instring[255], para_fname[255] = "oldname";
	FILE	*parafile;

	printf("\n\tYour input file will be convolved with a model of the brain\n");
	printf("\timpulse response, in order to give an estimate of the vascular\n");
	printf("\tresponse to this input.\n\n");
	printf("\tYou will also have the option of saving a file called,\n");
	printf("\t'ParadigmDataFile', containing the paradigm, the estimated response\n");
	printf("\tand the time points, in a format that can be plotted directly\n");
	printf("\tusing either 'gnuplot' (unix), or Macintosh tools (e.g. DeltaGraph)\n");

	printf("\nEnter the TR (in seconds) used in the experiment): ");
	scanf ("%s",instring);
	tr = atof(instring);

/* initialize the impulse response vector, based on tr */

	ImpLen = (int)(15/tr);	/* The signal is close to zero after 15 seconds. */
	if (ImpLen > MAXIMP) {   /* The buffer for ImpLen is only 255 points long. */
		printf("ERROR:  TR is too short to use this program.\n");
		exit(-1);
	}

/* create a vector to store the convolved points */
	if ( (ov = (float *) calloc(para_size+ImpLen, sizeof(float) )) == NULL) {
		printf("Can't allocate storage.\n");
		exit (-1);
	}
	
	*Imp = 0.0;
	for (i=1; i<ImpLen; i++) {
		*(Imp+i) = exp(log(tr*i)*8.59921) * exp(-(tr*i)/0.5474691) * 2.212954E-3 * tr;
	}

/* Print the impulse vector     
	printf("\ntime\tImpulse\n");
	for (i=0; i<ImpLen; i++) {
		printf("%.2f\t%f\n",i*tr,Imp[i]);
	}
*/

	/*************************************************************************
	*  As I see it
	*  ov[j] = iv[j-ImpLen]*Imp[ImpLen] + iv[j-ImpLen+1]*Imp[ImpLen-1] + ...
	*          + iv[j-1]*Imp[1] + iv[j]*Imp[0]
	**************************************************************************/

	/* clear the output buffer */
	for(i=0; i<para_size; i++) {
		ov[i] = 0;
	}
	
/* perform the convolution */
	printf("Para_size:%d\n",para_size);

	for (i=0; i<para_size; i++)   {      /* i will count the positions in the iv */
		if ( iv[i] == IGNORE ) {
			ov[i] = IGNORE;
		} else {
			for (j=0; j<ImpLen; j++) {
				if ( (i+j-1) < para_size )  {   /* don't write past the buffer */
					ov[i+j] += iv[i] * Imp[j];
				}
			}
		}
	}

/* Reopen the output file */
	if ((fp = fopen(outfile,"w")) == NULL) {
		printf("\nError opening %s as output\n",outfile);
		exit (-1);
	}

	for (i=0; i<para_size; i++) {
		fprintf(fp,"%f\n",ov[i]);
	}
	fclose (fp);

	/*  OUTPUT A TEST FILE?	*/
	printf("\nSave paradigm plot file (for graphing) (y/n)?: ");
	gets(instring);   /* clear the input buffer */
	c=getchar();
	switch (c)  {
		case 'y':
		case 'Y':
			gets( instring );  /* clear the input buffer */
			strcpy( para_fname, "ParadigmDataFile" );
			printf("Enter a name for the paradigm plot file [ %s ]: ",para_fname);
			gets( instring );
			if( strlen( instring ) != 0 ) {
				strcpy( para_fname, instring );
			}
			
			printf("Data will be saved in: %s\n",para_fname);
			if ( (parafile = fopen( para_fname,"w" ) ) == NULL) {
				printf("\nError opening %s as output\n",para_fname);
				exit (-1);
			}
			
			fprintf(parafile,"Time\tParadigm\tResponse\n");
			for (i=0; i<para_size; i++) {
				fprintf(fp,"%.3f\t%f\t%f\n",i*tr,iv[i],ov[i]);
			}
			fclose (parafile);
			break;
		default:
			break;
	}  /* switch */

	free(iv);
	free(ov);
	
	return 0;
}	/* convolve */

main()
{
	char		ampstr[20], instring[255];
	int			number[255];	/* vectors that mimick the input */
	float		amplitude[255];
	int			i,j,k,c, pts, numpairs, numPos, numNeg, done, balance;
/* numpairs is the number of input pairs, pts is the number if points in the para file. */

#ifdef MAC
	SetupSIOUX();
#endif

/* read a standard file of pairs, return a prototype vector */
	printf("       *********************************\n");
	printf("  .    *                               *\n");
	printf("       *     PARADIGM FILE CREATOR     *\n");
	printf("       *                               *\n");
	printf("       *********************************\n\n");
	printf("Enter the name of your new paradigm file: ");
	scanf ("%s",outfile);

/* test for legal file on open, so that the users needn't waste a lot of time */
	if ((fp = fopen(outfile,"w")) == NULL) {
		printf("\nError opening %s as output\n", outfile);
		exit (-1);
	}
	fclose( fp );
	
	printf("\n\n ------------------INSTRUCTIONS:---------------------------\n");
	printf(" |  ENTER THE EVENT PAIRS (number,amplitude)              |\n");
	printf(" |     -> Enter '0 <return>' to finish data entry         |\n");
	printf(" |     -> Enter 'i' as amplitude to ignore a timepoint    |\n");
	printf(" |        for CC_gr (correlation tests)                   |\n");
	printf(" |     -> Enter '0' (zero) as amplitude to ignore         |\n");
	printf(" |        a timepoint in T_gr (t-tests).                  |\n");
	printf(" |     -> If you enter the sign of the amplitude only     |\n");
	printf(" |        (e.g. + or -) all amplitudes will be adjusted   |\n");
	printf(" |        to sum to zero.                                 |\n");
	printf(" ----------------------------------------------------------\n\n");

	balance = false;	/* do not adjust the amplitudes */
	done = false;
	numpairs = 0;
	para_size = 0;
	numPos = 0;
	numNeg = 0;
	gets(instring);	/* flush the input buffer */   

	while ( !done )
	{
		scanf ("%d,%s",&pts,ampstr);
		if ( pts == 0 )  {
			done = true;
			break;
		}         

		if ( pts < 0 ) {
			printf("\nERROR:  Number of events must be 1 or more\n");
			abort();
		}
		
		if ( strcmp( ampstr, "i" ) == 0 )  {
			amplitude[numpairs]=IGNORE;
		} else if ( strcmp( ampstr, "+" ) == 0 )  {
			balance = true;
			amplitude[numpairs] = 1;
			numPos += pts;
		} else if ( strcmp( ampstr, "-" ) == 0 )  {
			balance = true;
			amplitude[numpairs] = -1;
			numNeg += pts;
		} else {
			amplitude[numpairs] = atof( ampstr );
			if (amplitude[numpairs] >0 )
				numPos += pts;
			if (amplitude[numpairs] <0 )
				numNeg += pts;
		}
		para_size += pts;
		number[numpairs] = pts;
		numpairs++;
	}

	printf( "numPos: %d, numNeg: %d\n", numPos, numNeg );

	fprintf( stderr, "Total of %d images\n", para_size );

	/* Allocate and populate the input vector */
	if ( (invec = (float *) calloc(para_size, sizeof(float) )) == NULL) {
		printf("Can't allocate storage.\n");
		exit (-1);
	}

	k=0;
	if( !balance )  {
		for (i=0; i<numpairs; i++) {
			for(j=0; j<number[i]; j++)  {
				*(invec+k) = amplitude[i];
				k++;
			}
		}
	} else {  /* balance the input values */
		for (i=0; i<numpairs; i++ ) {
			for (j=0; j<number[i]; j++ )  {
				if ( amplitude[i] < 0 )
					*(invec+k) = -numPos;
				if ( amplitude[i] == 0 )
					*(invec+k) = 0;
				if ( amplitude[i] > 0 && amplitude[i] != IGNORE )
					*(invec+k) = numNeg;
				if ( amplitude[i] == IGNORE )
					*(invec+k) = IGNORE;
				k++;
			}
		}
	}

for( i=0; i<numpairs; i++ ) {
	printf( "%f\n", invec[i]);
}

	printf("\nEstimate the response function in the paradigm file (y/n)?: ");
	gets(instring);   /* clear the input buffer */
	c=getchar();

	switch (c)  {
		case 'y':
		case 'Y':
			convolve(invec);
			break;
		default:
			fp = fopen( outfile, "w" );
			for (i=0; i<para_size; i++)
				fprintf( fp, "%f\n", *(invec+i) );
			fclose(fp);
			break;
	}  /* switch */
	
	return 0;   
}
/* </PRE> </CODE> */
