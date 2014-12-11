#include <stdio.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/msg.h>
#include <string.h>
#include <errno.h>

#define KEY         0xaabb1122
#define MODE        0644

#define MSG_OK      0
#define MSG_FAILED  -1

#define MAX_LEN     2048

#define MSG_TYPE    523

// define msgbuf
typedef struct mymsgbuf
{
    long mtype;
    char mtext[MAX_LEN];
} mymsgbuf;


int test_create();
int test_send();
int test_recv();
int test_delete();


int main(int argc, char *argv[])
{
    if (argc < 2)
    {
        goto help;
    }

    if (0 == strcmp(argv[1], "create"))
    {
        test_create();
    }
    else if (0 == strcmp(argv[1], "send"))
    {
        test_send();
    }
    else if (0 == strcmp(argv[1], "recv"))
    {
        test_recv();
    }
    else if (0 == strcmp(argv[1], "delete"))
    {
        test_delete();
    }

    return MSG_OK;

help:
    printf("Usage: msg COMMAND\n\n"
           "The most used commands are:\n" 
           "create      Create a message queue\n"
           "send        Send a message to the message queue\n"
           "recv        Receive a message from the messag queue\n"
           "delete      Delete the message queue\n"
            );

    return MSG_OK;
}

int test_create()
{
    int msg_id = 0;

    msg_id = msgget(KEY, IPC_CREAT | IPC_EXCL | MODE);
    if (-1 == msg_id)
    {
        if (EEXIST == errno)
        {
            printf("key:%#x has been existed!\n", KEY);
        }
        else
        {
            printf("[%s : %s : %d] msgget failed! key:%#x! errno:%d, err:%s\n",
                    __FILE__, __func__, __LINE__, KEY, errno, strerror(errno));
        }
        return MSG_FAILED;
    }

    return MSG_OK;
}

int test_send()
{
    int msg_id = 0;
    int ret = MSG_OK;
    size_t len = 0;
    const char *str = "abc";

    struct mymsgbuf mbuf = {0};

    msg_id = msgget(KEY, MODE);
    if (-1 == msg_id)
    {
        printf("[%s : %s : %d] msgget failed! key:%#x! errno:%d, err:%s\n",
                __FILE__, __func__, __LINE__, KEY, errno, strerror(errno));

        return MSG_FAILED;
    }

    memset(&mbuf, 0, sizeof(struct mymsgbuf));
    mbuf.mtype = MSG_TYPE;

    len = snprintf(mbuf.mtext, sizeof(mbuf.mtext), "%s", str);
    len++; // adding the number for '\0'

    ret = msgsnd(msg_id, &mbuf, len, IPC_NOWAIT);
    if (-1 == ret)
    {
        printf("[%s : %s : %d] msgsnd failed! key:%#x! errno:%d, err:%s\n",
                __FILE__, __func__, __LINE__, KEY, errno, strerror(errno));

        return ret;
    }

    return MSG_OK;
}

int test_recv()
{
    int msg_id = 0;
    int ret = MSG_OK;

    struct mymsgbuf mbuf = {0};

    msg_id = msgget(KEY, MODE);
    if (-1 == msg_id)
    {
        printf("[%s : %s : %d] msgget failed! key:%#x! errno:%d, err:%s\n",
                __FILE__, __func__, __LINE__, KEY, errno, strerror(errno));

        return MSG_FAILED;
    }

    memset(&mbuf, 0, sizeof(struct mymsgbuf));
    ret = msgrcv(msg_id, &mbuf, MAX_LEN, MSG_TYPE, IPC_NOWAIT);
    if (-1 == ret)
    {
        printf("[%s : %s : %d] msgget failed! key:%#x! errno:%d, err:%s\n",
                __FILE__, __func__, __LINE__, KEY, errno, strerror(errno));

        return MSG_FAILED;
    }

    printf("%s\n", mbuf.mtext);

    return MSG_OK;
}

int test_delete()
{
    int ret = MSG_OK;
    int msg_id = 0;

    msg_id = msgget(KEY, MODE);
    if (-1 == msg_id)
    {
        printf("[%s : %s : %d] msgget failed! key:%#x! errno:%d, err:%s\n",
                __FILE__, __func__, __LINE__, KEY, errno, strerror(errno));

        return MSG_FAILED;
    }

    ret = msgctl(msg_id, IPC_RMID, NULL);
    if (-1 == ret)
    {
        printf("[%s : %s : %d] msgctl failed! key:%#x! errno:%d, err:%s\n",
                __FILE__, __func__, __LINE__, KEY, errno, strerror(errno));

        return MSG_FAILED;
    }

    return MSG_OK;
}
