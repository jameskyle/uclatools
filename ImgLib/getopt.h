/*
<html><head><title>getopt.h</title></head><body><pre>

Standard Unix getopt */

extern char *optarg;
extern int optind, opterr;
int getopt(int argc, char **argv, char *optstring);

/* </pre></body></html> */
