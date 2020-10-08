#!/bin/sh
# Testing script for querier.c
# Author: Antony Guzman
# Inspired by CS50 given example of testing.sh
# Date: Feb 2020
#
# usage: bash -v testing.sh

# make a copy of specific directories from the testing
# of crawler output provided by the cs50 server account
# into the indexer directory (will later be removed)
cp -r ~cs50/data/tse-output/letters-depth-3 data1
../indexer/indexer data1 data1/oldIndexFile

# directory not created by crawler
mkdir data

# unwriteable file
touch test_file
chmod -r test_file

#####################################
### These tests should fail ###

# 1 argument
 ./querier

# 2 arguments
 ./querier data1

# 3 arguments + nonreadable file 
 ./querier data1 test_file

# 3 arguments + fake directory
./querier data indexFile

######################################
### These tests should pass ####

valgrind --leak-check=full --show-leak-kinds=all  echo "biology or page" | ./querier data1 data1/oldIndexFile

# test cases
 ./querier data1 data1/oldIndexFile < testCases





