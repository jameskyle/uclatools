#ifndef lint
static char *rcs_xds_c = "XDS Copyright 1993 Timothy L. Davis\n\
$Header: /export/homes/tldavis/src/RCS/xds.c,v 1.68 1994/12/08 19:30:38 tldavis Exp tldavis $";
#endif	/* lint */

#ifdef BSD /* e.g. SunOS 4.1 */
#  undef HAVE_SIGSET
#  undef NO_LONG_DOUBLE
#  define HAVE_MADVISE
#  include <sys/mman.h>
//#undef  HAVE_MADVISE
//#  define HAVE_VADVISE
#  include <sys/vadvise.h> /* For image-processing style virt. memory*/
#endif /*BSD*/

#ifdef SVR4 /* e.g. Solaris 2.x, SGI 5.x */
#  define HAVE_SIGSET
#  undef NO_LONG_DOUBLE
/* #  define HAVE_MADVISE */
#undef HAVE_MADVISE /* Want to see how this goes... */
#  undef  HAVE_VADVISE
#  include <sys/types.h>
#  include <sys/mman.h>
#  ifndef NO_IEEEFP
#    include <ieeefp.h>
#  endif /*NO_IEEEFP*/
#endif /*SVR4*/

#if (defined(titan)||defined(AIX)||(defined(SYSV)&& !defined(SVR4))) /*SYSV.3*/
#  define HAVE_SIGSET
#  define NO_LONG_DOUBLE
#  undef HAVE_MADVISE
#  undef HAVE_VADVISE
#  define NO_FLOAT_H
#  define NO_MMAP
#  define NO_SELECT
#  define NO_ISNAN
#  define NO_FINITE
#  define NEED_GETOPT_H
#  define signed /*signed not recognized on titan SVR3*/
#  define const  /*const not treated right on titan SVR3 */
#endif /* SYSV.3, Stardent */

// #ifdef AIX Like SYSV but for the following */
//#  undef NO_FINITE
#  define NO_NAN_H
// #endif 


#ifndef __DATE__
#  define __DATE__ "$Date: 1994/12/08 19:30:38 $"
#endif

#ifdef NO_ISNAN
#  define isnan(X) (IsNANorINF(X) && !IsINF(X))
#endif

#ifdef NO_FINITE
#  define finite(X) (!IsNANorINF(X))
#endif

#ifdef NEED_GETOPT_H
#  include <getopt.h>
#endif

/*
TODO:
* Display outline as ring around roi.
* Choose roi by clicking on outline points.
* Multiple ROIs --> Multiple graphs
* Histogram display
* Histogram (or manual) equalization
* Keep track of that first wasted read_image() data.

 *$Source: /export/homes/tldavis/src/RCS/xds.c,v $
 *$Header: /export/homes/tldavis/src/RCS/xds.c,v 1.68 1994/12/08 19:30:38 tldavis Exp tldavis $
 *$Log: xds.c,v $
 * Revision 1.68  1994/12/08  19:30:38  tldavis
 * After adding Greg's additions (AGS)
 *
 * Revision 1.67  1994/09/19  03:34:50  tldavis
 * Made quieter.
 *
 * Revision 1.66  1994/09/19  03:32:04  tldavis
 * Made more quiet with the -q option.
 *
 * Revision 1.65  1994/09/19  03:12:58  tldavis
 * Just removed an extra include (num2.h)
 *
 * Revision 1.64  1994/06/28  14:49:03  tldavis
 * Incorporating Helsinki additions, chiefly reading ACR/NEMA images.
 *
 * Revision 1.63  1994/05/04  06:33:38  tldavis
 * Added negative colormap activations.
 *
 * Revision 1.63  1994/05/04  06:33:38  tldavis
 * Added negative colormap activations.
 *
 * Revision 1.62  1994/05/04  04:40:57  tldavis
 * New functionality:
 * Works with -A timecourse and a single static hires anatomical image,
 * even with higher resolution in anatomy than functional,
 * shows graph to be the timecourse functional data when that is greater.
 *  Not tested on old types of data, specifically doesn't work for
 *  KS grinder output at present time.
 *  (Because it will use every image in the -A file, rather than only the last.)
 *
 * Revision 1.61  1994/05/04  01:46:10  tldavis
 * Checkpointed adding multi-image activation colormaps;
 * works when numimages >= NumActivationMaps, but doesn't duplicate a single
 * hires image stack through or show activation map in graph.
 *
 * Revision 1.60  1994/05/03  23:47:34  tldavis
 * Coalescing home, AIX (RS/6000) and lab versions, fixed one small bug
 * with graphing routine.
 *
 * Revision 1.59  1993/12/12  05:36:58  tldavis
 * Fixes re: graph min/max and estalished grayscale min/max limits.
 * Added Bool LoadedLockVal to trip after user has set grayscale.
 * Before this is tripped, loaded min/max updates as images are loaded from
 * files.
 * Should fix the following bugs:
 * 1) Occasionally in N (local scale) mode graph reverts to global scale.
 * 2) monotonic data doesn't get correct graph scaling after read.
 * 3) Recent bug introduced preventing "r" or "w"-key rescaling.
 *
 * Revision 1.58  1993/12/08  21:31:17  tldavis
 * Small mods: ifndef __DATE__ fixed, and Intro_Usage/CmdName basename.
 *
 * Revision 1.57  1993/12/08  19:32:18  tldavis
 * Fixed 2 bugs:
 * 1) finding minima/maxima was broken because of extra else's everywhere.
 * Fixed by removing else's so that monotonic increasing functions are right.
 * 2) Made graph etc. keep up with data, changing graphMin/graphMax as they
 * change from new data.  This should be tested awhile for reasonableness.
 *
 * Revision 1.56  1993/12/07  17:37:50  tldavis
 * Fixed bug accessing .im files.
 * ck_imfile() was broken for .im files.
 *
 * Revision 1.53  1993/12/05  21:47:39  tldavis
 * Fix negative activation to carry sign around log/exp for negative
 * tail of activation map.
 *
 * Revision 1.52  1993/12/02  19:34:17  tldavis
 * Added +/-/both tails option to ^T command for activation.
 *
 * Revision 1.55  1993/11/19  02:19:42  tldavis
 * Beautified colorbar, made disapear when activation not displayed.
 *
 * Revision 1.54  1993/11/18  23:39:14  tldavis
 * input filename (otl, log) bugfix
 * Allows reading of signa i.??? files without renaming them,
 * assumes type gedno. (must be used with hacked read_image.web)
 *
 * Revision 1.53  1993/11/15  17:59:02  tldavis
 * Better error trapping and reporting of bad filenames, etc.
 *
 * Revision 1.52  1993/11/10  23:26:49  tldavis
 * Added colorbar with axis, arrow keys move otl,
 * ovls move separately, boundaries check ok,
 * pageup/down step through images.
 *
 * Revision 1.51  1993/11/10  19:51:29  tldavis
 * Always reports revision now.
 *
 * Revision 1.50  1993/11/10  19:47:10  tldavis
 * Added Revision: line
 *
 * Revision 1.49  1993/11/10  19:43:56  tldavis
 * Only use activationmap colors when -A active.
 * Align activationmap to interpolated image.
 *
 * Revision 1.48  1993/11/05  16:19:03  tldavis
 * Ready for site visit (including functional maps!)
 * Small bug in rescaling after Clearing, leavewindow...
 * the graph window scale suddenly includes zero.
 *
 * Revision 1.47  1993/11/04  22:58:39  tldavis
 * Dynamic map patch fix.
 * Also, 'N' does the right things and graph window has an info line with useful stuff.
 *
 * Revision 1.46  1993/11/04  20:54:42  tldavis
 * Replaced spacebar function
 * Added 'N' option, No Deviations, Local Scale mode.
 * Works on SGI, not yet tested well on suns.  Installing as xds.new
 *
 * Revision 1.44  1993/10/26  22:44:16  tldavis
 * Fix for Stardent again.
 *
 * Revision 1.43  1993/10/26  18:58:30  tldavis
 * Fixes for blitz, not uses read_otl() on sgi
 *
 * Revision 1.42  1993/10/24  04:43:54  tldavis
 * Fixed ovl bug introduced in last release.
 *
 *
 * Revision 1.38  1993/10/16  21:52:24  tldavis
 * Version which should work on Sunos4, Solaris2, SGI, Stardent titan,
 * but descended from a version which might have bugs in sunos4... we'll see.
 *
 * Revision 1.47  1993/10/07  15:15:22  tldavis
 * Compiles with -pedantic.
 * Doesn't like signedness of pointers in line 1154, but compiles.
 * Probably will work with acc now.
 *
 * Revision 1.46  1993/10/07  04:48:43  tldavis
 * One strange crash, but seems to work for:
 * -L low -H high command-line window/level and
 * -m colorfile substitution of colormap, takes image as indices.
 * and -A activationfile and 'A' activation-color loading, ' ' toggle and
 * Greg's HJKLd interactions (moving all overlays simultaneously)
 * A few other things....
 *
 * Revision 1.45  1993/10/06  21:40:07  tldavis
 * Add colormap works, but still rescaling data (bad news)
 * Hacked-in spot-colors seem to work, fixed at 9 values for now, ascii
 * file input.
 * NEW COMMAND LINE: -m colormapfile -A activationmapfile
 * NEW INTERACTION:  'A' load activationmapfile, ' ' toggle showing spotcolors,
 *                   'd': make a box of given size
 * 		HJKL: move the overlay left right up down from AGSorensen
 * Cool!
 *
 * Revision 1.44  1993/10/05  18:18:22  tldavis
 * Fixed read_image, now it is possible to substitute a colormap of interest
 * for the entire image via '-m colormap_file'
 *
 * Revision 1.43  1993/09/27  01:56:24  tldavis
 * Fixed to react to ICCCM ClientMessage for WM_DELETE.
 *
 * Revision 1.42  1993/07/15  05:20:19  tldavis
 * Compiles for solaris 2.2 (removed infinity() refs)
 *
 * Revision 1.41  1993/07/15  04:29:49  tldavis
 * Incorporating changes with ANMR_read_image function dependency.
 * Note that this had strange bugs reported by Kwong, Iris, etc al.
 *
 * Revision 1.35  1992/12/05  05:42:42  tldavis
 * Works with optimized integer interpolation with decimation algorithm.
 *
 * Revision 1.33  1992/10/15  17:28:48  tldavis
 * Maybe fixed the bugs in the previous (1.32) revision:
 * coredump small graph widths,
 * Bug in graph sizing.
 *
 * Revision 1.32  1992/10/15  17:18:32  tldavis
 * Added checking to mallocs (a long time ago)
 * Changed color of stdevs
 * Keep from making graph bigger than screen
 *
 * Revision 1.31  1992/07/13  20:48:11  tldavis
 * Added .im file reading ability.
 * Doesn't reorder input files by image number, however.
 * Perhaps this should be a flag?
 *
 * Revision 1.30  1992/05/16  01:07:31  tldavis
 * Major additions:  subtraction/ratio/-lnratio modes
 * Interaction ability optimized with 0.5 second wait for new events.
 * Lots of grayscale and interaction settings fixed.
 *
 * Revision 1.29  1992/05/14  19:05:31  tldavis
 * Loading files and refreshing pixmaps is now in the background,
 * but some quirks remain re: grayscale settings and interaction 
 * speed during loading.
 * Multiple files can now be specified on the command line, each with
 * its own set of image choice parameters.
 * Subtraction mode works in its most primitive form.  Not fixed yet.
 *
 * Revision 1.26  1992/05/12  16:06:59  tldavis
 * Improvements:
 * 1) Writes float files by hitting "f" key (.flt for seg).
 * 2) Understands the .otl extension outline files from seg.
 * 3) Keeps the rest of the screen readable when cursor is in window.
 *
 * Revision 1.25  1992/05/08  23:48:26  tldavis
 * A few more graph-related bugs fixed.  Now graph is allocated at the beginning.
 *
 * Revision 1.24  1992/05/08  23:28:39  tldavis
 * Made graph window much more robust, fixing lots of re-configure bugs.
 * Now redraws self.
 *
 * Revision 1.23  1992/05/08  21:40:25  tldavis
 * Added graph value storage and reporting.
 * Still need to fix graph limit bug.
 *
 * Revision 1.22  1992/05/07  19:49:55  tldavis
 * Bugfix for resizing while showing clipped window.
 * Next to fix: black stay black and white stay white.
 *
 * Revision 1.21  1992/05/07  19:14:57  tldavis
 * Changed ovl to use the clipmask of a GC.
 * Added background-masking for spacebar.
 *
 * Revision 1.20  1992/04/27  22:17:29  tldavis
 * New additions:
 * Interactive pixel-value time display.
 * Draw time curve for averaged ROI data
 * ROI-autolevel
 * Separate NaN and Inf colors.
 * Redo window and level to use clinical standard
 *
 * Revision 1.10  1992/02/06  08:00:14  tldavis
 * First limited alpha release.
 *
 * Revision 1.7  1992/02/06  05:54:13  tldavis
 * Many improvements, including NaN and Inf - awareness (with a special pixel),
 * a help page, and multiple overlays (currently 8 max).  Almost done!
 *
 * xds.c Grayscale Viewer
 * Timothy L. Davis, 1/92
 */

/* Get the nice ANSI C error checking from Xlib calls. */
#define NeedFunctionPrototypes 1
#include <X11/Xlib.h>     
#include <X11/Xutil.h> /* Gets XSizeHints, XGetVisualInfo, XLookupString */
#include <X11/cursorfont.h> /* for crosshair cursor */
#include <X11/keysym.h> /* For non-ascii keys */
#include <sys/types.h> /* For select() arguments */
#include <sys/stat.h> /* For stat() to check filenames for saneness */
#include <sys/time.h> /* For itimer animation alarm and timeval for select()*/
#ifdef NO_SELECT
#include <stropts.h>
#include <poll.h>
#undef STR /* clash with read_image.h */
#endif /*NO_SELECT*/

#include <stdio.h>
/*<ctype.h>*/
#include <string.h>
#include <math.h>
#include <limits.h>
#include <float.h>
// We must define DBL_MAX and DBL_MIN to conform with the new limits.h notation
#ifndef NO_NAN_H
#  include <nan.h> /* SVID ? NaN/Inf handling */
#endif
#include <stdlib.h>
#include <assert.h>
/*<stdarg.h>, <setjmp.h>*/
#include <signal.h>   /* For alarm interrupt handling */
/*<time.h>, <limits.h>, <float.h>*/
#include "complex.h" /* Uses macros from here, but not complex.o */
#include "read_image.h" /* Mike Vevea's read_image() routine */
#include "read_otl.h"
#include "assert_print.h"
/* From read_im.c */
#include "xds_help.h" /* defines Command_Usage and Key_Pointer_Usage. */
#include "read_im.h"  /* for ANMR_read_image() and report_header(). */

extern int read_image(char *fname, int type, struct image *image, 
		      int image_num, int rescale_ok);

char *Intro_Usage = "%s, X Display Stack $Revision: 1.68 $, compiled %s.\n\
Copyright 1993 by Timothy L. Davis, <tldavis@mit.edu>.  Please e-mail comments!\n\
Type 'h' or '?' in the image window for help on interactive features.\n";

/* The X display */
Display *dpy;
int screen;
Window root, map_win=0, graph_win=0;
Visual *visual=0;
Pixmap *zoom_pix=0, colorbar_pix;
Pixmap ovl_clip_pix=0;
GC imageGC=0, ovlGC=0, ovl_clippedGC=0, stringGC=0, hilightGC=0;

/* Skip over NSKIPPED_LOCOLORS to reduce flashing of other windows */
/* Skip the highest colortable value to avoid a bug on exiting openwindows */
/* NOVERLAYS maximum of 8 */
int NPLANES=0, NSKIPPED_LOCOLORS, NSKIPPED_HICOLORS, NOVERLAYS;
int NGRAYS, INF_PIXEL, NAN_PIXEL, HILIGHT_PIXEL, OVERLAY_PIXEL0,
    BLACK_PIXEL, WHITE_PIXEL, PRISTINE_GRAY_COUNT, PRISTINE_GRAY_CENTER;
int NACTIVATIONCOLORS, ACTIVATION_PIXEL0, 
    NEGACTIVATIONCOLORS, NEGACTIVATION_PIXEL0;
double Spotcolor0Cutoff=4.6, SpotcolorLowCutoff=4.6, SpotcolorHighCutoff=20;

#define INFO_HEIGHT (15)
#define TEXTOFF (12)
#define STATUS_WIDTH (7)

#define MAXIMAGEFILES (2048)
#define MAXIMAGES (8192)

static char status_char = ' ';

