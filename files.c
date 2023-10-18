#include "mysync.h"

//Tested on kali linux 
//Filename needs to be the path to the file 
//Directory name: full path minus the /filename
//int modified: the time modified 
void add_File(char *filename, char* dir_name) {
  struct stat  stat_buffer;
  //Attempt to stat file's attributes
  //should be full path 
  if(stat(filename, &stat_buffer) != 0) {
    perror(filename);
    exit(EXIT_FAILURE);
  }
  else if(S_ISREG(stat_buffer.st_mode ) ) {
    //Gets required information
    mode_t permissions = stat_buffer.st_mode & (S_IRWXU | S_IRWXG | S_IRWXO); //set permissions
    time_t mod_time=stat_buffer.st_mtime;
    printf("mod time: %i\n", (int)mod_time); //mod time here is correct
    hashtable_add(file_list, filename, mod_time, permissions, dir_name);
  }
}

int compare_mtime_descending(const void *v1, const void *v2)
{
   LIST *f1 = (LIST *)v1;
   LIST *f2 = (LIST *)v2;
   printf("Modification time of the first file is %li, and the modification time of the second is %li\n", f1->modification, f2->modification);
    if (f2->modification < f1->modification) {
      printf("File %s is bigger", f1->file_name );
      return -1;
    }
    if (f2->modification > f1->modification) {
printf("File %s is bigger", f2->file_name );
    return 1;
    }
    return 0;
}

//Creates file if it doesn't exist, or overwrites it with the desired informatoin 
void copy_text_file(char destination[], char source[])
{
    FILE        *fp_in   = fopen(source, "r");
    FILE        *fp_out  = fopen(destination,  "w");
//  ENSURE THAT OPENING BOTH FILES HAS BEEN SUCCESSFUL
    if(fp_in != NULL && fp_out != NULL) {
        char    line[BUFSIZ];
        while( fgets(line, sizeof line, fp_in) != NULL) {  
            if(fputs(line, fp_out) == EOF) {
                printf("error copying file\n");
                exit(EXIT_FAILURE);
            }
        }
    }
//  ENSURE THAT WE ONLY CLOSE FILES THAT ARE OPEN
    if(fp_in != NULL) {
        fclose(fp_in);
    }
    if(fp_out != NULL) {
        fclose(fp_out);
    }
}


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
