#

#PARAM
CC 		= gcc
CFLAGS 	= -g -c -Wall
GPERF 	= -I/usr/local/gperftools/include -L/usr/local/gperftools/lib -fno-omit-frame-pointer \
		  -Bdynamic -ltcmalloc  -lprofiler
OBJS 	= check_stat.o

#.PHONY
.PHONY : all build clean

all : build clean

build : $(OBJS)
	$(CC) -o $@ $(OBJS)

clean : 
	rm -fr $(OBJS)

#OBJS
$(OBJS) : %.o : %.c
	$(CC) $(CFLAGS) -o $@ $<
