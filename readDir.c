#include "mysync.h"

/**
 * Syncs directories recurisively
 * \param: subdirectories: All subdirectories needing to be synced
 * \param: parentdir: The top level directory currently being synced
*/
void add_missing_dirs(const char *subdirectories, const char *parentdir) {

    char *subdirs_copy = strdup(subdirectories);
    char *last_slash = strrchr(subdirs_copy, '/');
    if (last_slash != NULL)
    {
        *last_slash = '\0';
    }
    char *token = strtok(subdirs_copy, "/");
    char path[PATH_MAX];
    snprintf(path, sizeof(path), "%s/", parentdir);
    while (token != NULL)
    {
        strcat(path, token);
        struct stat st = {0};
        printf("checking path %s\n", path);
        // Check if the path already exists
        if (stat(path, &st) == -1)
        {
            mkdir(path, 0777);
        }
        strcat(path, "/");
        token = strtok(NULL, "/");
    }
    free(subdirs_copy);
}

/**
 * Splits the full path into the top directory, stored in dirname, and the subdirectories, stored in path
 * \param: path: full path name for the file
 * \param: dirname: current directory being read
*/
void process_path(char *path, char *dirname) {
    char *slash = strchr(path, '/');
    if (slash != NULL)
    {
        size_t length = slash - path;
        strncpy(dirname, path, length);
        dirname[length] = '\0';
        memmove(path, slash + 1, strlen(slash));
    }
}

/** 
 *Reads each entry of the directories supplied in the command line arguments
 *Adds entry to the hashtable if the file doesn't exist, or is a newer version of an existing file
 *\param: hashtable: hashtable to add entries too
 *\param: dirname: directory being processed
 *\param: Parent directories if the current directory is a subdirectory
 */
void read_dir(HASHTABLE *hashtable, char *dirname, char *parentdirs)
{
    DIR *dirp;
    struct dirent *dp;
    dirp = opendir(dirname);
    if (!dirp)
    {
        printf("Directory doesn't exist\n");
        exit(EXIT_FAILURE);
    }
    CHECK_ALLOC(dirp);

    while ((dp = readdir(dirp)) != NULL)
    {
        struct stat info;
        char pathname[MAXPATHLEN];
        sprintf(pathname, "%s/%s", dirname, dp->d_name);
        if (stat(pathname, &info) != 0)
        {
            perror(pathname);
            printf("Directory doesn't exist\n");
            exit(EXIT_FAILURE);
        }
        if (S_ISDIR(info.st_mode))
        {
            if (r)
            {
                if (!a && (strcmp(dp->d_name, ".") == 0))
                {
                    continue;
                }
                read_dir(hashtable, pathname, dp->d_name);
            }
            else {continue;}
        }
        else
        {
            if (skipEntry(dp->d_name)) {continue;}
            if (strlen(parentdirs) > 0)
            {
                sprintf(pathname, "%s/%s", dirname, dp->d_name);
                process_path(pathname, dirname);
            }
            else {
                sprintf(pathname, "%s", dp->d_name);
            }
            if (!hashtable_find(hashtable, pathname))
            {
                if (n) {
                    printf("%-10s\tneeds to be copied\n", pathname);
                }
                else
                {
                    if(v) {
                    printf("File to be added: %s, directory %s\n", pathname, dirname);
                    }
                    hashtable_add(hashtable, pathname, info.st_mtim.tv_sec, info.st_mode, dirname);
                    nfiles += 1;
                    arrayAdd(pathname);
                }
            }
            else if (hashtable[hash_string(pathname) % HASHTABLE_SIZE]->modification < info.st_mtim.tv_sec)
                {
                    if (v)
                    {
                        printf("Updating hashlist with newest element:\n%svs\n%s\n", ctime(&hashtable[hash_string(pathname) % HASHTABLE_SIZE]->modification), ctime(&info.st_mtim.tv_sec));
                    }
                    hashtable_add(hashtable, pathname, info.st_mtim.tv_sec, info.st_mode, dirname);
            }
        }
    }
    closedir(dirp);
}

/**
 * Syncs the current directory with the hashtable 
 * \param hashtable the hashtable used to sync
 * \param dirname the directory being synced 
*/
void sync_directories(HASHTABLE *hashtable, char *dirname) {
    for (int i = 0; i < nfiles; ++i)
    {
        LIST *current = file_list[hash_string(filenames[i]) % HASHTABLE_SIZE];
        if (current != NULL)
        {
            char source[MAXPATHLEN];
            char destination[MAXPATHLEN];
            char *isDirectory = strchr(current->file_name, '/');
            if (isDirectory != NULL) { 
                add_missing_dirs(current->file_name, dirname); 
            }
            sprintf(source, "%s/%s", current->dir_name, current->file_name);
            sprintf(destination, "%s/%s", dirname, current->file_name);
            if (strcmp(source, destination) != 0)
            {
                copy_text_file(destination, source);
                if (v)
                {
                        printf("File %s just copied to %s\n", source, destination);
                }
                if (p)
                {
                        setPermissions(source, destination);
                        setModTime(source, destination);
                }
            }
        }
    }
}

/**
 * Prints the contents of the given directory
 * \param dirname Directory to print
*/
void printDir(char *dirname) {
    DIR *dirp;
    struct dirent *dp;
    struct stat fileStat;
    char filePath[PATH_MAX];
    dirp = opendir(dirname);
    CHECK_ALLOC(dirp);
    while ((dp = readdir(dirp)) != NULL)
    {
        snprintf(filePath, sizeof(filePath), "%s/%s", dirname, dp->d_name);
        if (stat(filePath, &fileStat) == -1)
        {
             perror("stat");
             continue; 
        }
        char *timestr = ctime(&fileStat.st_mtime);
        timestr[24] = '\0'; 
        printf("%s\t%s\t", timestr, dp->d_name);
        print_permissions(fileStat.st_mode);
    }
    closedir(dirp);
}

