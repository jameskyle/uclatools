/*
** Header:	@(#) error.h 1.4 90/12/03 16:29:17 baker penguin UCB/LBL
**
** Name:	error.h
**
** Purpose:
**		Definitions for function error routines.
**
** Dependencies:
**
*/


extern char	*ProgName;
extern char	*ErrorBuffer;

#define	Perror(str) {fprintf(stderr, "%s: ", ProgName); \
		     perror(str);                       \
		     exit(errno);                       \
                    }
