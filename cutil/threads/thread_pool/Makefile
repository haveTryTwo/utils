#

#PARAM
CC 		= gcc
CFLAGS 	= -g -c -Wall -pthread -fPIC
GPERF 	= -I/usr/local/gperftools/include -L/usr/local/gperftools/lib -fno-omit-frame-pointer \
		  -Bdynamic -ltcmalloc  -lprofiler
AR 		= ar
ARFLAGS = rcs
SOFLAGS = -shared -fPIC
SOOBJS 	= thread_pool.so
AOBJS 	= thread_pool.a
OBJS 	= thread_pool.o
C_SRC 	= thread_pool.c


#.PHONY
.PHONY : all $(SOOBJS) $(AOBJS) clean

all : $(SOOBJS) $(AOBJS) clean

$(SOOBJS) : $(C_SRC)
	$(CC) $(SOFLAGS) -o $@ $^ -lpthread

$(AOBJS) : $(OBJS)
	$(AR) $(ARFLAGS) $@ $^

clean : 
	rm -fr $(OBJS)

#OBJS
$(OBJS) : %.o : %.c
	$(CC) $(CFLAGS) -o $@ $<
