#VARIABLES
CC = gcc
CFLAGS = -Wall -g -pedantic -std=c11 -ggdb -I ../libcs50 -I ../common
CFLAGS1 = -Wall -g -pedantic -std=gnu11 -ggdb -I ../libcs50
PROG = querier
PROG2 = fuzzquery
OBJS = querier.o ../libcs50/webpage.o ../common/pagedir.o ../common/word.o ../common/index.o
OBJS2 = 
LLIBS = ../libcs50/libcs50-given.a ../common/common.a
LIBS = -lm

#RULES
all: querier fuzzquery

$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@

$(PROG2): fuzzquery.c $(OBJS2) $(LLIBS)
	$(CC) $(CFLAGS1) $(LIBS) $^ -o $@

webpage.o: ../libcs50/webpage.h ../libcs50/file.h
word.o: ../commmon/word.h
index.o: ../common/index.h

#PHONY
.PHONY: test clean

test: 
	bash -v testing.sh

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
	rm -f core

