PROG=		pipesize

CSTD?=		c89
WARNS?=		6
WFORMAT?=	1
NO_MAN=		yes

LDFLAGS+=	-lutil

CLEANFILES=	*.[Bb][Aa][Kk] *.core
CTAGS=		ctags

.include <bsd.prog.mk>
