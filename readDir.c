#include "mysync.h"
#include <string.h>

/** takes a hashtable and saves a directory to it
 * if the directory is not already present.
 *
 */
void read_dir(HASHTABLE *hashtable, char *dirname) {
    DIR *dirp;
    struct dirent *dp;

    dirp = opendir(dirname);
    if (dirp == NULL) {
        perror(dirname);
        exit(EXIT_FAILURE);
    }

    while ( (dp = readdir(dirp)) != NULL) 
    {
        if (i_index > 0 && is_match(dp->d_name, i, i_index)) {
            continue;
        }
        if (o_index > 0 && !is_match(dp->d_name, o, o_index)) {
            continue;
        }
        if (!a) {
            if (dp->d_name[0] == '.') {
                continue;
            }
        }

        struct stat info;
        char pathname[MAXPATHLEN];
        sprintf(pathname, "%s/%s", dirname, dp->d_name); 
        if (stat(pathname, &info) != 0) {
            perror(pathname);
            exit(EXIT_FAILURE);
        }
        // printf("%-10s\tm_tim: %-10ld\tst_mode: %-10u\n", pathname, info.st_mtim.tv_sec, info.st_mode);
        if (!hashtable_find(hashtable, dp->d_name)) {
            hashtable_add(hashtable, dp->d_name, info.st_mtim.tv_sec, info.st_mode, dirname);
        }
        
    }
}
