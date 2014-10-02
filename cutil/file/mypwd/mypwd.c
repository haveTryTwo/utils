#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>

#define DEFAULT_PATH_MAX    4096

#define FILE_FAILED                 -1
#define FILE_OK                     0
#define FILE_ERR_PARAMS             1
#define FILE_ERR_MALLOC_FAILED      2

static int get_path_max(long *path_max);
int get_mypwd();

int main(int argc, char* argv[])
{
    get_mypwd();
    
    return FILE_OK;
}

int get_mypwd()
{
    long path_max = 0;
    int ret = FILE_OK;
    char *buf = NULL;
    char *tmp = NULL;

    ret = get_path_max(&path_max);
    if (ret != FILE_OK)
    {
        printf("get_path_max failed! ret:%d\n", ret);
        return ret;
    }

    buf = (char*) malloc(path_max * sizeof(char));
    if (buf == NULL)
    {
        printf("malloc len:%ld failed! errno:%d, err:%s\n", path_max, errno, strerror(errno));
        return FILE_ERR_MALLOC_FAILED;
    }
    memset(buf, 0, path_max);

    tmp = getcwd(buf, path_max);
    if (tmp == NULL)
    {
        printf("getcwd failed! errno:%d, err:%s\n", errno, strerror(errno));
        return FILE_FAILED;
    }

    printf("%s\n", buf);

    return ret;
}

static int get_path_max(long *path_max)
{
    if (path_max == NULL)
    {
        return FILE_ERR_PARAMS;
    }

#ifdef PATH_MAX
    *path_max = PATH_MAX;
#else
    *path_max = pathconf("/", _PC_PATH_MAX);
    if (*path_max == -1)
    {
        printf("pathconf of _PC_PATH_MAX failed! errno:%d, err:%s\n", errno, strerror(errno));
        *path_max = DEFAULT_PATH_MAX;
    }
#endif

    return FILE_OK;
}
