#include "mysync.h"


int main(int argc, char *argv[])
{
    file_list=hashtable_new(); //Initialises hashtable to be used ;

    save_args(argc, argv);
    printf("a: %d, i: %zu, n: %d, o: %zu, p: %d, r: %d, v: %d\n", a, i_index, n, o_index, p, r, v);
    for (int i = optind; i<argc; i++) {
        read_dir(file_list, argv[i], "");
    }
    printf("Printing arr:\n---\n");
    printArray();
    printf("---\n");
    for (int i = optind; i<argc; i++) {
        if(v) {
        printf("Directory name about to be synced: %s\n", argv[i]);
        }
       sync_directories(file_list, argv[i]);
       if(v) {
        printf("Updated directory contents of %s\n", argv[i]);
        printDir(argv[i]);
       }
    }
    free(file_list);
    return EXIT_SUCCESS;
}


