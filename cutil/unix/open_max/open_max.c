#include <stdio.h>
#include <limits.h>
#include <unistd.h>
#include <errno.h>

#ifdef OPEN_MAX
static long _open_max = OPEN_MAX;
#else
static long _open_max = 0;
#endif

#define LOCAL_OPEN_MAX  512

#define MY_FAILED       -1
#define MY_OK           0
#define MY_ERR_PARM     1

int get_open_max(long *open_max);

int main(int argc, char* argv[])
{
    long open_max = 0;
    int ret = MY_OK;

    ret = get_open_max(&open_max);
    if (MY_OK == ret)
    {
        printf("open max is : %ld\n", open_max);
    }

    return ret;
}

int get_open_max(long *open_max)
{
    if (NULL == open_max)
    {
        return MY_ERR_PARM;
    }

    if (0 == _open_max)
    {
        _open_max = sysconf(_SC_OPEN_MAX);
        if (-1 == _open_max)
        {
            printf("sysconf(_SC_OPEN_MAX) failed! errno: %d\n", errno);
            *open_max = LOCAL_OPEN_MAX;
        }
        else 
        {
            printf("sysconf(_SC_OPEN_MAX) success!\n");
            *open_max = _open_max;
        }
    }
    else
    {
        printf("OPEN_MAX existed!\n");
        *open_max = _open_max;
    }

    return MY_OK;
}
