#include "mysync.h"

/** Gets the hash index of a string.
 *
 * \param string The string we want the index of
 * \return The index of the string
*/
uint32_t hash_string(char *string)
{
    uint32_t hash = 0;

    while(*string != '\0') {
        hash = hash*33 + *string;
        ++string;
    }
    return hash;
}

/** Creates and allocates memory to a new hashtable and returns it 
 *
 * \return the hashtable
*/
HASHTABLE *hashtable_new(void)
{
    HASHTABLE   *new = calloc(HASHTABLE_SIZE, sizeof(LIST *));
    CHECK_ALLOC(new);
    return new;
}

/** Adds a file and its info to a hashtable
 *
 * \param hashtable the hashtable we want to update
 * \param filename the filename of the file we want to add
 * \param mtime the last modification time of the file
 * \param permissions the file permissions of the file
 * \param dirname the directory that the file is located
*/
void hashtable_add(HASHTABLE *hashtable, char *filename, time_t mtime, mode_t permissions, char* dirname)
{
    uint32_t h   = hash_string(filename) % HASHTABLE_SIZE;
    hashtable[h] = list_add(hashtable[h], filename, mtime, permissions, dirname);
    if(v) {
    printf("File '%s' has been added to the hash table, at %i\n", filename, h);
    printf("The information stored:\n\tmod_time: %s\tfilename: '%s'\n\tdir: '%s'\n", ctime(&hashtable[h]->modification), hashtable[h]->file_name, hashtable[h]->dir_name);
    } //mod time not being stored properly
}

/** Checks if a file (string) is in a hashtable
 *
 * \param hashtable the hashtable to check
 * \param filename The file we are trying to find
 * \return true if file exists else false
*/
bool hashtable_find(HASHTABLE *hashtable, char *filename)
{
    uint32_t h	= hash_string(filename) % HASHTABLE_SIZE;     // choose list
    return list_find(hashtable[h], filename);
}

