#memory

#PARAM
CC 		= gcc
CFLAGS 	= -g -c -Wall --coverage
OBJS 	= analyze.o main_test.o 

#.PHONY
.PHONY : all build clean

all : build clean

build : $(OBJS)
	$(CC) -o $@ $^ --coverage

clean : 
	rm -fr $(OBJS)

clean-all : 
	rm -fr $(OBJS) *.gcno *.gcda *.gcov

#OBJS
$(OBJS) : %.o : %.c
	$(CC) $(CFLAGS) -o $@ $<