#define show_info(OFF, BUF) \
  (XClearArea(dpy, map_win, 5*(OFF), Mzoom, 0, 0, False),\
   XDrawImageString(dpy, map_win, stringGC, 5*(OFF), \
		    WinHeight-INFO_HEIGHT+TEXTOFF, (BUF), strlen((BUF))),\
   XDrawImageString(dpy, map_win, stringGC, WinWidth-STATUS_WIDTH,\
		    WinHeight-INFO_HEIGHT+TEXTOFF, &status_char, 1))

#define show_info_graph(OFF, BUF) \
  (XClearArea(dpy, graph_win, 5*(OFF), graphHeight, 0, 0, False),\
   XDrawImageString(dpy, graph_win, stringGC, 5*(OFF),\
		    graphHeight+TEXTOFF,(BUF),strlen((BUF))))

#define show_status(CHAR) \
  (XClearArea(dpy, map_win, WinWidth-STATUS_WIDTH, Mzoom, 0, 0, False),\
   status_char = (CHAR),\
   XDrawImageString(dpy, map_win, stringGC, WinWidth-STATUS_WIDTH, \
		    WinHeight+TEXTOFF-INFO_HEIGHT, &status_char, 1))

unsigned int N=0, M=0, zoomx=1, zoomy=1, numImages=0, numPixmaps;
int WinWidth=0, WinHeight=0, ColorbarWidth=0;
double aspect_ratio = 1.0;
/* These are premultiplied concatenations of the above */
unsigned int NM, Nzoom, Mzoom, NMzoom, zoomxy, Nzoomxy;

void mk_dirty(char dirtflags);
void askto_load_images(double min, double max, Bool newzoom);
void askfor_image(int imagenum);
void redisplay(void);
void redisplay_graph(Bool rescale_x);
Colormap load_colormap(int gray_count, int gray_center);
void read_activation(char *fname);
void zoom_activationmap(void);

#ifdef _BSD_SIGNALS
int  timer_handler(int sig);
#else
void timer_handler(int sig);
#endif /* _BSD_SIGNALS */
void image_event_handler(XEvent *event), graph_event_handler(XEvent *event);

/* Set this to zero to exit */
Bool Running = True;
/* Mode flags (True when appropriate button is down) */
Bool Animating = False, Levelling = False, Roistretch = False;
Bool Interpolating = False;
Bool ShowCurNumber = True, ShowOvl = True, TimerHandleOk = False;
Bool ShowSpotColor = False, ShowNegSpotColor = False;
int  NumActivationMaps = 0;
double activationMin, activationMax;
int ActivationZoomMore = 1;
int  ActivationTails = 0x1; /* Bit field,1=+,2=-,3=both,0=none */
Bool ClipBackground = False;
Bool AutoWindowAll = False, DynamicGraph = False, Quiet = False;
Bool ActivationGraph = False; /* Show timecourse of activation vs. data */
Bool DynamicColorTable = False;
Bool ShowingDevs = False, DontShowDevs = False;
Bool Percent = False; /*AGS*/
Bool OtlMode = False;
Bool ShowingCurrent = True; /* line in graph window */
unsigned char OvlMask = 1; /* Start with just the first overlay */
unsigned int currentPix = 0;
Bool DirtyFlag=True; /* some images need reloading etc. */
Bool BusyFlag=False; /* Don't do background tasks while I'm busy */
int round_robin = 0, robin_start=0;

enum {t_simple=0, t_magnitude=1, t_phase=2, t_real=3, t_imaginary=4} 
     DisplayType = t_simple;
char *DispString[] = {"","magnitude","phase","real","imaginary"};

enum {ref_none=0, ref_difference, ref_ratio, ref_logratio} 
     ReferenceType=ref_none;
float *refImage=0;
fcomplex *crefImage=0;
char *ReferenceString[] = {"none", "difference", "ratio", "-log ratio"};

#define image_dirty 1
#define pixmap_dirty 2
#define data_dirty 4
char *loadImageFlags;
Bool LoadingImages = False;
double loadingMin, loadingMax;

const double Inf = DBL_MAX / DBL_MIN; /*Please ignore compile warning.*/

float **input, ***input_rows; /* The float data stack */
fcomplex **cinput, ***cinput_rows;
unsigned char *overlays, *temp_overlays;
float ***activationmap=0, **activationmap_zoom=0;

/* Variables for windowing and levelling */
double loadedMin = 0, loadedMax = 0, dataMin=0, dataMax=0;
Bool DataLockVal = False; /* True when data limits forced by user. */
Bool LoadedLockVal = False; /* True after user has modified contrast/scale. */
int cmap_gray_count, cmap_gray_center;
Colormap the_colormap = 0;

/* Variables for graphing */
XPoint *graphPoints = 0;
XSegment *graphSegments = 0;
double *graphMeans=0, *graphSdevs=0, graphMin, graphMax;
unsigned int graphHeight, graphWidth, graphReserveX=0, graphReserveY=INFO_HEIGHT;

Atom wm_delete_window = 0;

/* Input-related structures */
char *scriptfilename = 0; /* stdout is the default */
char *ovlfilename = "default.ovl", *CmdName, *nametail = 0;
char *activationfname = 0;
char *colormapfilename = 0;
int numimagefiles=0;

typedef enum filetype_tag { NotNew_t=0, ANMR_t, APD2_t, ACR_t } filetype_t;

struct {
  char *fname; int skip; int num; int end;
  filetype_t NewFiletype;
} file_info[MAXIMAGEFILES] = {0, 0, 0, 0, 0};
struct {int filenum; int imagenum; } image_info[MAXIMAGES];

char *ck_malloc(size_t size, char *msg)
{ void *rtn = (void *) malloc(size);
  if (!rtn) {
    fprintf(stderr,"Error allocating memory for: %s !\n", msg);fflush(stderr);
    abort();
  } return(rtn);
}
char *ck_calloc(size_t nobj, size_t size, char *msg)
{ void *rtn = (void *) calloc(nobj, size);
  if (!rtn) {
    fprintf(stderr,"Error allocating memory for: %s !\n", msg);fflush(stderr);
    abort();
  } return(rtn);
}

#define IsSignaFilename(FNAME) (strncmp((FNAME),"i.???",2)==0 \
				&& strlen((FNAME)) == 5)

Bool ck_imfile(char *path) {
  struct stat statbuf;
  char *type=strrchr(path,'.');
  char *basename=strrchr(path,'/');
  if (!basename) basename = path; else basename++;

  /* Exclude filenames other than signa files, .im files, and files
   * with at least 3-character extensions.
   */
  if (!type || (strlen(type) < 4 && strcmp(type,".im")
      && !IsSignaFilename(basename))) goto bad;

  else type++;

  /* Check to see that main file is readable. */
  if (stat(path, &statbuf) != 0) { perror(path); goto bad; }
  if ( ! (statbuf.st_mode & 0444) )    
    { fprintf(stderr,"Can't read %s\n",path);  goto bad; }
  
  if (type[0] == 'b' || !strcmp(type,"anmr") || !strcmp(type,"ome") 
      || !strcmp(type,"gen")) { 
    /* These must have .hdr component.  Check to see that it's there. */
    char *assoc_path = strdup(path);
    char *pos=strrchr(assoc_path,'.')+1;
    strcpy(pos,"hdr");
    if (stat(assoc_path, &statbuf) != 0) { perror(assoc_path); goto bad; }
    if ( ! (statbuf.st_mode & 0444) )    
      { fprintf(stderr,"Can't read %s\n", assoc_path);  goto bad; }
  }

  return True;
 bad:
  fprintf(stderr,
	  "\n**** Error Reading file %s, continuing anyway... ****\n",
	  path);
  return False;
}

fcomplex FCdiv(fcomplex a, fcomplex b)
{ fcomplex c;  double r,den;
  if (fabs(b.r) >= fabs(b.i)) 
    { r=b.i/b.r; den=b.r+r*b.i; c.r=(a.r+r*a.i)/den; c.i=(a.i-r*a.r)/den; } 
  else { r=b.r/b.i; den=b.i+r*b.r; c.r=(a.r*r+a.i)/den; c.i=(a.i*r-a.r)/den; }
  return c;
}

