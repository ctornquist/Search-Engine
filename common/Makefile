#VARIABLES
CC = gcc
CFLAGS = -Wall -g -pedantic -std=c11 -ggdb -I ../libcs50/
LIB = common.a
LIBS = -lm
OBJS = pagedir.o word.o index.o

#RULES
$(LIB): $(OBJS)
	ar cr $(LIB) $(OBJS)

pagedir.o: ../libcs50/webpage.h ../libcs50/memory.h pagedir.h
word.o: word.h
index.o: index.h

#PHONY
.PHONY:  clean

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
	rm -f core

