/*-
 * Copyright (c) 1990, 1993
 *	The Regents of the University of California.  All rights reserved.
 *
 * %sccs.include.redist.c%
 */

#ifndef lint
static char sccsid[] = "@(#)proc_compare.c	8.1 (Berkeley) 06/06/93";
#endif /* not lint */

/*
 * Returns 1 if p2 is more active than p1
 *
 * The algorithm for picking the "more active" process is thus:
 *
 *	1) Runnable processes are favored over anything
 *	   else.  The runner with the highest cpu
 *	   utilization is picked (p_cpu).  Ties are
 *	   broken by picking the highest pid.
 *	2) Next, the sleeper with the shortest sleep
 *	   time is favored.  With ties, we pick out
 *	   just short-term sleepers (p_pri <= PZERO).
 *	   Further ties are broken by picking the highest
 *	   pid.
 *
 *	NOTE - if you change this, be sure to consider making
 *	   the change in the kernel too (^T in kern/tty.c).
 *
 *	TODO - consider whether pctcpu should be used
 *
 */

#include <sys/param.h>
#include <sys/time.h>
#include <sys/proc.h>

#include "extern.h"

#define isrun(p)	(((p)->p_stat == SRUN) || ((p)->p_stat == SIDL))

#define	TESTAB(a, b)	((a)<<1 | (b))
#define	ONLYA	2
#define	ONLYB	1
#define	BOTH	3

int
proc_compare(p1, p2)
	register struct proc *p1, *p2;
{

	if (p1 == NULL)
		return (1);
	/*
	 * see if at least one of them is runnable
	 */
	switch (TESTAB(isrun(p1), isrun(p2))) {
	case ONLYA:
		return (0);
	case ONLYB:
		return (1);
	case BOTH:
		/*
		 * tie - favor one with highest recent cpu utilization
		 */
		if (p2->p_cpu > p1->p_cpu)
			return (1);
		if (p1->p_cpu > p2->p_cpu)
			return (0);
		return (p2->p_pid > p1->p_pid);	/* tie - return highest pid */
	}
	/*
	 * weed out zombies
	 */
	switch (TESTAB(p1->p_stat == SZOMB, p2->p_stat == SZOMB)) {
	case ONLYA:
		return (1);
	case ONLYB:
		return (0);
	case BOTH:
		return (p2->p_pid > p1->p_pid);	/* tie - return highest pid */
	}
	/* 
	 * pick the one with the smallest sleep time
	 */
	if (p2->p_slptime > p1->p_slptime)
		return (0);
	if (p1->p_slptime > p2->p_slptime)
		return (1);
	/*
	 * favor one sleeping in a non-interruptible sleep
	 */
	 if (p1->p_flag&SSINTR && (p2->p_flag&SSINTR) == 0)
		 return (1);
	 if (p2->p_flag&SSINTR && (p1->p_flag&SSINTR) == 0)
		 return (0);
	return(p2->p_pid > p1->p_pid);		/* tie - return highest pid */
}
