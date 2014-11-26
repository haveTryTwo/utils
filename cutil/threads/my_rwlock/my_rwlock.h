#ifndef __MY_RWLOCK_H__
#define __MY_RWLOCK_H__

typedef struct my_pthread_rwlock_t
{
    pthread_mutex_t rw_mutex;       // lock for read and write
    pthread_cond_t rw_condreaders;  // for read threads waiting
    pthread_cond_t rw_condwriters;  // for write threads waiting
    int rw_magic;       // for error checking
    int rw_nwaitreaders;    // number of waiting readers
    int rw_nwaitwriters;    // number of waitting writers
    int rw_refcount;        // -1: writer holds lock; 
                            // 0: no threads hold lock
                            // > 0 : readers hold the lock
} my_pthread_rwlock_t;

typedef int my_pthread_rwlockattr_t;

#define MY_RW_MAGIC     0x19870523

#define MY_OK       0
#define MY_FAILED   -1


int my_pthread_rwlock_init(my_pthread_rwlock_t *rwlock, my_pthread_rwlockattr_t *attr);
int my_pthread_rwlock_destroy(my_pthread_rwlock_t *rwlock);
int my_pthread_rwlock_rdlock(my_pthread_rwlock_t *rwlock);
int my_pthread_rwlock_wrlock(my_pthread_rwlock_t *rwlock);
int my_pthread_rwlock_unlock(my_pthread_rwlock_t *rwlock);

#endif
