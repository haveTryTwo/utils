#include "time_util.h"
#include <stdio.h>

int print_time(struct timeval *tv)
{
    printf("sec: %lu, microsecond: %lu\n", tv->tv_sec, tv->tv_usec);

    return 0;
}

int print_time_difference(struct timeval *tv_begin, struct timeval *tv_end)
{
    if (tv_begin == NULL || tv_end == NULL)
    {
        return -1;
    }

    printf("begin ");
    print_time(tv_begin);
    printf("end   ");
    print_time(tv_end);

    if (tv_end->tv_sec < tv_begin->tv_sec)
    {
        return -1;
    }
    else if (tv_end->tv_sec == tv_begin->tv_sec)
    {
        printf("time diff: %lu microseconds\n", tv_end->tv_usec - tv_begin->tv_usec);
    }
    else
    {
        if (tv_end->tv_usec < tv_begin->tv_usec)
        {
            printf("time diff: %lu seconds, %lu microseconds\n", tv_end->tv_sec - tv_begin->tv_sec - 1, 
                    tv_end->tv_usec + 1000000 - tv_begin->tv_usec);
        }
        else
        {
            printf("time diff: %lu seconds, %lu microseconds\n", tv_end->tv_sec - tv_begin->tv_sec, 
                    tv_end->tv_usec - tv_begin->tv_usec);
        }
    }

    return 0;
}

