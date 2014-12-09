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
    struct thread_pool_unit *running_queue;     // threads in this queue are running
    struct thread_pool_unit *sleeping_queue;    // threads in this queue are sleeping
    pthread_mutex_t queue_mutex;    // lock for opreating the queue
    pthread_cond_t queue_cond;      // condition for destroy
    int queue_locking_num;          // number of waiting for queue mutex
    int max_num;                    // the max number of threads in the pool
    int current_num;                // the number of current threads in the pool
    int running_num;                // the number of running threads
    int sleeping_num;               // the number of sleeping threads
    int enable_flags;               // the flags whether current pool is available
} thread_pool;


int thread_pool_init(thread_pool *pool, int init_num);
int thread_pool_destroy(thread_pool *pool);
int thread_pool_start_running(thread_pool *pool, FUNC func, void *arg);

#ifdef __cplusplus
}
#endif

#endif
