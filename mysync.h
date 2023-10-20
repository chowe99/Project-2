#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h> 
#include <bits/getopt_core.h>
#include <dirent.h>
#include <regex.h>
#include <time.h>
#include <fcntl.h>
#include <utime.h>
#include <asm-generic/errno-base.h>

#define	HASHTABLE_SIZE		997

#define OPTS                    "ai:no:prv"

#define CHECK_ALLOC(p) if(p==NULL) {\
    fprintf(stderr, "cannot allocate memory\n");\
    exit(EXIT_FAILURE);}

//GLOBAL VARIABLES
extern int      nfiles;
extern bool     verbose; //true iff -v or -n provided 
extern bool     a;
extern bool     n;
extern bool     p;
extern bool     r;
extern bool     v;
extern char     **o;
extern size_t   o_index;
extern char     **i;
extern size_t   i_index;


typedef struct _list {
    char            *file_name; //filename
    char            *dir_name;
    time_t          modification;
    mode_t          permissions;
    struct _list    *next;
} LIST;

extern char** filenames;
typedef	LIST        *HASHTABLE;
extern HASHTABLE    *file_list;

//FUNCTIONS IN GLOB2REGEX.C
extern char *glob2regex(char *glob);
extern bool is_match(char *filename, char *arr[], size_t num_items);

//FUNCTIONS IN HASHTABLE.C
extern uint32_t hash_string(char *string);
extern HASHTABLE *hashtable_new(void);
extern void hashtable_add(HASHTABLE *hashtable, char *filename, time_t mtime, mode_t permissions, char* dirname);
extern bool hashtable_find(HASHTABLE *, char *filename);

//FUNCTIONS IN LIST.C
extern LIST *list_new(void);
extern bool list_find (LIST *list, char *wanted);
extern bool dir_find(LIST *list, char *wanted);
extern LIST *list_new_item(char* filename, time_t mtime, mode_t permissions, char* dirname);
extern LIST *list_add(LIST *list, char* filename, time_t mtime, mode_t permissions, char* dirname);
extern void list_print(LIST *list);
extern void arrayAdd(char *filename);
extern void printArray();

//FUNCTIONS IN READDIR.C



//FUNCTIONS IN FILES.C
extern void copy_text_file(char destination[], char source[]);
extern void print_permissions(mode_t mode);
extern void setPermissions(char *source, char *dest);
extern void setModTime(char *source, char *dest);


//FUNCTIONS IN GLOBALS.C
extern void save_args(int argc, char *argv[]);



//  THESE FUNCTIONS ARE DECLARED HERE, AND DEFINED IN list.c :

//  'CREATE' A NEW, EMPTY LIST



//  ADD A NEW (STRING) ITEM TO AN EXISTING LIST




//  DETERMINE IF A REQUIRED ITEM (A STRING) IS STORED IN A GIVEN LIST


//  PRINT EACH ITEM (A STRING) IN A GIVEN LIST TO stdout

extern int          compare_mtime_descending(const void *v1, const void *v2);
//FROM hashtable
//  WE DEFINE A HASHTABLE AS A (WILL BE, DYNAMICALLY ALLOCATED) ARRAY OF LISTs

//  THESE FUNCTIONS ARE DECLARED HERE, AND DEFINED IN hashtable.c :

//  ALLOCATE SPACE FOR A NEW HASHTABLE (AND ARRAY OF LISTS)

//  ADD A NEW STRING TO A GIVEN HASHTABLE


//  DETERMINE IF A REQUIRED STRING ALREADY EXISTS IN A GIVEN HASHTABLE


//EXTERNAL FUNCTIONS
extern char         *strdup(const char *);

//FUNCTIONS FROM globals.c
//commment

//FUNCTIONS FROM readDir.c
extern int read_dir(HASHTABLE *hashtable, char *dirname, char *parentdirs);
extern void sync_directories(HASHTABLE *hashtable, char *dirname); 
extern void  printDir(char *dirname);
extern void add_missing_dirs(const char *subdirectories, const char *parentdir);
extern void process_path(char *path, char *dirname);


//FUNCTIONS FROM files.c
extern void         add_File(char *filename, char* dir_name);


//FUNCTIONS from glob2regex

//FUNCTIONS from patterns.c


extern bool         includeFile(char* fileName); 

//Takes each file of each directory,
// and determines whether it should be included 
//in the syncing process depending on the flags given
//if a flag given: then include files starting with .
//if -i flag given: check if string matches the expression
//if -o flag given: check if string doesn't match the expression 



