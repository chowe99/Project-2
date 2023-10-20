#include "mysync.h"


int main(int argc, char *argv[])
{
    file_list=hashtable_new(); //Initialises hashtable to be used ;
    save_args(argc, argv);
    for (int i = optind; i<argc; i++) {
        read_dir(file_list, argv[i], "");
    }
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
    free(filenames);
    return EXIT_SUCCESS;
}


