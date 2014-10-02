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

int get_dev(const char* path);

int main(int argc, char* argv[])
{
    if (argc <= 1)
    {
        printf("please input file name!\n");
        return FILE_FAILED;
    }

    int i = 0;

    for (i = 1; i < argc; i++)
    {
        get_dev(argv[i]);
    }
    
    return FILE_OK;
}

int get_dev(const char* path)
{
    if (path == NULL || *path == '\0')
    {
        return FILE_ERR_PARAMS;
    }

    int ret = FILE_OK;
    struct stat st;

    memset(&st, 0, sizeof(struct stat));

    ret = lstat(path, &st);
    if (ret == -1)
    {
        printf("lstat path:%s failed! errno:%d, err:%s\n", path, errno, strerror(errno));
        return FILE_FAILED;
    }

    printf("%s: ", path);
    printf("dev = %d/%d ", major(st.st_dev), minor(st.st_dev));

    if (S_ISCHR(st.st_mode))
    {
        printf(" (character) rdev = %d/%d", major(st.st_rdev), minor(st.st_rdev));
    }
    else if (S_ISBLK(st.st_mode))
    {
        printf(" (block) rdev = %d/%d", major(st.st_rdev), minor(st.st_rdev));
    }
    printf("\n");

    return ret;
}
