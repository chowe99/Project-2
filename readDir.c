#include "mysync.h"

void add_missing_dirs(const char *subdirectories, const char *parentdir) {

    char *subdirs_copy = strdup(subdirectories);
    char *last_slash = strrchr(subdirs_copy, '/');
    if (last_slash != NULL) {
        *last_slash = '\0';
    }
        //printf("Sub dir is %s and parent dir is %s\n", subdirs_copy, parentdir);
    char *token = strtok(subdirs_copy, "/");
    char path[PATH_MAX]; // Assuming PATH_MAX is defined in your environment, it represents the maximum length of a file path
    // Iterate through subdirectories and create missing directories

    snprintf(path, sizeof(path), "%s/", parentdir);
    while (token != NULL) {
        strcat(path, token);
        struct stat st = {0};
        printf("checking path %s\n", path);
        if (stat(path, &st) == -1) {
            // Directory does not exist, create it
            printf("Path %s doesn't exist, being creating\n", path);
            mkdir(path, 0777);
            //printf("Created directory: %s\n", path);
        }
        strcat(path, "/"); 
        token = strtok(NULL, "/");
    }
    free(subdirs_copy); // Free the dynamically allocated memory
}

void process_path(char *path, char *dirname) {
char *slash = strchr(path, '/');
    if (slash != NULL) {
        size_t length = slash - path;
        strncpy(dirname, path, length);
        dirname[length] = '\0';  // Null-terminate the directory string
        memmove(path, slash + 1, strlen(slash));   // Move characters after slash one position to the left
    }
    //printf("New path %s, new directory %s", path, dirname);
    //printf("New path %s, new directory %s", path, dirname);
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
        if (S_ISDIR(info.st_mode)) {
            if (r) {
                if(!a) {
                if (strcmp(dp->d_name, ".") == 0) {
                    continue;
                }
                }
                    read_dir(hashtable, pathname, dp->d_name);
        }
        } else {
        if (strlen(parentdirs) > 0) {
            sprintf(pathname, "%s/%s", dirname, dp->d_name);
            printf("Original pathname: %s\n", pathname);
            
            process_path(pathname, dirname);
            //printf("WITH PARENTDIR: %s\n", pathname);
            
        } else {
            sprintf(pathname, "%s", dp->d_name);
            //printf("WITHOUT PARENTDIR: %s\n", pathname);
        }
        if (!hashtable_find(hashtable, pathname)) {
            if (n) {
                //change print message 
                printf("%-10s\tneeds to be synchronized\n", pathname);
            } else {
                // abs path is wrong
                // printf("ABS PATH: %s/%s\n", dirname, pathname);
                printf("File to be added: %s, directory %s\n", pathname, dirname);
                hashtable_add(hashtable, pathname, info.st_mtim.tv_sec, info.st_mode, dirname);
                nfiles += 1;
                arrayAdd(pathname);
                printf("current status:  \n");
                printf("Printing arr:\n---\n");
                printArray();
                printf("---\n");
            }
        } else {
            //If the current file has been modified more recently, then add that 
            if (hashtable[hash_string(pathname)%HASHTABLE_SIZE]->modification < info.st_mtim.tv_sec) {
                if(v){
                    printf("updating hashlist with newest element:\n%svs\n%s\n", ctime(&hashtable[hash_string(pathname)%HASHTABLE_SIZE]->modification), ctime(&info.st_mtim.tv_sec));
                    printf("olddir: %s\n", hashtable[hash_string(dp->d_name)%HASHTABLE_SIZE]->dir_name);
                     printf("name of file %s\n", pathname);
                }
                hashtable_add(hashtable, pathname, info.st_mtim.tv_sec, info.st_mode, dirname);
                // printf("ABS PATH: %s/%s\n", dirname, pathname);

                printf("current status:  \n");
                printf("Printing arr:\n---\n");
                printArray();
                printf("---\n");
                // printf("File %s added to list with dir: %s\n", pathname, dirname);
                //printf("newdir: %s\n", hashtable[hash_string(dp->d_name)%HASHTABLE_SIZE]->dir_name);
            }
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
            //printf("current location: %s/%s\n", current->dir_name, current->file_name);
            char source[MAXPATHLEN];
            char destination[MAXPATHLEN];
//potentially include if dir, or if / exists in filename, then call add_missing_dir 
//if dir doesn't exist with full path (this includes dirname), then create it. once dir subpath created
//printf("What's been passed to missing directories: %s, %s\n", dirname, current->file_name);
//    char *slashPosition = strchr(inputString, '/');
    //if (slashPosition != NULL)
                printf("Current directory name is %s, current file name is %s\n", current->dir_name, current->file_name);
            char *isDirectory = strchr(current->file_name, '/');
            //If filename contains slash 
            if (isDirectory != NULL) {
            add_missing_dirs(current->file_name, dirname);
            printf("after syncing missing directories: \n ");
            printf("Current directory name is %s, current file name is %s\n", current->dir_name, current->file_name);
            }
            sprintf(source, "%s/%s", current->dir_name, current->file_name);
            sprintf(destination, "%s/%s", dirname, current->file_name);
            //need to compare full path 
            if (strcmp(source, destination) == 0) {
                 continue;
             }
             printf("Copying %s to %s\n", source, destination);
             //sourceis what is getting messed up 
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