int main(int argc,char *argv[])
{
  struct image im;
  int ans;
  int i, k, l;
  int argp;
#ifdef DEBUG
  static Bool synchronize = True;
  fprintf(stderr,"Warning: COMPILED WITH -DDEBUG OPTION.\n");
#else
  static Bool synchronize = False;
#endif /*DEBUG*/

#ifdef HAVE_VADVISE
  vadvise(VA_SEQL); /* Sequential pages will be used, not least-recent */
#endif /*HAVE_VADVISE*/

  if (!(CmdName=strrchr(argv[0],'/'))) CmdName=argv[0]; else CmdName++;
  
  dataMax = -(dataMin = Inf);

  while (optind < argc) { /* WARNING: USES optind MOD HACK */
    if ((argp=getopt(argc, argv, "L:H:A:i:v:l:o:O:s:#:e:z:r:qhWMPRIN:bm:p:")) 
	== -1)
      { argp = 'i'; optarg=argv[optind++]; /*|||optind mod hack!!!*/ }
    switch ((char)argp) {
    case 'i': {
      /* Support the reading of new ANMR .im files */
      char *type = strrchr(optarg,'.')+1;
      if (ck_imfile(optarg)) {
	if (strcmp(type, "im") == 0) file_info[numimagefiles].NewFiletype = ANMR_t;
	else if (strcmp(type, "img") == 0) file_info[numimagefiles].NewFiletype = APD2_t;
	else if (strcmp(type, "ima") == 0) file_info[numimagefiles].NewFiletype = ACR_t;
	else file_info[numimagefiles].NewFiletype = NotNew_t;
	file_info[numimagefiles++].fname = optarg;
	file_info[numimagefiles].skip = file_info[numimagefiles-1].skip;
	file_info[numimagefiles].num = file_info[numimagefiles-1].num;
	file_info[numimagefiles].end = file_info[numimagefiles-1].end;
      } 
    } break;
    case 'L': dataMin = atof(optarg); DataLockVal=True; break;
    case 'H': dataMax = atof(optarg); DataLockVal=True; break;
    case 'A': 
      if (ck_imfile(optarg)) {
	activationfname=optarg; 	
	ShowSpotColor = True;
	ShowNegSpotColor = True;
	ColorbarWidth=50;
      } break;
    case 'o':
    case 'v':
      ovlfilename=optarg; 
      if (strcmp(ovlfilename+strlen(ovlfilename)-4,".otl")==0) OtlMode=True; 
      else OtlMode=False;
      break;
    case 'l': scriptfilename=optarg; break;
    case 'm': if (!ck_imfile(optarg)) break;
      colormapfilename=optarg; break;
    case 's': file_info[numimagefiles].skip = atoi(optarg); break;
    case '#':
      file_info[numimagefiles].num  = atoi(optarg); 
      file_info[numimagefiles].end = 0;
      break;
    case 'e': 
      file_info[numimagefiles].end =  atoi(optarg);
      file_info[numimagefiles].num  = 0; 
      break;
    case 'z': zoomx = atoi(optarg); break;
    case 'r': aspect_ratio = atof(optarg); break;
    case 'W': AutoWindowAll = True; break;
    case 'q': Quiet = !Quiet; break;
    case 'R': DisplayType = t_real; break;
    case 'I': DisplayType = t_imaginary; break;
    case 'M': DisplayType = t_magnitude; break;
    case 'P': DisplayType = t_phase; break;
    case 'S': DisplayType = t_simple; break;
    case 'b': Interpolating = !Interpolating; break;
    case 'N': CmdName = optarg; break;
    case 'p': NPLANES = atoi(optarg); break;
    case 'h': case '?':  /* Drop Thru */
      Quiet = 0; /* Quiet or not, we give the default info response. */
    default: 
      if (!Quiet) {
	fprintf(stderr,Key_Pointer_Usage);
	fprintf(stderr,"\n\n--------------------------------------------\n\n");
	fprintf(stderr,Command_Usage,CmdName);
      }
      exit(0);
    }

    if (numimagefiles >= MAXIMAGEFILES)
      fprintf(stderr, "**** Error: Too many image files! Max is %d\n",
	      MAXIMAGEFILES);
  }

  if (!Quiet) fprintf(stderr,Intro_Usage, CmdName, __DATE__);/*Type info*/
  
  if (numimagefiles == 0) {
    fprintf(stderr, "**** Error: No image file(s) specified! ****\n\n");
    if (!Quiet) fprintf(stderr, Command_Usage, CmdName);
    exit(-1);
  }
  
  N=M=numImages=numPixmaps=0;
  k=0;
  for (l=0; l < numimagefiles; l++) {
    switch (file_info[l].NewFiletype) {
    case ANMR_t:
      ANMR_read_image(file_info[l].fname, &im, 0, 0);
      break;
    case APD2_t:
      APD2_read_image(file_info[l].fname, &im, file_info[l].skip);
      break;
    case ACR_t:
      ACR_read_image(file_info[l].fname, &im, 0, 0);
      break;
    case NotNew_t:
      ans=read_image(file_info[l].fname, T_FLOAT, &im, file_info[l].skip, 1);
      assert_print(im.version == READ_IMAGE_VERSION,
		   "read_image header incompatible with object code!\n");
      if (ans < 0)
	switch (ans) {
	case NO_SUCH_FILE: fprintf(stderr,"read_image(): No such file %s\n",
				   file_info[l].fname); 
	  exit(-1);
	case BAD_FILE:
	  fprintf(stderr,"read_image(): Bad file: %s\n",file_info[l].fname);
	  exit(-1);
	case BAD_SCALE:
	  fprintf(stderr,"read_image(): Bad scale: %s\n",file_info[l].fname);
	  exit(-1);
	default:  fprintf(stderr,"Unknown err in read_image(): %s\n",
			  file_info[l].fname);
	  exit(-1);
	}
      break;
    }
    if (!DataLockVal) { /* Try best guess first-pass min/max for data. */
      /* This only considers the first image of each file, so may be wrong. */
      /* Updates to dataMin/dataMax in askfor_image(). */
      if (im.min < dataMin) dataMin = im.min; 
      if (im.max > dataMax) dataMax = im.max;
    }
    if (! (dataMin < dataMax) ) dataMax = dataMin + fabs(0.00001*dataMin);
    free(im.image.c); /* Only using the info, not the data */
    free(im.row_ptr.c); /* I know, it's wasteful, but only 1 image/file. */
    if (im.cols > N) N = im.cols; 
    if (im.rows > M) M = im.rows;
    if (file_info[l].end && file_info[l].end < im.images) 
      im.images = file_info[l].end;
    if (file_info[l].num == 0 || file_info[l].num> im.images-file_info[l].skip)
      file_info[l].num = im.images-file_info[l].skip;
    
    if (!Quiet)
      fprintf(stderr,
	     "To load from %s: %d %dx%d image(s) (displaying %ss) skip %d.\n", 
	      file_info[l].fname, file_info[l].num, DisplayType?N/2:N, 
	      M, DispString[DisplayType], file_info[l].skip);
    
    /* Define the mapping from file images to stack image number:
     * Stack image k comes from file file_info[image_info[k].filenum].fname
     * image # image_info[k].imagenum
     */
    for (i=0; i < file_info[l].num; i++) {
      if (k >= MAXIMAGES) {
	fprintf(stderr,"Too many images!  Maximum allowed is %d.\n",
		MAXIMAGES);
	exit(-1);
      }
      image_info[k].filenum = l;
      image_info[k++].imagenum = i+file_info[l].skip;
    }
    numImages += file_info[l].num;
  }
    
  zoomy = zoomx*aspect_ratio;
  NM = N*M;
  Nzoom = N*zoomx; Mzoom = M*zoomy; NMzoom = Nzoom*Mzoom;
  zoomxy = zoomx*zoomy;
  Nzoomxy = N*zoomxy;

  input = (float **) ck_malloc(numImages*sizeof(float *),"input");
  cinput = (fcomplex **) input;
  input_rows = (float ***) ck_calloc(numImages, sizeof(float **),"input_rows");
  cinput_rows = (fcomplex ***) input_rows;
  overlays = (unsigned char *) ck_calloc(NM,sizeof(*overlays),"overlays");
  temp_overlays = (unsigned char *) ck_calloc(NM,sizeof(*temp_overlays),
					      "temp_overlays");

  if (DisplayType != t_simple) N/=2;  /* Want a window half in dim */
  NM = N*M; Nzoom = N*zoomx; Mzoom = M*zoomy; 
  NMzoom = Nzoom*Mzoom;  zoomxy = zoomx*zoomy; Nzoomxy = N*zoomxy;

  { /* X Initialization: Open display, setup screen, root, visual type. */
    XVisualInfo vinfo; /* Keep around for variable "visual" */

    /* COLORMAP LAYOUT is described here.
     * colors 0 <= n < 24 are skipped
     * color 24 is INF_PIXEL
     * color 25 is NAN_PIXEL
     * color 30 is HILIGHT_PIXEL
     * colors 31 <= n < 35 (NOVERLAYS of them) are overlays,OVERLAY_PIXEL0=31
     * colors 35 <= n < 100 (NACTIVATIONCOLORS) are activationpixels
     *                     ACTIVATION_PIXEL0 is the first right now.
     * colors 101 <= n < 255 are grayscale, BLACK_PIXEL to WHITE_PIXEL (NGRAYS)
     * color 255 is skipped.
     */

    if (!NPLANES) NPLANES=8;
    NSKIPPED_LOCOLORS = 24;
    NSKIPPED_HICOLORS = 1;
    NOVERLAYS = 4;
    NACTIVATIONCOLORS = ShowSpotColor ? 64 : 0;
    NEGACTIVATIONCOLORS = ShowNegSpotColor ? 32 : 0;

    INF_PIXEL = NSKIPPED_LOCOLORS;
    NAN_PIXEL = INF_PIXEL+1;
    HILIGHT_PIXEL = NAN_PIXEL+1;
    OVERLAY_PIXEL0 = HILIGHT_PIXEL+1;
    NEGACTIVATION_PIXEL0 = OVERLAY_PIXEL0 + NOVERLAYS;
    ACTIVATION_PIXEL0 = NEGACTIVATION_PIXEL0 + NEGACTIVATIONCOLORS;
    BLACK_PIXEL = ACTIVATION_PIXEL0 + NACTIVATIONCOLORS;
    WHITE_PIXEL = (1<<NPLANES)-1 - NSKIPPED_HICOLORS;
    NGRAYS = WHITE_PIXEL-BLACK_PIXEL+1;
    PRISTINE_GRAY_COUNT = NGRAYS;
    PRISTINE_GRAY_CENTER = NGRAYS/2;

    if ((dpy = XOpenDisplay(getenv("DISPLAY"))) == NULL)
      { fprintf(stderr, "in %s:  can't open display\n",CmdName); return(-1);  }
    if (synchronize) XSynchronize(dpy, 1);/* Smooth, controllable animation*/
    screen = XDefaultScreen(dpy);
    root = DefaultRootWindow(dpy);
    if (XMatchVisualInfo(dpy, screen, NPLANES, PseudoColor, &vinfo)) 
      visual = vinfo.visual;
    else {
      fprintf(stderr,"%s: Require %d plane PseudoColor-compatible display.\n",
	      *argv, NPLANES); 
      visual = XDefaultVisual(dpy, screen);/*Better to use GrayScale*/
    }
  }
    { /* Create window with given colormap, events, and cursor; set WM props */
    XSetWindowAttributes attrs;
    XSizeHints normal_hints;
    XWMHints wm_hints;
    XClassHint class_hints;
    XTextProperty window_name;
    char *buf = (char *) ck_malloc(256,"buf for colormap");

    cmap_gray_count = PRISTINE_GRAY_COUNT;
    cmap_gray_center = PRISTINE_GRAY_CENTER;

    attrs.border_pixel = BLACK_PIXEL;
    attrs.background_pixel = BLACK_PIXEL;
    attrs.colormap = load_colormap(cmap_gray_count, cmap_gray_center);
    attrs.cursor = XCreateFontCursor(dpy, XC_crosshair);
    attrs.event_mask = ExposureMask|ButtonPressMask|ButtonReleaseMask
         |PointerMotionMask|KeyPressMask|StructureNotifyMask|LeaveWindowMask ;
    map_win=XCreateWindow(dpy, root, 10, 10, (WinWidth=Nzoom+ColorbarWidth), 
			  (WinHeight=Mzoom+INFO_HEIGHT), 

			  1, NPLANES, InputOutput, visual,
			  CWBorderPixel /*Saves NPLANES!=default*/ | 
			  CWColormap|CWEventMask|CWCursor|CWBackPixel,&attrs);

/*    normal_hints.flags = PMinSize|PResizeInc|PAspect|PBaseSize|PWinGravity;*/
    normal_hints.flags = PMinSize|PResizeInc|PBaseSize|PWinGravity;
    normal_hints.min_width = N + ColorbarWidth;
    normal_hints.min_height = M + INFO_HEIGHT;
    normal_hints.width_inc = N;
    normal_hints.height_inc = M;
    normal_hints.min_aspect.x = normal_hints.max_aspect.x = N;
    normal_hints.min_aspect.y = M*aspect_ratio;
    normal_hints.max_aspect.y = M*aspect_ratio;
    normal_hints.base_width = ColorbarWidth;
    normal_hints.base_height = INFO_HEIGHT;
    normal_hints.win_gravity = NorthWestGravity;
    wm_hints.flags = InputHint;
    wm_hints.input = True;
    class_hints.res_name = "xds";
    class_hints.res_class = "Xds";
    nametail = file_info[0].fname + strlen(file_info[0].fname);
    while (nametail > file_info[0].fname)
      /* Display the file name, not the path. */
      if (*(--nametail) == '/') { nametail++; break; }
    if (ovlfilename) 
      sprintf(buf, "%s: %s, ovl: %s", CmdName, nametail, ovlfilename);
    else sprintf(buf, "%s: %s", CmdName, nametail);
    XStringListToTextProperty(&buf, 1, &window_name);
    XSetWMProperties(dpy, map_win, &window_name, &window_name,
		     argv, argc, &normal_hints, &wm_hints, &class_hints);
    XInstallColormap(dpy, the_colormap); /* WM Function, help for startup */
  }
  
  if (activationfname) read_activation(activationfname);

  if (numImages > NumActivationMaps) 
    numPixmaps = numImages;
  else {
    numPixmaps = NumActivationMaps; 
    ActivationGraph = True; /* use whichever is larger for graph, by default.*/
  }
  zoom_pix = (Pixmap *) ck_calloc(numPixmaps,sizeof(Pixmap),"zoom_pix");
  
  imageGC = XCreateGC(dpy, map_win, 0, NULL);
  XSetGraphicsExposures(dpy, imageGC, False);
  XSetState(dpy, imageGC, BLACK_PIXEL, BLACK_PIXEL, GXcopy, AllPlanes);

  stringGC = XCreateGC(dpy, map_win, 0, NULL);
  XSetGraphicsExposures(dpy, stringGC, False);
  XSetState(dpy, stringGC, WHITE_PIXEL, BLACK_PIXEL, GXcopy, AllPlanes);

  hilightGC = XCreateGC(dpy, map_win, 0, NULL);
  XSetGraphicsExposures(dpy, hilightGC, False);
  XSetState(dpy, hilightGC, HILIGHT_PIXEL, BLACK_PIXEL, GXcopy, AllPlanes);

  ovlGC = XCreateGC(dpy, map_win, 0, NULL);
  XSetGraphicsExposures(dpy,ovlGC, False);
  XSetForeground(dpy, ovlGC, OVERLAY_PIXEL0);
  XSetFillStyle(dpy, ovlGC, FillSolid);

  ovl_clippedGC = XCreateGC(dpy, map_win, 0, NULL);
  XSetGraphicsExposures(dpy,ovl_clippedGC, False);
  XSetForeground(dpy, ovl_clippedGC, OVERLAY_PIXEL0);
  XSetFillStyle(dpy, ovl_clippedGC, FillSolid);
     
  { /* Create window with given colormap, events, and cursor */
    unsigned int width=XDisplayWidth(dpy,screen), height=200, ujunk;
    double widthfactor = ((double) XDisplayWidth(dpy,screen))/numPixmaps;
    int ijunk;
    Window winjunk;
    XSizeHints normal_hints;
    XWMHints wm_hints;
    XClassHint class_hints;
    XTextProperty window_name;
    char *buf = (char *) ck_malloc(256,"buf for window creation");
    XSetWindowAttributes attrs;

    attrs.colormap = the_colormap;
    attrs.background_pixel = BLACK_PIXEL;
    attrs.cursor = XCreateFontCursor(dpy, XC_crosshair);
    attrs.event_mask = ExposureMask|ButtonPressMask|ButtonReleaseMask
      |PointerMotionMask|KeyPressMask|StructureNotifyMask|LeaveWindowMask;
    if (widthfactor > 4) width = numPixmaps*4;
    else if (widthfactor==0) width = XDisplayWidth(dpy,screen);
    else width = numPixmaps*widthfactor;
    graph_win = 
      XCreateWindow(dpy, root, 10, 10, width, height, 
		    0, NPLANES, InputOutput, visual,
		    CWBorderPixel /*Saves NPLANES!=default*/ | 		    
		    CWColormap|CWEventMask|CWCursor|CWBackPixel, &attrs);
    XGetGeometry(dpy, graph_win, &winjunk, &ijunk, &ijunk, 
		 &width, &height, &ujunk, &ujunk);
    graphWidth = width - graphReserveX;
    graphHeight = height - graphReserveY;
    graphPoints = (XPoint *) ck_malloc(numPixmaps*sizeof(XPoint),"graphPoints");
    graphSegments = (XSegment *) ck_malloc(numPixmaps*sizeof(XSegment),
                    "graphSegments");
    graphMeans = (double *) ck_malloc(numPixmaps*sizeof(double),"graphMeans");
    graphSdevs = (double *) ck_malloc(numPixmaps*sizeof(double),"graphSdevs");
     /* Set window manager properties */
    normal_hints.flags = 0;
    wm_hints.flags = InputHint;
    wm_hints.input = True;
    class_hints.res_name = "xds";
    class_hints.res_class = "Xds";
    if (ovlfilename) sprintf(buf, "%s: %s, ovl: %s", "time-course graph", 
                             nametail, ovlfilename);
    else sprintf(buf, "%s: %s", "time-course graph", nametail);
    XStringListToTextProperty(&buf, 1, &window_name);
    XSetWMProperties(dpy, graph_win, &window_name, &window_name,
		     argv, argc, &normal_hints, &wm_hints, 
		     &class_hints);
  }
  
  wm_delete_window = XInternAtom(dpy, "WM_DELETE_WINDOW", False);
  
  (void) XSetWMProtocols(dpy, map_win, &wm_delete_window, 1);
  (void) XSetWMProtocols(dpy, graph_win, &wm_delete_window, 1);

  /* Start the ball rolling: Map the window and handle events.
   * images will be loaded when the Expose handler notices
   * that they've never been loaded before; at that time, window dimensions
   * will dictate the scaling factor.
   *
   * Animation will interrupt this loop to do its business via timer alarms.
   */
  XMapWindow(dpy, map_win);
#ifdef HAVE_SIGSET
  sigset(SIGALRM, timer_handler);
#else /* BSD signals */
  signal(SIGALRM, timer_handler);
#endif
  zoom_activationmap();
  askto_load_images(dataMin, dataMax, True);

  {
    static XEvent the_event;

    while (Running) {
      
      if (DirtyFlag && !BusyFlag && !XEventsQueued(dpy, QueuedAfterFlush)) {
	/* Wait 100`s of msecs for another event */
#ifdef NO_SELECT
	static struct pollfd readfd;
	readfd.fd = ConnectionNumber(dpy); readfd.events = POLLIN|POLLPRI;
	if (poll(&readfd, 1, 500/*ms*/) == 0)
#else /* use select() instead of poll() */
	static fd_set readfds;	static struct timeval timeout;
	FD_ZERO(&readfds); FD_SET(ConnectionNumber(dpy), &readfds);
	timeout.tv_sec = 0; timeout.tv_usec = 500000;
	if (select(FD_SETSIZE, &readfds, 0, 0, &timeout) == 0) 
#endif
	  /* Make a run for it while we have the time: load images */
	  while (DirtyFlag && !BusyFlag &&
		 !XEventsQueued(dpy, QueuedAfterReading)) {
	    askfor_image(round_robin);
	    round_robin=(round_robin+1)%numPixmaps;
	    if (round_robin == robin_start) {
	      DirtyFlag=False; /*Once around nicely to ensure all are loaded*/
	      show_status(' ');
	    } else show_status('*');
	  }
      }
      
      /* Only handle timer events inside blocking XNextEvent */
      TimerHandleOk = True; XNextEvent(dpy,&the_event); TimerHandleOk = False; 
      if (the_event.xany.window == map_win) image_event_handler(&the_event);
      else graph_event_handler(&the_event);
    }
  }

  XCloseDisplay(dpy);
  exit(0);
  }

Colormap load_colormap(int gray_count/*"window"*/, int gray_center/*"level"*/)
{
  static XColor *colors = 0;
  static Colormap cmap = 0;
  static char *overlayColorNames[] = 
    {"yellow","magenta","cyan","orange","green","blue","violet","pink"};
  const double slope=65536/(gray_count-1);
  int xintercept = gray_center - floor((gray_count-1)/(double)2);
  int i, n;

  if (colormapfilename) { /* External colormap */
    if (!cmap) {
      int ncolors = 1<<NPLANES;
      XColor *color_list = (XColor *) ck_malloc(ncolors*sizeof(XColor),
						"color_list");
      int color_count=0;
      int r, g, b;
      FILE *colorfile = fopen(colormapfilename, "r");
      assert_print(colorfile,"Couldn't open color file.\n");
      while (fscanf(colorfile, "%d %d %d\n",&r,&g,&b) == 3 
	     && color_count < ncolors ) {
	color_list[color_count].red   = r;
	color_list[color_count].green = g;
	color_list[color_count].blue  = b;
	color_list[color_count].flags = DoRed|DoGreen|DoBlue;
	color_list[color_count].pixel = color_count;
	color_count++;
      }
      cmap = XCreateColormap(dpy, root, visual, AllocAll);
      XStoreColors(dpy, cmap, color_list, color_count);
      BLACK_PIXEL = 0; WHITE_PIXEL = ncolors-1; /* Truncation to colormap */
    }
  } else { /* Internal grayscale windowable colormap */
    if (!cmap) {
      XWindowAttributes wattr;
      XColor *copycolor = (XColor *) ck_malloc(NSKIPPED_LOCOLORS*sizeof(XColor),
					      "copycolor");
      XColor *spotColors = (XColor *) 
	(NACTIVATIONCOLORS ? ck_malloc((NACTIVATIONCOLORS+NEGACTIVATIONCOLORS)
				       *sizeof(XColor), "spotColors") : 0);

      /* Copy low-order values in the table so other windows are readable*/
      XGetWindowAttributes(dpy, root, &wattr);
      cmap = XCreateColormap(dpy, root, visual, AllocAll);
      for (i=0; i < NSKIPPED_LOCOLORS; i++) {
	copycolor[i].pixel = i; copycolor[i].flags=DoRed|DoGreen|DoBlue;
	XQueryColor(dpy, wattr.colormap, &copycolor[i]);
      }
      XStoreColors(dpy, cmap, copycolor, NSKIPPED_LOCOLORS);
      /*|||Don't bother copying the NSKIPPED high colors, just leave it alone*/
      
      XStoreNamedColor(dpy, cmap, "brown", INF_PIXEL, DoRed|DoGreen|DoBlue);
      XStoreNamedColor(dpy, cmap, "firebrick", NAN_PIXEL,DoRed|DoGreen|DoBlue);
      XStoreNamedColor(dpy, cmap,"skyblue",HILIGHT_PIXEL,DoRed|DoGreen|DoBlue);
      for (i = 0; i < NOVERLAYS; i++)
	XStoreNamedColor(dpy, cmap, overlayColorNames[i], OVERLAY_PIXEL0+i,
			 DoRed|DoGreen|DoBlue);
      
      /* Spotcolors for Blue Red Yellow color progression */
      for (i=0; i < NACTIVATIONCOLORS/2; i++) {
	spotColors[i].pixel = ACTIVATION_PIXEL0 + i;
	spotColors[i].flags=DoRed|DoGreen|DoBlue;
	spotColors[i].red = i*65536/(NACTIVATIONCOLORS*0.5);
	spotColors[i].green = 0;
	spotColors[i].blue = 65535-i*65536/(NACTIVATIONCOLORS*0.5);
      }
      for (n=0; i < NACTIVATIONCOLORS; i++, n++) {
	spotColors[i].pixel = ACTIVATION_PIXEL0 + i;
	spotColors[i].flags=DoRed|DoGreen|DoBlue;
	spotColors[i].red = 65535;
	spotColors[i].green = n*65536/(NACTIVATIONCOLORS*0.5);
	spotColors[i].blue = 0;
      }

      /* Spotcolors for Green ... Cyan (NEGATIVE) color progression */
      for (n=0; n < NEGACTIVATIONCOLORS/2; i++, n++) {
	spotColors[i].pixel = NEGACTIVATION_PIXEL0 + n;
	spotColors[i].flags=DoRed|DoGreen|DoBlue;
	spotColors[i].red = 32767+n*32767/(NEGACTIVATIONCOLORS*0.5);
	spotColors[i].green = 0;
	spotColors[i].blue = n*65535/(NEGACTIVATIONCOLORS);
      }
      for (n=0; n < NEGACTIVATIONCOLORS/2; i++, n++) {
	spotColors[i].pixel = NEGACTIVATION_PIXEL0 + n + NEGACTIVATIONCOLORS/2;
	spotColors[i].flags=DoRed|DoGreen|DoBlue;
	spotColors[i].red =  65535;
	spotColors[i].green = 0;
	spotColors[i].blue = 32768+n*65536/(NEGACTIVATIONCOLORS);
      }

      if (NACTIVATIONCOLORS) XStoreColors(dpy, cmap, spotColors, NACTIVATIONCOLORS+NEGACTIVATIONCOLORS);
    }
    
    if (!colors) {
      colors = (XColor *) ck_malloc(NGRAYS*sizeof(XColor),"colors");
      for (i = 0; i < NGRAYS; i++) {
	colors[i].pixel = i+BLACK_PIXEL; 
	colors[i].flags=DoRed|DoGreen|DoBlue;
      }
    }
    if (!DynamicColorTable) {
      for (i = 0; i < NGRAYS; i++) {
	/* Only fill in the values */    
	signed long int val = (int)(slope*((double)i-(double)xintercept));
	if (val < 0) val = 0; else if (val > 65535) val = 65535;
	if (i == 0) val = 0;   else if (i == NGRAYS-1) val = 65535;
	colors[i].red = colors[i].green = colors[i].blue = (unsigned short) val;
      }
    } else {  /* do a dynamic colorlookuptable manipulation */
      for (i = 0; i < NGRAYS; i++) {
	signed long int redval = (int)(slope*((double)i-(double)xintercept));
	signed long int greenval = 
	  (int)((double) 2.0 * slope*((double)i-(double)xintercept));
	if (redval < 0) redval = 0; else if (redval > 65535) redval = 65535;
	if (i == 0) redval = 0;   else if (i == NGRAYS-1) redval = 65535;
	colors[i].red = (unsigned short) redval;
	if (greenval < 0) {
	  greenval = 0;
	} else if (greenval > 65535) {
	  if (greenval >= 2 * 65535) greenval = 0;
	  else greenval = 65535 - (greenval - 65535);
	}
	if (i == 0) greenval = 0;   else if (i == NGRAYS-1) greenval = 0;
	colors[i].green = (unsigned short) greenval;
	if (i==0) colors[i].blue = (unsigned short) 0;
	else colors[i].blue = (unsigned short) (65535 - redval);
      }
    }/* End changes by AGS 11/2/93 */
    XStoreColors(dpy, cmap, colors, NGRAYS);
  }
  the_colormap = cmap;
  return(cmap);
}

