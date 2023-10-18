#include "mysync.h"


//  ON LINUX WE NEED TO PROTOTYPE THE (NON-STANDARD) strdup() FUNCTION 
//  WHY?  https://stackoverflow.com/questions/32944390/what-is-the-rationale-for-not-including-strdup-in-the-c-standard

//  ---------------------------------------------------------------------

//  'CREATE' A NEW, EMPTY LIST - JUST A NULL POINTER
LIST *list_new(void)
{
    return NULL;
}

//  DETERMINE IF A REQUIRED ITEM (A STRING) IS STORED IN A GIVEN LIST
//Will be used to see if directory exists in the list 
bool list_find(LIST *list, char *wanted)
{
    while(list != NULL) {
	if(strcmp(list->file_name, wanted) == 0) {
	    return true;
	}
	list	= list->next;
    }
    return false;
}

bool dir_find(LIST *list, char *wanted)
{
    while(list != NULL) {
	if(strcmp(list->dir_name, wanted) == 0) {
	    return true;
	}
	list	= list->next;
    }
    return false;
}


//  ALLOCATE SPACE FOR A NEW LIST ITEM, TESTING THAT ALLOCATION SUCCEEDS
//Must change, not a list of strings, list of structs 
LIST *list_new_item(char* filename, time_t mtime, mode_t permissions, char* dirname)
{
    LIST *new       = calloc(1, sizeof(LIST));
    CHECK_ALLOC(new);
    new->file_name     =  strdup(filename);
    CHECK_ALLOC(new->file_name);
    new->modification = mtime;
    new->permissions = permissions;
    new->dir_name = dirname;
    new->next       =  NULL;
    return new;
}

//  ADD A NEW (STRING) ITEM TO AN EXISTING LIST
LIST *list_add(LIST *list, char* filename, time_t mtime, mode_t permissions, char* dirname)
{
    //check if null, if so create null pointer to point to 
        LIST *new  = list_new_item(filename, mtime, permissions,dirname );
        new->next   = list;
        return new;

}

//  PRINT EACH ITEM (A STRING) IN A GIVEN LIST TO stdout
void list_print(LIST *list)
{
    if(list != NULL) {
        while(list != NULL) {
	    printf("%s", list->file_name);
	    if(list->next != NULL) {
	        printf(" -> ");
            }
	    list	= list->next;
        }
	printf("\n");
    }
}

void listAdd(char *filename) {
    if(copy_files == NULL) {           // append to an empty list   
        copy_files = malloc( sizeof(FILEITEM) );
        if(copy_files == NULL) {
            perror( __func__ );
            exit(EXIT_FAILURE);
        }
        copy_files->filename  =  strdup(filename);
        copy_files->next    =  NULL;
    }
    else {                       // append to an existing list
        FILEITEM *p = copy_files;
        while(p->next != NULL) { // walk to the end of the list  
            p  =  p->next;
        }
        p->next = malloc( sizeof(FILEITEM) );
        if(p->next == NULL) {
            perror( __func__ );
            exit(EXIT_FAILURE);
        }
        p          =  p->next;   // append after the last item
        p->filename  =  strdup(filename);
        p->next    =  NULL;
    }
}

char* listIterate() {
    static FILEITEM *current_file = NULL;
    if (current_file == NULL) {
        current_file = copy_files;
    } else {
        if (current_file->next != NULL) {
            current_file = current_file->next;
        } else {
            return NULL; // No more items in the list
        }
    }
    return current_file->filename;
}
