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
    new->dir_name = strdup(dirname);
    new->next       =  NULL;
    return new;
}

//  ADD A NEW (STRING) ITEM TO AN EXISTING LIST
LIST *list_add(LIST *list, char* filename, time_t mtime, mode_t permissions, char* dirname)
{
    //check if null, if so create null pointer to point to 
        LIST *new  = list_new_item(filename, mtime, permissions,dirname );
        new->next   = list;
        list_print(new);
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


void arrayAdd(char *filename) {
        // Allocate memory for the new filename
        char* newFilename = malloc(MAXPATHLEN);
        if (newFilename == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
        // Copy the filename to the allocated memory
        strncpy(newFilename, filename, MAXPATHLEN - 1);
        newFilename[MAXPATHLEN - 1] = '\0'; // Ensure null-termination

        // Resize the array to accommodate the new filename
        filenames = realloc(filenames, nfiles * sizeof(char*));
        if (filenames == NULL) {
            fprintf(stderr, "Memory reallocation error\n");
            exit(EXIT_FAILURE);
        }
        // Add the new filename to the array
        filenames[nfiles - 1] = newFilename;
    }

void printArray() {
            for (int i = 0; i < nfiles; ++i) {
            printf("File %d: %s \n", i + 1, filenames[i]);
            LIST *current = file_list[hash_string(filenames[i]) % HASHTABLE_SIZE];
            printf("Full path before syncing: %s/%s\n", current->dir_name, current->file_name);
        }
    }



    

    
