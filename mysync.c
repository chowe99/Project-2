#include "mysync.h"
#include <stdio.h>

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
			printf("%s",line);
        }
    }
	printf("Files haven't been opened\n");
//  ENSURE THAT WE ONLY CLOSE FILES THAT ARE OPEN
    if(fp_in != NULL) {
        fclose(fp_in);
    }
    if(fp_out != NULL) {
        fclose(fp_out);
    }
}



int main(int argc, char *argv[])
{
    file_list=hashtable_new(); //Initialises hashtable to be used 
    char* destination_point = "test1.txt";
    char* source_point = "test2.txt";
    char destination[100]; //To change to MAXPATHLEN 
    char source[100];
    sprintf(destination, "%s", destination_point);
    sprintf(source, "%s", source_point);
    copy_text_file(destination, source);
    copy_text_file("test1.txt", "test2.txt");
    printf("%s,%s", destination, source);

    //To do:
    //open file, and pass in the necessary info to file_add function call:
    //new version: add_File(char *filename);
    //void add_File(char* filename, time_t mtime, mode_t permissions, char* dirname)  
    //test_hashtable(); for testing purposes later
    save_args(argc, argv);
    printf("a: %b, i: %zu, n: %b, o: %zu, p: %b, r: %b, v: %b", a, i_index, n, o_index, p, r, v);
    read_dir(argv[1]);
    return EXIT_SUCCESS;
}


