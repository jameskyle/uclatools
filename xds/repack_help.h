char *Usage = "\
Repackage an image set.   Usage:\n\
\n\
%s [(options1)] -i <image1.ext> [(options2)] -i <image2.ext> ... \n\
\n\
Images must be in acceptable formats for read_image() (or new anmr .im),\n\
and all the same dimensions.\n\
\n\
Per-input options (supply once per input file):\n\
         [-s skip] to skip images in the input stream.\n\
         [-# numimages] or [-e endimage] Write out a maximum of <numimages>\n\
                                         images or until image # endimage.\n\
These truncate and concatenate in time (stack depth).  If not given, \n\
default is the previous file's values.  A parameter of 0 for -# or -e reads \n\
to end of file.\n\
e.g. '-s 10 -# 1 -i file1.bfloat -s 0 -# 0 -i file2.bfloat -i file3.bfloat'\n\
concatenates 10th image from file1 and entire datasets from file2 and file3.\n\
\n\
Options to truncate in space (supply once only):\n\
         [-x xstart][-y ystart][-c cols][-r rows]\n\
         [-2] take the middle half (of an oversampled image) in horiz dir.\n\
         [-v <overlayfile>] Crop to bounding box of given ovl or .otl file.\n\
\n\
Options to modify brightness values (supply once only):\n\
         [-RIMP] treat as complex input and return real, imag, mag, or phase\n\
         [-l data_min] [-m data_max] crop input data to these limits.\n\
         [-S] Force rescale of output; stretch input to cover output type.\n\
         [-N] Force no rescaling even if output can't represent input data.\n\
         [-D] Decouple the rescaling of individual files in a concatenation.\n\
\n\
General options:\n\
         -o <outfile>   Specifies output file.  Creates file with type\n\
                        drawn from file extension.  Currently, \n\
                        *.anmr, *.flt, and all block formats are supported.\n\
         [-O] Order output same as input (normal except for .im files)\n\
         [-p num_partitions] Write out only one image per num_partitions.\n\
         [-a num_averages ] [-C average_cutoff]  Output one image per\n\
	   num_averages blocks input, averaging the first average_cutoff\n\
	   images in each block.  If -C is absent, all num_averages are \n\
	   averaged.\n\
         [-q] Quiet operation.\n\
         [-h] Show this usage message.\n\
Defaults: No truncation, write bfloat to stdout, entire stacks.\n\
"; /* END of Usage */


