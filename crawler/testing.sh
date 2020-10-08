#!/bin/sh
# Testing script for crawler.c
# Author: Antony Guzman
# Inspired by CS50 given example of testing.sh
# Date: Feb 5, 2020
#
# usage: bash -v testing.sh

# Define variables
seedURL=http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html
externalURL=www.google.com
fakepage=http://old-www.cs.dartmouth.edu/thisdoesntwork.html
seedURL2=http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/B.html 
wikipage=http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/index.html

# make different directories for different seeds
mkdir data1
mkdir data2
mkdir data3

#####################################
### These tests should fail ###

# 1 argument
./crawler

# 2 arguments
./crawler $seedURL

# 3 arguments
./crawler $seedURL data1

# 4 arguments + externalURL
./crawler $externalURL data1 2

# 4 arguments + fakepage
./crawler $fakepage data1 2

# 4 arguments and nonexisting directory
./crawler $seedURL not_real 2

######################################
### These tests should pass ####

# at depth 0
./crawler $seedURL data1 0

# at depth 1
./crawler $seedURL data1 1

# at depth 2
./crawler $seedURL data1 2

# at depth 3
./crawler $seedURL data1 3

# at depth 4
./crawler $seedURL data1 4

# at depth 5
./crawler $seedURL data1 5

# at depth 0 wikipedia page
./crawler $wikipage data2 0

# at depth 1 wikipedia page
./crawler $wikipage data2 1

# at depth 2 wikipedia page
# WARNING: TAKES EXTREMELY LONG
# ./crawler $wikipage data2 2

# at depth 0 with seedURL2
./crawler $seedURL2 data3 0

# at depth 1 with seed URL2
./crawler $seedURL2 data3 1











