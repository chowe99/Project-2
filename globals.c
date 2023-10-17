#include "mysync.h"

//to be filled in more
//allocating space and initial values for global variables 
bool a;
bool n;
bool p;
bool r = false;
char *o;
size_t o_index = 0;
char *i;
size_t i_index = 0;
HASHTABLE *file_list;
char OPTLIST[] = "ai:no:prv";

void save_args(int argc, char *argv[]) {
    argc--;
    int c;
    while ((c = getopt(argc, argv, OPTLIST)) != -1) {

        switch (c) {
            case 'a':
            break;

            case 'i':
                i_index++; 
                *i = realloc(i, i_index * sizeof(i[0]));
                
            break;

            case 'n':
            break;

            case 'o':
            break;

            case 'p': 
            break;

            case 'r':
            break;

            case 'v':
            break;

        }
    }
}