/* Load to server from client memory */
void load_one_image(int imagenum, double min, double max)
{  
  char *binned = (char *) ck_malloc(NMzoom*sizeof(char), "binned");
  XImage *tmp_image = 
    XCreateImage(dpy, visual, NPLANES, ZPixmap,0,binned,Nzoom,Mzoom,8,Nzoom);
  double scl = NGRAYS/(max-min)*0.999;
  float *inptr = input[imagenum%numImages];

  if (colormapfilename) {
    /* Effectively disable the rescaling that is coded here. 
     *  Images are treated directly as pixel values.
     */
    scl = 1.0;  min = BLACK_PIXEL;
  }
  
  if (!Interpolating) {
    unsigned char *bptr = (unsigned char *) binned; 
    int jspotstart = 0, jspotend= Nzoomxy;
    register int m,n;
    
    for (jspotstart = 0; jspotstart < NMzoom; 
	 jspotstart+=Nzoomxy,jspotend+=Nzoomxy) {
      int ispotstart=0, ispotend=zoomx;
      for (ispotstart = 0; ispotstart < Nzoom; 
	   ispotstart+=zoomx, ispotend+=zoomx) {
	double dval;
	int ival;
	register unsigned char cval;
	switch (DisplayType) {
	case t_simple:    dval = *inptr++; break;
	case t_magnitude: dval = MCabs(*(fcomplex *)inptr); inptr+=2; break;
	case t_phase:     dval = MCarg(*(fcomplex *)inptr); inptr+=2; break;
	case t_real:      dval = ((fcomplex *)inptr)->r; inptr+=2; break;
	case t_imaginary:dval = ((fcomplex *)inptr)->i; inptr+=2; break;
	}
	ival = (dval-min)*scl + BLACK_PIXEL;
	cval = (unsigned char)
	  (!finite(dval) ? (isnan(dval) ? NAN_PIXEL : INF_PIXEL) : 
	   ( ival<BLACK_PIXEL ? BLACK_PIXEL : 
	    (ival>WHITE_PIXEL ? WHITE_PIXEL : ival)));
	for (m = jspotstart; m < jspotend; m+=Nzoom)
	  for (n = ispotstart; n < ispotend; n++)
	    bptr[n+m] = cval;
      }
    }
  } else { /* Interpolating */
    switch (DisplayType) {
    case t_simple:
      {	unsigned int xoff = zoomx/2, yoff = Nzoom*(zoomy/2),
	             xoff2 =zoomx-xoff, yoff2 =Nzoomxy-yoff;
	unsigned char *bptr = (unsigned char *) binned, 
	              *bbptr = bptr + xoff + yoff;
	register int m,n,mm;
	int istart, jstart;
	
	for (jstart=0; jstart < NMzoom; jstart+=Nzoomxy) {
	  for (istart=0; istart < Nzoom; istart+=zoomx) {
	    double dval = *inptr++;
	    int ival = (dval-min)*scl + BLACK_PIXEL;
	    register unsigned char cval = 
	      (unsigned char)
		(!finite(dval) ? (isnan(dval) ? NAN_PIXEL : INF_PIXEL) : 
		 ( ival<BLACK_PIXEL ? BLACK_PIXEL : 
		  (ival>WHITE_PIXEL ? WHITE_PIXEL : ival)));
	    bbptr[jstart+istart] = cval;
	  }
	}

	for (jstart=0; jstart<NMzoom-Nzoomxy; jstart+=Nzoomxy) {
	  for (m = 0, mm=0; m < zoomy; m++,mm+=Nzoom)
	    for (n = 0; n < xoff; n++) /* leftmost column */
	      bptr[n + mm + jstart+yoff] =
		( (zoomy-m) * bbptr[jstart]
		 +       m  * bbptr[jstart+Nzoomxy])/zoomy;
	  for (m = 0, mm=0; m < zoomy; m++,mm+=Nzoom)
	    for (n = xoff; n < zoomx; n++) /* rightmost column */
	      bptr[n + mm + jstart+Nzoom-zoomx+yoff] =
		( (zoomy-m) * bbptr[jstart+Nzoom-zoomx]
                 +       m  * bbptr[jstart+Nzoom-zoomx+Nzoomxy])/zoomy;
	}
	for (mm=0; mm < yoff; mm+=Nzoom)
	  for (istart=0; istart<Nzoom-zoomx; istart+=zoomx)
	    for (n = 0; n < zoomx; n++) /* topmost row */
	      bptr[n+mm+istart+xoff] = 
		( (zoomx-n)*bbptr[istart]
	         +       n *bbptr[istart+zoomx])/zoomx;
	for (mm=yoff; mm < Nzoomxy; mm+=Nzoom)
	  for (istart=0; istart<Nzoom-zoomx; istart+=zoomx)
	    for (n = 0; n < zoomx; n++) /* bottommost row */
	      bptr[n+mm+istart+xoff+NMzoom-Nzoomxy] = 
		( (zoomx-n)*bbptr[istart+NMzoom-Nzoomxy]
		 +       n *bbptr[istart+zoomx+NMzoom-Nzoomxy])/zoomx;

	for (mm=0; mm<yoff; mm+=Nzoom)
	  for (n=0; n<xoff; n++) bptr[n+mm] = bbptr[0];
	for (mm=0; mm<yoff; mm+=Nzoom)
	  for (n=0; n<xoff2; n++)
	    bptr[n+mm+Nzoom-xoff2] = bbptr[Nzoom-zoomx];
	for (mm=0; mm<yoff2; mm+=Nzoom)
	  for (n=0; n<xoff; n++)
	    bptr[n+mm+NMzoom-yoff2] = bbptr[NMzoom-Nzoomxy];
	for (mm=0; mm<yoff2; mm+=Nzoom)
	  for (n=0; n<xoff2; n++)
	    bptr[n+mm+NMzoom-yoff2+Nzoom-xoff2] = 
	      bbptr[NMzoom-Nzoomxy+Nzoom-zoomx];

	{ unsigned int z=0, foo=zoomx;
	  unsigned int xstride = zoomx, xstride1=xstride>>1U;
	  unsigned int ystride = Nzoomxy, ystride1=ystride>>1U;

	  while ((foo >>= 1U)>0) z++;

	  if (1<<z == zoomx && zoomx == zoomy) {
	    /* Divide and conquer method for zoomx=zoomy=2^N */
	    while (xstride>1) {
	      xstride1=xstride>>1U; ystride1=ystride>>1;
	      for (jstart=0; jstart<NMzoom-Nzoomxy; jstart+=ystride) {
		for (istart=0; istart<Nzoom-zoomx; istart+=xstride) {
		  bbptr[jstart+istart+xstride1] = ((unsigned)
		       bbptr[jstart+istart]+bbptr[jstart+istart+xstride])>>1U;
		  bbptr[jstart+istart+ystride1] = ((unsigned)
		       bbptr[jstart+istart]+bbptr[jstart+istart+ystride])>>1U;
		  bbptr[jstart+istart+xstride1+ystride1] = ((unsigned)
		      bbptr[jstart+istart]+bbptr[jstart+istart+ystride]
		     +bbptr[jstart+istart+xstride]
		     +bbptr[jstart+istart+xstride+ystride])>>2U;
		}
	      }
	      xstride=xstride1;
	      ystride=ystride1;
	    }
	  } else {
	    /* General bilinear interpolation, much slower. */
	    show_info(3,"Performing slow interp for non-2^n zoom.");
	    for (jstart=0; jstart<NMzoom-Nzoomxy; jstart+=Nzoomxy) {
	      for (istart=0; istart<Nzoom-zoomx; istart+=zoomx) {
		for (m = 0, mm=0; m < zoomy; m++,mm+=Nzoom)
		  for (n = 0; n < zoomx; n++) {
		    bbptr[n + mm + jstart+istart ] = 
		      ((zoomx-n)*(zoomy-m)* bbptr[jstart+istart]
		       +(zoomx-n)*     m  * bbptr[jstart+istart+Nzoomxy]
		       +   n  * (zoomy-m) * bbptr[jstart+istart+zoomx]
		       +   n  *        m  * bbptr[jstart+istart+Nzoomxy+zoomx])
			/zoomxy;
		  }
	      }
	    }
	  }
	}
      } break;
    case t_magnitude:
    case t_phase:
    case t_real:
    case t_imaginary:
      show_info(3,"Sorry, can't interpolate complex data.");
      Interpolating = False;
      break;
    }
  }

  if (ShowSpotColor && activationmap) {
    register int i;
    int offset = Nzoom*floor(0.5*zoomy) + floor(0.5*zoomx);
    int scalefac = NACTIVATIONCOLORS/(SpotcolorHighCutoff-SpotcolorLowCutoff);
    int negscalefac = NEGACTIVATIONCOLORS/(SpotcolorHighCutoff-SpotcolorLowCutoff);
    const unsigned int high_cutoff = ACTIVATION_PIXEL0 + NACTIVATIONCOLORS;
    const unsigned int low_cutoff = ACTIVATION_PIXEL0;
    const unsigned int neghigh_cutoff = NEGACTIVATION_PIXEL0 + NEGACTIVATIONCOLORS;
    const unsigned int neglow_cutoff = NEGACTIVATION_PIXEL0;
    float *mapzoom = activationmap_zoom[imagenum%NumActivationMaps];
 
    for (i=0; i<NMzoom-Nzoomxy-zoomx; i++) {
      if (mapzoom[i] > Spotcolor0Cutoff) {
	register unsigned char 
	  cval = (mapzoom[i]-SpotcolorLowCutoff)*scalefac+ACTIVATION_PIXEL0;
	binned[i+offset] = cval >= high_cutoff ?  high_cutoff-1 : 
	  (cval < low_cutoff ? low_cutoff : cval);
      } else if (mapzoom[i] < -Spotcolor0Cutoff) {
	register unsigned char 
	  cval = ((-mapzoom[i])-SpotcolorLowCutoff)*negscalefac
	    +NEGACTIVATION_PIXEL0;
	binned[i+offset] = cval >= neghigh_cutoff ?  neghigh_cutoff-1 : 
	  (cval < neglow_cutoff ? neglow_cutoff : cval);
      }
    }  
  }
  
  XSetClipMask(dpy, imageGC, None);
  XPutImage(dpy, zoom_pix[imagenum], imageGC, tmp_image, 0,0,0,0,Nzoom,Mzoom);
  if (ClipBackground)    XSetClipMask(dpy, imageGC, ovl_clip_pix);
  else                   XSetClipMask(dpy, imageGC, None);
  XDestroyImage(tmp_image); /* also deallocates *binned ?????? */
} /* END load_one_image() */

void zoom_activationmap(void)
{ /* Bilinear interpolate zoom activationmap overlay */
  int i, k;
  int jstart, jend=NMzoom-Nzoomxy, istart, iend=Nzoom-zoomx, m, n, mm;
  
  if (!activationmap) return;
  if (activationmap_zoom) 
    for (i=0; i<NumActivationMaps; i++) free(activationmap_zoom[i]);
  else activationmap_zoom=(float **)ck_calloc(NumActivationMaps,sizeof(float *),
					      "activationmap_zoom");
  for (i=0; i<NumActivationMaps; i++)
    activationmap_zoom[i]=(float *)ck_calloc(NMzoom,sizeof(float *),
					     "activationmap_zoom data");
  
  for (k=0; k<NumActivationMaps; k++) {
    int A_Nzoomxy = Nzoomxy * ActivationZoomMore,
        A_zoomx   = zoomx   * ActivationZoomMore,
        A_zoomy   = zoomy   * ActivationZoomMore,
        A_zoomxy  = zoomxy  * ActivationZoomMore * ActivationZoomMore;
    
    float *inptr  = activationmap[k][0];
    float *outptr = activationmap_zoom[k];/*Center over the middle of pixels*/
    for (jstart=0; jstart < NMzoom; jstart+=A_Nzoomxy)
      for (istart=0; istart < Nzoom; istart+=A_zoomx) {
	double nonneg = *inptr++;
	if (ActivationTails == 2) /* negative tail only */ nonneg = -nonneg;
	else if (ActivationTails == 3) /* both tails */ nonneg = fabs(nonneg);
	else if (ActivationTails == 1) /* pos tail only */ ;
	else fprintf(stderr,"Error: Neither tail selected.\n");
#ifdef NOTDEF /* Comment out for 2-tail map with 2 colormaps */
	if (nonneg < 0.0) nonneg = 0.0; 
#endif /*NOTDEF*/
	outptr[jstart+istart] = nonneg; /* Seed the interpolation */
      }
    /* General bilinear interpolation, much slower but okay for 1-time use. */
    for (jstart=0; jstart<jend; jstart+=A_Nzoomxy) {
      for (istart=0; istart<iend; istart+=A_zoomx) {
	for (m = 0, mm=0; m < A_zoomy; m++,mm+=Nzoom)
	  for (n = 0; n < A_zoomx; n++) {
	    outptr[n + mm + jstart+istart ] = 
	     ( (A_zoomx-n)*(A_zoomy-m)*outptr[jstart+istart]
	      +(A_zoomx-n)*       m *outptr[jstart+istart+A_Nzoomxy]
	      +       n *(A_zoomy-m)*outptr[jstart+istart+A_zoomx]
	      +       n *       m *outptr[jstart+istart+A_Nzoomxy+A_zoomx])
	       / A_zoomxy;
	  }
      }
    }
  }
  
  {  /* Make activation map color bar */
    int i, laststring=-100;
    double p /*, q*/;
    
    double fctr = ((double)Mzoom)/NACTIVATIONCOLORS;
    double negfctr = ((double)Mzoom)/NEGACTIVATIONCOLORS;
    GC gc = XCreateGC(dpy, map_win, 0, 0);
    if (colorbar_pix) XFreePixmap(dpy, colorbar_pix);
    colorbar_pix = XCreatePixmap(dpy, root, ColorbarWidth, Mzoom, NPLANES);

    XSetForeground(dpy, gc, BLACK_PIXEL);
    XFillRectangle(dpy, colorbar_pix, gc, 0, 0, ColorbarWidth, Mzoom);
    for (i=0; i< NACTIVATIONCOLORS; i++) {
      XSetForeground(dpy, gc, i+ACTIVATION_PIXEL0);
      XFillRectangle(dpy,colorbar_pix,gc,0,i*fctr,ColorbarWidth,(int)fctr+2);
    }
    
    for (p = 0.1; p > 1e-20; p *= 0.1) {
      
      static char buf[80];
      int y = ((-log(p)-SpotcolorLowCutoff)
	       /(SpotcolorHighCutoff-SpotcolorLowCutoff)) * Mzoom;
      XDrawLine(dpy, colorbar_pix, stringGC, 
		0, y, ColorbarWidth/5 + 1, y);
      sprintf(buf,"%.0e", p);
      if (y - laststring > TEXTOFF) { /* Keep labels sufficiently sparse */
	laststring = y;
	XDrawString(dpy, colorbar_pix, stringGC, 5 + ColorbarWidth/5,
			 y + TEXTOFF/3, buf, strlen(buf));
      }
    }

#ifdef NOTDEF
    for (q = 0.1; q > 1e-20; q *= 0.1) 
      for (p = q; p < 10*q; p += q+q) {
	int y = ((-log(p)-SpotcolorLowCutoff)
		 /(SpotcolorHighCutoff-SpotcolorLowCutoff)) * Mzoom;
	XDrawLine(dpy, colorbar_pix, stringGC, 
		  ColorbarWidth/10, y, ColorbarWidth/5 - 1, y);
      }
#endif /*NOTDEF*/
  }
} /*END zoom_activationmap() */

