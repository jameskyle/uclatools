/* $Id */
#include <stdio.h>
#include <string.h>

/* This program simply renames files */

main (int argc, char *argv[] )
{
   char buff1[128];
   char buff2[128];
   char buff3[300];

   char *numstring;
   int  number;
   int i;

   if( argc < 2 )
   {
      fprintf( stderr,"Usage: %s *.MR\n\
      Renumber all files ending in \'.MR\' with 3 digit numbers\n", argv[0] );
      exit(-1);
   }

   for( i=argc-1; i>0; i-- )
   {
		strcpy( buff1, argv[i] );
		strcpy( buff2, argv[i] );
		
		numstring = strrchr( buff2, 'I' )+1;
printf( numstring );
printf( "\t" );
printf( numstring + 1 );
printf( "\n" );

		number = atoi( numstring );
	
		sprintf( numstring, "%0.3d.MR", number );
	    fprintf( stderr, "%s --> %s\n", buff1, buff2 );
	    sprintf( buff3, "mv %s %s\n", buff1, buff2 );
	    system( buff3 );
	}
}
