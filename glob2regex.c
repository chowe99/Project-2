#include "mysync.h"

/** Convert a filename pattern (glob) to a regex, filename
 *  patterns may not include directory separators.
 *
 *  \param glob the filepattern to convert
 *  \return the a regex pointer on success and NULL on failure
*/
char *glob2regex(char *glob)
{
    char *re = NULL;

    if(glob != NULL) {
	re	= calloc(strlen(glob)*2 + 4, sizeof(char));
	if(re == NULL) {
	    return NULL;
	}

	char *r	= re;

	*r++	= '^';
	while(*glob != '\0')
	    switch (*glob) {
		case '.' :
		case '\\':
		case '$' : *r++ = '\\'; *r++ = *glob++;	break;
		case '*' : *r++ = '.';  *r++ = *glob++;	break;
		case '?' : *r++ = '.'; glob++;		break;
		case '/' : free(re);
		    re	= NULL;
		break;
		default  : *r++ = *glob++;
		break;
	}
	if(re) {
	    *r++	= '$';
	    *r		= '\0';
	}
    }
    return re;
}

/** Checks if filename matches regex.
 *
 * \param filename The file to be tested
 * \param arr The regex's to check
 * \param num_items The number of regex's to check
 * \return true if it matches else false
*/
bool is_match(char *filename, char *arr[], size_t num_items) {
    for (size_t i = 0; i<num_items; i++) {
        regex_t reegex;
        if (regcomp(&reegex, glob2regex(arr[i]), 0) != 0) {
            perror(glob2regex(arr[i]));
            exit(EXIT_FAILURE);
        }
        if (regexec(&reegex, filename, 0, NULL, 0) == 0)
        {
            return true;
        }
    }
    return false;
}
