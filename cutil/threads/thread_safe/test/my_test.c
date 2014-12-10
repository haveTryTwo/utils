#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>

#include "thread_safe.h"

int test_main();
void* test_my_strerror_r_1(void *p);
void* test_my_strerror_r_2(void *p);

int main(int argc, char *argv[])
{
    test_main();

    return 0;
}

int test_main()
{
    pthread_t th[2];
    
    pthread_create(&th[0], NULL, test_my_strerror_r_1, NULL);
    pthread_create(&th[1], NULL, test_my_strerror_r_2, NULL);

    pthread_join(th[0], NULL);
    pthread_join(th[1], NULL);

    return 0;
}

void* test_my_strerror_r_1(void *p)
{
    int my_errno = EPERM;
    char * buf = NULL;

    buf = my_strerror_r(my_errno);
    if (buf == NULL)
    {
        printf("my_strerror_r failed!\n");
        pthread_exit(NULL);
    }
    printf("err:%s\n", buf);

    usleep(100000);

    pthread_exit(NULL);
}

void* test_my_strerror_r_2(void *p)
{
    int my_errno = EINTR;
    char * buf = NULL;

    buf = my_strerror_r(my_errno);
    if (buf == NULL)
    {
        printf("my_strerror_r failed!\n");
        pthread_exit(NULL);
    }
    printf("err:%s\n", buf);

    pthread_exit(NULL);
}
