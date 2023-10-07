#!/bin/sh

NWORDS="1000 2500 5000 10000 25000 50000"

for n in `echo $NWORDS`
do
	./testlist $n
	./testhashtable $n
	echo
done
