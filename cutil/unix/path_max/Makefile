#memory

#PARAM
CC 		= gcc
CFLAGS 	= -g -c -Wall 
#CFLAGS 	= -g -c -Wall --coverage
OBJS 	= path_max.o 

#.PHONY
.PHONY : all path_max clean

all : path_max clean

path_max : $(OBJS)
	$(CC) -o $@ $^ 
#	$(CC) -o $@ $^ --coverage

clean : 
	rm -fr $(OBJS)

clean-all : 
	rm -fr $(OBJS) *.gcno *.gcda *.gcov

#OBJS
$(OBJS) : %.o : %.c
	$(CC) $(CFLAGS) -o $@ $<
