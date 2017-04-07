#!/bin/sh +x
#
#
#

genFlags() {
	for a in a _ ; do
	for c in c _ ; do
	for m in m _ ; do
	for T in T _ ; do
	for t in t _ ; do
		echo $a$c$m$T$t;
	done;
	done;
	done;
	done;
	done;
}

ALL_FLAGS=$(genFlags)

mkdir -p results
rm -rf results/*

for input in inputs/* ; do
	for FLAGS in ${ALL_FLAGS}; do

		CALL_FLAGS=$(echo -${FLAGS} |sed s/_//g)
		if [ X"${CALL_FLAGS}" = X"-" ] ; then
			CALL_FLAGS="";
		fi

		../c0chk ${CALL_FLAGS} ${input} > results/${input#*/}.${FLAGS}
		mv results/${input#*/}.${FLAGS} results/${input#*/}.${FLAGS}.$?
	done;
done

diff -Nur known_good results
