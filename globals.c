#include "mysync.h"

//to be filled in more
//allocating space and initial values for global variables 
bool        a = false;
bool        n = false;
bool        p = false;
bool        r = false;
bool        v = false;
char        **o = NULL;
size_t      o_index = 0;
char        **i = NULL;
size_t      i_index = 0;
int nfiles =0;
char** filenames = NULL;
HASHTABLE   *file_list;

/** Gets the command line arguments and saves them to global 
 *  variables.
 *
 *  \param argc the number of command line arguments
 *  \param the command line arguments 
*/
void save_args(int argc, char *argv[]) {
    int c;
    argc -= optind;
    while ((c = getopt(argc, argv, OPTS)) != -1) 
    {
        printf("optind: %d, argc: %d\n", optind, argc);
        switch (c) 
        {
            case 'a':
                a = true;
                //printf("found -a\n");
                break;

            case 'i':
                //printf("found -i: ");
                i = realloc(i, (i_index+1) * sizeof(i[0]));
                i[i_index++] = strdup(optarg);
                //printf("%s\n", i[i_index-1]);
                break;

            case 'n':
                //printf("found -n\n");
                n = true;
                v = true;
                break;

            case 'o':
                //printf("found -o\n");
                o = realloc(o, (o_index+1) * sizeof(o[0]));
                o[o_index++] = strdup(optarg);
                break;

            case 'p': 
                //printf("found -p\n");
                p = true;
                break;

            case 'r':
                //printf("found -r\n");
                r = true;
                break;

            case 'v':
                //printf("found -v\n");
                v = true;
                break;

            case ':':
                //printf("option needs a value\n");
                break;

            case '?':
                //printf("unknown option: %c\n", optopt);
                break;
            
            default:
                break;
        }
    }
}

