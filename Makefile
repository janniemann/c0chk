CC=cc

CFLAGS=-x c -std=c99
CFLAGS+= -Wall -Werror
CFLAGS+= -fstack-protector-all
CFLAGS+= -Wstrict-prototypes -Wmissing-prototypes
CFLAGS+= -Wmissing-declarations
CFLAGS+= -Wshadow -Wpointer-arith -Wcast-qual
CFLAGS+= -Wsign-compare
CFLAGS+= -O2 -g

CFLAGS+= -D_POSIX_C_SOURCE=199309L
LDFLAGS=

all: c0chk

c0chk: c0chk.o
	${CC} $^ ${LDFLAGS} -o $@

pchk: pchk.o
	${CC} $^ ${LDFLAGS} -o $@

clean:
	rm -f c0chk pchk
	rm -f *.o
	rm -f *~

.PHONY: clean
