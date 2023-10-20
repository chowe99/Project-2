PROJECT =  mysync
HEADERS =  $(PROJECT).h
OBJ     =  globals.o mysync.o files.o readDir.o glob2regex.o list.o hashtable.o


C11     =  cc -std=c11
CFLAGS  =  -Wall -Werror


$(PROJECT) : $(OBJ)
	$(C11) $(CFLAGS) -o $(PROJECT) $(OBJ) -lm


%.o : %.c $(HEADERS)
	$(C11) $(CFLAGS) -c $<

clean:
	rm -f $(PROJECT) $(OBJ)
