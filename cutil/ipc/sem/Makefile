#

#PARAM
CC 		= gcc
CFLAGS 	= -g -c -Wall
GPERF 	= -I/usr/local/gperftools/include -L/usr/local/gperftools/lib -fno-omit-frame-pointer \
		  -Bdynamic -ltcmalloc  -lprofiler
OBJS 	= sem_test.o

#.PHONY
.PHONY : all sem clean

all : sem clean

sem : $(OBJS)
	$(CC) -o $@ $(OBJS)

clean : 
	rm -fr $(OBJS)

#OBJS
$(OBJS) : %.o : %.c
	$(CC) $(CFLAGS) -o $@ $<
