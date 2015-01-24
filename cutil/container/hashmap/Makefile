#hashmap

#PARAM
CC			= gcc
CFLAGS 		= -g -c -fPIC -Wall -D__HASHMAP_MAIN_TEST__
OBJS		= hashmap.o crc32.o

#.PHONY
.PHONY : all build clean

all : build clean

build : $(OBJS)
	$(CC) -o $@ $^

clean : 
	rm -fr $(OBJS)

#OBJS
$(OBJS) : %.o : %.c
	$(CC) $(CFLAGS) -o $@ $<
