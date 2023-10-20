#include "mysync.h"

/** Creates file if it doesn't exist, or overwrites it with the desired information 
 *
 * \param destination the location we want to copy the file to 
 * \param source the location of the file
*/
void copy_text_file(char destination[], char source[])
{
    FILE        *fp_in   = fopen(source, "r");
    FILE        *fp_out  = fopen(destination,  "w");
    if(fp_in != NULL && fp_out != NULL) {
        char    line[BUFSIZ];
        while( fgets(line, sizeof line, fp_in) != NULL) { 
            if(fputs(line, fp_out) == EOF) {
                exit(EXIT_FAILURE);
            }
        }
    }
    if(fp_in != NULL) {
        fclose(fp_in);
    }
    if(fp_out != NULL) {
        fclose(fp_out);
    }
}

/** Check the file permissions given a mode_t
 *
 * \param mode the permission to check
*/
void print_permissions(mode_t mode) {
    //printf("File Permissions: ");

    // Owner's permissions
    printf((mode & S_IRUSR) ? "r" : "-");
    printf((mode & S_IWUSR) ? "w" : "-");
    printf((mode & S_IXUSR) ? "x" : "-");

    // Group's permissions
    printf((mode & S_IRGRP) ? "r" : "-");
    printf((mode & S_IWGRP) ? "w" : "-");
    printf((mode & S_IXGRP) ? "x" : "-");

    // Others' permissions
    printf((mode & S_IROTH) ? "r" : "-");
    printf((mode & S_IWOTH) ? "w" : "-");
    printf((mode & S_IXOTH) ? "x" : "-");
    printf("\n");

}

/** Sets the permission of the destination file 
 *  with the source file permissions.
 *
 *  \param source the source file
 *  \pram dest the destination file
*/
void setPermissions(char *source, char *dest) {
    const char *sourceFile = source; // Path to the source file
    char *destination = dest;
    struct stat statBuffer;
    if (stat(sourceFile, &statBuffer) == -1) {
        perror("Error getting file permissions");
        exit(EXIT_FAILURE);
    }
    mode_t permissions = statBuffer.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
    if (chmod(destination, permissions) == 0) {
        struct stat newBuffer;
        if (stat(destination, &newBuffer) == -1) {
            perror("Error getting file permissions");
            exit(EXIT_FAILURE);
        }
    } else {
        perror("Error changing permissions");
    }
}

/** Set the modification of the dest file as the 
 *  modification time of the source file
 *
 *  \param source the source file
 *  \param dest the destination file
*/
void setModTime(char *source, char *dest) {
    // Path to the source file, change it to not be constant
    struct stat source_stat;
    if (stat(source, &source_stat) != 0) {
        perror("Error getting file mod time");
        exit(EXIT_FAILURE);
    }

    struct utimbuf new_times;
    new_times.modtime = source_stat.st_mtime; 
    if (utime(dest, &new_times) != 0) {
        perror("Error setting modification time");
        exit(EXIT_FAILURE);
    } 

}
