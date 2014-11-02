#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <string.h>
#include <errno.h>

#define KEY    0x11112222
#define SIZE    256
#define MODE    0644
#define OPEN_MODE   0444


int test_creat();
int test_write();
int test_read();
int test_delete();

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        printf("Please input shm type:\n");
        return -1;
    }

    if (strcmp(argv[1], "create") == 0)
    {
        return test_creat();
    }
    else if (strcmp(argv[1], "write") == 0)
    {
        return test_write();
    }
    else if (strcmp(argv[1], "read") == 0)
    {
        return test_read();
    }
    else if (strcmp(argv[1], "delete") == 0)
    {
        return test_delete();
    }

    return 0;
}

int test_creat()
{
    int shmid = 0;

    shmid = shmget(KEY, SIZE, IPC_CREAT | IPC_EXCL | MODE);
    if (shmid == -1)
    {
        if (errno == EEXIST)
        {
            printf("key:%#x already exist\n", KEY);
        }
        else
        {
            printf("create key:%#x failed! errno:%d, err:%s\n", KEY, errno, strerror(errno));
        }
        return -1;
    }

    return 0;
}

int test_write()
{
    int i = 0;
    int shmid = 0;
    void *ptr = NULL;
    char *p = NULL;
    int ret = 0;
    struct shmid_ds ds;

    shmid = shmget(KEY, 0, MODE);
    if (shmid == -1)
    {
        printf("shmget key:%#x failed! errno:%d, err:%s\n", KEY, errno, strerror(errno));
        return -1;
    }

    ptr = shmat(shmid, NULL, 0);
    if (ptr == (void*) -1)
    {
        printf("shmat shmid:%d failed! errno:%d, err:%s\n", shmid, errno, strerror(errno));
        return -1;
    }

    memset(&ds, 0, sizeof(struct shmid_ds));
    ret = shmctl(shmid, IPC_STAT, &ds);
    if (ret == -1)
    {
        printf("shmctl shmid:%d failed! errno:%d, err:%s\n", shmid, errno, strerror(errno));
        return -1;
    }
    printf("size of shmid:%d is %zd\n", shmid, ds.shm_segsz);

    p = (char*)ptr;
    for (i = 0; i < ds.shm_segsz; i++)
    {
        *p = 'a' + i % 26;
        p++;
    }

    ret = shmdt(ptr);
    if (ret == -1)
    {
        printf("shmdt shmid:%d failed! errno:%d, err:%s\n", shmid, errno, strerror(errno));
        return -1;
    }

    return 0;
}

int test_read()
{
    int i = 0;
    int shmid = 0;
    void *ptr = NULL;
    char *p = NULL;
    int ret = 0;
    struct shmid_ds ds;

    shmid = shmget(KEY, 0, OPEN_MODE);
    //shmid = shmget(KEY, 0, 0666);
    if (shmid == -1)
    {
        printf("line:%d, shmget key:%#x failed! errno:%d, err:%s\n", __LINE__,  KEY, errno, strerror(errno));
        return -1;
    }

    ptr = shmat(shmid, NULL, SHM_RDONLY);
    //ptr = shmat(shmid, NULL, 0);
    if (ptr == (void*) -1)
    {
        printf("line:%d, shmat shmid:%d failed! errno:%d, err:%s\n", __LINE__, shmid, errno, strerror(errno));
        return -1;
    }

    memset(&ds, 0, sizeof(struct shmid_ds));
    ret = shmctl(shmid, IPC_STAT, &ds);
    if (ret == -1)
    {
        printf("line:%d, shmctl shmid:%d failed! errno:%d, err:%s\n", __LINE__, shmid, errno, strerror(errno));
        return -1;
    }
    printf("size of shmid:%d is %zd, ptr:%p\n", shmid, ds.shm_segsz, ptr);

    p = (char*)ptr;
    for (i = 0; i < ds.shm_segsz; i++)
    {
        if (*p == '\0')
            break;
        fputc(*p, stdout);
        p++;
    }
    fputc('\n', stdout);

    ret = shmdt(ptr);
    if (ret == -1)
    {
        printf("shmdt shmid:%d failed! errno:%d, err:%s\n", shmid, errno, strerror(errno));
        return -1;
    }

    return 0;
}

int test_delete()
{
    int shmid = 0;
    int ret = 0;

    shmid = shmget(KEY, 0, MODE);
    if (shmid == -1)
    {
        printf("shmget key:%#x failed! errno:%d, err:%s\n", KEY, errno, strerror(errno));
        return -1;
    }

    ret = shmctl(shmid, IPC_RMID, NULL);
    if (ret == -1)
    {
        printf("shmctl RMID shmid:%d failed! errno:%d, err:%s\n", shmid, errno, strerror(errno));
        return -1;
    }

    return 0;
}
