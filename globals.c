#include "mysync.h"

//to be filled in more
//allocating space and initial values for global variables 
bool        a           = false;
bool        n           = false;
bool        p           = false;
bool        r           = false;
bool        v           = false;
char        **o         = NULL;
size_t      o_index     = 0;
char        **i         = NULL;
size_t      i_index     = 0;
int         nfiles      = 0;
char**      filenames   = NULL;
HASHTABLE   *file_list  = NULL;

/** Gets the command line arguments and saves them to global 
 *  variables.
 *
 *  \param argc the number of command line arguments
 *  \param the command line arguments 
*/
void save_args(int argc, char *argv[]) {
    int c;

    while ((c = getopt(argc, argv, OPTS)) != -1) 
    {
        switch (c) 
        {
            case 'a':
                a = true;
                break;

            case 'i':
                i = realloc(i, (i_index+1) * sizeof(i[0]));
                i[i_index++] = strdup(optarg);
                break;

            case 'n':
                n = true;
                v = true;
                break;

            case 'o':
                o = realloc(o, (o_index+1) * sizeof(o[0]));
                o[o_index++] = strdup(optarg);
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

            case ':':
                break;

            case '?':
                break;
            
            default:
                break;
        }
    }
    argc -= optind;
    if (argc < 2) {
        printf("Usage: ./mysync  [options]  directory1  directory2  [directory3  ...]\n");
        exit(EXIT_FAILURE);
    }
}

