#log

#PARAM
CC 		= gcc
CFLAGS 	= -g -c -Wall -pthread -D__LOG_MAIN_TEST
OBJS 	= log.o

#PHONY
.PHONY : all log clean

all : log clean

log : $(OBJS)
	$(CC) -o $@ $^ -lpthread

clean : 
	rm -fr $(OBJS)

#OBJS
$(OBJS) : %.o : %.c
	$(CC) $(CFLAGS) -o $@ $<
