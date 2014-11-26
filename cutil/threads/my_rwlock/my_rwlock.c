#include <stdio.h>
#include <pthread.h>
#include <errno.h>

#include "my_rwlock.h"

static void my_pthread_rwlock_cancel_rdlock(void *p);
static void my_pthread_rwlock_cancel_wrlock(void *p);

int my_pthread_rwlock_init(my_pthread_rwlock_t *rwlock, my_pthread_rwlockattr_t *attr)
{
    if (rwlock == NULL)
    {
        return MY_FAILED;
    }

    if (attr != NULL)
    { // currnet not support attr
        return MY_FAILED;
    }

    int ret = 0;

    ret = pthread_mutex_init(&(rwlock->rw_mutex), NULL);
    if (ret != 0)
    {
        goto err1;
    }
    ret = pthread_cond_init(&(rwlock->rw_condreaders), NULL);
    if (ret != 0)
    {
        goto err2;
    }
    ret = pthread_cond_init(&(rwlock->rw_condwriters), NULL);
    if (ret != 0)
    {
        goto err3;
    }

    rwlock->rw_nwaitwriters = 0;
    rwlock->rw_nwaitreaders = 0;
    rwlock->rw_refcount = 0;
    rwlock->rw_magic = MY_RW_MAGIC;

    return MY_OK;

err3:
    pthread_cond_destroy(&(rwlock->rw_condwriters));
err2:
    pthread_cond_destroy(&(rwlock->rw_condreaders));
err1:
    pthread_mutex_destroy(&(rwlock->rw_mutex));

    return ret;
}

int my_pthread_rwlock_destroy(my_pthread_rwlock_t *rwlock)
{
    if (rwlock == NULL)
    {
        return MY_FAILED;
    }

    if (rwlock->rw_magic != MY_RW_MAGIC)
    {
        return EINVAL;
    }

    if (rwlock->rw_refcount != 0 || rwlock->rw_nwaitreaders != 0 
            || rwlock->rw_nwaitwriters != 0)
    {
        return EBUSY;
    }

    pthread_mutex_destroy(&(rwlock->rw_mutex));
    pthread_cond_destroy(&(rwlock->rw_condwriters));
    pthread_cond_destroy(&(rwlock->rw_condreaders));
    rwlock->rw_magic = 0;

    return MY_OK;
}

int my_pthread_rwlock_rdlock(my_pthread_rwlock_t *rwlock)
{
    if (rwlock == NULL)
    {
        return MY_FAILED;
    }

    if (rwlock->rw_magic != MY_RW_MAGIC)
    {
        return EINVAL;
    }

    int ret = 0;

    ret = pthread_mutex_lock(&(rwlock->rw_mutex));
    if (ret != 0)
    {
        return EINVAL;
    }

    while (rwlock->rw_refcount < 0 || rwlock->rw_nwaitwriters > 0)
    {
        rwlock->rw_nwaitreaders++;
        pthread_cleanup_push(my_pthread_rwlock_cancel_rdlock, (void*)rwlock);
        ret = pthread_cond_wait(&(rwlock->rw_condreaders), &(rwlock->rw_mutex));
        pthread_cleanup_pop(0);
        rwlock->rw_nwaitreaders--;
        if (ret != 0)
        {
            break;
        }
    }

    if (ret == 0)
    {
        rwlock->rw_refcount++;
    }

    pthread_mutex_unlock(&(rwlock->rw_mutex));

    return MY_OK;
}

int my_pthread_rwlock_tryrdlock(my_pthread_rwlock_t *rwlock)
{
    if (rwlock == NULL)
    {
        return MY_FAILED;
    }

    if (rwlock->rw_magic != MY_RW_MAGIC)
    {
        return EINVAL;
    }

    int ret = 0;

    ret = pthread_mutex_lock(&(rwlock->rw_mutex));
    if (ret != 0)
    {
        return EINVAL;
    }

    if (rwlock->rw_refcount < 0 || rwlock->rw_nwaitwriters > 0)
    {
        ret = EBUSY;
    }
    else
    {
        rwlock->rw_refcount++;
    }

    pthread_mutex_unlock(&(rwlock->rw_mutex));

    return ret;
}

int my_pthread_rwlock_wrlock(my_pthread_rwlock_t *rwlock)
{
    if (rwlock == NULL)
    {
        return MY_FAILED;
    }

    if (rwlock->rw_magic != MY_RW_MAGIC)
    {
        return EINVAL;
    }

    int ret = 0;

    ret = pthread_mutex_lock(&(rwlock->rw_mutex));
    if (ret != 0)
    {
        return EINVAL;
    }

    while (rwlock->rw_refcount != 0)
    {
        rwlock->rw_nwaitwriters++;
        pthread_cleanup_push(my_pthread_rwlock_cancel_wrlock, (void*)rwlock);
        ret = pthread_cond_wait(&(rwlock->rw_condwriters), &(rwlock->rw_mutex));
        pthread_cleanup_pop(0);
        rwlock->rw_nwaitwriters--;
        if (ret != 0)
        {
            break;
        }
    }

    if (ret == 0)
    {
        rwlock->rw_refcount = -1;
    }

    pthread_mutex_unlock(&(rwlock->rw_mutex));

    return MY_OK;
}

int my_pthread_rwlock_trywrlock(my_pthread_rwlock_t *rwlock)
{
    if (rwlock == NULL)
    {
        return MY_FAILED;
    }

    if (rwlock->rw_magic != MY_RW_MAGIC)
    {
        return EINVAL;
    }

    int ret = 0;

    ret = pthread_mutex_lock(&(rwlock->rw_mutex));
    if (ret != 0)
    {
        return EINVAL;
    }

    if (rwlock->rw_refcount != 0)
    {
        ret = EBUSY;
    }
    else
    {
        rwlock->rw_refcount = -1;
    }

    pthread_mutex_unlock(&(rwlock->rw_mutex));

    return ret;
}

int my_pthread_rwlock_unlock(my_pthread_rwlock_t *rwlock)
{
    if (rwlock == NULL)
    {
        return MY_FAILED;
    }

    if (rwlock->rw_magic != MY_RW_MAGIC)
    {
        return EINVAL;
    }

    int ret = 0;

    ret = pthread_mutex_lock(&(rwlock->rw_mutex));
    if (ret != 0)
    {
        return EINVAL;
    }

    if (rwlock->rw_refcount > 0)
    {
        rwlock->rw_refcount--;
    }
    else if (rwlock->rw_refcount == -1)
    {
        rwlock->rw_refcount = 0;
    }
    else
    {
        // It's not right to unlock an unused lock
        return MY_FAILED;
    }

    if (rwlock->rw_nwaitwriters > 0)
    {
        if (rwlock->rw_refcount == 0)
        {
            pthread_cond_signal(&(rwlock->rw_condwriters));
        }
    }
    else
    {
        pthread_cond_broadcast(&(rwlock->rw_condreaders));
    }

    pthread_mutex_unlock(&(rwlock->rw_mutex));

    return MY_OK;
}

static void my_pthread_rwlock_cancel_rdlock(void *p)
{
    my_pthread_rwlock_t *rwlock = NULL;

    rwlock = (my_pthread_rwlock_t*)p;

    rwlock->rw_nwaitreaders--;
    pthread_mutex_unlock(&(rwlock->rw_mutex));
}

static void my_pthread_rwlock_cancel_wrlock(void *p)
{
    my_pthread_rwlock_t *rwlock = NULL;

    rwlock = (my_pthread_rwlock_t*)p;

    rwlock->rw_nwaitwriters--;
    pthread_mutex_unlock(&(rwlock->rw_mutex));
}
