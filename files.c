#include "mysync.h"

//Tested on kali linux 

void print_permissions(mode_t mode) {
    printf("File Permissions: ");
    
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

void setPermissions(char *source, char *dest) {
    const char *sourceFile = source; // Path to the source file
    char *destination = dest;

    // Retrieve file permissions of the source file
    struct stat statBuffer;
    if (stat(sourceFile, &statBuffer) == -1) {
        perror("Error getting file permissions");
        exit(EXIT_FAILURE);
    }

    // Get the permissions from the stat structure
    mode_t permissions = statBuffer.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);

    if (chmod(destination, permissions) == 0) {
        printf("Permissions changed successfully.\n");
        struct stat newBuffer;
            if (stat(destination, &newBuffer) == -1) {
            perror("Error getting file permissions");
            exit(EXIT_FAILURE);
            }
            mode_t new = newBuffer.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO);
            printf("File permissions of %s\n", destination);
            print_permissions(new);
    } else {
        perror("Error changing permissions");
    }

    printf("File permissions of %s\n", sourceFile);
    print_permissions(permissions);
}