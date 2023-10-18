#include "mysync.h"
#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <unistd.h>


int save_args(int argc, char *argv[]) {
    int c;
    argc -= optind;
    while ((c = getopt(argc, argv, OPTS)) != -1) 
    {
        printf("optind: %d, argc: %d\n", optind, argc);
        switch (c) 
        {
            case 'a':
                a = true;
                printf("found -a\n");
                break;

            case 'i':
                printf("found -i: ");
                i = realloc(i, (i_index+1) * sizeof(i[0]));
                i[i_index++] = strdup(optarg);
                printf("%s\n", i[i_index-1]);
                break;

            case 'n':
                printf("found -n\n");
                n = true;
                break;

            case 'o':
                printf("found -o\n");
                o = realloc(o, (o_index+1) * sizeof(o[0]));
                o[o_index++] = strdup(optarg);
                break;

            case 'p': 
                printf("found -p\n");
                p = true;
                break;

            case 'r':
                printf("found -r\n");
                r = true;
                break;

            case 'v':
                printf("found -v\n");
                v = true;
                break;

            case ':':
                printf("option needs a value\n");
                break;

            case '?':
                printf("unknown option: %c\n", optopt);
                break;
            
            default:
                break;
        }
    }
    return argc;
}


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
                //change print message 
                printf("%-10s\tneeds to be synchronized\n", dp->d_name);
            } else {
                hashtable_add(hashtable, dp->d_name, info.st_mtim.tv_sec, info.st_mode, dirname);
            }
        } else {
            //If the current file has been modified more recently, then add that 
            if (hashtable[hash_string(dp->d_name)%HASHTABLE_SIZE]->modification < info.st_mtim.tv_sec) {
                printf("updating hashlist with newest element:\n%svs\n%s\n", ctime(&hashtable[hash_string(dp->d_name)%HASHTABLE_SIZE]->modification), ctime(&info.st_mtim.tv_sec));
                printf("olddir: %s\n", hashtable[hash_string(dp->d_name)%HASHTABLE_SIZE]->dir_name);
                printf("name of file %s\n",dp->d_name);
                hashtable_add(hashtable, dp->d_name, info.st_mtim.tv_sec, info.st_mode, dirname);
                printf("newdir: %s\n", hashtable[hash_string(dp->d_name)%HASHTABLE_SIZE]->dir_name);
            }
        }
    }
}
//Notes: need to close directory afterwards
void sync_directories(HASHTABLE *hashtable, char *dirname) {
    //Should be made more efficient
    for (int i = 0; i < HASHTABLE_SIZE; ++i) {
        LIST *current = hashtable[i];
        if(current != NULL) {
             char source[MAXPATHLEN];
             char destination[MAXPATHLEN];
             sprintf(destination, "%s/%s", dirname, current->file_name);
             printf("The path used for the file to be overwritten: %s\n", destination); //gets full path of the file in the current directory
             sprintf(source, "%s/%s", current->dir_name, current->file_name); //gets full path of the most recently modified file stored in the hashtable
             printf("The path used for the file to be copied over: %s\n", source);
            printf("The most recently modified version of %s is in the path %s\n", current->file_name, source);
            if (strcmp(source, destination)==0){
                printf("Same file don't need to copy\n");
                break;
            }
             copy_text_file(destination,source);
             if(p) {
                printf("Mod time and file permissions are being changed\n");
                setPermissions(source,destination);
                setModTime(source,destination);
             }
        }
    }
}

void printDir(char *dirname) {
    DIR *dirp;
    struct dirent *dp;
    struct stat fileStat;
    char filePath[PATH_MAX];
    dirp = opendir(dirname);
    if (dirp == NULL)
    {
        perror(dirname);
        exit(EXIT_FAILURE);
    }
    while ((dp = readdir(dirp)) != NULL)
    {
        snprintf(filePath, sizeof(filePath), "%s/%s", dirname, dp->d_name);
        if (stat(filePath, &fileStat) == -1)
        {
             perror("stat");
             continue; // Move to the next file if stat fails
        }
        char *timestr = ctime(&fileStat.st_mtime);
        timestr[24] = '\0'; // remove pesky trailing newline
        printf("%s\t%s\t", timestr, dp->d_name);
        print_permissions(fileStat.st_mode);
    }
    closedir(dirp);
}

