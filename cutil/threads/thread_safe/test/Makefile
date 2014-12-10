#

#PARAM
CC 		= gcc
CFLAGS 	= -g -c -Wall -pthread -I../
GPERF 	= -I/usr/local/gperftools/include -L/usr/local/gperftools/lib -fno-omit-frame-pointer \
		  -Bdynamic -ltcmalloc  -lprofiler
LIB_OBJS = ../thread_safe.a
OBJS 	= my_test.o

#.PHONY
.PHONY : all build clean

all : build clean

build : $(OBJS) $(LIB_OBJS)
	$(CC) -o $@ $^ -lpthread

clean : 
	rm -fr $(OBJS)

#OBJS
$(OBJS) : %.o : %.c
	$(CC) $(CFLAGS) -o $@ $<

$(LIB_OBJS) : 
	(cd ../ ; make)
