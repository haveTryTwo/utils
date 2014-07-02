#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#ifdef PATH_MAX
static long _path_max = PATH_MAX;
#else
static long _path_max = 0;
#endif

#define LOCAL_PATH_MAX  1024

#define MY_FAILED       -1
#define MY_OK           0
#define MY_ERR_PARM     1

int get_path_max(long *path_max);

int main(int argc, char* argv[])
{
    int ret = MY_OK;
    long path_max = 0;

    ret = get_path_max(&path_max);
    if (MY_OK != ret)
    {
        printf("get path max failed! ret: %d\n", ret);
        return ret;
    }

    printf("path max: %ld\n", path_max);

    return MY_OK;
}

int get_path_max(long *path_max)
{
    if (0 == _path_max)
    {
        _path_max = pathconf("/", _PC_PATH_MAX);
        if (-1 == _path_max)
        {
            printf("pathconf failed! errno: %d\n", errno);
            _path_max = LOCAL_PATH_MAX;
        }
        else
        {
            printf("pathconf get SUCCESS!\n");
        }
    }

    *path_max = _path_max;

    return MY_OK;
}
