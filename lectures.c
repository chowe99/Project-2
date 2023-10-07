//Reading directories

#include  <dirent.h> //may need to add this into header file 
#include "mysync.h"


void list_directory(char *dirname)
{
    char  fullpath[MAXPATHLEN];

    .....
    while((dp = readdir(dirp)) != NULL) {
        struct stat  stat_buffer;
        struct stat  *pointer = &stat_buffer;

        sprintf(fullpath, "%s/%s", dirname, dp->d_name );

        if(stat(fullpath, pointer) != 0) {
             perror( progname );
        }
        else if( S_ISDIR( pointer->st_mode )) {
            printf( "%s is a directory\n", fullpath );
        }
        else if( S_ISREG( pointer->st_mode )) {
            printf( "%s is a regular file\n", fullpath );
        }
        else {
            printf( "%s is unknown!\n", fullpath );
        }
    }
    closedir(dirp);
}