#include <stdio.h>
#include <unistd.h>

#include "thread_pool.h"

static void print(void *p);

int thread_pool_test();


int main(int argc, char *argv[])
{
    thread_pool_test();

    return 0;
}


int thread_pool_test()
{
    thread_pool pool;
    int ret = 0;
    int i = 0;
    int j = 0;
    char str[1024] = "abc, yes";

    ret = thread_pool_init(&pool, 20);
    printf("ret:%d\n", ret);
    printf("running_num:%d; sleeping_num:%d\n", pool.running_num, pool.sleeping_num);

    for (i = 0; i < 21; i++)
    {
        ret = thread_pool_start_running(&pool, print, str);
        if (ret == THREAD_POOL_OK)
        {
            j++;
        }
    }
    printf("running_num:%d; sleeping_num:%d\n", pool.running_num, pool.sleeping_num);
    sleep(1);
    printf("running_num:%d; sleeping_num:%d\n", pool.running_num, pool.sleeping_num);

    for (i = 0; i < 122; i++)
    {
        ret = thread_pool_start_running(&pool, print, str);
        if (ret == THREAD_POOL_OK)
        {
            j++;
        }
    }
    printf("running_num:%d; sleeping_num:%d\n", pool.running_num, pool.sleeping_num);
    sleep(2);
    printf("running_num:%d; sleeping_num:%d\n", pool.running_num, pool.sleeping_num);
    printf("j:%d\n", j);

    ret = thread_pool_destroy(&pool);

    return 0;
}

static void print(void *p)
{
    const char *str = p;
    printf("%s\n", str);
}
