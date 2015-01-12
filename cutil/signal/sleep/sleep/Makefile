#

#PARAM
CC 		= gcc
CFLAGS 	= -g -c -Wall -fPIC -D__SLEEP_MAIN_DEBUG__
OBJS 	= sleep.o

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
