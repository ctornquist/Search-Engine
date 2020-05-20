#VARIABLES
CC = gcc
CFLAGS = -Wall -g -pedantic -std=c11 -ggdb -I ../libcs50 -I ../common
PROG = querier
OBJS = querier.o ../libcs50/webpage.o ../common/pagedir.o ../common/word.o ../common/index.o
LLIBS = ../libcs50/libcs50-given.a ../common/common.a
LIBS = -lm

#RULES
$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@

webpage.o: ../libcs50/webpage.h ../libcs50/file.h
word.o: ../commmon/word.h
index.o: ../common/index.h

#PHONY
.PHONY: test clean

test: 
	./$(PROG)

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
	rm -f core