void update_clip(Bool new_zoom)
{
  static XRectangle *rects = 0;
  static int nrects = 0;
  int i, j;
  static GC localGC;
  
  if (!rects) {
    rects = (XRectangle *) ck_malloc(NM*sizeof(XRectangle),"rects");
  }
  if (new_zoom || !ovl_clip_pix) {
    if (ovl_clip_pix) {XFreePixmap(dpy, ovl_clip_pix); ovl_clip_pix=0;}
    if (!ovl_clip_pix) ovl_clip_pix = XCreatePixmap(dpy,root,Nzoom,Mzoom,1);
    for (i=0; i<NM; i++) {rects[i].width = zoomx; rects[i].height = zoomy;}
  }
  if (!localGC) {
    localGC = XCreateGC(dpy, ovl_clip_pix, 0, NULL);
    XSetGraphicsExposures(dpy, localGC, False);
  }

  nrects=0;
  for (j=0; j<NM; j+=N)
    for (i=0; i<N; i++)
      if (overlays[i+j]&OvlMask) 
	{ rects[nrects].x = i*zoomx; rects[nrects++].y = j*zoomy/N; }
  
  /* Clear ovl_clip_pix */
  XSetForeground(dpy, localGC, 0);
  XFillRectangle(dpy, ovl_clip_pix, localGC, 0, 0, Nzoom, Mzoom);
  
  /* Set clipmask to list of rectangles */
  XSetForeground(dpy, localGC, 1);
  if (nrects) XFillRectangles(dpy, ovl_clip_pix, localGC, rects, nrects);
  
  XSetClipMask(dpy, ovl_clippedGC, ovl_clip_pix);
}

void read_overlay(char *fname, int rows, int cols, char **overlays,int *factor)
{
  int x=0, y=0, n, end = 0;
  FILE *ovlfile = fopen(fname, "r");
  
  (*overlays) = (char *) ck_calloc(rows*cols, sizeof(char),"overlays");
  *factor = 1;
  
  if (!ovlfile) {fprintf(stderr,"Can't find file '%s'.\n",fname); exit(-1); }
  while (!end) {
    if ((n=fscanf(ovlfile,"%d %d", &x, &y)) == EOF) {
      if (!Quiet) fprintf(stderr,"Done loading ovlfile from file '%s'.\n", 
			  fname);
      end = 1;
    } else {
      if (n != 2) {fprintf(stderr,"Problem reading ovlfile.\n");end = 1; }
      if (x >= cols || y >= rows) {
	fprintf(stderr,"WARNING! Bad ROI data input.\n");
      } else (*overlays)[x+y*cols] = 1;
    }
  }
  fclose(ovlfile);
}

/* Can only be called after initial anatomy size known*/
void read_activation(char *fname) 
{ struct image im;
  int i;
  int ans=read_image(fname, T_FLOAT, &im, 0, 1);
  assert_print(im.version == READ_IMAGE_VERSION,
	       "read_image header incompatible with object code!");
  if (ans < 0)
    switch (ans) {
    case NO_SUCH_FILE:fprintf(stderr,"read_image(): No such file %s\n",fname); 
      exit(-1);
    case BAD_FILE:
      fprintf(stderr,"read_image(): Bad file: %s\n",fname);
      exit(-1);
    case BAD_SCALE:
      fprintf(stderr,"read_image(): Bad scale: %s\n",fname);
      exit(-1);
    default:  fprintf(stderr,"Unknown err in read_image(): %s\n", fname);
      exit(-1);
    }
  activationmap = (float ***) ck_malloc(NumActivationMaps * sizeof(float **),
					"activationmap");
  NumActivationMaps = im.images;
  activationmap[0] = im.row_ptr.f;
  activationMin = im.min;
  activationMax = im.max;
  for (i=1; i < NumActivationMaps; i++) {
    ans=read_image(fname, T_FLOAT, &im, i, 1);
    assert_print(ans>=0,"Error reading an activation image.");
    activationmap[i] = im.row_ptr.f;
    if (im.min < activationMin) activationMin = im.min;
    if (im.max > activationMax) activationMax = im.max;
  }

  if (!Quiet) {
    fprintf(stderr,
	    "Using file %s image(s) 0..%d as -log(p-value) for activation map.\n",
	    fname, im.images-1);
    fprintf(stderr,"Darkest blue p=0.01; bright yellow p=exp(-20).\n");
  }

  if (!N) {  /* zoom_activationmap needs these */
    N=im.cols; M=im.rows;
    zoomy = zoomx*aspect_ratio;
    NM = N*M;
    Nzoom = N*zoomx; Mzoom = M*zoomy; NMzoom = Nzoom*Mzoom;
    zoomxy = zoomx*zoomy;
    Nzoomxy = N*zoomxy;
  } else if (im.cols != N) {
    /* Must zoom activation images differently from base images! */
    /* Assume identical aspect ratio. */
    assert_print(N%im.cols == 0,
		 "Aspect ratios not the same for maps and anatomical images.");
    ActivationZoomMore = N/im.cols;
  }
}

static struct itimerval timerstart = { 0, 5E4, 0, 1E3 }, 
  timercancel = {0,0,0,0};

#ifdef _BSD_SIGNALS
int  timer_handler(int sig)
#else
void timer_handler(int sig)
#endif /* _BSD_SIGNALS */
{
  if (!TimerHandleOk) return;
  if (Animating) {
    if (!XEventsQueued(dpy, QueuedAfterFlush)) {
      currentPix = (currentPix + 1) % numPixmaps;
      redisplay();
    }
  } else {
    if (setitimer(ITIMER_REAL, &timercancel, 0))
      { fprintf(stderr,"ITIMER_REAL error\n"); exit(-1); }
  }
}


void graph_event_handler(XEvent *event)
{
  switch (event->xany.type) {
  case ButtonPress: { /* Jump to the image corresponding to this time pt */
    currentPix= event->xbutton.x*(numPixmaps-1)/
      (graphPoints[numPixmaps-1].x-graphPoints[0].x);
    if (currentPix >= numPixmaps) currentPix = numPixmaps-1;
    redisplay();
  } break;
  case ConfigureNotify: {
    graphWidth = event->xconfigure.width - graphReserveX;
    graphHeight = event->xconfigure.height - graphReserveY;
    redisplay_graph(True);
  } break;
  case Expose: 
    redisplay_graph(True);
    break;
  case MotionNotify: {
    char buf[256], buf2[256];
    unsigned int pointerImage = 
      event->xbutton.x*(numPixmaps-1)/(graphPoints[numPixmaps-1].x-graphPoints[0].x);
    double pointerVal = graphMax - event->xbutton.y*(graphMax-graphMin)/graphHeight;
    if (pointerImage >= numPixmaps) pointerImage = numPixmaps-1;
    if (ShowingDevs) {
      sprintf(buf,"Graph value (image %02d)=%.4g +- %.4g", pointerImage,
	      graphMeans[pointerImage], graphSdevs[pointerImage]);
      sprintf(buf2,"Image %d intensity %.4g +- %.4g, pointer at %.4g",pointerImage,graphMeans[pointerImage],
	      graphSdevs[pointerImage], pointerVal); 
      show_info(3, buf);
      if (ShowCurNumber) show_info_graph(0, buf2);
    } else {
      sprintf(buf,"Graph value (image %02d)=%.4g ", pointerImage,
	      graphMeans[pointerImage]);
      sprintf(buf2,"Image %d intensity %.4g, pointer at %.4g",pointerImage,graphMeans[pointerImage],
	      pointerVal); 
      show_info(3, buf);
      if (ShowCurNumber) show_info_graph(0, buf2);
    }
  } break;
  case KeyPress:
    image_event_handler(event); /* Let the rest act as if typed to image */
    break;
  case LeaveNotify: {
    char buf[128];
    sprintf(buf,"%02d", currentPix);
    show_info(0,buf);
    redisplay_graph(False);
  } break;
  case ClientMessage:
    if (event->xclient.data.l[0] == wm_delete_window) {
      DynamicGraph=False; /* Will be inverted when shown again to True */
      XUnmapWindow(dpy, graph_win);
    }
    break;
  default:
    break;
  }
}

