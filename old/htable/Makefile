#	@(#)Makefile	5.13 (Berkeley) 06/18/92

PROG=	htable
SRCS=	htable.c parse.c scan.c
MAN8=	htable.0
CFLAGS+=-I. -I${.CURDIR}
CLEANFILES+=parse.c scan.c y.tab.h
DPADD=	${LIBL}
LDADD=	-ll

.include <bsd.prog.mk>

${OBJS}: scan.c parse.c
