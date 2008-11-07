#include <stdio.h>
#include <stdlib.h>

int main(int argc, char **argv)
{
	int x, y;
	double xscale=atof(argv[1]), yscale=atof(argv[2]);
	
	while (fscanf(stdin,"%d %d",&x, &y) == 2)
		fprintf(stdout,"%d %d\n",(int)(x*xscale),(int)(y*yscale));
	exit(0);
}
