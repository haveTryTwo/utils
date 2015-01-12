#include <stdio.h>
#include <signal.h>
#include <unistd.h>

static void sig_alarm(int signo);

unsigned int sleep(unsigned int seconds)
{
    struct sigaction new_act, old_act;
    sigset_t old_mask, new_mask, sus_mask;
    unsigned int unslept = 0;

    // set new signal handler for signal SIGALRM
    new_act.sa_handler = sig_alarm;
    sigemptyset(&(new_act.sa_mask));
    new_act.sa_flags = 0;
    sigaction(SIGALRM, &new_act, &old_act);

    // block SIGALRM
    sigemptyset(&new_mask);
    sigaddset(&new_mask, SIGALRM);
    sigprocmask(SIG_BLOCK, &new_mask, &old_mask);

    alarm(seconds);

    // wait for signal including SIGALRM
    sus_mask = old_mask;
    sigdelset(&sus_mask, SIGALRM);
    sigsuspend(&sus_mask);

    // get the left seconds
    unslept = alarm(0);

    // recover the signal handler and signal mask
    sigaction(SIGALRM, &old_act, NULL);
    sigprocmask(SIG_SETMASK, &old_mask, NULL);

    return unslept;
}

static void sig_alarm(int signo)
{
    // do nothing
}

#ifdef __SLEEP_MAIN_DEBUG__

int main(int argc, char *argv[])
{
    unsigned int left_time = 0;

    printf("start\n");
    left_time = sleep(2);
    printf("left time:%u end\n", left_time);

    return 0;
}

#endif
