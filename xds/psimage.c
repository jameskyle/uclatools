/* becomes PSIMAGE which accepts complex floats (8bytes/pel) tr */

#include <stdio.h>
#include <math.h>
#include <malloc.h>
static char tohex[17];
static int npixo, copy, typ, quarter;
double mmax, mmin;

/************************************************************************/
/*                                                                      */
/* This little program lets you print an "image" file on a Laser Writer */
/* using PostScript's "image" operator.  This program is based on the   */
/* example given in the PostScript Cookbook.  It is set up for a 300dpi */
/* machine.  The image dimensions default to 256*256, with 1 byte per   */
/* image pixel.  See the manual page for more details.                  */
/*                                                                      */
/************************************************************************/

main(argc,argv)
int argc;
char *argv[];
{
    FILE *ifp, *fopen();
    int r, c, nr, nc, row, col, sc, argn, win, im;
    int r1, c1, r2, c2, x1, y1, x2, y2; 
	int i;
    unsigned char pix;
    char head[1024],fnam[50];
    double bscale, boff, temp, min, max, range;
	float *data;

    strcpy(tohex,"0123456789ABCDEF");
    npixo = 0;
    x1 = 400;
    y1 = 2048;
    x2 = 2048;
    y2 = 400;
    copy = 1;
    im = 0;
    nr = 256;
    nc = 256;
    typ = 0;
    quarter = 0;

    for (argn = 1; argn < argc; argn++) {
        if (argv[argn][0] == '-') {
            switch (argv[argn][1]) {
                case 'w':
                    win = argn;
                    r1 = atoi(argv[++win]);
                    c1 = atoi(argv[++win]);
                    r2 = atoi(argv[++win]);
                    c2 = atoi(argv[++win]);
                    x1 = c1;
                    y1 = 2560 - r1;
                    x2 = c2;
                    y2 = 2560 - r2;
                    win = 1;
                    argn += 4;
                    break;
                case 'i':
                    nr = atoi(argv[++argn]);
                    nc = atoi(argv[++argn]);
                    break;
                case 'c': copy = atoi(argv[++argn]); break;
		case 't': typ = atoi(argv[++argn]); break;
		case 'q': quarter = 1; break;
                default:
                    fprintf(stderr,
                    "psimage file [-w r1 c1 r2 c2] [-c n] [-i nr nc] [-q]\n");
                    fprintf(stderr,
		    "  file  input image\n");
                    fprintf(stderr,
		    "  -c    copies        [1]\n");
                    fprintf(stderr,
		    "  -i    input size    [256 256]\n");
                    fprintf(stderr,
		    "  -w    output window [400 400 2048 2048]\n");
                    fprintf(stderr,
		    "  -q    quarter size\n");
                    exit(0);
            }
        }
        else {
             if (im) {
                fprintf(stderr,
                    "psimage [file] [-w r1 c1 r2 c2] [-c n] [-i nr nc]\n");
                exit(0);
            }
            im = 1;
            strcpy(fnam,argv[argn]);
            ifp = fopen(argv[argn],"r");
            if (ifp == NULL) {
                fprintf(stderr,"psimage: can't open image file %s\n",
			argv[argn]);
                exit(1);
            }
        }
    }

    /* 
	if (!im) ifp = stdin;
	*/
	if(!im) 
		{
		fprintf(stderr,"psimage: must specify an input file\n");
		exit(255);
		}
    
    prologue(nr,nc,x1,y1,x2,y2,fnam);
   
	if((data = (float *)malloc(2*sizeof(float)*nr*nc)) == NULL)
		{
		fprintf(stderr,"cannot allocate data array\n");
		exit(255);
		}

	if(fread(data,2*sizeof(float),nr*nc,ifp) != nr*nc)
		{
		fprintf(stderr,"data read error\n");
		exit(255);
		}

	mmax = -1.0e12;
	mmin = 1.0e12;
	for(i=0;i < nr*nc;i++)
		{
		if(typ == 2)
			data[2*i] = sqrt(data[2*i]*data[2*i] 
			+ data[2*i + 1]*data[2*i + 1]);
		else 
			{
			if(typ == 1)
				data[2*i] = data[2*i + 1];
			else 
				data[2*i] = data[2*i];
			}
		if(data[2*i] > mmax) 
			mmax = data[2*i];
		if(data[2*i] < mmin) 
			mmin = data[2*i];
		}

	fprintf(stderr,"Max: %g, Min: %g\n",mmax,mmin);	
    for (r = 0; r < nr; r++) 
        for (c = 0; c < nc; c++) {
	/*
       	pix = getc(ifp);
	*/
	pix = (char)(255.0*(data[2*r*nc + 2*c] - mmin)/(mmax - mmin));
	puthexpix(pix);
        }

    epilogue();
    
    fclose(ifp);
    exit(0);
}

prologue(nr,nc,x1,y1,x2,y2,string)
int nr,nc,x1,y1,x2,y2;
char    string[50];
{
    printf("%%!\n");
    printf("/Times-Roman findfont 30 scalefont setfont\n");
    printf("100 700 moveto\n(%s) show\n",string);
    printf("gsave\n");
    printf("initgraphics\n");
    	if(quarter)
    		printf("0.12 %g scale\n",0.12*(float)nr/(float)nc);
	else
    		printf("0.24 %g scale\n",0.24*(float)nr/(float)nc);
    printf("/imline %d string def\n",nc*2);
    printf("/drawimage {\n");
    printf("    %d %d 8\n",nc,nr);
    printf("    [%d 0 0 %d 0 %d]\n",nc,-1*nr,nr);
    printf("    { currentfile imline readhexstring pop } image\n");
    printf("} def\n");
    printf("%d %d translate\n",x1,y2);
    printf("%d %d scale\n",x2-x1,y1-y2);
    printf("drawimage\n");
}

epilogue()
{
    if (npixo) printf("\n");
    if (copy > 1) printf("%d {copypage} repeat\n",copy-1);
    printf("showpage\n");
    printf("grestore\n");
}

puthexpix(p)
unsigned char p;
{
    char hi, lo;

    hi = (p >> 4) & 15;
    lo = p & 15;
    
    printf("%c%c",tohex[hi],tohex[lo]);
    npixo += 1;
    if (npixo >= 32) {
        printf("\n");
        npixo = 0;
    }
}

