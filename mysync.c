#include "mysync.h"


int main(int argc, char *argv[])
{
    file_list=hashtable_new(); //Initialises hashtable to be used 

    //To do:
    //open file, and pass in the necessary info to file_add function call:
    //new version: add_File(char *filename);
    //void add_File(char* filename, time_t mtime, mode_t permissions, char* dirname)  
    //test_hashtable(); for testing purposes later
    save_args(argc, argv);
    printf("a: %d, i: %zu, n: %d, o: %zu, p: %d, r: %d, v: %d\n", a, i_index, n, o_index, p, r, v);
    return EXIT_SUCCESS;
}


