#ifndef __THREAD_POOL_H__
#define __THREAD_POOL_H__

#define THREAD_POOL_FAILED              -1
#define THREAD_POOL_OK                  0
#define THREAD_POOL_BAD_ARGS            1
#define THREAD_POOL_MUTEX_INIT_FAILED   2
#define THREAD_POOL_FULL                3

#define THREAD_SLEEPING     0 
#define THREAD_RUNNING      1

#define THREAD_ENABLE       0
#define THREAD_DISENABLE    1

#define TRUE                1
#define FALSE               0

#define MAX_NUM             100

#include <pthread.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*FUNC)(void*); 

typedef struct thread_pool_unit
{
    FUNC func;
    void *arg;
    int working_status;
    pthread_t th;
    pthread_mutex_t mutex;
    pthread_cond_t cond;
    struct thread_pool_unit *next;
    struct thread_pool *pool;
} thread_pool_unit;

typedef struct thread_pool
{
    struct thread_pool_unit *running_queue;
    struct thread_pool_unit *sleeping_queue;
    pthread_mutex_t queue_mutex;
    int max_num;
    int current_num;
    int running_num;
    int sleeping_num;
    int enable_flags;
} thread_pool;


int thread_pool_init(thread_pool *pool, int init_num);
int thread_pool_destroy(thread_pool *pool);
int thread_pool_start_running(thread_pool *pool, FUNC func, void *arg);

#ifdef __cplusplus
}
#endif

#endif
