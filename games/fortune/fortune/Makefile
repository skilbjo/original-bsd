#	@(#)Makefile	8.1 (Berkeley) 05/31/93

PROG=	fortune
MAN6=	fortune.0
CFLAGS+=-I${.CURDIR}/../strfile
DPADD=	${COMPAT}
LDADD=	-lcompat

.include "${.CURDIR}/../../Makefile.inc"
.include <bsd.prog.mk>
