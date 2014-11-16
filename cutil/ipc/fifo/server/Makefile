#

#PARAM
CC 		= gcc
CFLAGS 	= -g -c -Wall -I../include
GPERF 	= -I/usr/local/gperftools/include -L/usr/local/gperftools/lib -fno-omit-frame-pointer \
		  -Bdynamic -ltcmalloc  -lprofiler
OBJS 	= server.o

#.PHONY
.PHONY : all server clean

all : server clean

server : $(OBJS)
	$(CC) -o $@ $(OBJS)

clean : 
	rm -fr $(OBJS)

#OBJS
$(OBJS) : %.o : %.c
	$(CC) $(CFLAGS) -o $@ $<
