#memory

#PARAM
CC 		= gcc
CFLAGS 	= -g -c -Wall --coverage
OBJS 	= open_max.o 

#.PHONY
.PHONY : all open_max clean

all : open_max clean

open_max : $(OBJS)
	$(CC) -o $@ $^ --coverage

clean : 
	rm -fr $(OBJS)

clean-all : 
	rm -fr $(OBJS) *.gcno *.gcda *.gcov

#OBJS
$(OBJS) : %.o : %.c
	$(CC) $(CFLAGS) -o $@ $<
