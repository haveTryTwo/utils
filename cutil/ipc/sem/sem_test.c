#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <string.h>
#include <errno.h>

#define KEY         0xbbff1122
#define MODE        0644

#define SEM_OK      0
#define SEM_FAILED  -1

#define MAX_LEN     2048

#define true        1
#define false       0

union semun 
{
    int              val;    /* Value for SETVAL */
    struct semid_ds *buf;    /* Buffer for IPC_STAT, IPC_SET */
    unsigned short  *array;  /* Array for GETALL, SETALL */
    struct seminfo  *__buf;  /* Buffer for IPC_INFO
                                (Linux-specific) */
};

int sem_create();
int sem_lock();
int sem_unlock();
int sem_delete();

int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        goto help;
    }

    if (0 == strcmp(argv[1], "create"))
    {
        sem_create();
    }
    else if (0 == strcmp(argv[1], "lock"))
    {
        sem_lock();
    }
    else if (0 == strcmp(argv[1], "unlock"))
    {
        sem_unlock();
    }
    else if (0 == strcmp(argv[1], "delete"))
    {
        sem_delete();
    }

    return 0;

help:
    printf("Usage: sem COMMAND\n\n"
           "The most used commands are:\n" 
           "create      Create a sem lock\n"
           "lock        Lock the sem to dosomething\n"
           "unlock      Unlock the sem\n"
           "delete      Delete the sem\n"
            );

    return SEM_FAILED;
}

int sem_create()
{
    int sem_id = 0;
    int ret = 0;
    union semun un;
    struct sembuf buf;

    sem_id = semget(KEY, 1, IPC_CREAT | IPC_EXCL | MODE);
    if (-1 == sem_id)
    {
        if (EEXIST == errno)
        {
            printf("key:%#x has been existed!\n", KEY);
        }
        else
        {
            printf("[%s : %s : %d] semget failed! key:%#x! errno:%d, err:%s\n",
                    __FILE__, __func__, __LINE__, KEY, errno, strerror(errno));
        }
        return SEM_FAILED;
    }
    else
    {
        memset(&un, 0, sizeof(union semun));
        un.val = 1;
        
        ret = semctl(sem_id, 0, SETVAL, un);
        if (-1 == ret)
        {
            printf("[%s : %s : %d] semctl failed! key:%#x! errno:%d, err:%s\n",
                    __FILE__, __func__, __LINE__, KEY, errno, strerror(errno));
            return SEM_FAILED;
        }

        // just for init the sem_otime to make sure the init success
        buf.sem_num = 0;
        buf.sem_op = 1;
        buf.sem_flg = SEM_UNDO | IPC_NOWAIT;
        ret = semop(sem_id, &buf, 1);
        if (-1 == ret)
        {
            printf("[%s : %s : %d] semctl failed! key:%#x! errno:%d, err:%s\n",
                    __FILE__, __func__, __LINE__, KEY, errno, strerror(errno));
            return SEM_FAILED;
        }
    }

    return SEM_OK;
}

int sem_lock()
{
    int ret = 0;
    int sem_id = 0;
    union semun un;
    struct semid_ds seminfo;
    struct sembuf buf;

    sem_id = semget(KEY, 1, MODE);
    if (-1 == sem_id)
    {
        printf("[%s : %s : %d] semget failed! key:%#x! errno:%d, err:%s\n",
                __FILE__, __func__, __LINE__, KEY, errno, strerror(errno));
        return SEM_FAILED;
    }

    memset(&un, 0, sizeof(union semun));
    memset(&seminfo, 0, sizeof(struct semid_ds));
    un.buf = &seminfo;
    ret = semctl(sem_id, 0, IPC_STAT, un);
    if (-1 == ret)
    {
        printf("[%s : %s : %d] semctl failed! key:%#x! errno:%d, err:%s\n",
                __FILE__, __func__, __LINE__, KEY, errno, strerror(errno));
        return SEM_FAILED;
    }
    while (true)
    {
        if (0 == un.buf->sem_otime)
        { // wait for the initializer success
            continue;
        }
        break;
    }

    buf.sem_num = 0;
    buf.sem_op = -1;
    buf.sem_flg = SEM_UNDO;

    printf("start to sem lock ...\n");
    ret = semop(sem_id, &buf, 1);
    if (-1 == ret)
    {
        printf("[%s : %s : %d] semctl failed! key:%#x! errno:%d, err:%s\n",
                __FILE__, __func__, __LINE__, KEY, errno, strerror(errno));
        return SEM_FAILED;
    }
    printf("sem lock SUCCESS ...\n");

    return 0;
}

int sem_unlock()
{
    int ret = 0;
    int sem_id = 0;
    union semun un;
    struct semid_ds seminfo;
    struct sembuf buf;

    sem_id = semget(KEY, 1, MODE);
    if (-1 == sem_id)
    {
        printf("[%s : %s : %d] semget failed! key:%#x! errno:%d, err:%s\n",
                __FILE__, __func__, __LINE__, KEY, errno, strerror(errno));
        return SEM_FAILED;
    }

    memset(&un, 0, sizeof(union semun));
    memset(&seminfo, 0, sizeof(struct semid_ds));
    un.buf = &seminfo;
    ret = semctl(sem_id, 0, IPC_STAT, un);
    if (-1 == ret)
    {
        printf("[%s : %s : %d] semctl failed! key:%#x! errno:%d, err:%s\n",
                __FILE__, __func__, __LINE__, KEY, errno, strerror(errno));
        return SEM_FAILED;
    }
    while (true)
    {
        if (0 == un.buf->sem_otime)
        { // wait for the initializer success
            continue;
        }
        break;
    }

    buf.sem_num = 0;
    buf.sem_op = 1;
    buf.sem_flg = SEM_UNDO;

    printf("start to sem unlock ...\n");
    ret = semop(sem_id, &buf, 1);
    if (-1 == ret)
    {
        printf("[%s : %s : %d] semctl failed! key:%#x! errno:%d, err:%s\n",
                __FILE__, __func__, __LINE__, KEY, errno, strerror(errno));
        return SEM_FAILED;
    }
    printf("sem unlock SUCCESS ...\n");

    return 0;
}

int sem_delete()
{
    int ret = 0;
    int sem_id = 0;
    union semun un;
    struct semid_ds seminfo;

    sem_id = semget(KEY, 1, MODE);
    if (-1 == sem_id)
    {
        printf("[%s : %s : %d] semget failed! key:%#x! errno:%d, err:%s\n",
                __FILE__, __func__, __LINE__, KEY, errno, strerror(errno));
        return SEM_FAILED;
    }

    memset(&un, 0, sizeof(union semun));
    memset(&seminfo, 0, sizeof(struct semid_ds));
    un.buf = &seminfo;
    ret = semctl(sem_id, 0, IPC_STAT, un);
    if (-1 == ret)
    {
        printf("[%s : %s : %d] semctl failed! key:%#x! errno:%d, err:%s\n",
                __FILE__, __func__, __LINE__, KEY, errno, strerror(errno));
        return SEM_FAILED;
    }
    while (true)
    {
        if (0 == un.buf->sem_otime)
        { // wait for the initializer success
            continue;
        }
        break;
    }

    ret = semctl(sem_id, 0, IPC_RMID);
    if (-1 == ret)
    {
        printf("[%s : %s : %d] semctl failed! key:%#x! errno:%d, err:%s\n",
                __FILE__, __func__, __LINE__, KEY, errno, strerror(errno));
        return SEM_FAILED;
    }

    return 0;
}
