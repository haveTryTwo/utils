#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

int check_fd(int fd);
int set_fl(int fd, int flags);
int clr_fl(int fd, int flags);

int main(int argc, char* argv[])
{
    if (argc != 2)
    {
        printf("please input fd \n");
        return -1;
    }
    int ret = 0;

    ret = check_fd(atoi(argv[1]));
    if (ret < 0)
    {
        printf("check fd failed!\n");
        return -1;
    }

    return 0;
}

int check_fd(int fd)
{
    int ret = 0;

    ret = fcntl(fd, F_GETFL, 0);
    if (ret < 0)
    {
        printf("fcntl get the fd flags failed!fd:%d, errno:%d\n", fd, errno);
        return -1;
    }

    switch (ret & O_ACCMODE)
    {
        case O_RDONLY:
            printf("read only");
            break;
        case O_WRONLY:
            printf("writ only");
            break;
        case O_RDWR:
            printf("read and write");
            break;
        default:
            printf("unknown access mode");
            break;
    }

    if (ret & O_APPEND)
    {
        printf(", append");
    }
    if (ret & O_NONBLOCK)
    {
        printf(", nonblocking");
    }

#ifdef O_SYNC
    if (ret & O_SYNC)
    {
        printf(", synchronous writes");
    }
#endif
#if !defined(_POSIX_C_SOURCE) && defined(O_FSYNC)
    if (ret & O_FSYNC)
    {
        printf(", synchronous writes");
    }
#endif

    printf("\n");
    return 0;
}

int set_fl(int fd, int flags)
{
    int ret = 0;
    int f = 0;

    f = fcntl(fd, F_GETFL, 0);
    if (f < 0)
    {
        printf("fcntl get fd:%d failed! errno:%d\n", fd, errno);
        return -1;
    }

    f |= flags;

    ret = fcntl(fd, F_SETFL, f);
    if (ret < 0)
    {
        printf("fcntl set fd:%d failed! errno:%d\n", fd, errno);
        return -1;
    }

    return 0;
}

int clr_fl(int fd, int flags)
{
    int ret = 0;
    int f = 0;

    f = fcntl(fd, F_GETFL, 0);
    if (f < 0)
    {
        printf("fcntl get fd:%d failed! errno:%d\n", fd, errno);
        return -1;
    }

    f &= ~flags;

    ret = fcntl(fd, F_SETFL, f);
    if (ret < 0)
    {
        printf("fcntl set fd:%d failed! errno:%d\n", fd, errno);
        return -1;
    }

    return 0;
}
