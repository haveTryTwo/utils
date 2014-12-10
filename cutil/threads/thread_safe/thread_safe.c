#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <errno.h>

#include "thread_safe.h"

static pthread_key_t my_safe_key;
static pthread_once_t my_once_control = PTHREAD_ONCE_INIT;

static void my_init_key();
static void my_destructor(void *p);

char* my_strerror_r(int my_errno)
{
    char *buf = NULL;

    pthread_once(&my_once_control, my_init_key);

    buf = (char*)pthread_getspecific(my_safe_key);
    if (buf == NULL)
    {
        buf = (char*) malloc(sizeof(char) * THREAD_MAX_LEN);
        if (buf == NULL)
        {
            return NULL;
        }
        memset(buf, 0, sizeof(char) * THREAD_MAX_LEN);
        pthread_setspecific(my_safe_key, (void*)buf);
    }

    switch (my_errno)
    {
        case EPERM:
            strcpy(buf, "Operation not permitted");
            break;
        case ENOENT:
            strcpy(buf, "No such file or directory");
            break;
        case ESRCH:
            strcpy(buf, "No such process");
            break;
        case EINTR:
            strcpy(buf, "Interrupted system call");
            break;
        case EIO:
            strcpy(buf, "Input/output error");
            break;
        default:
            break;
    }

    return buf;
}

static void my_init_key()
{
    pthread_key_create(&my_safe_key, my_destructor);
}

static void my_destructor(void *p)
{
    free(p);
}
