#include <iostream>
#include <vector>

#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

#include "my_rwlock.h"

using namespace std;

int threads_test();
void* one_producer(void *p);
void* two_consumer(void *p);

std::vector<int> contain;
typedef std::vector<int>::iterator v_iterator;

int main(int argc, char *argv[])
{
    threads_test();

    return 0;
}

my_pthread_rwlock_t rwlock;
pthread_t one_th;
pthread_t two_th[2];


int threads_test()
{
    my_pthread_rwlock_init(&rwlock, NULL);

    pthread_create(&one_th, NULL, one_producer, NULL);
    pthread_create(&two_th[0], NULL, two_consumer, NULL);
    pthread_create(&two_th[1], NULL, two_consumer, NULL);

//    printf("main start to read ...\n");
//    my_pthread_rwlock_rdlock(&rwlock);
//    sleep(5);
//    my_pthread_rwlock_unlock(&rwlock);
//    printf("main end to read ...\n");

    pthread_join(one_th, NULL);
    pthread_join(two_th[0], NULL);
    pthread_join(two_th[1], NULL);


    my_pthread_rwlock_destroy(&rwlock);

    return 0;
}

void* one_producer(void *p)
{
    int i = 0;
    while (1)
    {
        sleep(3);
        printf("\n\033[31;1mstart to try producer ...\033[0m\n");
        my_pthread_rwlock_wrlock(&rwlock);
        printf("\n\033[31;1mproducer success ...\033[0m\n");
        contain.push_back(i);
        printf("produer v:%d\n", i);
//        pthread_cancel(two_th[0]);
//        pthread_cancel(two_th[1]);
//        printf("\n\033[31;1mcancel threads succcess ...\033[0m\n");
//        sleep(1);

        my_pthread_rwlock_unlock(&rwlock);
        printf("\033[31;1mend to producer ... \033[0m\n\n");

        i++;
     //   break;
    }

    pthread_exit(NULL);
}

void* two_consumer(void *p)
{
    size_t size = 0;
    v_iterator it;
    pthread_t pth = 0;

    while (1)
    {
        sleep(1);
        printf("\nstart to try to consumer ... \n");
        my_pthread_rwlock_rdlock(&rwlock);
        printf("\nconsumer success... \n");

        pth = pthread_self();
        size = contain.size();
        printf("pth:%#lx; size:%zd\n", pth, size);
        for (it = contain.begin(); it != contain.end(); it++)
        {
            printf("pth:%#lx; v:%d\n", pth, *it);
        }

        my_pthread_rwlock_unlock(&rwlock);
        printf("end to consumer ... \n\n");

//        break;
    }

    pthread_exit(NULL);
}
