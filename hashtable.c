#include "mysync.h"

//  RESEARCH SHOWS THAT USING PRIME-NUMBERS CAN IMPROVE PERFORMANCE
//  c.f.  https://www.quora.com/Why-should-the-size-of-a-hash-table-be-a-prime-number

//  --------------------------------------------------------------------

//  FUNCTION hash_string() ACCEPTS A STRING PARAMETER,
//  AND RETURNS AN UNSIGNED 32-BIT INTEGER AS ITS RESULT
//
//  see:  https://en.cppreference.com/w/c/types/integer

uint32_t hash_string(char *string)
{
    uint32_t hash = 0;

    while(*string != '\0') {
        hash = hash*33 + *string;
        ++string;
    }
    return hash;
}

//  ALLOCATE AND INITIALISE SPACE FOR A NEW HASHTABLE (AN ARRAY OF LISTS)
//Add check to see if malloc is needed 
HASHTABLE *hashtable_new(void)
{
    HASHTABLE   *new = calloc(HASHTABLE_SIZE, sizeof(LIST *));
    CHECK_ALLOC(new);
    return new;
}

//  ADD A NEW STRING TO A GIVEN HASHTABLE
void hashtable_add(HASHTABLE *hashtable, char *filename, time_t mtime, mode_t permissions, char* dirname)
{
    uint32_t h   = hash_string(filename) % HASHTABLE_SIZE;
    hashtable[h] = list_add(hashtable[h], filename, mtime, permissions, dirname);
    if(v) {
    printf("File '%s' has been added to the hash table, at %i\n", filename, h);
    printf("The information stored:\n\tmod_time: %s\tfilename: '%s'\n\tdir: '%s'\n", ctime(&hashtable[h]->modification), hashtable[h]->file_name, hashtable[h]->dir_name);
    } //mod time not being stored properly
}

//  DETERMINE IF A REQUIRED STRING ALREADY EXISTS IN A GIVEN HASHTABLE
bool hashtable_find(HASHTABLE *hashtable, char *filename)
{
    uint32_t h	= hash_string(filename) % HASHTABLE_SIZE;     // choose list
    return list_find(hashtable[h], filename);
}

