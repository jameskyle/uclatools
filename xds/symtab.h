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
int symCreate (char *prefix, char *filename);
int symAdd (char *prefix, char *filename);
int symCreateString (char *prefix, char *instring);
int symAddString (char *prefix, char *instring);
void symDestroy (void);
int symSearch (char *key, char *format, void *value);

int symGetString (char **string);
