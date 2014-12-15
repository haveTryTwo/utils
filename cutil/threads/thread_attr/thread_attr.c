#include <stdio.h>
#include <string.h>
#include <pthread.h>

#define THREAD_ATTR_OK          0
#define THREAD_ATTR_FAILED      -1

static int print_thread_attr(pthread_attr_t *attr);
static int print_mutexattr(const pthread_mutexattr_t *mutex_attr);
static int print_condattr(const pthread_condattr_t *cond_attr);
static int print_rwlockattr(const pthread_rwlockattr_t *rwlock_attr);
int get_thread_attr();

int main(int argc, char *argv[])
{
    get_thread_attr();

    return 0;
}

int get_thread_attr()
{
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    print_thread_attr(&attr);
    pthread_attr_destroy(&attr);
    printf("\n");

    pthread_mutexattr_t mutex_attr;
    pthread_mutexattr_init(&mutex_attr);
    print_mutexattr(&mutex_attr);
    pthread_mutexattr_destroy(&mutex_attr);
    printf("\n");

    pthread_rwlockattr_t rwlock_attr;
    pthread_rwlockattr_init(&rwlock_attr);
    print_rwlockattr(&rwlock_attr);
    pthread_rwlockattr_destroy(&rwlock_attr);
    printf("\n");

    pthread_condattr_t cond_attr;
    pthread_condattr_init(&cond_attr);
    print_condattr(&cond_attr);
    pthread_condattr_destroy(&cond_attr);
    printf("\n");

    return THREAD_ATTR_OK;
}

static int print_thread_attr(pthread_attr_t *attr)
{
    if (attr == NULL)
    {
        return THREAD_ATTR_FAILED;
    }

    int detach_state = 0;
    int ret = 0;
    char *stack_addr = NULL;
    size_t stack_size = 0;
    size_t guard_size = 0;

    ret = pthread_attr_getdetachstate(attr, &detach_state);
    if (detach_state == PTHREAD_CREATE_JOINABLE)
    {
        printf("Thread detach state: JOINABLE\n");
    }
    else if (detach_state == PTHREAD_CREATE_DETACHED)
    {
        printf("Thread detach state: DETACHED\n");
    }

    ret = pthread_attr_getstack(attr, (void**)&stack_addr, &stack_size);
    printf("stack_addr:%p, stack_size:%zd\n", stack_addr, stack_size);

    ret = pthread_attr_getstacksize(attr, &stack_size);
    printf("stack size:%#x\n", stack_size);

    ret = pthread_attr_getguardsize(attr, &guard_size);
    printf("guard size:%#x\n", guard_size);


    ret = pthread_getconcurrency();
    printf("concurrency is %d\n", ret);
    
    return THREAD_ATTR_OK;
}

static int print_mutexattr(const pthread_mutexattr_t *mutex_attr)
{
    if (mutex_attr == NULL)
    {
        return THREAD_ATTR_FAILED;
    }

    int ret = 0;
    int pshare = 0;
    int type = 0;

    ret = pthread_mutexattr_getpshared(mutex_attr, &pshare);
    if (pshare == PTHREAD_PROCESS_PRIVATE)
    {
        printf("mutex shared: PRIVATE\n");
    }
    else if (pshare == PTHREAD_PROCESS_SHARED)
    {
        printf("mutex shared: SHARED\n");
    }
    
    ret = pthread_mutexattr_gettype(mutex_attr, &type);
    if (type == PTHREAD_MUTEX_NORMAL)
    {
        printf("mutex NORMAL\n");
    }
    else if (type == PTHREAD_MUTEX_ERRORCHECK)
    {
        printf("mutex ERRORCHECK\n");
    }
    else if (type == PTHREAD_MUTEX_RECURSIVE)
    {
        printf("mutex RECURSIVE\n");
    }
    else if (type == PTHREAD_MUTEX_DEFAULT)
    {
        printf("mutex DEFAULT\n");
    }
    
    return THREAD_ATTR_OK;
}

static int print_rwlockattr(const pthread_rwlockattr_t *rwlock_attr)
{
    if (rwlock_attr == NULL)
    {
    printf("yes\n");
        return THREAD_ATTR_FAILED;
    }

    int ret = 0;
    int pshare = 0;

    ret = pthread_rwlockattr_getpshared(rwlock_attr, &pshare);
    if (pshare == PTHREAD_PROCESS_PRIVATE)
    {
        printf("rwlock shared: PRIVATE\n");
    }
    else if (pshare == PTHREAD_PROCESS_SHARED)
    {
        printf("rwlock shared: SHARED\n");
    }

    return THREAD_ATTR_OK;
}

static int print_condattr(const pthread_condattr_t *cond_attr)
{
    if (cond_attr == NULL)
    {
        return THREAD_ATTR_FAILED;
    }

    int ret = 0;
    int pshare = 0;

    ret = pthread_condattr_getpshared(cond_attr, &pshare);
    if (pshare == PTHREAD_PROCESS_PRIVATE)
    {
        printf("cond shared: PRIVATE\n");
    }
    else if (pshare == PTHREAD_PROCESS_SHARED)
    {
        printf("cond shared: SHARED\n");
    }

    return THREAD_ATTR_OK;
}
