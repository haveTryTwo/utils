#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "server.h"
#include "util.h"

int server(int rfd, int wfd)
{
    int fd = 0;
    int ret = IPC_OK;
    ssize_t n = 0;
    char buf[MAX_LEN] = {0};

    if ((n = read(rfd, buf, sizeof(buf))) == 0)
    {
        printf("This is end of %d while reading pathname\n", rfd);
        return IPC_FAILED;
    }
    buf[n] = '\0';

    if ((fd = open(buf, O_RDONLY)) < 0)
    {
        snprintf(buf + n, sizeof(buf) - n, ": can't open, err:%s\n", strerror(errno));
        n = strlen(buf);
        write(wfd, buf, n);
        return IPC_FAILED;
    }
    else
    {
        while ((n = read(fd, buf, sizeof(buf))) > 0)
        {
            write(wfd, buf, n);
        }
        close(fd);
    }

    return ret;
}
