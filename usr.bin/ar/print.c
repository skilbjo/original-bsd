/*-
 * Copyright (c) 1990 The Regents of the University of California.
 * All rights reserved.
 *
 * This code is derived from software contributed to Berkeley by
 * Hugh Smith at The University of Guelph.
 *
 * %sccs.include.redist.c%
 */

#ifndef lint
static char sccsid[] = "@(#)print.c	5.5 (Berkeley) 03/11/91";
#endif /* not lint */

#include <sys/param.h>
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <stdio.h>
#include "archive.h"
#include "extern.h"

extern CHDR chdr;			/* converted header */
extern char *archive;			/* archive name */

/*
 * print --
 *	Prints archive members on stdout - if member names given only
 *	print those members, otherwise print all members.
 */
print(argv)
	char **argv;
{
	CF cf;
	register int afd, all;
	char *file;

	afd = open_archive(O_RDONLY);

	/* Read from an archive, write to stdout; pad on read. */
	SETCF(afd, archive, STDOUT_FILENO, "stdout", RPAD);
	for (all = !*argv; get_header(afd);) {
		if (all)
			file = chdr.name;
		else if (!(file = files(argv))) {
			skipobj(afd);
			continue;
		}
		if (options & AR_V) {
			(void)printf("\n<%s>\n\n", file);
			(void)fflush(stdout);
		}
		copyfile(&cf, chdr.size);
		if (!all && !*argv)
			break;
	}
	close_archive(afd);

	if (*argv) {
		orphans(argv);
		return(1);
	}
	return(0);
}
