char *Command_Usage = "Usage:\n\
%s [-z <zoomfactor>] [-r <aspectratio>] [-hqMPRI] \n\
        [-A <activationfile> ] \n\
	[-v <overlayfile>] [-m <colormapfile>] [-l <logfile>]\n\
	[-L <black_dataval>] [-H <white_dataval>]\n\
	[-s <startimagenum>] [-# <numimages>] [-e <endimagenum>]\n\
	<imagefile.ext> [[-s <n>] [-e <n>] [-# <n>] <file2.ext2>] ...\n\
\n\
  [-b] bilinear interpolation mode.\n\
  [-z <n>] Increase image size by a factor of <n> in linear dimensions.\n\
  [-r <float>] Make the pixels <float> times larger in the vertical direction.\n\
  [-h] Print help information. (Same as 'h' key).\n\
  [-q] Toggle quiet mode (don't print statistics). (Same as 'Q' key).\n\
  -[M]|[P]|[R]|[I] Define data as complex and select mag, phase, real, \n\
       or imaginary. (Same as '1 D', '2 D', '3 D', and '4 D' respectively.)\n\
  [-A <file>] Filename for statistical image (KS_grinder or T_grinder output)\n\
  [-v <file>] Filename for overlay data (ascii x,y pairs) or seg .otl file.\n\
  [-m <file>] Filename to load 256 ascii triples as 0-65535 rgb color map.\n\
              Data file then must vary only [0..255], interpreted as colors.\n\
  [-l <file>] Filename to save log data into (output of 's' command).\n\
  [-W] Auto window and level each window individually (for disparate images).\n\
  [-L <float>],[-H <float>] User-defined data limits to force image contrast.\n\
	      Data outside this range will be displayed as black or white.\n\
  [-s <n>] Start loading at image <n>, where n starts at 0.\n\
  [-# <n>]|[-e <n>] Load the following file for <n> images or at image <n>.\n\
  <imagefile.ext> Image stack filenames, named with appropriate extension;\n\
  Input files must be acceptable to read_image(), or ANMR .im files.\n\
  N.B.  The -s, -e, -# parameters apply only to SUBSEQUENT files, and until\n\
        replaced. e.g. xds -s 0 -# 10 f1.bshort -s1 -#10 f2.bshort\n\
	reads 20 images, but the second -#10 is redundant.\n\
"; /* END of Command_Usage */

char *Key_Pointer_Usage = "\n\
MOUSE ACTIONS:  \n\
Button 1:  Draw rectangle in selected Overlay (for ROI choice).\n\
Button 2:  Grayscale window (move horizontal), Center level (move vertical).\n\
Button 3:  Animate sequence. Move horizontal to change speed.\n\
Button 1 in Graph Window: Move to image corresponding to this time point.\n\
\n\
KEYBOARD ACTIONS:             [<n> signifies a typed integer; default is 0]\n\
N:  Toggle `No Deviations, Local Scale' mode: graph always utilizes full range.\n\
M:  toggle dynamic color lookup table mode. AGS\n\
spacebar:  Toggle showing of overlay vs. masking out area outside ROI.\n\
<n> RETURN: go to image number <n>.\n\\n\
<n> D: Choose display type <n>, where 0=scalars, 1=mag, 2=phase, 3=re, 4=im.\n\
<n> R: Choose reference type <n> with current image as reference point,\n\
       where <n> = 0 for NONE, 1:SUBTRACTION, 2:RATIO, 3: -LOG(RATIO).\n\
f:  Write out current image as a .flt file, with filename <imagefile>_<n>.flt\n\
a:  toggle animation (without the need to hold Button 3 down).\n\
+ or =:  Increment image number and display; also <n> + to inc by <n>.\n\
-:  Decrement image number and display; also <n> - to inc by <n>.\n\
r:  Reload and bin the image stack with current window & level, choosing\n\
    optimal pixel bins for viewing data within the current window & level.\n\
P:  Reload image stack, resetting to the pristine window/level state.\n\
w:  Auto window and level stack to the currently-active overlay region.\n\
W:  Auto window and level the current image separately, not the stack.\n\
n:  Toggle image number display.\n\
g:  Create or toggle activity of time-course graph display.\n\
h:  Print help summary (this page).\n\
Q:  Toggle quiet mode.\n\
q:  Quit (after typing 'y' to verify).\n\
\n\
STATISTICS OPERATIONS: (after reading in a -A stats overlay) \n\
A:  Toggle showing activation overlay\n\
<n> ^L: (control-L) Set low activation threshold (default 4.6, or p=0.01)\n\
<n> ^H: (control-H) Set high activation threshold (default 20)\n\
<n> ^A: (control-A) Change p-value for show-through w/o changing colormap.\n\
^T: (control-T) Cycle through viewing (+, -, both) tails of activation.\n\
\n\
OVERLAY OPERATIONS:                            (Button 1 to draw rectangles)\n\
o:  Load command-line -o file data into the working overlay set.\n\
O:  Rewrite (save) current overlay set data as the -o file in ascii int pairs.\n\
\n\
<n> z:  Set working overlay to overlay <n> (a single overlay, <n> is 0-3).\n\
<n> Z:  Toggle overlay <n> in/out of working set (multiple active overlays).\n\
x:  Set pixel at pointer into the overlay.\n\
X:  Toggle pixel at pointer into or out of the overlay.\n\
c:  Clear pixel at pointer out of the overlay.\n\
C:  Clear the current overlay completely.\n\
    (w and W above also work on ROI if selected.)\n\
d<n>: Mark a square region n by n pixels in size.\n\
HJKL: Move the overlays left,right,up,down.\n\
\n\
s:  Report statistics (mean and stdev) for overlayed area for image stack.\n\
    If graph is showing ('g' command) then show mean and stdev on graph as well\n\
    If not in Quiet mode, prints data in columns to stdout.\n\
S:  Prints overlay stats for current image to stdout if not in Quiet mode.\n\
"; /* END of Key_Pointer_Usage */
