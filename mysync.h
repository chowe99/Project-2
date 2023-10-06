
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <string.h>
#include <stdlib.h>

#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> //fine on wsl


#define CHECK_ALLOC(p) if(p==NULL) {\
                            fprintf(stderr, "cannot allocate memory\n");\
                            exit(EXIT_FAILURE);}
                            
//GLOBAL VARIABLES
extern int ndirs;
extern int nfiles;
extern char *argv0; //program name
extern bool verbose; //true iff -v or -n provided 

//EXTERNAL FUNCTIONS
extern char *strdup(const char *);

//FUNCTIONS FROM directories.c

//FUNCTIONS FROM files.c

//FUNCTIONS FROM ..other c files defined 