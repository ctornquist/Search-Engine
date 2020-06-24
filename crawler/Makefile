#VARIABLES
CC = gcc
CFLAGS = -Wall -g -pedantic -std=c11 -ggdb -I ../libcs50 -I ../common
PROG = crawler
PROG2 = testing.sh
OBJS = crawler.o ../libcs50/webpage.o ../common/pagedir.o
LLIBS = ../libcs50/libcs50.a ../common/common.a
LIBS = -lm

#RULES
$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@

crawler.o: ../common/pagedir.h ../libcs50/webpage.h ../libcs50/memory.h ../libcs50/bag.h ../libcs50/set.h ../libcs50/hashtable.h
webpage.o: ../libcs50/webpage.h ../libcs50/file.h

#PHONY
.PHONY: test clean

test: 
	bash testing.sh

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG)
	rm -f core
	rm -f test/*
	rm -f test*/*
	rm -f wikiD2/*