void image_event_handler(XEvent *event)
{
  static int xLevel,yLevel,xRoi,yRoi,xAnimate,yAnimate,xRoiCorner,yRoiCorner;
  static int oldxRoi, oldyRoi, oldxRoiCorner, oldyRoiCorner;
  static const double windowFctr = 0.25, levelFctr = 0.25;
  auto int x = event->xbutton.x, y = event->xbutton.y;

/*
 * Button 1 DOWN: box ROI choice on motion
 * Button 2 DOWN: window and level on motion
 * Button 3 DOWN: animate sequence, speed altered by motion; UP: stop animation
 */
  switch (event->type) {
  case KeyPress: {
    char keybuf[32];
    static int num0 = 0;
    char mychar=0;
    switch(XLookupKeysym(&event->xkey, 0)) {
    case XK_Up: mychar = 'K'; break;
    case XK_Down: mychar = 'J'; break;
    case XK_Right: mychar = 'L'; break;
    case XK_Left:  mychar = 'H'; break;
    case XK_Help:  mychar = 'h'; break;
    case XK_Prior: mychar = '+'; break;
    case XK_Next: mychar = '-'; break;
    case XK_Home: num0=0; mychar = '\r'; break;
    case XK_End:  num0=numPixmaps-1; mychar = '\r'; break;
    default: 
      mychar = 0; 
#ifdef DEBUG
      fprintf(stderr,"Received key %x\n", XLookupKeysym(&event->xkey, 0));
#endif /* DEBUG */
      break;
    }
    keybuf[0] = mychar;
    if (mychar || XLookupString(&(event->xkey), keybuf, 32, 0, 0) > 0) 
      switch (keybuf[0]) {
      case 'f': {
	char *fltname = (char *) ck_malloc(strlen(file_info[0].fname)+8,
					   "fltname"), 
	*fltend = fltname;
	FILE *fltfile;
	int i;
	char buf[256];
	strcpy(fltname, file_info[0].fname);
	fltname += strlen(fltname)-1;
	while (fltname > fltend) /* Discard absolute path */
	  if (*(--fltname) == '/') { fltname++; break; }
	if ((fltend = strrchr(fltname, '.'))) {
	  if (DisplayType != t_simple)
	    sprintf(fltend,"_%02d%1s.flt",currentPix,
		    DispString[DisplayType]);
	  else
	    sprintf(fltend,"_%02d.flt",currentPix);
	  fltfile = fopen(fltname,"w");
	  { /* WRITE A flt HEADER COMPATIBLE WITH SEG */
	    long Nlong = (long)N; long Mlong = (long)M; long zerolong = 0;
	    fwrite(&Mlong, sizeof(long), 1, fltfile); 
	    for (i=1; i< 32; i++) fwrite(&zerolong, sizeof(long),1,fltfile);
	    fwrite(&Nlong, sizeof(long), 1, fltfile); 
	    for (i=33;i<128; i++) fwrite(&zerolong,sizeof(long),1,fltfile);
	  }
	  for (i=0; i < NM; i++) {
	    float val;
	    switch (DisplayType) {
	    case t_simple:    val=input[currentPix%numImages][i]; break;
	    case t_magnitude: val=MCabs(cinput[currentPix%numImages][i]);break;
	    case t_phase:     val=MCarg(cinput[currentPix%numImages][i]);break;
	    case t_real:      val = cinput[currentPix%numImages][i].r;   break;
	    case t_imaginary:val = cinput[currentPix%numImages][i].i;    break;
	    }
	    fwrite(&val, sizeof(float), 1, fltfile);
	  }
	  fclose(fltfile);
	  sprintf(buf,"Wrote %s (current dir)",fltname);
	  show_info(3,buf);
	}
      } break;
      case 'a':
	Animating = !Animating;
	if (Animating) {
	  if (setitimer(ITIMER_REAL, &timerstart, 0))
	    { fprintf(stderr,"ITIMER_REAL error\n"); exit(-1); }
	  BusyFlag=True;
	} else BusyFlag=False;
	break;
      case 'b':
	if (DisplayType != t_simple) {
	  show_info(3,"Sorry, can't interpolate complex data.");
	  Interpolating = False;
	} else {
	  Interpolating = !Interpolating;
	  askto_load_images(loadedMin, loadedMax, False);
	  if (Interpolating)
	    show_info(3,"Computing bilinear interpolation.");
	  else
	    show_info(3,"Standard pixel replication.");
	  redisplay();
	  if (Interpolating)
	    show_info(3,"Bilinear interpolation on.");
	  else
	    show_info(3,"Standard pixel replication.");
	} break;
      case ' ':
	ShowOvl = !ShowOvl;
	if ((ClipBackground = !ClipBackground))
	  XSetClipMask(dpy, imageGC, ovl_clip_pix);
	else
	  XSetClipMask(dpy, imageGC, None);
	XClearWindow(dpy, map_win);
	redisplay();
	if (ShowOvl)
	  show_info(3, "Displaying overlay pixels.");
	else
	  show_info(3, "Overlay display off; masking background.");
	num0=0;
	break;
      case 'x': { /* Set ROI members */
	unsigned char *ovl;
	if (!ShowOvl) break;
	xRoiCorner = x/zoomx; 
	yRoiCorner = y/zoomy;
	
	ovl = &overlays[xRoiCorner+yRoiCorner*N];
	if (*ovl !=  (*ovl | OvlMask)) {  /* OR to SET*/
	  *ovl |= OvlMask;
	  update_clip(False);
	  redisplay()
	    ;
	}
	num0=0;
      } break;
      case 'X': /* Toggle ROI members */
	if (!ShowOvl) break;
	xRoiCorner = x/zoomx; 
	yRoiCorner = y/zoomy;
	overlays[xRoiCorner+yRoiCorner*N] ^= OvlMask;/* XOR to toggle*/
	update_clip(False);
	redisplay();
	num0=0;
	break;
      case 'c': { /* Clear ROI members */ 
	unsigned char *ovl;
	if (!ShowOvl) break;
	xRoiCorner = x/zoomx;
	yRoiCorner = y/zoomy;
	ovl = &overlays[xRoiCorner+yRoiCorner*N];
	if (*ovl != (*ovl & ~OvlMask)) { /* AND NOT to CLEAR */
	  *ovl &= ~OvlMask;
	  update_clip(False);
	  redisplay();
	}
	num0=0;
      } break;
      case 'C': { /* Clear entire  ROI */
	int i; 
	if (!ShowOvl) break;
	for (i = 0; i < NM; i++) overlays[i] &= ~OvlMask;
	update_clip(False);
	redisplay();
	num0=0;
      } break;
      case 'S': { /* Be sure to exclude NaN's and Infs! */
	/* Compute statistics on the current image */
	double sum = 0.0, sumsq = 0.0;
	int n = 0, i, j, nNaN=0, nInf=0;
	for (i = 0; i < N; i++)
	  for (j = 0; j < M; j++)
	    if (overlays[i+j*N]&OvlMask) {
		double point;
		switch (DisplayType) {
		case t_simple:     point = input_rows[currentPix%numImages][j][i]; 
		  break;
		case t_magnitude: point=MCabs(cinput_rows[currentPix%numImages][j][i]); 
		  break;
		case t_phase:    point = MCarg(cinput_rows[currentPix%numImages][j][i]);
		  break;
		case t_real:       point = cinput_rows[currentPix%numImages][j][i].r; 
		  break;
		case t_imaginary: point = cinput_rows[currentPix%numImages][j][i].i; 
		  break;
		}
		if (finite(point)) {
		  n++; sum += point; sumsq += point*point;
		} else if (isnan(point)) nNaN++;
		else /*infinity*/ nInf++;
	      }
	sum /= n; sumsq /=n; 
	if (!Quiet) {
	  FILE *scriptfile = 
	    scriptfilename ? fopen(scriptfilename,"a") : stdout;
	  if (scriptfile)
	    fprintf(scriptfile,
		    "Image=%d N=%d Ave=%g Stdev=%g #Inf=%d #NaN=%d\n", 
		    currentPix, n, sum, sqrt(sumsq-sum*sum), nInf, nNaN);
	  if (scriptfilename && scriptfile) fclose(scriptfile);
	} else show_info(3,"Quiet mode: no stats saved.");
	num0=0;
      } break;
      case 'V': { /* Be sure to exclude NaN's and Infs! */
	/* Print values in ROI on the current image */
	double sum = 0.0, sumsq = 0.0;
	int n = 0, i, j, nNaN=0, nInf=0;
	FILE *scriptfile;
	if (!Quiet) {
	  scriptfile =  scriptfilename ? fopen(scriptfilename,"a") : stdout;
	}
	for (i = 0; i < N; i++)
	  for (j = 0; j < M; j++)
	    if (overlays[i+j*N]&OvlMask) {
		double point;
		switch (DisplayType) {
		case t_simple:     point = input_rows[currentPix%numImages][j][i]; 
		  break;
		case t_magnitude: point=MCabs(cinput_rows[currentPix%numImages][j][i]); 
		  break;
		case t_phase:    point = MCarg(cinput_rows[currentPix%numImages][j][i]);
		  break;
		case t_real:       point = cinput_rows[currentPix%numImages][j][i].r; 
		  break;
		case t_imaginary: point = cinput_rows[currentPix%numImages][j][i].i; 
		  break;
		}
		if (finite(point)) {
		  n++; sum += point; sumsq += point*point;
		  if (!Quiet && scriptfile) fprintf(scriptfile,"%g\n",point);
		} else if (isnan(point)) nNaN++;
		else /*infinity*/ nInf++;
	      }
	sum /= n; sumsq /=n; 
	if (!Quiet) {
	  if (scriptfile)
	    fprintf(scriptfile,
		    "Image=%d N=%d Ave=%g Stdev=%g #Inf=%d #NaN=%d\n", 
		    currentPix, n, sum, sqrt(sumsq-sum*sum), nInf, nNaN);
	  if (scriptfilename && scriptfile) fclose(scriptfile);
	} else show_info(3,"Quiet mode: no stats saved.");
	num0=0;
      } break;
      case 's': {
	/* Compute statistics on the entire image Stack */
	int i, j, k;
	double min = ((double)Inf), max= -((double)Inf);
	FILE *scriptfile =
	  scriptfilename ? fopen(scriptfilename,"a") : stdout;
	if (!Quiet)
	  fprintf(scriptfile,
		  "Image\tNpts\tAverage  \tStd Dev  \tInf's  \tNaN's\n");
	else show_info(3,"Quiet mode: no stats saved.");
	for (k=0; k < numPixmaps; k++) {
	  double sum = 0.0, sumsq = 0.0, sdev = 0.0;
	  int n=0, nNaN=0, nInf=0;
	  if (data_dirty&loadImageFlags[k]) {  /* Not loaded! */
	    n=0; sum=0; sumsq=0; sdev=0; nInf=0; nNaN=0;
	  } else {
	    for (i = 0; i < N; i++)
	      for (j = 0; j < M; j++)
		if (overlays[i+j*N]&OvlMask) {
		  double point;
		  if (ActivationGraph) 
		    point = activationmap[k%NumActivationMaps][j/ActivationZoomMore][i/ActivationZoomMore];
		  else switch (DisplayType) {
		  case t_simple:   point = input_rows[k%numImages][j][i]; break;
		  case t_magnitude:point =MCabs(cinput_rows[k%numImages][j][i]);break;
		  case t_phase:    point =MCarg(cinput_rows[k%numImages][j][i]);break;
		  case t_real:     point = cinput_rows[k%numImages][j][i].r; break;
		  case t_imaginary: point = cinput_rows[k%numImages][j][i].i; break;
		  }
		  if (finite(point)) {
		    n++; sum += point; sumsq += point*point;
		  } else if (isnan(point)) nNaN++;
		  else /*infinity*/ nInf++;
		}
	    sum /= n; sumsq /=n; 
	    sdev = sqrt(sumsq - sum*sum);
	  }
	  if (!Quiet && scriptfile)
	    fprintf(scriptfile,"%d\t%d\t%g\t%g\t%d\t%d\n",k, n, sum, 
		    sdev, nInf, nNaN);
	  graphMeans[k] = sum;
	  graphSdevs[k] = sdev;
	  if (DontShowDevs) sdev = 0;
	  if (sum+sdev > max) max = sum+sdev;
	  if (sum-sdev < min) min = sum-sdev;
	}
	
	/* Set graph scale and graph offset */
	graphMax = max;
	graphMin = Percent ? 0 : min;
	if (!DontShowDevs) ShowingDevs = True;
	DynamicGraph = False;
	redisplay_graph(False);
	num0=0;
	if (scriptfilename && scriptfile) fclose(scriptfile);
      } break;
      case 'O': {  /* Write overlay file */
	int i, j;
	FILE *ovlfile;
	if (OtlMode) { show_info(3,"Don't know how to write .otl!"); break; }
	ovlfile = fopen(ovlfilename,"w");
	if (!ovlfile) {
	  char buf[256];
	  sprintf(buf,"Couldn't open '%s' for writing!\n",ovlfilename);
	  XBell(dpy, 0);
	  show_info(3,buf);
	  break;
	}
	for (j=0; j<M; j++) {
	  Bool here=False;
	  for (i=0; i<N; i++) 
	    if (overlays[i+j*N]&OvlMask) {
	      fprintf(ovlfile,"%d %d\t",i,j); here = True;
	    }
	  if (here) fprintf(ovlfile,"\n");
	}
	fclose(ovlfile);
	if (!Quiet)fprintf(stderr,"Wrote overlay to file '%s'.\n",ovlfilename);
	show_info(3, "Wrote overlay.");
	num0=0;
      } break;
      case 'z': { /* Set overlay mask to number (one only) */
	int i; 
	char buf[256], *bp = buf;
	if (num0 < NOVERLAYS) {
	  OvlMask = (1<<num0);
	  XSetForeground(dpy, ovlGC, OVERLAY_PIXEL0+num0);
	  XSetForeground(dpy, ovl_clippedGC, OVERLAY_PIXEL0+num0);
	}
	num0 = 0;
	strcpy(buf, "Using Overlay(s): "); bp += strlen(buf)-1;
	for (i = 0; i < NOVERLAYS; i++) 
	  if ((1<<i)&OvlMask) { sprintf(bp,"%d ",i); bp+=2; }
	update_clip(False);
	redisplay();
	show_info(3, buf);
      } break;
      case 'Z': { /* Toggle given overlay mask into active overlay set.*/
	int i;
	char buf[256], *bp = buf;
	if (num0 < NOVERLAYS) {
	  OvlMask ^= (1<<num0);
	  XSetForeground(dpy, ovlGC, OVERLAY_PIXEL0+num0);
	  XSetForeground(dpy, ovl_clippedGC, OVERLAY_PIXEL0+num0);
	}
	num0 = 0;
	strcpy(buf, "Using Overlay(s): "); bp += strlen(buf)-1;
	for (i = 0; i < NOVERLAYS; i++) 
	  if ((1<<i)&OvlMask) { sprintf(bp,"%d ",i); bp+=2; }
	update_clip(False);
	redisplay();
	show_info(3, buf);
      } break;
      case 'A': /* Activate/Deactivate spot color overlay */
	ShowSpotColor = !ShowSpotColor;
	askto_load_images(loadedMin, loadedMax, False);
	redisplay();
	if (ShowSpotColor) {
	  char buf[256];
	  sprintf(buf,"Showing activation map, blue p=%g, yellow p=%g",
		  SpotcolorLowCutoff, SpotcolorHighCutoff);
	  show_info(3,buf);
	} else
	  show_info(3,"Not showing activation map.");
	break;
      case 'A'-'@': { /*Control-A*/
	/* Change cutoff for spot color (activation) map */
	char buf[256];
	Spotcolor0Cutoff = num0; num0=0;
	ShowSpotColor = True;
	askto_load_images(loadedMin, loadedMax, False);
	zoom_activationmap();
	redisplay();
	sprintf(buf,"Showing activation map, min = %g", Spotcolor0Cutoff);
	show_info(3,buf);
      } break;
      case 'H'-'@': /* Control-H */
	SpotcolorHighCutoff = num0; num0=0;
	askto_load_images(loadedMin, loadedMax, False);
	zoom_activationmap();
	redisplay();
	break;
      case 'L'-'@': /* Control-L */
	SpotcolorLowCutoff = Spotcolor0Cutoff = num0; num0=0;
	askto_load_images(loadedMin, loadedMax, False);
	zoom_activationmap();
	redisplay();
	break;
      case 'T'-'@': /* Control-T */
	ActivationTails = (ActivationTails % 3) + 1; /* 1, 2, 3, 1, 2, 3 ... */
	askto_load_images(loadedMin, loadedMax, False);
	zoom_activationmap();
	redisplay();
	switch (ActivationTails) {
	case 1: show_info(3,"Showing positive activation tail"); break;
	case 2:	show_info(3,"Showing negative activation tail"); break;
	case 3: show_info(3,"Showing both (+/-) activation tails"); break;
	} break;
      case 'o': { /* Read overlay file (xor into current overlay) */
	int x=0, y=0, n;
	Bool end = False;
	char *buf;
	if (OtlMode) { /* Read .otl file */
#ifdef NO_READ_OTL
	  fprintf(stderr,"Sorry, no read_otl() available for this machine.\n");
#else
	  unsigned char *otl_raster = (unsigned char *) ck_malloc(NM,"otl_raster");
	  unsigned int otl_factor;
	  read_otl(ovlfilename, "", M, N, &otl_raster, (int *) &otl_factor);
	  otl_factor /= 2;
	  for (n=0; n < NM; n++) 
	    if (otl_raster[n] > otl_factor) overlays[n] |= OvlMask;
	  buf = "Done loading .otl file.";
#endif /* NO_READ_OTL */
	} else { /* Read (my ASCII format) overlay file */
	  FILE *ovlfile = fopen(ovlfilename,"r");
	  if (!ovlfile) {
	    fprintf(stderr,"Couldn't find input file '%s'.\n",ovlfilename);
	    break;
	  }
	  while (!end) {
	    if ((n=fscanf(ovlfile,"%d %d", &x, &y)) == EOF) {
	      if (!Quiet) fprintf(stderr,"Done loading ovlfile from file '%s'.\n",
				  ovlfilename);
	      buf = "Done loading ovl file.";
	      end = True;
	    } else {
	      if (n != 2) {
		fprintf(stderr,"Problem reading ovlfile.\n");
		buf = "Problem reading ovlfile.";
		end = True;
	      }
	      if (x >= N || y >= M) {
		fprintf(stderr,"WARNING! Bad ovl file data input.\n");
		buf = "WARNING! Bad ovl file data input.";
	      } else {
		overlays[x+y*N] |= OvlMask;
	      }
	    }
	  }
	  fclose(ovlfile);
	}
	update_clip(False);
	redisplay();
	show_info(3, buf);
	num0=0;
      } break;
      case '+':
      case '=': /* So you don't have to shift it */
	Animating = False;
	BusyFlag=False;
	currentPix = (currentPix + (num0 ? num0 : 1))%numPixmaps;
	redisplay();
	num0 = 0;
	break;
      case '-':
	Animating = False;
	BusyFlag=False;
	currentPix = (currentPix + numPixmaps - (num0 ? num0 : 1))%numPixmaps;
	redisplay();
	num0 = 0;
	break;
      case 'R': { 
	/* Change to reference image mode: This image is ref, num0 is type of 
	 *  reference (none, difference, ratio, -logratio).
	 */ 
	int i;
	char buf[256];
	int prev_ref = ReferenceType;

	if (num0 >= 4) { show_info(3, "Reference type 0-3!"); num0=0; break; }
	if (!refImage) {
	  refImage=(float*)ck_malloc(NM*sizeof(float),"refImage");
	  crefImage = (fcomplex *)refImage;
	}
	ReferenceType = ref_none;
	loadImageFlags[currentPix] |= data_dirty;
	askfor_image(currentPix); /*Uses loadImageFlags and ReferenceType*/
	ReferenceType = prev_ref;
	if (DisplayType == t_simple)
	  for (i=0; i<NM; i++) refImage[i] = input[currentPix%numImages][i];
	else for (i=0; i<NM; i++) crefImage[i] = cinput[currentPix%numImages][i];
	ReferenceType = num0;
	cmap_gray_count = PRISTINE_GRAY_COUNT;
	cmap_gray_center = PRISTINE_GRAY_CENTER;
	mk_dirty(data_dirty);
	askto_load_images(100, -100, False); /* Flag to set to new data */
	load_colormap(cmap_gray_count, cmap_gray_center);
	if (ReferenceType) sprintf(buf,"%s reference image: #%02d", 
				   ReferenceString[ReferenceType], currentPix);
	else strcpy(buf,"Displaying natural data (no reference image)");
	currentPix=(currentPix+1)%numPixmaps;/* Bump up so we don't see 0 */
	redisplay();
	show_info(3, buf);
	num0=0;
      } break;
      case 'p': {/* Photo mode: make everything else black until hit p again */
	/* Not done yet. */
      } break;
      case 'P': /* Reload data in its "pristine" configuration */
	cmap_gray_count = PRISTINE_GRAY_COUNT;
	cmap_gray_center = PRISTINE_GRAY_CENTER;
	loadedMin = dataMin;
	loadedMax = dataMax;
	if (!ActivationGraph) {
	  graphMin = Percent ? 0 : loadedMin; graphMax = loadedMax;
	} else {
	  graphMin = Percent ? 0 : activationMin; graphMax = activationMax;
	}
	redisplay_graph(False);
	/* Fall through to case 'r': */
      case 'r': { /* Reload data based on new window & level */
	const double slope=cmap_gray_count/(double)NGRAYS, 
	intercept = -slope*((double)cmap_gray_center
			    -(double)cmap_gray_count/(double)2);
	const double imin = -(double)intercept/slope, 
	             imax = imin + (double)cmap_gray_count;
	const double newmin = (loadedMax-loadedMin)*imin/(double)NGRAYS + loadedMin,
	             newmax = (loadedMax-loadedMin)*imax/(double)NGRAYS + loadedMin;
	/* Reset to best colormap on reloaded data to match the chosen 
	 * windowing and levelling.
	 */
	cmap_gray_count = PRISTINE_GRAY_COUNT;
	cmap_gray_center = PRISTINE_GRAY_CENTER;
	(void)load_colormap(cmap_gray_count, cmap_gray_center);
	LoadedLockVal=True; /* Don't update loadedMin/loadedMax any more */
	askto_load_images(newmin, newmax, False);
	redisplay();
	num0=0;
      } break;
/* AGS CODE BEGIN */
      case 'd': {
        int xoffset;
        char buf[256];
        int yoffset;
        XKeyEvent yn_event;
        xRoi = x/zoomx; 
        yRoi = y/zoomy;
        BusyFlag=True;
        show_info(0, "input offset (0-9)");
        do XMaskEvent(dpy, KeyPressMask, (XEvent *)&yn_event);
          while (yn_event.type != KeyPress);
        XLookupString(&yn_event, keybuf, 32, 0, 0);
        if ('0' <= keybuf[0] && keybuf[0] <= '9')
          xoffset = yoffset = keybuf[0] - '0';
        else {
          XBell(dpy, 0);
          break;
        }
        sprintf(buf, "offset is %d", xoffset);
        show_info(0,buf);
        BusyFlag=False;
        xRoiCorner = xRoi + xoffset;
        yRoiCorner = yRoi + yoffset;
        /* Keep within bounds of image, with Pt at u.l., Corner at l.r. */
        if (xRoiCorner < 0) xRoiCorner = 0; 
        else if (xRoiCorner >= N) xRoiCorner = N-1;
        if (yRoiCorner < 0) yRoiCorner = 0; 
        else if (yRoiCorner >= M) yRoiCorner = M-1;
        if (xRoi > xRoiCorner) 
          {int tmp = xRoi; xRoi = xRoiCorner; xRoiCorner = tmp;}
        if (yRoi > yRoiCorner) 
          {int tmp = yRoi; yRoi = yRoiCorner; yRoiCorner = tmp;}
        { int i,j;
          for (i = xRoi; i <= xRoiCorner; i++)
            for (j = yRoi; j <= yRoiCorner; j++)
              overlays[i+j*N] ^= OvlMask;
        }
        update_clip(False);
        redisplay();
      } break;	/* AGS CODE END */
      case 'H': { /* move left */
        int i, j; 
        if (!ShowOvl) break;
        for (i = 0; i < NM; i++) temp_overlays[i] = overlays[i]&OvlMask; 
        for (i = 0; i < NM; i++) { 
            if(i==NM-1) j = 0; else j = i+1;  /* shift left */
            overlays[i] = (overlays[i] & ~OvlMask) | temp_overlays[j]; 
        }
        update_clip(False);
        redisplay();
        } break;
      case 'J': { /* move down */
        int i, j; 
        if (!ShowOvl) break;
        for (i = 0; i < NM; i++) temp_overlays[i] = overlays[i]&OvlMask; 
        for (i = 0; i < NM; i++) { 
            if(i<N) j = NM + i-N;  else j = i-N;  /* shift down */
            overlays[i] = (overlays[i] & ~OvlMask) | temp_overlays[j]; 
        }
        update_clip(False);
        redisplay();
        } break;
      case 'K': { /* move up */
        int i, j; 
        if (!ShowOvl) break;
	for (i = 0; i < NM; i++) temp_overlays[i] = overlays[i]&OvlMask; 
	for (i = 0; i < NM; i++) { 
	  if(i>=NM-N) j = i+N - NM; else j = i+N;  /* shift up */
	  overlays[i] = (overlays[i] & ~OvlMask) | temp_overlays[j]; 
        }
        update_clip(False);
        redisplay();
      } break;
      case 'L': { /* move right */
        int i, j; 
        if (!ShowOvl) break;
        for (i = 0; i < NM; i++) temp_overlays[i] = overlays[i]&OvlMask; 
        for (i = 0; i < NM; i++) { 
	  if(i==0) j = NM-1 ; else j = i-1;  /* shift right */
	  overlays[i] = (overlays[i] & ~OvlMask) | temp_overlays[j]; 
        }
        update_clip(False);
        redisplay();
      } break;
      case 'n':
	ShowCurNumber = !ShowCurNumber;
	redisplay();
	num0=0;
	break;
      case 'N': {
	register double min=Inf, max=-Inf, tmp;
	int k;
	/* Make sigmas go away. */
	DontShowDevs = !DontShowDevs;
	ShowingDevs = !DontShowDevs && !DynamicGraph;
	/* Compute new graphMin, graphMax */
	if (ShowingDevs) 
	  for (k=0; k<numPixmaps; k++) {
	    if ((tmp=graphMeans[k]+graphSdevs[k]) > max) max = tmp;
	    if ((tmp=graphMeans[k]-graphSdevs[k]) < min) min = tmp;
	  }
	else
	  for (k=0; k<numPixmaps; k++) {
	    if (graphMeans[k] > max) max = graphMeans[k];
	    if (graphMeans[k] < min) min = graphMeans[k];
	  }
	graphMin = Percent ? 0 : min;
	graphMax = max;
	  if (DynamicGraph && !DontShowDevs) {
	    if (ActivationGraph) {
	      graphMin = Percent ? 0 : activationMin; graphMax = activationMax;
	    } else {
	      graphMin = Percent ? 0 : loadedMin; graphMax = loadedMax;
	    }
	  }
	redisplay_graph(False);
	num0=0;
      } break;
      case '%': { /*AGS*/
 	register double min=Inf, max=-Inf, tmp;
 	int k;
 	Percent = !Percent;
 	if (Percent) graphMin = 0.0;
	else {	  
	  if (ShowingDevs)
	    for (k=0; k<numImages; k++) {
	      if ((tmp=graphMeans[k]+graphSdevs[k]) > max) max = tmp;
	      if ((tmp=graphMeans[k]-graphSdevs[k]) < min) min = tmp;
	    } else for (k=0; k<numImages; k++) {
	      if (graphMeans[k] > max) max = graphMeans[k];
	      if (graphMeans[k] < min) min = graphMeans[k];
	    }
	  graphMin = min;
	  graphMax = max;
	}
 	redisplay_graph(False);
 	num0=0;
      }	break;
      case 'g':
	if (numPixmaps == 1) 
	  { show_info(3, "Too few images for time course."); break; }
	DynamicGraph = !DynamicGraph;
	ShowingDevs = False;
	if (!DontShowDevs) {
	  if (ActivationGraph) {
	    graphMin = Percent ? 0 : activationMin; graphMax = activationMax;
	  } else {
	    graphMin = Percent ? 0 : loadedMin; graphMax = loadedMax;
	  }
	}
	XMapWindow(dpy, graph_win);
	redisplay_graph(True);
	num0=0;
	break;
      case '\r':
	if (num0 < 0 || num0 >= numPixmaps) {
	  char buf[256];
	  XBell(dpy, 0);
	  sprintf(buf,"Image # %d doesn't exist!",num0);
	  show_info(3, buf);
	} else {
	  Animating = False;
	  BusyFlag=False;
	  currentPix = num0;
	  redisplay();
	}
	XSync(dpy, False);
	num0 = 0;
	break;
      case 'D':
	if (num0 < 0 || num0 > 4  || num0 == DisplayType) XBell(dpy, 0);
	else {
	  if (DisplayType == t_simple && num0 != t_simple) {
	    zoomx *= 2; N /= 2; 
	  } else if (DisplayType != t_simple && num0 == t_simple) {
	    /* Halve the zooming or double the image size */ 
	    if (zoomx/2 > 0) { /* Halve the zoom */
	      zoomx /= 2; N *= 2; 
	    } else {
	      /* DOUBLE THE IMAGE SIZE! ||| */
	      static char *message = 
		"Resize image up before converting to simple  Display!";
	      show_info(3, message);
	      XBell(dpy, 0);
	      num0=0;
	      break;
	    }
	  }
	  NM = N*M;
	  Nzoom = N*zoomx; Mzoom = M*zoomy; NMzoom = Nzoom*Mzoom;
	  zoomxy = zoomx*zoomy;
	  Nzoomxy = N*zoomxy;
	  DisplayType = num0;
	  Animating = False;
	  BusyFlag=False;
	  cmap_gray_count = PRISTINE_GRAY_COUNT;
	  cmap_gray_center = PRISTINE_GRAY_CENTER;
	  (void) load_colormap(cmap_gray_count, cmap_gray_center);	    
	  askto_load_images(100, -100, False); /* min > max is another flag */
	}
	num0=0;
	break;

      case 'M':	/* Additional AGS code */
	DynamicColorTable = !DynamicColorTable;
	(void) load_colormap(cmap_gray_count, cmap_gray_center);      
	num0=0;
	break;/* End additional AGS code */
      case 'Q':
	Quiet = !Quiet;
	num0=0;
	break;
      case 'q': {
	XKeyEvent yn_event;
	BusyFlag=True;
	show_info(0, "Really Quit? [ny]");
	do XMaskEvent(dpy, KeyPressMask, (XEvent *)&yn_event);
	  while (yn_event.type != KeyPress);
	XLookupString(&yn_event, keybuf, 32, 0, 0);
	if (keybuf[0] == 'y' || keybuf[0] == 'Y') {
	  show_info(0,"Bye!");
	  XSync(dpy, True);
	  Running = False;
	}
	else show_info(0,"continuing...");
	BusyFlag=False;
      } break;
      case '?':
      case 'h':
	fprintf(stderr,"Command Line:\n------------\n");
	fprintf(stderr, Command_Usage,CmdName);
	fprintf(stderr,"Key and pointer usage:\n---------------------\n");
	fprintf(stderr,Key_Pointer_Usage);
	num0=0;
	break;
      case 'W': {
	double min = (double)Inf, max = -(double)Inf, val;
	int i, j;
	Bool emptyOverlays=True;
	char buf[256];
	for (i=0; i<NM; i++) 
	  if (overlays[i]&OvlMask) {emptyOverlays=False; break;}
	for (j=0; j < M; j++) 
	  for (i=0; i < N; i++)
	    if (emptyOverlays || overlays[i+j*N]&OvlMask) {
 	      switch (DisplayType) {
	      case t_simple:  val = input_rows[currentPix%numImages][j][i]; break;
	      case t_magnitude:val=MCabs(cinput_rows[currentPix%numImages][j][i]);break;
	      case t_phase:    val=MCarg(cinput_rows[currentPix%numImages][j][i]);break;
	      case t_real:      val = cinput_rows[currentPix%numImages][j][i].r; break;
	      case t_imaginary:val = cinput_rows[currentPix%numImages][j][i].i; break;
	      }
	      if (finite(val)) { if (min>val) min=val; if (max<val) max=val; }
	    }
	if (min <  max) {
	  cmap_gray_count = PRISTINE_GRAY_COUNT;
	  cmap_gray_center = PRISTINE_GRAY_CENTER;
	  load_one_image(currentPix, min, max);
	  load_colormap(cmap_gray_count, cmap_gray_center);
	  redisplay();
	  sprintf(buf,"Autoloaded image: window=%.3g, level=%.3g",
		  max-min, max+min/2);
	  show_info(3, buf);
	}
	num0=0;
      } break;
      case 'w': {
	double min = Inf, max = -Inf, val;
	Bool emptyOverlays=True;
	int i, j;
	char buf[256];
	for (i=0; i<NM; i++) 
	  if (overlays[i]&OvlMask) {emptyOverlays=False; break;}
	for (j=0; j < M; j++) 
	  for (i=0; i < N; i++)
	    if (emptyOverlays || (overlays[i+j*N]&OvlMask)) {
	      switch (DisplayType) {
	      case t_simple:  val = input_rows[currentPix%numImages][j][i]; break;
	      case t_magnitude:val=MCabs(cinput_rows[currentPix%numImages][j][i]);break;
	      case t_phase:    val=MCarg(cinput_rows[currentPix%numImages][j][i]);break;
	      case t_real:      val = cinput_rows[currentPix%numImages][j][i].r; break;
	      case t_imaginary:val = cinput_rows[currentPix%numImages][j][i].i; break;
	      }
	      if (finite(val)) { if (min>val) min=val; if (max<val) max=val; }
	    }
	if (min < max) {
	  cmap_gray_count = PRISTINE_GRAY_COUNT;
	  cmap_gray_center = PRISTINE_GRAY_CENTER;
	  LoadedLockVal=True;/* Don't update loadedMin/loadedMax any more*/
	  askto_load_images(min, max, False);
	  load_colormap(cmap_gray_count, cmap_gray_center);
	  redisplay();
	  sprintf(buf,"Autoload image stack: window=%.3g, level=%.3g",
		  max-min, max+min/2);
	  show_info(3, buf);
	}
	num0=0;
      } break;
      default:
	if ('0' <= keybuf[0] && keybuf[0] <= '9')
	  num0 = 10*num0 + keybuf[0] - '0';
	else {
	  XBell(dpy, 0);
#ifdef DEBUG
	  fprintf(stderr,"Pressed %s == %x\n", keybuf, keybuf[0]);
#endif /*DEBUG*/
	  num0 = 0;
	} break;
      } /* END OF switch (keybuf[0]) */
  } break; /* END OF case KeyPress: */
  case ButtonPress:
    BusyFlag = True;
    switch (event->xbutton.button) {
    case 1: 
      if (ShowOvl && OvlMask) {
	Roistretch = True;
	/* Stretch a rubber band, on button up then average and report */
	xRoi = x/zoomx; /* save in image zoomed coords */
	yRoi = y/zoomy;
	oldxRoi = -1;
      }
      break; 
    case 2:
      Levelling = True;
      /* Locate button and save coords */
      xLevel = x; yLevel = y;
      break;
    case 3:
      Animating = True;
      /* Locate button and save coords */
      xAnimate = x; yAnimate = y;
      if (setitimer(ITIMER_REAL, &timerstart, 0))
	{ fprintf(stderr,"ITIMER_REAL error\n"); exit(-1); }
      break;
    default:
      fprintf(stderr, "more buttons?  how gauche...\n");
    }
    break; /* END OF case ButtonPress: */
  case ButtonRelease:
    BusyFlag=False;
    switch(event->xbutton.button) {
    case 1:
      if (ShowOvl && OvlMask) {
	Roistretch = False;
	xRoiCorner = x/zoomx;
	yRoiCorner = y/zoomy;
	/* Keep within bounds of image, with Pt at u.l., Corner at l.r. */
	if (xRoiCorner < 0) xRoiCorner = 0; 
	else if (xRoiCorner >= N) xRoiCorner = N-1;
	if (yRoiCorner < 0) yRoiCorner = 0; 
	else if (yRoiCorner >= M) yRoiCorner = M-1;
	if (xRoi > xRoiCorner) 
	  {int tmp = xRoi; xRoi = xRoiCorner; xRoiCorner = tmp;}
	if (yRoi > yRoiCorner) 
	  {int tmp = yRoi; yRoi = yRoiCorner; yRoiCorner = tmp;}
	{ int i,j;
	  for (i = xRoi; i <= xRoiCorner; i++)
	    for (j = yRoi; j <= yRoiCorner; j++)
	      overlays[i+j*N] ^= OvlMask;
	}
	update_clip(False);
	redisplay();
      }
      break;
    case 2: {
      int tmpwin = cmap_gray_count+(int)(((double)x-(double)xLevel)*windowFctr), 
      tmplev = cmap_gray_center+(int)(((double)y-(double)yLevel)*levelFctr);
      char buf[256];
      if (tmpwin < 2) tmpwin = 2;
      else if (tmpwin >= NGRAYS<<1) tmpwin = NGRAYS<<1;
      if (tmplev < 0) tmplev = 0;
      else if (tmplev >= NGRAYS) tmplev = NGRAYS-1;
      (void) load_colormap(tmpwin, tmplev); /* Update while waiting. */
      
      /* Update window and level */
      cmap_gray_count = tmpwin;
      cmap_gray_center = tmplev;
#ifdef NOTDEF
      XWarpPointer(dpy, None, map_win, 0,0,0,0, xLevel,yLevel); /* Put it back */
#endif /*NOTDEF*/
      sprintf(buf,"window= %d pixelvals, level= pixelval %d",
	      cmap_gray_count, cmap_gray_center);
      show_info(3, buf);
      XSync(dpy,False);
      Levelling = False;
    } break;
    case 3:
      Animating = False;
      redisplay();
      break;
    }
    break; /* END OF case ButtonRelease: */
  case MotionNotify: 
    if (Roistretch && OvlMask) {
      xRoiCorner = x/zoomx;
      yRoiCorner = y/zoomy;
      /* Keep within bounds of image, with Pt at u.l., Corner at l.r. */
      if (xRoiCorner < 0) xRoiCorner = 0; 
      else if (xRoiCorner >= N) xRoiCorner = N-1;
      if (yRoiCorner < 0) yRoiCorner = 0; 
      else if (yRoiCorner >= M) yRoiCorner = M-1;
      if (xRoi > xRoiCorner) 
	{int tmp = xRoi; xRoi = xRoiCorner; xRoiCorner = tmp;}
      if (yRoi > yRoiCorner) 
	{int tmp = yRoi; yRoi = yRoiCorner; yRoiCorner = tmp;}
      /* Keep screen updated with a rectangular ROI definition */
      if (oldxRoi != xRoi || oldyRoi != yRoi || oldxRoiCorner != xRoiCorner || 
	  oldyRoiCorner != yRoiCorner) {
	if (oldxRoi != -1 && zoom_pix[currentPix])
	  XCopyArea(dpy, zoom_pix[currentPix], map_win, imageGC, 
		    oldxRoi*zoomx, oldyRoi*zoomy, 
		    (oldxRoiCorner-oldxRoi+1)*zoomx, 
		    (oldyRoiCorner-oldyRoi+1)*zoomy,
		    oldxRoi*zoomx, oldyRoi*zoomy); 
	
	XFillRectangle(dpy, map_win, ovlGC, xRoi*zoomx,yRoi*zoomy, 
		       (xRoiCorner-xRoi+1)*zoomx, (yRoiCorner-yRoi+1)*zoomy); 
      }
      { /* Show current location */
	char buf[256];
	sprintf(buf,"Box [%d,%d]-[%d,%d]",xRoi,yRoi,xRoiCorner,yRoiCorner);
	show_info(0, buf);
      }
      XFlush(dpy);
      oldxRoi = xRoi; oldxRoiCorner = xRoiCorner; 
      oldyRoi = yRoi; oldyRoiCorner = yRoiCorner;
    } else if (Levelling) {
      int tmpwin = cmap_gray_count+(int)(((double)x-(double)xLevel)*windowFctr), 
      tmplev = cmap_gray_center+(int)(((double)y-(double)yLevel)*levelFctr);
      if (tmpwin < 2) tmpwin = 2;
      else if (tmpwin >= NGRAYS<<1) tmpwin = NGRAYS<<1;
      if (tmplev < 0) tmplev = 0;
      else if (tmplev >= NGRAYS) tmplev = NGRAYS-1;
      (void) load_colormap(tmpwin, tmplev);
    } else if (Animating) {
      if (x != xAnimate) {
	struct itimerval timerval;
	static const long longest = 9E5, shortest = 1E4;
	const long cur = timerstart.it_interval.tv_usec-(((x-xAnimate)<<10));
	if (getitimer(ITIMER_REAL, &timerval))
	  { fprintf(stderr,"ITIMER_REAL error\n"); exit(-1); }
	timerstart.it_interval.tv_usec = 
	  (cur > longest) ? longest : ((cur < shortest) ? shortest : cur) ;
	xAnimate = x;
	if (timerval.it_value.tv_usec > 0) /* Don't disable timer! */
	  timerstart.it_value.tv_usec = timerval.it_value.tv_usec;
	else timerstart.it_value.tv_usec = timerstart.it_interval.tv_usec; 
	if (setitimer(ITIMER_REAL, &timerstart, 0))
	  { fprintf(stderr,"ITIMER_REAL error\n"); exit(-1); }
      }
    } else { /* Pointer motion with no buttons down: Report pos & value */
      char buf[256];
      int xx = x < 0 ? 0 : (x >= Nzoom ? N-1 : x/zoomx), 
      yy = y < 0 ? 0 : (y >= Mzoom ? M-1 : y/zoomy);
      int i;
      double scl = NGRAYS/(loadedMax-loadedMin)*0.999, min = loadedMin;
      double dval = input_rows[currentPix%numImages][yy][xx];
      int ival = (dval-min)*scl + BLACK_PIXEL;
      unsigned char cval = (unsigned char)
	(!finite(dval) ? (isnan(dval) ? NAN_PIXEL : INF_PIXEL) : 
	 ((ival<BLACK_PIXEL) ? BLACK_PIXEL : 
	  ((ival>WHITE_PIXEL)?WHITE_PIXEL:ival)));
      if (colormapfilename)
	sprintf(buf,"%02d (%d,%d): data pixelval %g", 
		currentPix, xx, yy,dval);
      else if (activationmap) {
	double mval = activationmap[currentPix%NumActivationMaps][yy/ActivationZoomMore][xx/ActivationZoomMore]; 
	double sgn=1;
	if (mval < 0) { sgn=-sgn; mval = -mval; }
	sprintf(buf,"%02d (%d,%d): %.4g; p = %g", 
		currentPix, xx, yy, dval, sgn*exp(-mval));
      } else if (DisplayType == t_simple) 
	sprintf(buf,"%02d (%d,%d): %.4g (pixelval %d)", currentPix, xx, yy, 
		dval, cval);
      else {
	float r;
	fcomplex c; c = cinput_rows[currentPix%numImages][yy][xx];
	switch (DisplayType) {
	case t_real:       r = c.r; break;
	case t_imaginary: r = c.i; break;
	case t_magnitude:  r = MCabs(c); break;
	case t_phase:      r = MCarg(c); break;
	case t_simple:  /* impossible */ break;
	}
	sprintf(buf,"%02d (%d,%d): %.4g (%.4g + %.4gi %s)", currentPix, 
		xx, yy, r, c.r, c.i, DispString[DisplayType]);
      }
      show_info(0, buf);
      if (DynamicGraph) { /* Display this pixel through time in the plot */
	if (ActivationGraph)
	  for (i=0;i<numPixmaps;i++) 
	    if (loadImageFlags[i]&data_dirty) graphMeans[i] = 0;
	    else graphMeans[i]=activationmap[i%NumActivationMaps][yy/ActivationZoomMore][xx/ActivationZoomMore];
	else switch (DisplayType) {
	case t_simple:
	  for (i=0;i<numImages;i++) 
	    if (loadImageFlags[i]&data_dirty) graphMeans[i] = 0;
	    else graphMeans[i]=input_rows[i][yy][xx];
	  break;
	case t_real: 
	  for (i=0;i<numImages;i++) 
	    if (loadImageFlags[i]&data_dirty) graphMeans[i] = 0;
	    else graphMeans[i]=cinput_rows[i][yy][xx].r;
	  break;
	case t_imaginary: 
	  for (i=0;i<numImages;i++) 
	    if (loadImageFlags[i]&data_dirty) graphMeans[i] = 0;
	    else graphMeans[i]=cinput_rows[i][yy][xx].i;
	  break;
	case t_magnitude: 
	  for (i=0;i<numImages;i++) 
	    if (loadImageFlags[i]&data_dirty) graphMeans[i] = 0;
	    else graphMeans[i]=MCabs(cinput_rows[i][yy][xx]);
	  break;
	case t_phase: 
	  for (i=0;i<numImages;i++) 
	    if (loadImageFlags[i]&data_dirty) graphMeans[i] = 0;
	    else graphMeans[i]=MCarg(cinput_rows[i][yy][xx]);
	  break;
	}
	if (DontShowDevs) {/*Overloaded as flag to show full scale everywhere*/
	  register int k;
	  register double max=-Inf, min=Inf;
	  for (k=0; k<numPixmaps; k++) {
	    if (graphMeans[k] > max) max = graphMeans[k];
	    if (graphMeans[k] < min) min = graphMeans[k];
	  }
	  graphMin = Percent ? 0 : min;
	  graphMax = max;
	}
	redisplay_graph(False);
      }
      if (activationmap && x >= (WinWidth-ColorbarWidth)) { 
	/* Past the right side: report p-val*/
	static char buf[256];
	double mlogp = (((double)y)/Mzoom)
	  *(SpotcolorHighCutoff-SpotcolorLowCutoff) + SpotcolorLowCutoff;
	sprintf(buf,"This color for p=%.3g, -log(p)=%.3g", exp(-mlogp), mlogp);
	show_info(3,buf);
      }
    } break; /* END OF case MotionNotify: */
  case LeaveNotify: {
    char buf[128];
    sprintf(buf,"%02d", currentPix);
    show_info(0,buf);
    redisplay_graph(False);
  } break;
  case Expose:
    update_clip(False);
    redisplay();
    break;
  case ConfigureNotify: {
    /* Compute largest fixed-ratio image which will fit in window. */
    int newzoomx = event->xconfigure.width/N;
    int newzoomy = event->xconfigure.height/M;
    WinWidth = event->xconfigure.width;
    WinHeight = event->xconfigure.height;
    if (newzoomy >= newzoomx*aspect_ratio) newzoomy = newzoomx*aspect_ratio;
    else newzoomx = newzoomy/aspect_ratio;
    if (newzoomx != zoomx || newzoomy != zoomy) {
      static char *buf = "Rescaling image(s), please be patient...";
      zoomx = newzoomx; zoomy = newzoomy;
      Nzoom = N*zoomx; Mzoom = M*zoomy; NMzoom = Nzoom*Mzoom;
      zoomxy = zoomx*zoomy;
      Nzoomxy = N*zoomxy;
      update_clip(True);
      XClearWindow(dpy, map_win);
      show_info(3, buf);
      if (loadedMin != loadedMax) {
	askto_load_images(loadedMin, loadedMax, True);
      }
      redisplay();
    }
  } break;
  case MapNotify:/* Expose does it all */
    break;
  case ReparentNotify: /* ConfigureNotify does it all */
    break;
  case ClientMessage:
    if (event->xclient.data.l[0] == wm_delete_window) {
      show_info(0,"Bye!");
      XSync(dpy, True);
      Running = False;
    }
    break;
  default: ;
#ifdef DEBUG
    fprintf(stderr,"Extra event: type %d \n",event->type);
#endif /*DEBUG*/
  } /* END OF switch(event->type) */
} /* END OF image_event_handler() */

