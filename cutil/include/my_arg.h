#ifndef __MY_ARG_H__
#define __MY_ARG_H__

#define _MY_INTSIZEOF(n) ((sizeof(n) + sizeof(int) - 1) & ~(sizeof(int) - 1))

typedef char* my_va_list;

#define my_va_start(ap, v) (ap = (my_va_list)&v + _MY_INTSIZEOF(v))
#define my_va_arg(ap, t) (*(t*)((ap += _MY_INTSIZEOF(t)) - _MY_INTSIZEOF(t)))
#define my_va_end(ap) (ap = (my_va_list)0)

#endif // __MY_ARG_H__
