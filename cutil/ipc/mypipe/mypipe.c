#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

#include "client.h"
#include "server.h"
#include "util.h"

static int test_mypipe();

int main(int argc, char* argv[])
{
    test_mypipe();

    return 0;
}

static int test_mypipe()
{
    int pipe1[2] = {0};
    int pipe2[2] = {0};
    pid_t child_pid = 0;
    int ret = IPC_OK;

    pipe(pipe1);
    pipe(pipe2);

    if ((child_pid = fork()) == 0)
    {
        close(pipe1[1]);
        close(pipe2[0]);

        server(pipe1[0], pipe2[1]);
        return ret;
    }

    close(pipe1[0]);
    close(pipe2[1]);

    client(pipe2[0], pipe1[1]);

    waitpid(child_pid, NULL, 0);
    return 0;
}
