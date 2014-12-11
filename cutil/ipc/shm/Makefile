#path

#param
CC 		= gcc
CFLAGS 	= -g -c -Wall
OBJS 	= shm_test.o

#.PHONY
.PHONY : all shm clean

all : shm clean

shm : $(OBJS)
	$(CC) -o $@ $^ -pthread

clean : 
	rm -fr $(OBJS)

#OBJS
$(OBJS) : %.o : %.c
	$(CC) $(CFLAGS) -o $@ $<
