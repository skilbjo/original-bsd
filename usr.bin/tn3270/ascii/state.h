/*-
 * Copyright (c) 1988, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * %sccs.include.redist.c%
 *
 *	@(#)state.h	8.1 (Berkeley) 06/06/93
 */

#define	INCLUDED_STATE

/* this defines the state structure used by the key mapping routines */


#define	STATE_NULL	-1		/* Falls off edge */
#define	STATE_GOTO	-2		/* GOTO internal state */

#define state	struct State
struct State {
    int		match;		/* character to match */
    int		result;		/* 3270 control code */
    state	*next;		/* next entry in this same state */
    state	*address;	/* if goto, where is next state */
};
