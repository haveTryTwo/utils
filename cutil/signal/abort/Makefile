#

#PARAM
CC 		= gcc
CFLAGS 	= -g -c -Wall -fPIC -D__ABORT_MAIN_DEBUG__
OBJS 	= abort.o

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
