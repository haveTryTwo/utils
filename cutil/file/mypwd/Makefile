#

#PARAM
CC 		= gcc
CFLAGS 	= -g -c -Wall
GPERF 	= -I/usr/local/gperftools/include -L/usr/local/gperftools/lib -fno-omit-frame-pointer \
		  -Bdynamic -ltcmalloc  -lprofiler
OBJS 	= mypwd.o
BIN 	= mypwd

#.PHONY
.PHONY : all $(BIN) clean

all : $(BIN) clean

$(BIN) : $(OBJS)
	$(CC) -o $@ $(OBJS)

clean : 
	rm -fr $(OBJS)

#OBJS
$(OBJS) : %.o : %.c
	$(CC) $(CFLAGS) -o $@ $<
