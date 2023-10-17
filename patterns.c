#include "mysync.h"

bool is_match(char *filename, char *arr[], size_t num_items) {
    for (size_t i = 0; i<num_items; i++) {
        regex_t reegex;
        if (regcomp(&reegex, glob2regex(arr[i]), 0) != 0) {
            perror(glob2regex(arr[i]));
            exit(EXIT_FAILURE);
        }
        if (regexec(&reegex, filename, 0, NULL, 0) == 0)
        {
            printf("found match\n");
            return true;
        }
    }
    printf("no match\n");
    return false;
}
