#include "mysync.h"


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



int main(int argc, char *argv[])
{
	file_list = &(file=hashtable_new());
	//To do:
	//open file, and pass in the necessary info to file_add function call:
	//new version: add_File(char *filename, char *dirname);
	//void add_File(char* filename, time_t mtime, mode_t permissions, char* dirname)  
	//test_hashtable(); for testing purposes later
	return EXIT_SUCCESS;
}