void mk_dirty(char dirtflags) 
{ int i;
  for (i = 0; i < numPixmaps; i++) {
    loadImageFlags[i] |= dirtflags;
    if (dirtflags&data_dirty) graphMeans[i] = graphSdevs[i] = 0;
  }
  DirtyFlag = True;  round_robin = robin_start = currentPix;
}

void askto_load_images(double min, double max, Bool newzoom)
{
  char flgs=image_dirty|(newzoom?pixmap_dirty:0)|(loadImageFlags?0:data_dirty);
  if (!loadImageFlags) 
    loadImageFlags = (char *) ck_calloc(numPixmaps, 1, "loadImageFlags");
  mk_dirty(flgs);
  loadingMin = min; loadingMax = max;
  if (newzoom) zoom_activationmap();
}

/* Load image from file if necessary, scale, send to server if necessary. */
void askfor_image(int inum_request)
{
  int inum = inum_request % numImages; /* image rather than activationmap */
  char flags;
  if ((flags=loadImageFlags[inum_request])) {
    int i;
    if (loadImageFlags[inum]&data_dirty) { /* Load the image from its file */
      struct image im;
      int ans;
      show_status('F'); XFlush(dpy);

      switch (file_info[image_info[inum].filenum].NewFiletype) {
      case ANMR_t:
	ANMR_read_image(file_info[image_info[inum].filenum].fname, &im, 0, 0);
	break;
      case APD2_t:
	APD2_read_image(file_info[image_info[inum].filenum].fname, &im, 
			image_info[inum].imagenum);
	break;
      case ACR_t:
	ACR_read_image(file_info[image_info[inum].filenum].fname, &im, 0, 0);
	break;
      case NotNew_t:
	ans=read_image(file_info[image_info[inum].filenum].fname, 
		       T_FLOAT, &im, image_info[inum].imagenum, 1);
	assert_print(im.version == READ_IMAGE_VERSION,
		     "read_image header incompatible with object code!\n");
	if (ans < 0) { fprintf(stderr,"Error in read_image()\n");exit(-1);}
      }
#ifdef HAVE_MADVISE
      madvise((caddr_t) im.image.c, (size_t) im.rows*im.cols*sizeof(float), 
	      (int) MADV_SEQUENTIAL);
#endif /*HAVE_MADVISE*/
      input[inum] = im.image.f;
      input_rows[inum] = im.row_ptr.f;
      switch (ReferenceType) {
      case ref_none: break;
      case ref_difference:
	if (DisplayType != t_simple) 
	  for (i=0; i<NM; i++) MCsub(cinput[inum][i],cinput[inum][i],
				     crefImage[i]);
	else for (i=0; i<NM; i++) input[inum][i] -= refImage[i];
	break;
      case ref_ratio: 
	if (DisplayType != t_simple) 
	  for (i=0; i<NM; i++) 
	    cinput[inum][i] = FCdiv(cinput[inum][i],crefImage[i]);
	else for (i=0; i<NM; i++) input[inum][i] /= refImage[i];
	break;
      case ref_logratio: 
	if (DisplayType != t_simple) 
	  for (i=0; i<NM; i++) { /* log is Principle Value of the log */
	    fcomplex cval; cval = FCdiv(cinput[inum][i],crefImage[i]);
	    /* The following ||| fixes an sgi compiler bug (was MCarg): */
	    {
	      double  cabs = MCabs(cval), carg = atan2(cval.r, cval.i);
	      if (cabs > 0 && finite(cabs))
		MComplex(cinput[inum][i],-log(cabs), carg);
	      else
		MComplex(cinput[inum][i],Inf, 0);
	    }
	  }
	else for (i=0; i<NM; i++) {
	  double val = (double)input[inum][i]/(double)refImage[i];
	  if (val > 0 && finite(val)) input[inum][i] = -log(val);
	  else input[inum][i] = Inf;
	}
	break;
      } /* End of switch to load referenced data */
      
      if (im.min >= 1E30 || im.max <= -1E30  || DisplayType != t_simple) { 
	/* Find min/max */
	double min = (double)Inf, max = -(double)Inf;
	float *ptr = im.image.f, *ptrend = im.image.f+NM;
	switch (DisplayType) {
	case t_simple:
	  while (ptr < ptrend) { 
	    double val = (double) (*ptr++);
	    if (finite(val)){if (val<min) min= val; if (val>max) max = val; }
	  } break;
	case t_magnitude:
	  while (ptr < ptrend) { 
	    double val = MCabs(*(fcomplex *)ptr);
	    if (finite(val)) { if (val<min)min=val; if (val > max) max = val; }
	    ptr+=2;
	  } break;
	case t_phase: /* Assume we have all phases present. */
	  min = -M_PI/2; max = M_PI/2;
	  break;
	case t_real:
	  while (ptr < ptrend) { 
	    double val = ((fcomplex *)ptr)->r;
	    if (finite(val)) { if (val<min)min=val; if (val>max) max = val; }
	    ptr+=2;
	  } break;
	case t_imaginary:
	  while (ptr < ptrend) { 
	    double val = ((fcomplex *)ptr)->i;
	    if (finite(val)){ if (val<min)min=val; if (val > max) max = val; }
	    ptr+=2;
	  } break;
	}
	im.min=min; im.max=max;
      }
      if (!DataLockVal && im.min < dataMin && finite(im.min)) dataMin = im.min;
      if (!DataLockVal && im.max > dataMax && finite(im.max)) dataMax = im.max;
      flags |= image_dirty; /* Image must be dirty now, not necc pixmap */

      /* NEW try at dynamic updates of these during loading. ||| */
      if (!LoadedLockVal) {
	if (loadingMin > dataMin) 
	  { loadingMin=dataMin; graphMin = Percent ? 0 : loadingMin; }
	if (loadingMax < dataMax) graphMax=loadingMax=dataMax;
      }
    }
    if (loadingMin > loadingMax) { loadingMin=dataMin; loadingMax=dataMax; }

    if (flags&pixmap_dirty) {
      /* Redefine (different sized) pixmap */
      if (zoom_pix[inum_request]) XFreePixmap(dpy, zoom_pix[inum_request]);
      zoom_pix[inum_request] = XCreatePixmap(dpy, root,Nzoom,Mzoom,NPLANES);
      flags |= image_dirty; /* Image must be dirty now */
    }
    if (flags&image_dirty) {/* Load an image from the data into the screen */
      show_status('S'); XFlush(dpy);
      load_one_image(inum_request, loadingMin, loadingMax);
      loadedMin = loadingMin; loadedMax = loadingMax;
    }
    loadImageFlags[inum_request]=0;
  }
} /* END of askfor_image() */

