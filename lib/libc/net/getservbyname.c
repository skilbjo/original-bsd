/*
 * Copyright (c) 1983 Regents of the University of California.
 * All rights reserved.
 *
 * %sccs.include.redist.c%
 */

#if defined(LIBC_SCCS) && !defined(lint)
static char sccsid[] = "@(#)getservbyname.c	5.6 (Berkeley) 06/01/90";
#endif /* LIBC_SCCS and not lint */

#include <netdb.h>

extern int _serv_stayopen;

struct servent *
getservbyname(name, proto)
	char *name, *proto;
{
	register struct servent *p;
	register char **cp;

	setservent(_serv_stayopen);
	while (p = getservent()) {
		if (strcmp(name, p->s_name) == 0)
			goto gotname;
		for (cp = p->s_aliases; *cp; cp++)
			if (strcmp(name, *cp) == 0)
				goto gotname;
		continue;
gotname:
		if (proto == 0 || strcmp(p->s_proto, proto) == 0)
			break;
	}
	if (!_serv_stayopen)
		endservent();
	return (p);
}
