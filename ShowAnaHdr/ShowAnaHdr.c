#include <stdio.h>
#include "ImgLib.h"

int main()
{
	FILE *f;
	char buff[256];
	dsr	theDSR;

	printf( "Enter the name of an analyze header (with extension): " );
	scanf( "%s", buff );

	f = fopen( buff, "rb" );
	fread( &theDSR, sizeof( dsr ), 1, f );

	if( theDSR.dime.dim[0] != 4 ) {
		printf( "SWAPPED BYTE ORDER!!\n" );
		swapHdrBytes( &theDSR );
	}

	ShowAnalyzeHeader( &theDSR, stdout, buff );
	fclose( f );

	return 0;
}
