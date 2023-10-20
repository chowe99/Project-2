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
extern int read_dir(HASHTABLE *hashtable, char *dirname, char *parentdirs);
extern void sync_directories(HASHTABLE *hashtable, char *dirname); 
extern void  printDir(char *dirname);
extern void add_missing_dirs(const char *subdirectories, const char *parentdir);
extern void process_path(char *path, char *dirname);


//FUNCTIONS IN FILES.C
extern void copy_text_file(char destination[], char source[]);
extern void print_permissions(mode_t mode);
extern void setPermissions(char *source, char *dest);
extern void setModTime(char *source, char *dest);
extern void add_File(char *filename, char* dir_name);

//FUNCTIONS IN GLOBALS.C
extern void save_args(int argc, char *argv[]);

//EXTERNAL FUNCTIONS
extern char         *strdup(const char *);





