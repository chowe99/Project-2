
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> 


#define CHECK_ALLOC(p) if(p==NULL) {\
                            fprintf(stderr, "cannot allocate memory\n");\
                            exit(EXIT_FAILURE);}
                            
//GLOBAL VARIABLES
extern int ndirs;
extern int nfiles;
extern char *argv0; //program name
extern bool verbose; //true iff -v or -n provided 
extern bool a;
extern bool n;
extern bool p;
extern bool r;
extern char* o [];
extern char* i [];

//EXTERNAL FUNCTIONS
extern char *strdup(const char *);

//FUNCTIONS FROM readDir.c
extern void read_dir(char *dirname);



//FUNCTIONS FROM files.c
extern void add_File(char* filename, int modified, int permissions, char* dirname);
extern void print_permissions(mode_t mode);
extern void setPermissions(char *source, char *dest);
//FUNCTIONS from glob2regex
extern char *glob2regex(char *glob);
//FUNCTIONS from patterns.c
extern bool includeFile(char* fileName); 
//Takes each file of each directory,
// and determines whether it should be included 
//in the syncing process depending on the flags given
//if a flag given: then include files starting with .
//if -i flag given: check if string matches the expression
//if -o flag given: check if string doesn't match the expression 


//FUNCTIONS FROM ..other c files defined 
//FROM list:
    //  OUR SIMPLE LIST DATATYPE - A DATA ITEM, AND A POINTER TO ANOTHER LIST
    typedef struct _list {
        char           *string; //filename
        char           *dir_name;
        int             modification;
        mode_t permissions;
        struct _list   *next;
    } LIST;

    //  THESE FUNCTIONS ARE DECLARED HERE, AND DEFINED IN list.c :

    //  'CREATE' A NEW, EMPTY LIST
    extern	LIST	*list_new(void);

    //  ADD A NEW (STRING) ITEM TO AN EXISTING LIST
    extern	LIST	*list_add(  LIST *list, char *newstring);

    //  DETERMINE IF A REQUIRED ITEM (A STRING) IS STORED IN A GIVEN LIST
    extern	bool	 list_find (LIST *list, char *wanted);

    //  PRINT EACH ITEM (A STRING) IN A GIVEN LIST TO stdout
    extern	void	 list_print(LIST *list);
//FROM hashtable
//  WE DEFINE A HASHTABLE AS A (WILL BE, DYNAMICALLY ALLOCATED) ARRAY OF LISTs
typedef	LIST * HASHTABLE;
//  THESE FUNCTIONS ARE DECLARED HERE, AND DEFINED IN hashtable.c :

//  ALLOCATE SPACE FOR A NEW HASHTABLE (AND ARRAY OF LISTS)
extern	HASHTABLE	*hashtable_new(void);

//  ADD A NEW STRING TO A GIVEN HASHTABLE
extern	void		 hashtable_add( HASHTABLE *, char *string);

//  DETERMINE IF A REQUIRED STRING ALREADY EXISTS IN A GIVEN HASHTABLE
extern	bool		 hashtable_find(HASHTABLE *, char *string);
