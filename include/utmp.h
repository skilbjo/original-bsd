/*
 * Copyright (c) 1988 The Regents of the University of California.
 * All rights reserved.
 *
 * %sccs.include.redist.c%
 *
 *	@(#)utmp.h	5.10 (Berkeley) 06/01/90
 */

#define	_PATH_UTMP	"/var/run/utmp"
#define	_PATH_WTMP	"/var/log/wtmp"
#define	_PATH_LASTLOG	"/var/log/lastlog"

#define	UT_NAMESIZE	8
#define	UT_LINESIZE	8
#define	UT_HOSTSIZE	16

struct lastlog {
	time_t	ll_time;
	char	ll_line[UT_LINESIZE];
	char	ll_host[UT_HOSTSIZE];
};

struct utmp {
	char	ut_line[UT_LINESIZE];
	char	ut_name[UT_NAMESIZE];
	char	ut_host[UT_HOSTSIZE];
	long	ut_time;
};
