# include "conf.h"
# include <sys/time.h>
# ifndef V6
# include <sys/types.h>
# include <sys/timeb.h>
# endif
# include "useful.h"

SCCSID(@(#)arpadate.c	4.3		10/01/83);

/*
**  ARPADATE -- Create date in ARPANET format
**
**	Parameters:
**		ud -- unix style date string.  if NULL, one is created.
**
**	Returns:
**		pointer to an ARPANET date field
**
**	Side Effects:
**		none
**
**	WARNING:
**		date is stored in a local buffer -- subsequent
**		calls will overwrite.
**
**	Bugs:
**		Timezone is computed from local time, rather than
**		from whereever (and whenever) the message was sent.
**		To do better is very hard.
**
**		Some sites are now inserting the timezone into the
**		local date.  This routine should figure out what
**		the format is and work appropriately.
*/

char *
arpadate(ud)
	register char *ud;
{
	register char *p;
	register char *q;
	static char b[40];
	extern char *ctime();
	register int i;
	extern struct tm *localtime();
	extern bool fconvert();
# ifdef V6
	long t;
	extern char *StdTimezone, *DstTimezone;
	extern long time();
# else
	struct timeb t;
	extern struct timeb *ftime();
	extern char *timezone();
# endif

	/*
	**  Get current time.
	**	This will be used if a null argument is passed and
	**	to resolve the timezone.
	*/

# ifdef V6
	(void) time(&t);
	if (ud == NULL)
		ud = ctime(&t);
# else
	ftime(&t);
	if (ud == NULL)
		ud = ctime(&t.time);
# endif

	/*
	**  Crack the UNIX date line in a singularly unoriginal way.
	*/

	q = b;

	p = &ud[0];		/* Mon */
	*q++ = *p++;
	*q++ = *p++;
	*q++ = *p++;
	*q++ = ',';
	*q++ = ' ';

	p = &ud[8];		/* 16 */
	if (*p == ' ')
		p++;
	else
		*q++ = *p++;
	*q++ = *p++;
	*q++ = ' ';

	p = &ud[4];		/* Sep */
	*q++ = *p++;
	*q++ = *p++;
	*q++ = *p++;
	*q++ = ' ';

	p = &ud[22];		/* 79 */
	*q++ = *p++;
	*q++ = *p++;
	*q++ = ' ';

	p = &ud[11];		/* 01:03:52 */
	for (i = 8; i > 0; i--)
		*q++ = *p++;

				/* -PST or -PDT */
# ifdef V6
	if (localtime(&t)->tm_isdst)
		p = DstTimezone;
	else
		p = StdTimezone;
# else
	p = timezone(t.timezone, localtime(&t.time)->tm_isdst);
# endif V6
	if ((strncmp(p, "GMT", 3) == 0 || strncmp(p, "gmt", 3) == 0) &&
	    p[3] != '\0')
	{
		/* hours from GMT */
		p += 3;
		*q++ = *p++;
		if (p[1] == ':')
			*q++ = '0';
		else
			*q++ = *p++;
		*q++ = *p++;
		p++;		/* skip ``:'' */
		*q++ = *p++;
		*q++ = *p++;
		*q = '\0';
	}
	else if (!fconvert(p, q))
	{
		*q++ = ' ';
		*q++ = *p++;
		*q++ = *p++;
		*q++ = *p++;
		*q = '\0';
	}

	return (b);
}
/*
**  FCONVERT -- convert foreign timezones to ARPA timezones
**
**	This routine is essentially from Teus Hagen.
**
**	Parameters:
**		a -- timezone as returned from UNIX.
**		b -- place to put ARPA-style timezone.
**
**	Returns:
**		TRUE -- if a conversion was made (and b was filled in).
**		FALSE -- if this is not a recognized local time.
**
**	Side Effects:
**		none.
*/

/* UNIX to arpa conversion table */
struct foreign
{
	char *f_from; 
	char *f_to; 
};

static struct foreign Foreign[] =
{
	{ "eet",	" -0200" },	/* eastern europe */
	{ "met",	" -0100" },	/* middle europe */
	{ "wet",	" GMT"   },	/* western europe */
	{ "eet dst",	" -0300" },	/* daylight saving times */
	{ "met dst",	" -0200" },
	{ "wet dst",	" -0100" },
	{ NULL,		NULL	 }
};

bool
fconvert(a, b)
	register char *a;
	char *b;
{
	register struct foreign *euptr;
	register char *p;

	makelower(a);
	for (euptr = Foreign; euptr->f_from != NULL; euptr++)
	{
		if (strcmp(euptr->f_from, a) == 0)
		{
			p = euptr->f_to;
			while (*p)
				*b++ = *p++;
			*b = '\0';
			return (TRUE);
		}
	}
	return (FALSE);
}
