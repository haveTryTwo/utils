#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "util.h"

static int mypopen();

int main(int argc, char* argv[])
{
    mypopen();

    return 0;
}

static int mypopen()
{
    int ret = 0;
    FILE *fp = NULL;
    char cmd[MAX_LEN] = {0};
    char buf[MAX_LEN] = {0};
    size_t len = 0;
    ssize_t n = 0;

    fputs("please input command: ", stdout);
    fgets(buf, sizeof(buf), stdin);
    len = strlen(buf);
    if (buf[len - 1] == '\n')
    {
        buf[len - 1] = '\0';
    }
    len = len - 1;

    snprintf(cmd, sizeof(cmd), "%s", buf);

    fp = popen(cmd, "r");
    if (fp == NULL)
    {
        printf("popen failed! errno:%d, err:%s\n", errno, strerror(errno));
        return IPC_FAILED;
    }

    memset(buf, 0, sizeof(buf));
    while ((n = fread(buf, 1, sizeof(buf), fp)) > 0)
    {
        fputs(buf, stdout);
    }

    fclose(fp);

    return ret;
}
