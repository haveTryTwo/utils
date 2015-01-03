#include <stdio.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define PROC_OK         0
#define PROC_FAILED     -1
#define PROC_NULL_PARM  1


static int test_getallrlimit();
static int test_getrlimit(const char *res_name, int res);
static int test_setrlimit(const char *res_name, int res, rlim_t rlim_cur);

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        goto help;
    }
    else if (strcmp(argv[1], "get") == 0)
    {
        if (argc == 2)
        {
            return test_getallrlimit();
        }
        else 
        {
            if (strcmp(argv[2], "RLIMIT_CORE") == 0)
            {
                test_getrlimit("RLIMIT_CORE", RLIMIT_CORE);
            }
            else if (strcmp(argv[2], "RLIMIT_AS") == 0)
            {
                test_getrlimit("RLIMIT_AS", RLIMIT_AS);
            }
        }
        return PROC_OK;
    }
    else if (strcmp(argv[1], "set") == 0)
    {
        if (argc == 2)
        {
            goto help;
        }
        
        if (strcmp(argv[2], "RLIMIT_CORE") == 0)
        {
            test_setrlimit("RLIMIT_CORE", RLIMIT_CORE, atoi(argv[3]));
        }
        else if (strcmp(argv[2], "RLIMIT_AS") == 0)
        {
            test_setrlimit("RLIMIT_AS", RLIMIT_AS, atoi(argv[3]));
        }
        else if (strcmp(argv[2], "RLIMIT_NOFILE") == 0)
        {
            test_setrlimit("RLIMIT_NOFILE", RLIMIT_NOFILE, atoi(argv[3]));
        }
        return PROC_OK;
    }

help:
    printf("Usage: rlimit command [args]\n"
           "\n"
           "The commands are: \n"
           "get         get the resource limit information\n"
           "set         set the resource limit information\n"
          );

    return PROC_OK;
}

static int test_getrlimit(const char *res_name, int res)
{
    struct rlimit rlim;
    int ret = 0;

    memset(&rlim, 0, sizeof(struct rlimit));
    ret = getrlimit(res, &rlim);
    if (ret != 0)
    {
        printf("getrlimit failed! errno:%d, err:%s\n", errno, strerror(errno));
        return PROC_FAILED;
    }
    printf("%-20s: soft:%10lu, hard:%10lu\n", res_name, rlim.rlim_cur, rlim.rlim_max);

    return PROC_OK;
}

static int test_getallrlimit()
{
    test_getrlimit("RLIMIT_AS", RLIMIT_AS);
    test_getrlimit("RLIMIT_CORE", RLIMIT_CORE);
    test_getrlimit("RLIMIT_CPU", RLIMIT_CPU);
    test_getrlimit("RLIMIT_FSIZE", RLIMIT_FSIZE);
    test_getrlimit("RLIMIT_DATA", RLIMIT_DATA);
    test_getrlimit("RLIMIT_STACK", RLIMIT_STACK);
    test_getrlimit("RLIMIT_RSS", RLIMIT_RSS);
    test_getrlimit("RLIMIT_NOFILE", RLIMIT_NOFILE);
    test_getrlimit("RLIMIT_OFILE", RLIMIT_OFILE);
    test_getrlimit("RLIMIT_NPROC", RLIMIT_NPROC);
    test_getrlimit("RLIMIT_MEMLOCK", RLIMIT_MEMLOCK);
    test_getrlimit("RLIMIT_LOCKS", RLIMIT_LOCKS);
    test_getrlimit("RLIMIT_SIGPENDING", RLIMIT_SIGPENDING);
    test_getrlimit("RLIMIT_MSGQUEUE", RLIMIT_MSGQUEUE);
    test_getrlimit("RLIMIT_NICE", RLIMIT_NICE);
    test_getrlimit("RLIMIT_RTPRIO", RLIMIT_RTPRIO);
    test_getrlimit("RLIMIT_NLIMITS", RLIMIT_NLIMITS);

    return PROC_OK;
}

static int test_setrlimit(const char *res_name, int res, rlim_t rlim_cur)
{
    struct rlimit rlim;
    int ret = 0;

    ret = getrlimit(res, &rlim);
    if (ret != 0)
    {
        printf("getrlimit failed! errno:%d, err:%s\n", errno, strerror(errno));
        return PROC_FAILED;
    }
    printf("%-20s: soft:%10lu, hard:%10lu\n", res_name, rlim.rlim_cur, rlim.rlim_max);

    rlim.rlim_cur = rlim_cur;
    ret = setrlimit(res, &rlim);
    if (ret != 0)
    {
        printf("setrlimit failed! errno:%d, err:%s\n", errno, strerror(errno));
        return PROC_FAILED;
    }
    test_getrlimit(res_name, res);

    return PROC_OK;
}
