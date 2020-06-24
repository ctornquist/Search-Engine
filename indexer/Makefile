#VARIABLES
CC = gcc
CFLAGS = -Wall -g -pedantic -std=c11 -ggdb -I ../libcs50 -I ../common
PROG1 = indexer
PROG2 = indextest
OBJS1 = indexer.o ../libcs50/webpage.o ../common/pagedir.o ../common/word.o ../common/index.o
OBJS2 = indextest.o ../libcs50/webpage.o ../common/pagedir.o ../common/word.o ../common/index.o
LLIBS = ../libcs50/libcs50.a ../common/common.a
LIBS = -lm

#RULES
all: indexer indextest

$(PROG1): $(OBJS1) $(LLIBS)
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@

$(PROG2): $(OBJS2) $(LLIBS)
	$(CC) $(CFLAGS) $(LIBS) $^ -o $@

crawler.o: ../common/pagedir.h ../libcs50/webpage.h ../libcs50/memory.h ../libcs50/bag.h ../libcs50/set.h ../libcs50/hashtable.h
webpage.o: ../libcs50/webpage.h ../libcs50/file.h
word.o: ../commmon/word.h
index.o: ../common/index.h

#PHONY
.PHONY: test clean

test: 
	bash -v testing.sh

clean:
	rm -f *~ *.o *.dSYM
	rm -f $(PROG1)
	rm -f $(PROG2)
	rm -f core
	rm -f idx1* idx2* idx3* idx4*

