#include <inttypes.h>
#include <sys/time.h>

//  RETURNS THE CURRENT TIME, AS A NUMBER OF MILLISECONDS, IN A 64-bit INTEGER
int64_t milliseconds(void)
{
    struct timeval  now;

    gettimeofday( &now, NULL );       // timezone not required, so we pass NULL
    return ((int64_t)now.tv_sec * 1000) + now.tv_usec/1000;
}
