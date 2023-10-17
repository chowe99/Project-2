#include "mysync.h"
#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <unistd.h>

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
        if (!a && dp->d_name[0] == '.') {
            continue;
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
            if (n) {
                printf("%-10s\tneeds to be synchronized\n", dp->d_name);
            } else {
                hashtable_add(hashtable, dp->d_name, info.st_mtim.tv_sec, info.st_mode, dirname);
            }
        } else {
            if (hashtable[hash_string(dp->d_name)%HASHTABLE_SIZE]->modification < info.st_mtim.tv_sec) {
                printf("updating hashlist with newest element:\n%svs\n%s\n", ctime(&hashtable[hash_string(dp->d_name)%HASHTABLE_SIZE]->modification), ctime(&info.st_mtim.tv_sec));
                printf("olddir: %s\n", hashtable[hash_string(dp->d_name)%HASHTABLE_SIZE]->dir_name);
                hashtable_add(hashtable, dp->d_name, info.st_mtim.tv_sec, info.st_mode, dirname);
                printf("newdir: %s\n", hashtable[hash_string(dp->d_name)%HASHTABLE_SIZE]->dir_name);
            }
        }
    }
}

void sync_directories(HASHTABLE *hashtable, char *dirname) {
    DIR *dirp;
    struct dirent *dp;

    dirp = opendir(dirname);
    if (dirp == NULL) {
        perror(dirname);
        exit(EXIT_FAILURE);
    }

    while ( (dp = readdir(dirp)) != NULL) 
    {
        if (hashtable_find(hashtable, dp->d_name)) {
            char file[MAXPATHLEN];
            sprintf(file, "%s/%s", dirname, dp->d_name); 
            int fd = fileno(fopen(file, "w"));

            memset(file, 0, MAXPATHLEN);
            sprintf(file, "%s/%s", hashtable[hash_string(dp->d_name)%HASHTABLE_SIZE]->dir_name, dp->d_name);
            FILE *latest = fopen(file, "r");
            write(fd, latest, sizeof(*latest));

            fclose(latest);
            close(fd);
        }
    }
}
