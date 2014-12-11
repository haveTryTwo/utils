#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <unistd.h>

#include "thread_pool.h"

static int move_to_running_queue(thread_pool *pool, FUNC func, void *arg);
static int move_to_sleeping_queue(thread_pool *pool, thread_pool_unit *unit);
static void* init_func(void *p);
static void cancel_cleanup_mutex(void *p);
static int fetch_and_add(int *operand, int incr);

int thread_pool_init(thread_pool *pool, int init_num)
{
    if (pool == NULL)
    {
        return THREAD_POOL_BAD_ARGS;
    }

    int total_num = 0;
    int ret = THREAD_POOL_OK;
    int i = 0;
    thread_pool_unit *unit = NULL; 
    
    total_num = (init_num <= 0 || init_num > MAX_NUM) ? MAX_NUM / 2 : init_num; 

    pool->sleeping_queue = NULL;
    pool->running_queue = NULL;

    for (i = 0; i < total_num; i++)
    {
        unit = (thread_pool_unit*) malloc(sizeof(thread_pool_unit));
        if (unit == NULL)
        {
            while ((unit = pool->sleeping_queue) != NULL)
            {
                pool->sleeping_queue = unit->next;
                
                pthread_cancel(unit->th);
                pthread_join(unit->th, NULL);
                pthread_mutex_destroy(&(unit->mutex));
                pthread_cond_destroy(&(unit->cond));
                free(unit);
                unit = NULL;
            }

            return THREAD_POOL_FAILED;
        }

        unit->func = NULL;
        unit->arg = NULL;
        unit->pool = pool;
        unit->working_status = THREAD_SLEEPING;
        pthread_mutex_init(&(unit->mutex), NULL);
        pthread_cond_init(&(unit->cond), NULL);
        ret = pthread_create(&(unit->th), NULL, init_func, (void*)unit);
        assert(ret == 0);

        unit->next = pool->sleeping_queue;
        pool->sleeping_queue = unit;
    }

    pthread_mutex_init(&(pool->queue_mutex), NULL);
    pthread_cond_init(&(pool->queue_cond), NULL);
    pool->queue_locking_num = 0;

    pool->max_num = MAX_NUM;
    pool->current_num = total_num;
    pool->sleeping_num = total_num;
    pool->running_num = 0;
    pool->enable_flags = THREAD_ENABLE;

    return THREAD_POOL_OK;
}

int thread_pool_destroy(thread_pool *pool)
{
    if (pool == NULL)
    {
        return THREAD_POOL_BAD_ARGS;
    }

    thread_pool_unit *unit = NULL;

    if (pool->enable_flags == THREAD_DISENABLE)
    {
        return THREAD_POOL_DESTROY;
    }
    // Set pool enable flags to be false
    pool->enable_flags = THREAD_DISENABLE;

    // wait for other threads to run over
    usleep(200000);

    pthread_mutex_lock(&(pool->queue_mutex));

    while (pool->queue_locking_num > 0)
    {
        pthread_cond_wait(&(pool->queue_cond), &(pool->queue_mutex));
    }

    if (pool->sleeping_num > 0)
    {
        while ((unit = pool->sleeping_queue) != NULL)
        {
            pool->sleeping_queue = unit->next;
            
            pthread_cancel(unit->th);
            pthread_join(unit->th, NULL);
            pthread_mutex_destroy(&(unit->mutex));
            pthread_cond_destroy(&(unit->cond));
            free(unit);
            unit = NULL;
        }
        pool->sleeping_num = 0;
    }

    if (pool->running_num > 0)
    {
        while ((unit = pool->running_queue) != NULL)
        {
            pool->running_queue = unit->next;

            pthread_cancel(unit->th);
            pthread_join(unit->th, NULL);
            pthread_mutex_destroy(&(unit->mutex));
            pthread_cond_destroy(&(unit->cond));
            free(unit);
            unit = NULL;
        }
        pool->running_num = 0;
    }

    pool->current_num = 0;

    pthread_mutex_unlock(&(pool->queue_mutex)); 

    pthread_mutex_destroy(&(pool->queue_mutex));

    return THREAD_POOL_OK;
}

int thread_pool_start_running(thread_pool *pool, FUNC func, void *arg)
{
    if (pool == NULL)
    {
        return THREAD_POOL_BAD_ARGS;
    }

    int ret = 0;

    ret = move_to_running_queue(pool, func, arg);
    if (ret != THREAD_POOL_OK)
    {
        return ret;
    }

    return THREAD_POOL_OK;
}

