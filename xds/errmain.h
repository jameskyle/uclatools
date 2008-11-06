/*
** Header:	@(#) errmain.h 1.5 90/12/03 16:29:32 baker penguin UCB/LBL
**
** Name:	errmain.h
**
** Purpose:
**		Definitions for main error routines.
**
** Dependencies:
**
*/

#ifndef BUFSIZ
#include <stdio.h>
#endif /* BUFSIZ */

char	ProgName[BUFSIZ] = "";
char	ErrorBuffer[BUFSIZ] = "";

#define	Perror(str) {fprintf(stderr, "%s: ", ProgName); \
		     perror(str);                       \
		     exit(errno);                       \
                    }

#define InitError() {(void) strcpy(ProgName, argv[0]);  \
		     errno = -1;                        \
		    }
