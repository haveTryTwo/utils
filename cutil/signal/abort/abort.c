#include <stdio.h>
#include <signal.h>
#include <stdlib.h>

void abort()
{
    struct sigaction act;
    sigset_t sig_mask;

    // Set signal handler to be SIG_DEF if it's SIG_IGN
    sigaction(SIGABRT, NULL, &act);
    if (act.sa_handler == SIG_IGN)
    {
        act.sa_handler = SIG_DFL;
        sigaction(SIGABRT, &act, NULL);
    }

    // If signal hander to be SIG_DFL, then fflush file stream
    if (act.sa_handler == SIG_DFL)
        fflush(NULL);

    sigfillset(&sig_mask);
    sigdelset(&sig_mask, SIGABRT);
    sigprocmask(SIG_SETMASK, &sig_mask, NULL);

    raise(SIGABRT);

    // If we're here, it means process has caught SIGABRT and 
    // calls signal handler
    fflush(NULL);
    sigaction(SIGABRT, &act, NULL);
    sigprocmask(SIG_SETMASK, &sig_mask, NULL);
    raise(SIGABRT);
    exit(-1);
}

#ifdef __ABORT_MAIN_DEBUG__

int main(int argc, char *argv[])
{
    printf("main begin\n");
    abort();
    printf("main end\n");

    return 0;
}

#endif
