#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/wait.h>

int system(const char *command)
{
    pid_t pid = 0;
    int status = -1;
    sigset_t block_mask, chld_mask;
    struct sigaction int_act, quit_act, ignore_act;

    // Ignore signal SIGINT and SIGQUIT
    ignore_act.sa_handler = SIG_IGN;
    sigemptyset(&(ignore_act.sa_mask));
    ignore_act.sa_flags = 0;

    sigaction(SIGINT, &ignore_act, &int_act);
    sigaction(SIGQUIT, &ignore_act, &quit_act);

    // Block signal SIGCHLD
    sigemptyset(&block_mask);
    sigaddset(&block_mask, SIGCHLD);
    sigprocmask(SIG_BLOCK, &block_mask, &chld_mask);

    pid = fork();
    if (pid < 0)
    {
        status = -1;
    }
    else if (pid == 0)
    {
        // Restore SIGINT and SIGQUIT, unblock SIGCHLD
        sigaction(SIGINT, &int_act, NULL);
        sigaction(SIGQUIT, &quit_act, NULL);
        sigprocmask(SIG_SETMASK, &chld_mask, NULL);

        // Exec command
        execl("/bin/sh", "sh", "-c", command, NULL);

        // Exec error
        exit(-1);
    }
    else
    {
        while (waitpid(pid, &status, 0) < 0)
        {
            if (errno != EINTR)
            {
                status = -1;
                break;
            }
        }
        if (WIFEXITED(status))
            status = WEXITSTATUS(status);
        else if (WIFSIGNALED(status))
            status = WTERMSIG(status);
    }

    // Restore signale SIGINT, SIGQUIT; unblock SIGCHLD
    sigaction(SIGINT, &int_act, NULL);
    sigaction(SIGQUIT, &quit_act, NULL);
    sigprocmask(SIG_SETMASK, &chld_mask, NULL);

    return status;
}

#ifdef __SYSTEM_MAIN_DEBUG__

int main(int argc, char *argv[])
{
    system("ls");
    system("echo a");

    return 0;
}

#endif
