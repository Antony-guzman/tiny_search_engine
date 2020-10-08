# Makefile for the 'indexer' module 
#
# Antony Guzman 

L = ../libcs50
C = ../common

# object files
PROG= indexer
TEST= indextest
OBJS= indexer.o
TESTOBJ= indextest.o
LIBS= $(C)/common.a $(L)/libcs50.a

# uncomment the following to turn on verbose memory logging
# TESTING=-DMEMTEST

CFLAGS= -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$C -I$L
CC= gcc
MAKE= make

all: $(PROG) $(TEST)

$(PROG): $(OBJS) $(LIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@


$(TEST): $(TESTOBJ) $(LIBS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@


.PHONY: all clean test

indexer.o: $L/file.h $L/webpage.h $C/index.h $C/pagedir.h

indextest.o:$L/file.h $L/webpage.h $C/index.h $C/pagedir.h

test: $(PROG) $(TEST)
	bash -v testing.sh

clean:
	rm -f $(PROG)
	rm -f $(TEST)
	rm -f *~ *.o
	rm -rf *.dSYM
	rm -rf data
	rm -rf data?
	rm -rf test_file
	rm -f core
