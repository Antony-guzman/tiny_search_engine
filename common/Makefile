# Makefile for the "commons" directory that includes pageDir and index(for future use).
#
# Antony Guzman, Feb,2020

# object files, and the target library
L = ../libcs50
OBJS = pagedir.o index.o word.o

CC=gcc
CFLAGS=-Wall -pedantic -std=c11 -ggdb -I$L
LIB = common.a
MAKE = make

.PHONY: clean sourcelist


# Build the library by archiving object files
$(LIB): $(OBJS) 
	ar cr $(LIB) $(OBJS)



# object files depend on include files
pagedir.o: $L/webpage.h pagedir.h $L/file.h
index.o:  $L/webpage.h index.h $L/hashtable.h $L/counters.h
index.o:  $L/file.h $L/memory.h pagedir.h word.h
word.o: word.h

# list all the sources and docs in this directory
sourcelist: Makefile *.md *.c *.h
	@echo $^

# clean up after our compilation
clean:
	rm -f core
	rm -f $(LIB) *~ *.o
