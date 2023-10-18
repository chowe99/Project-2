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
HASHTABLE   *file_list;

/* checks command line options and saves them to corresponding
 * global variables, returns number of options found.
 * 
 */
