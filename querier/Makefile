# Antony Guzman
#
# Makefile for the 'querier' program
#   Builds, compiles and tests the program


L = ../libcs50
C = ../common

CC = gcc
CFLAGS = -Wall -pedantic -std=c11 -ggdb -I$C -I$L
PROG = querier
OBJS = querier.o
LLIBS = $C/common.a $L/libcs50.a
MAKE = make


.PHONY: all clean

all: $(PROG)

#executable depends on object files
$(PROG): $(OBJS) $(LLIBS)
	$(CC) $(CFLAGS) $(OBJS) $(LLIBS) -o $(PROG)

querier.o: $L/hashtable.h $L/counters.h  $L/file.h $L/webpage.h 
querier.o: $C/index.h $C/pagedir.h $C/word.h

test: $(PROG)
	bash -v testing.sh

clean:
	rm -f *~ *.o
	rm -rf *.dSYM
	rm -f $(PROG)
	rm -f queriertest
	rm -f core
	rm -rf data?
	rm -rf data
	rm -rf test_file
