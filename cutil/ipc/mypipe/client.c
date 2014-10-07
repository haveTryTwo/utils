#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include "client.h"
#include "util.h"

int client(int rfd, int wfd)
{
    size_t len = 0;
    ssize_t n = 0;
    char buf[MAX_LEN] = {0};
    int ret = IPC_OK;

    fputs("Please input file path: ", stdout);
    fgets(buf, sizeof(buf), stdin);
    len = strlen(buf);

    if (buf[len - 1] == '\n')
    {
        buf[len - 1] = '\0';
    }
    len = len - 1;

    write(wfd, buf, len);

    while ((n = read(rfd, buf, sizeof(buf))) > 0)
    {
        write(STDOUT_FILENO, buf, n);
    }

    return ret;
}
