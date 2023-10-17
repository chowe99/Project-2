#include "mysync.h"
#include <stdlib.h>

//to be filled in more
//allocating space and initial values for global variables 
bool a = false;
bool n = false;
bool p = false;
bool r = false;
bool v = false;
char **o = NULL;
size_t o_index = 0;
char **i = NULL;
size_t i_index = 0;
HASHTABLE *file_list;
char OPTLIST[] = "ai:no:prv";

void save_args(int argc, char *argv[]) {
    argc--;
    int c;
    while ((c = getopt(argc, argv, OPTLIST)) != -1) 
    {
        switch (c) 
        {
            case 'a':
                a = true;
                break;

            case 'i':
                i = realloc(i, (i_index+1) * sizeof(i[0]));
                i[i_index++] = optarg;
                break;

            case 'n':
                n = true;
                break;

            case 'o':
                o = realloc(o, (o_index+1) * sizeof(o[0]));
                o[o_index++] = optarg;
                break;

            case 'p': 
                p = true;
                break;


            case 'r':
                r = true;
                break;

            case 'v':
                v = true;
                break;

            default:
                break;
            
        }
    }
}
