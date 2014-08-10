#include <stdio.h>
#include <sys/stat.h>
#include <errno.h>
#include <string.h>

#define STAT_OK     0
#define STAT_FAILED -1

static int check_stat(const char *path);

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Please input file\n");
        return STAT_OK;
    }

    int i = 0;

    for (i = 1; i < argc; i++)
    {
        check_stat(argv[i]);
    }

    return STAT_OK;
}

static int check_stat(const char *path)
{
    struct stat stat;
    int ret = 0;

    memset(&stat, 0, sizeof(struct stat));

    ret = lstat(path, &stat);
    if (ret == -1)
    {
        printf("lstat path: %s failed! errno:%d\n", path, errno);
        return STAT_FAILED;
    }

    if (S_ISREG(stat.st_mode))
    {
        printf("file:%s is \033[33;1mregular \033[0mfile\n", path);
        return STAT_OK;
    }
    else if (S_ISDIR(stat.st_mode))
    {
        printf("file:%s is \033[33;1mdirectory \033[0mfile\n", path);
        return STAT_OK;
    }
    else if (S_ISCHR(stat.st_mode))
    {
        printf("file:%s is \033[33;1mcharacter \033[0mfile\n", path);
        return STAT_OK;
    }
    else if (S_ISBLK(stat.st_mode))
    {
        printf("file:%s is \033[33;1mblock \033[0mfile\n", path);
        return STAT_OK;
    }
    else if (S_ISFIFO(stat.st_mode))
    {
        printf("file:%s is \033[33;1mFIFO \033[0mfile\n", path);
        return STAT_OK;
    }
    else if (S_ISLNK(stat.st_mode))
    {
        printf("file:%s is \033[33;1mlink \033[0mfile\n", path);
        return STAT_OK;
    }
    else if (S_ISSOCK(stat.st_mode))
    {
        printf("file:%s is \033[33;1msock \033[0mfile\n", path);
        return STAT_OK;
    }
    else 
    {
        printf("file:%s type is unknown\n", path);
        return STAT_FAILED;
    }

    return STAT_OK;
}
