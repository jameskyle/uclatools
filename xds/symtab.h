/*
** Header:	%Z% %M% %I% %E% %U% %Q%
**
** Name:	symtab.h
**
** Purpose:	Definitions for symbol table manipulation.
**
** Dependencies:
**
*/
#include<stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <search.h>
#include <string.h>
#include <memory.h>
#include <sys/types.h>
#include <sys/stat.h>
#include "error.h"

int symCreate (char *prefix, char *filename);
int symAdd (char *prefix, char *filename);
int symCreateString (char *prefix, char *instring);
int symAddString (char *prefix, char *instring);
void symDestroy (void);
int symSearch (char *key, char *format, void *value);
int symGetString (char **string);

char		*symKeyTable;
char		*symValueTable;
char		*symString;
ENTRY		*hsearch();

