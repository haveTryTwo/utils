#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "myfifo.h"

int client(const char *path);

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        printf("Please input the whole file path for server reading:\n");
        return -1;
    }

    client(argv[1]);

    return 0;
}

int client(const char *path)
{
    const char *common_path = MYFIFO_PATH;
    char client_path[1024] = {0};
    int fd_r = 0;
    int fd_w = 0;
    mode_t mask = 0664;
    int ret = 0;
    pid_t pid = 0;
    char buf[1024] = {0};
    size_t len = 0;

    pid = getpid();

    snprintf(client_path, sizeof(client_path), "/tmp/my_client.%d", pid);

    ret = mkfifo(client_path, mask);
    if (0 != ret)
    {
        printf("mkfifo failed! errno:%d, err:%s\n", errno, strerror(errno));
        return -1;
    }

    // 1. open common fifo, and write the info to server
    fd_w = open(common_path, O_WRONLY, 0);
    if (-1 == fd_w)
    {
        printf("open path:%s failed! errno:%d, err:%s\n", common_path, errno, strerror(errno));
        return -1;
    }

    len = snprintf(buf, sizeof(buf), "%d %s", pid, path);
    write(fd_w, buf, len + 1);

    // open client fifo for reading blocking until the server write it
    fd_r = open(client_path, O_RDONLY, 0);
    if (-1 == fd_r)
    {
        printf("open path:%s failed! errno:%d, err:%s\n", client_path, errno, strerror(errno));
        return -1;
    }

    memset(buf, 0, sizeof(buf));
    while ((len = read(fd_r, buf, MAX_LEN)) > 0)
    {
        printf("%d : %s", len, buf);
    }

    close(fd_r);
    close(fd_w);

    unlink(client_path);

    return 0;
}
