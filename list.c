#include "mysync.h"


/**
 * Creates a new ,empty list
 * \returns: NULL pointer
*/
LIST *list_new(void)
{
    return NULL;
}

/**
 * Determines if a given file name exists in the list 
 * \param: LIST list: the list to be searched
 * \param: wanted: pointer to the filename to be searched for
 * \return: true if the file was found, false otherwise
*/
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

/**
 * Allocates space for a new list item
 * \param: filename: filename of the file to be added
 * \param: mtime: modification time of the file
 * \param: permissions: permissions of the file
 * \param: dirname: Top level directory of the file 
 * \return: returns the new list item 
*/
 
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

/**
 * Adds a new list item to a given list 
 * \param: filename: filename of the file to be added
 * \param: list: the list to add the item too
 * \param: mtime: modification time of the file
 * \param: permissions: permissions of the file
 * \param: dirname: Top level directory of the file 
 * \return: returns the updated list  
*/
LIST *list_add(LIST *list, char* filename, time_t mtime, mode_t permissions, char* dirname)
{

        LIST *new  = list_new_item(filename, mtime, permissions,dirname );
        new->next   = list;
        list_print(new);
        return new;

}

/**
 * Print each item of the given list to stdout
*/
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

/**
 * Adds a new filename to the array of names that have been added to the hashtable 
 * \param: filename to be added
*/
void arrayAdd(char *filename) {
        char* newFilename = malloc(MAXPATHLEN);
        if (newFilename == NULL) {
            fprintf(stderr, "Memory allocation error\n");
            exit(EXIT_FAILURE);
        }
        // Copy the filename to the allocated memory
        strncpy(newFilename, filename, MAXPATHLEN - 1);
        newFilename[MAXPATHLEN - 1] = '\0'; 

        // Resize the array to accommodate the new filename
        filenames = realloc(filenames, nfiles * sizeof(char*));
        if (filenames == NULL) {
            fprintf(stderr, "Memory reallocation error\n");
            exit(EXIT_FAILURE);
        }
        // Add the new filename to the array
        filenames[nfiles - 1] = newFilename;
    }
/**
 * Prints all elements of the array to stdout
*/
void printArray() {
            for (int i = 0; i < nfiles; ++i) {
            printf("File %d: %s \n", i + 1, filenames[i]);
            LIST *current = file_list[hash_string(filenames[i]) % HASHTABLE_SIZE];
            printf("Full path before syncing: %s/%s\n", current->dir_name, current->file_name);
        }
    }



    

    
