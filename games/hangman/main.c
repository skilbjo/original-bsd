/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.
 *
 * %sccs.include.redist.c%
 */

#ifndef lint
char copyright[] =
"@(#) Copyright (c) 1983 Regents of the University of California.\n\
 All rights reserved.\n";
#endif /* not lint */

#ifndef lint
static char sccsid[] = "@(#)main.c	5.4 (Berkeley) 02/28/91";
#endif /* not lint */

# include	"hangman.h"

/*
 * This game written by Ken Arnold.
 */
main()
{
	void die();

	initscr();
	signal(SIGINT, die);
	setup();
	for (;;) {
		Wordnum++;
		playgame();
		Average = (Average * (Wordnum - 1) + Errors) / Wordnum;
	}
	/* NOTREACHED */
}

/*
 * die:
 *	Die properly.
 */
void
die()
{
	mvcur(0, COLS - 1, LINES - 1, 0);
	endwin();
	putchar('\n');
	exit(0);
}
