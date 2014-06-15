#ifndef __TIME_UTIL_H__
#define __TIME_UTIL_H__

#include <sys/time.h>


int print_time(struct timeval *tv);
int print_time_difference(struct timeval *tv_begin, struct timeval *tv_end);

#endif
