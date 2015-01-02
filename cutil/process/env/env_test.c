#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

#define PROC_FAILED         -1
#define PROC_OK             0
#define PROC_NULL_PARAM     1

extern char **environ;

static int test_env();
static int test_getenv(const char *name);
static int test_putenv(char *string);
static int test_setenv(const char *name, const char *value);
static int test_unsetenv(const char *name);

int main(int argc, char *argv[])
{
    if (argc == 1)
    {
        goto help;
    }

    if (strcmp(argv[1], "list") == 0)
    {
        return test_env();
    }
    else if (strcmp(argv[1], "get") == 0)
    {
        int i = 0;
        for (i = 2; i < argc; i++)
        {
            test_getenv(argv[i]);
        }
        return PROC_OK;
    }
    else if (strcmp(argv[1], "put") == 0)
    {
        return test_putenv(argv[2]); 
    }
    else if (strcmp(argv[1], "set") == 0)
    {
        return test_setenv(argv[2], argv[3]);
    }
    else if (strcmp(argv[1], "unset") == 0)
    {
        return test_unsetenv(argv[2]);
    }

help:
    printf("Usage: env command [args] );\n"
           "\n"
           "The commands are:\n"
           "\n"
           "list    list current whole environment variables\n"
           "get     get special environment variable\n"
           "put     put the environment variable, for example: put PATH=.:/root\n"
           "set     set the environt variable, for example: set PATH .:/root\n"
           "unset   delete the variable from the envrionment\n"
          );

    return PROC_OK;
}

static int test_env()
{
    char **tmp = NULL;

    for (tmp = environ; *tmp != NULL; tmp++)
    {
        printf("%s\n", *tmp);
    }

    return PROC_OK;
}

static int test_getenv(const char *name)
{
    if (name == NULL)
    {
        return PROC_NULL_PARAM;
    }

    char *value = NULL;

    value = getenv(name);
    if (value == NULL)
    {
        printf("name:%s is not exist!\n", name);
        return PROC_FAILED;
    }
    printf("%s=%s\n", name, value);

    return PROC_OK;
}

static int test_putenv(char *string)
{
    if (string == NULL)
    {
        return PROC_NULL_PARAM;
    }

    int ret = PROC_OK;
    char buf[1024] = {0};
    char *pos = NULL;

    ret = putenv(string);
    if (ret != 0)
    {
        printf("putenv failed! errno:%d, err:%s\n", errno, strerror(errno));
        return PROC_FAILED;
    }

    pos = strchr(string, '=');
    if (pos == NULL)
    {
        printf("not found = in string:%s\n", string);
        return PROC_FAILED;
    }

    memcpy(buf, string, pos - string);
    buf[pos - string] = '\0';

    test_getenv(buf);
    printf("put string:%s success!\n", string);

    return PROC_OK;
}

static int test_setenv(const char *name, const char *value)
{
    if (name == NULL || value == NULL)
    {
        return PROC_NULL_PARAM;
    }

    int ret = PROC_OK;

    ret = setenv(name, value, 1);
    if (ret == -1)
    {
        printf("setenv failed! errno:%d, err:%s\n", errno, strerror(errno));
        return PROC_FAILED;
    }

    test_getenv(name);
    printf("set %s=%s success!\n", name, value);

    return PROC_OK;
}

static int test_unsetenv(const char *name)
{
    if (name == NULL)
    {
        return PROC_NULL_PARAM;
    }

    int ret = PROC_OK;

    test_getenv(name);

    ret = unsetenv(name);
    if (ret == -1)
    {
        printf("unsetenv failed! errno:%d, err:%s\n", errno, strerror(errno));
        return PROC_FAILED;
    }

    test_getenv(name);

    return PROC_OK;
}
