ALL	= testlist testhashtable

C11	= cc -std=c11 -Wall -Werror


all:	$(ALL)

testlist: list.h testlist.c list.c support.c
	$(C11) -o testlist testlist.c list.c support.c

testhashtable: testhashtable.c hashtable.h hashtable.c list.h list.c support.c
	$(C11) -o testhashtable testhashtable.c hashtable.c list.c support.c


compare: $(ALL)
	@./compare.sh
	
plot: $(ALL)
	@./plot-to-html.sh
	@echo 'now open  plot.html  in your browser'


clean:
	rm -f $(ALL) plot.html
