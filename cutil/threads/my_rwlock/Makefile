#

#PARAM
CC 		= g++
CFLAGS 	= -g -c -Wall -pthread -fPIC
GPERF 	= -I/usr/local/gperftools/include -L/usr/local/gperftools/lib -fno-omit-frame-pointer \
		  -Bdynamic -ltcmalloc  -lprofiler
OBJS 	= my_rwlock.o
CCOBJS 	= my_rwlock_test.o

#.PHONY
.PHONY : all build clean

all : build clean

build : $(OBJS) $(CCOBJS)
	$(CC) -o $@ $^ -lpthread

clean : 
	rm -fr $(OBJS) $(CCOBJS)

#OBJS
$(OBJS) : %.o : %.c
	$(CC) $(CFLAGS) -o $@ $<

$(CCOBJS) : %.o : %.cc
	$(CC) $(CFLAGS) -o $@ $<
