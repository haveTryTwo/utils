#include <stdio.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>

#include "myfifo.h"

int server();

int main(int argc, char *argv[])
{
    server();

    return 0;
}

int server()
{
    const char *common_path = MYFIFO_PATH;
    int fd_r = 0;
    int fd_w = 0;
    mode_t mask = 0664;
    int ret = 0;
    char client_path[1024] = {0};
    char buf[1024] = {0};
    mode_t mode = 0;
    int client_fd_r = 0;
    int client_fd_fifo_w = 0;
    int len = 0;
    char *pos = NULL;
    pid_t pid = 0;

    ret = mkfifo(common_path, mask);
    if (0 != ret)
    {
        printf("mkfifo failed! errno:%d, err:%s\n", errno, strerror(errno));
        return -1;
    }

    // 1. read common fifo
    fd_r = open(common_path, O_RDONLY | O_NONBLOCK, 0);
    fd_w = open(common_path, O_WRONLY, 0);
    if (-1 == fd_r)
    {
        printf("open path:%s failed! errno:%d, err:%s\n", common_path, errno, strerror(errno));
        return -1;
    }

    mode = fcntl(fd_r, F_GETFL, 0);
    mode = mode & (~O_NONBLOCK);
    fcntl(fd_r, F_SETFL, mode);

    while ((len = read(fd_r, buf, sizeof(buf))) > 0)
    {
        if (len < 0)
        {
            printf("open path:%s failed! errno:%d, err:%s\n", common_path, errno, strerror(errno));
            break;
        }
        printf("buf:%s\n", buf);

        pos = strchr(buf, ' ');
        if (NULL == pos)
        {
            printf("buf is not right! buf");
            continue;
        }
        pos++;

        pid = atol(buf);
        snprintf(client_path, sizeof(client_path), "/tmp/my_client.%d", pid);
        client_fd_fifo_w = open(client_path, O_WRONLY, 0);
        if (client_fd_fifo_w < 0)
        {
            printf("[%s : %s : %d] open path:%s failed! errno:%d, err:%s\n",
                    __FILE__, __func__, __LINE__, client_path, errno, strerror(errno));
            continue;
        }

        client_fd_r = open(pos, O_RDONLY, 0644);
        if (client_fd_r < 0)
        {
            snprintf(buf + len, sizeof(buf) - len, "cann't open\n");
            len = strlen(buf);
            write(client_fd_fifo_w, buf, len);
            close(client_fd_fifo_w);
        }
        else
        {
            memset(buf, 0, sizeof(buf));
            while ((len = read(client_fd_r, buf, MAX_LEN)) > 0)
            {
                write(client_fd_fifo_w, buf, len);
            }
            close(client_fd_r);
            close(client_fd_fifo_w);
        }
    }

    close(fd_r);
    close(fd_w);

    unlink(common_path);

    return 0;
}
