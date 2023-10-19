#include "mysync.h"
#include <asm-generic/errno-base.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <sys/param.h>
#include <sys/stat.h>
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
                //printf("found -a\n");
                break;

            case 'i':
                //printf("found -i: ");
                i = realloc(i, (i_index+1) * sizeof(i[0]));
                i[i_index++] = strdup(optarg);
                //printf("%s\n", i[i_index-1]);
                break;

            case 'n':
                //printf("found -n\n");
                n = true;
                break;

            case 'o':
                //printf("found -o\n");
                o = realloc(o, (o_index+1) * sizeof(o[0]));
                o[o_index++] = strdup(optarg);
                break;

            case 'p': 
                //printf("found -p\n");
                p = true;
                break;

            case 'r':
                //printf("found -r\n");
                r = true;
                break;

            case 'v':
                //printf("found -v\n");
                v = true;
                break;

            case ':':
                //printf("option needs a value\n");
                break;

            case '?':
                //printf("unknown option: %c\n", optopt);
                break;
            
            default:
                break;
        }
    }
    return argc;
}


void add_missing_dirs(char *parentdir, char *subdir){
    if (strstr(subdir, "/")) {
        printf("trynna add %s\n to %s\n", subdir, parentdir);
        char *directory_list;
        directory_list = strtok(subdir, "/");
        int count = 0;

        // get number of directories nested
        while (directory_list != NULL) {
            count++;
            printf("DIRECTORY: %s\n", directory_list);
            directory_list = strtok(NULL, "/");
        }
        // remove file from dirlist
        count--;
        directory_list = strtok(subdir, "/");
        char fullpath[MAXPATHLEN];
        sprintf(fullpath, "%s/%s", parentdir, directory_list);
        for (size_t i = 0; i<count; i++) {
            printf("DIRECTORY ADDED: %s\n", fullpath);
            if (mkdir(fullpath, 0777) == 0 || errno == EEXIST){
                printf("Successfully made %s\n", fullpath);
                strcat(fullpath, "/");
                strcat(fullpath, directory_list);
            }
            directory_list = strtok(NULL, "/");
        }

    }

}

/** takes a hashtable and saves a directory to it
 * if the directory is not already present.
 *
 */
int read_dir(HASHTABLE *hashtable, char *dirname, char *parentdirs) {
    printf("reading: %s\n", dirname);
    DIR *dirp;
    struct dirent *dp;

    dirp = opendir(dirname);
    if (dirp == NULL) {
        perror(dirname);
        exit(EXIT_FAILURE);
    }

    int file_count = 0;
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

        if (S_ISDIR(info.st_mode)) {
            if (r) {
                if (strcmp(dp->d_name, ".") == 0) {
                    continue;
                }
                if (read_dir(hashtable, pathname, dp->d_name) == 0) {
                    continue;
                }
                // printf("pathname is %s\n", pathname);
                continue;
            } else {
                continue;
            }
        } else {
            file_count++;
        }

        if (strlen(parentdirs) > 0) {
            sprintf(pathname, "%s/%s", parentdirs, dp->d_name);
            printf("WITH PARENTDIR: %s\n", pathname);
        } else {
            sprintf(pathname, "%s", dp->d_name);
            printf("WITHOUT PARENTDIR: %s\n", pathname);
        }
        if (!hashtable_find(hashtable, pathname)) {
            if (n) {
                //change print message 
                printf("%-10s\tneeds to be synchronized\n", pathname);
            } else {
                printf("ABS PATH: %s/%s\n", dirname, pathname);
                hashtable_add(hashtable, pathname, info.st_mtim.tv_sec, info.st_mode, dirname);
                nfiles+=1;
                arrayAdd(pathname);
            }
        } else {
            //If the current file has been modified more recently, then add that 
            if (hashtable[hash_string(pathname)%HASHTABLE_SIZE]->modification < info.st_mtim.tv_sec) {
                if(v){
                    // printf("updating hashlist with newest element:\n%svs\n%s\n", ctime(&hashtable[hash_string(pathname)%HASHTABLE_SIZE]->modification), ctime(&info.st_mtim.tv_sec));
                    //printf("olddir: %s\n", hashtable[hash_string(dp->d_name)%HASHTABLE_SIZE]->dir_name);
                    // printf("name of file %s\n", pathname);
                }
                hashtable_add(hashtable, pathname, info.st_mtim.tv_sec, info.st_mode, dirname);
                printf("ABS PATH: %s/%s\n", dirname, pathname);
                nfiles++;
                // printf("File %s added to list with dir: %s\n", pathname, dirname);
                //printf("newdir: %s\n", hashtable[hash_string(dp->d_name)%HASHTABLE_SIZE]->dir_name);
            }
        }
    }
    closedir(dirp);
    return file_count;
}
//Notes: need to close directory afterwards
void sync_directories(HASHTABLE *hashtable, char *dirname) {
    for (int i = 0; i < nfiles; ++i) {
        printf("File %d: %s\n", i + 1, filenames[i]);
        LIST *current = file_list[hash_string(filenames[i]) % HASHTABLE_SIZE];
        if(current != NULL) {
            printf("current location: %s/%s\n", current->dir_name, current->file_name);
            char source[MAXPATHLEN];
            char destination[MAXPATHLEN];

            add_missing_dirs(dirname, current->file_name);

            sprintf(source, "%s/%s", current->dir_name, current->file_name);
            sprintf(destination, "%s/%s", dirname, current->file_name);
            // if (strcmp(source, destination) == 0) {
            //     continue;
            // }
            copy_text_file(destination, source);

            if(v) {
                printf("File %s just copied to %s\n", source, destination);
            }
            if(p) {
                //printf("Mod time and file permissions are being changed\n");
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