static int move_to_running_queue(thread_pool *pool, FUNC func, void *arg)
{
    if (pool == NULL)
    {
        return THREAD_POOL_BAD_ARGS;
    }

    if (pool->enable_flags == THREAD_DISENABLE)
    {
        return THREAD_POOL_DESTROY;
    }

    int ret = THREAD_POOL_OK;
    thread_pool_unit *unit = NULL;

    fetch_and_add(&(pool->queue_locking_num), 1);
    pthread_mutex_lock(&(pool->queue_mutex));
    if (pool->enable_flags == THREAD_DISENABLE)
    {
        fetch_and_add(&(pool->queue_locking_num), -1);
        pthread_cond_signal(&(pool->queue_cond));
        pthread_mutex_unlock(&(pool->queue_mutex));
        return THREAD_POOL_DESTROY;
    }
    fetch_and_add(&(pool->queue_locking_num), -1);

    if (pool->sleeping_num <= 0)
    {
        if (pool->current_num < pool->max_num)
        {
            unit = (thread_pool_unit*) malloc(sizeof(thread_pool_unit));
            if (unit == NULL)
            {
                pthread_mutex_unlock(&(pool->queue_mutex));
                return THREAD_POOL_FAILED;
            }
            unit->func = NULL;
            unit->arg = NULL;
            unit->working_status = THREAD_SLEEPING;
            pthread_mutex_init(&(unit->mutex), NULL);
            pthread_cond_init(&(unit->cond), NULL);
            unit->pool = pool;
            ret = pthread_create(&(unit->th), NULL, init_func, (void*)unit);
            assert(ret == 0);
            unit->next = pool->running_queue;
            pool->running_queue = unit;
            pool->running_num++;
            pool->current_num++;

            unit->func = func;
            unit->arg = arg;
            pthread_mutex_lock(&(unit->mutex));
            unit->working_status = THREAD_RUNNING;
            pthread_cond_signal(&(unit->cond));
            pthread_mutex_unlock(&(unit->mutex));

            pthread_mutex_unlock(&(pool->queue_mutex));

            return THREAD_POOL_OK;
        }

        pthread_mutex_unlock(&(pool->queue_mutex));
        return THREAD_POOL_FULL;
    }

    unit = pool->sleeping_queue;
    pool->sleeping_queue = unit->next;
    pool->sleeping_num--;

    unit->next = pool->running_queue;
    pool->running_queue = unit;
    pool->running_num++;

    unit->func = func;
    unit->arg = arg;
    pthread_mutex_lock(&(unit->mutex));
    unit->working_status = THREAD_RUNNING;
    pthread_cond_signal(&(unit->cond));
    pthread_mutex_unlock(&(unit->mutex));

    pthread_mutex_unlock(&(pool->queue_mutex));

    return THREAD_POOL_OK;
}

static int move_to_sleeping_queue(thread_pool *pool, thread_pool_unit *unit)
{
    if (pool == NULL || unit == NULL)
    {
        return THREAD_POOL_BAD_ARGS;
    }

    if (pool->enable_flags == THREAD_DISENABLE)
    {
        return THREAD_POOL_DESTROY;
    }

    thread_pool_unit *tmp_unit = NULL;
    int find = FALSE;

    fetch_and_add(&(pool->queue_locking_num), 1);
    pthread_mutex_lock(&(pool->queue_mutex));
    if (pool->enable_flags == THREAD_DISENABLE)
    {
        fetch_and_add(&(pool->queue_locking_num), -1);
        pthread_cond_signal(&(pool->queue_cond));
        pthread_mutex_unlock(&(pool->queue_mutex));
        return THREAD_POOL_DESTROY;
    }
    fetch_and_add(&(pool->queue_locking_num), -1);

    if (pool->running_num <= 0)
    {
        pthread_mutex_unlock(&(pool->queue_mutex));
        return THREAD_POOL_FAILED;
    }

    tmp_unit = pool->running_queue;
    if (tmp_unit == unit)
    {
        pool->running_queue = tmp_unit->next;
    }
    else
    {
        while (tmp_unit->next != NULL)
        {
            if (tmp_unit->next == unit)
            {
                tmp_unit->next = tmp_unit->next->next;
                find = TRUE;
                break;
            }
            
            tmp_unit = tmp_unit->next;
        }
        if (!find)
        {
            pthread_mutex_unlock(&(pool->queue_mutex));
            return THREAD_POOL_FAILED;
        }
    }
    pool->running_num--;

    unit->next = pool->sleeping_queue;
    pool->sleeping_queue = unit;
    pool->sleeping_num++;
    
    unit->working_status = THREAD_SLEEPING;

    pthread_mutex_unlock(&(pool->queue_mutex));

    return THREAD_POOL_OK;
}

static void* init_func(void *p)
{
    thread_pool_unit *unit = (void*)p;

    int ret = THREAD_POOL_OK;

    while (TRUE)
    {
        pthread_mutex_lock(&(unit->mutex));
        pthread_cleanup_push(cancel_cleanup_mutex, &(unit->mutex));
        while (unit->working_status == THREAD_SLEEPING)
        {
            pthread_cond_wait(&(unit->cond), &(unit->mutex));
        }
        pthread_cleanup_pop(0);
        pthread_mutex_unlock(&(unit->mutex));

        if (unit->func != NULL)
        {
            unit->func(unit->arg);
        }

        // TODO: push the unit into sleeping queue
        ret = move_to_sleeping_queue(unit->pool, unit);
        if (ret != THREAD_POOL_OK)
        {
            pthread_exit(NULL);
        }
    }

    pthread_exit(NULL);
}

static void cancel_cleanup_mutex(void *p)
{
    pthread_mutex_t *mutex = (pthread_mutex_t*)p;

    pthread_mutex_unlock(mutex);
}

static int fetch_and_add(int *operand, int incr)
{
    int result = 0; 
    asm __volatile__ (
            "lock xadd %0, %1"
            : "=r"(result), "=m"(*operand)
            : "0"(incr)
            : "memory"
            );

    return result;
}
