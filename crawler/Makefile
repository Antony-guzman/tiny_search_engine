# Makefile for 'crawler' module
#
# Antony Guzman, April 2016, 2017

L = ../libcs50
C = ../common

# object files, and the target library
PROG = crawler
OBJS = crawler.o
LIBS = $(L)/libcs50.a $(C)/common.a 

# uncomment the following to turn on verbose memory logging
# TESTING=-DMEMTEST

CFLAGS = -Wall -pedantic -std=c11 -ggdb $(TESTING) -I$L  -I$C
CC = gcc
MAKE = make

$(PROG): $(OBJS)
	$(CC) $(CFLAGS) $^ $(LIBS) -o $@


.PHONY: test clean

test: $(PROG)
	bash -v testing.sh

# clean up after our compilation
clean:
	rm -f *~ *.o *.dSYM
	rm -f core
	rm -f $(PROG)
	rm -f stock
	rm -f data/?
	rm -rf data?