/* On refresh, redisplay the pixmap. */
void redisplay(void)
{ 
  static char buf[128];
  static Bool Redisplaying = False;
  if (Redisplaying) return;
  Redisplaying = True;
  if (loadImageFlags[currentPix]) askfor_image(currentPix);
  if (zoom_pix[currentPix]) /* imageGC may have a clipmask if ClipBackground */
    XCopyArea(dpy, zoom_pix[currentPix], map_win,imageGC,0,0,Nzoom,Mzoom,0,0);
  sprintf(buf, "%02d (%s)", currentPix, DispString[DisplayType]);
  if (ShowCurNumber) show_info(0, buf); else show_info(0,"");
  if (ShowOvl && OvlMask)
    XFillRectangle(dpy, map_win, ovl_clippedGC, 0,0,Nzoom,Mzoom);

  if (ShowCurNumber && graph_win) { /* Current position on time plot */
    int xpos = (currentPix+0.5) * graphWidth/(double)numPixmaps;
    redisplay_graph(False);
    XDrawLine(dpy, graph_win, ovlGC, xpos, 0, xpos, graphHeight);
  }

  if (ShowCurNumber) {
    if (ShowSpotColor && ColorbarWidth)
      XCopyArea(dpy, colorbar_pix, map_win, imageGC, 0, 0, ColorbarWidth, 
		Mzoom, WinWidth-ColorbarWidth,0);
    else XClearArea(dpy, map_win, WinWidth-ColorbarWidth, 0, 
		    ColorbarWidth, WinHeight, False);
  }

  Redisplaying = False; 
  return;
}

void redisplay_graph(Bool rescale_x) 
{
  int k;
  double graphScale = graphHeight/(graphMax-graphMin);
  
  if (rescale_x) {
    double xscl = ((double)graphWidth)/numPixmaps;
    /* Reset x positions */
    for (k = 0; k < numPixmaps; k++)
      graphPoints[k].x = graphSegments[k].x1  =  graphSegments[k].x2 
	= (((double)k) + 0.5)*xscl;
  }
  /* Reset y positions */
  for (k = 0; k < numPixmaps; k++) {
    unsigned int y = graphPoints[k].y 
      = graphHeight - graphScale*(graphMeans[k] - graphMin);
    if (ShowingDevs) {
      graphSegments[k].y1 = y - graphScale*(graphSdevs[k]);
      graphSegments[k].y2 = y + graphScale*(graphSdevs[k]);
    } 
  }
  /* Display */
  XClearWindow(dpy, graph_win);
  if (ShowingDevs)
    XDrawSegments(dpy, graph_win, hilightGC, graphSegments, numPixmaps);
  XDrawLines(dpy, graph_win, stringGC, graphPoints, numPixmaps,CoordModeOrigin);
  { static char buf[160];
    sprintf(buf,"Graph range [%g..%g] (%s)",graphMin,graphMax,
	    DynamicGraph ? (DontShowDevs ? "single pixel" : "global") : "ROI");
    if (ShowCurNumber) show_info_graph(0, buf); else show_info_graph(0,"");
  }
}

