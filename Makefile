CC=cc -x c -std=c99

CFLAGS= -Wall -Werror
CFLAGS+= -fstack-protector-all
CFLAGS+= -Wstrict-prototypes -Wmissing-prototypes
CFLAGS+= -Wmissing-declarations
CFLAGS+= -Wshadow -Wpointer-arith -Wcast-qual
CFLAGS+= -Wsign-compare
CFLAGS+= -O2 -g

LDFLAGS=

EXECUTABLE=c0chk

${EXECUTABLE}: c0chk.c
	${CC} ${CFLAGS} c0chk.c ${LDFLAGS} -o $@

clean:
	rm -f ${EXECUTABLE}
	rm -f *.o
	rm -f *~

.PHONY: clean
