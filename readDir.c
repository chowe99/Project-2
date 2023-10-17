#include "mysync.h"

void read_dir(char *dirname) {
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

        struct stat info;
        char pathname[MAXPATHLEN];
        sprintf(pathname, "%s/%s", dirname, dp->d_name); 
        if (stat(pathname, &info) != 0) {
            perror(pathname);
            exit(EXIT_FAILURE);
        }
        printf("%s m_tim: %ld\n", pathname, info.st_mtim.tv_sec);
    }
}
