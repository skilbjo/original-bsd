/*-
 * Copyright (c) 1992 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Christos Zoulas of Cornell University.
 *
 * %sccs.include.redist.c%
 *
 *	@(#)chared.h	5.1 (Berkeley) 06/22/92
 */

/*
 * el.chared.h: Character editor interface
 */
#ifndef _h_el_chared
#define _h_el_chared

#include <ctype.h>
#include <string.h>

#include "editline.h"

#define EL_MAXMACRO 10

/*
 * This is a issue of basic "vi" look-and-feel. Defining VI_MOVE works
 * like real vi: i.e. the transition from command<->insert modes moves
 * the cursor.
 *
 * On the other hand we really don't want to move the cursor, because 
 * all the editing commands don't include the character under the cursor.
 * Probably the best fix is to make all the editing commands aware of
 * this fact.
 */
#define VI_MOVE


typedef struct c_macro_t {
    int    level;
    char **macro;
} c_macro_t;

/* 
 * Undo information for both vi and emacs
 */
typedef struct c_undo_t {
    int   action;
    int   isize;
    int   dsize;
    char *ptr;
    char *buf;
} c_undo_t;

/*
 * Current action information for vi
 */
typedef struct c_vcmd_t {
    int   action;
    char *pos;
    char *ins;
} c_vcmd_t;

/*
 * Kill buffer for emacs
 */
typedef struct c_kill_t {
    char *buf;
    char *last;
    char *mark;
} c_kill_t;

/*
 * Note that we use both data structures because the user can bind
 * commands from both editors!
 */
typedef struct el_chared_t {
    c_undo_t    c_undo;
    c_kill_t    c_kill;
    c_vcmd_t    c_vcmd;
    c_macro_t   c_macro;
} el_chared_t;


#define STReof "^D\b\b"
#define STRQQ  "\"\""

#define isglob(a) (strchr("*[]?", (a)) != NULL)
#define isword(a) (isprint(a))

#define NOP    	  0x00
#define DELETE 	  0x01
#define INSERT 	  0x02
#define CHANGE 	  0x04

#define CHAR_FWD	0
#define CHAR_BACK	1

#define MODE_INSERT	0
#define MODE_REPLACE	1
#define MODE_REPLACE_1	2

#include "vi.h"
#include "emacs.h"
#include "common.h"
#include "search.h"
#include "fcns.h"


protected int   cv__isword	__P((int));
protected void  cv_delfini	__P((EditLine *));
protected char *cv__endword	__P((char *, char *, int));
protected int   ce__isword	__P((int));
protected void  cv_undo		__P((EditLine *, int, int, char *));
protected char *cv_next_word	__P((EditLine*, char *, char *, int, 
				     int (*)(int)));
protected char *cv_prev_word	__P((EditLine*, char *, char *, int,
				     int (*)(int)));
protected char *c__next_word	__P((char *, char *, int, int (*)(int)));
protected char *c__prev_word	__P((char *, char *, int, int (*)(int)));
protected void  c_insert	__P((EditLine *, int));
protected void  c_delbefore	__P((EditLine *, int));
protected void  c_delafter	__P((EditLine *, int));
protected int   c_gets		__P((EditLine *, char *));
protected int   c_hpos		__P((EditLine *));

protected int   ch_init		__P((EditLine *));
protected void  ch_reset	__P((EditLine *));
protected void  ch_end		__P((EditLine *));

#endif /* _h_el_chared */
