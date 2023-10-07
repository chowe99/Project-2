#!/bin/bash

echo 'cc -std=c11 -Wall -Werror -o testhashtable testhashtable.c hashtable.c list.c support.c'
cc -std=c11 -Wall -Werror -o testhashtable testhashtable.c hashtable.c list.c support.c

cc -std=c11 -Wall -Werror -o testlist testlist.c list.c support.c
