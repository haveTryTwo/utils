#

#PARAM
CC 		= gcc
CFLAGS 	= -g -c -Wall -fPIC -D__SYSTEM_MAIN_DEBUG__
OBJS 	= system.o

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
