#!/bin/sh
# Testing script for indexer.c
# Author: Antony Guzman
# Inspired by CS50 given example of testing.sh
# Date: Feb 2020
#
# usage: bash -v testing.sh

# make a copy of specific directories from the testing
# of crawler output provided by the cs50 server account
# into the indexer directory (will later be removed)
cp -r ~cs50/data/tse-output/letters-depth-0 data1
cp -r ~cs50/data/tse-output/letters-depth-3 data2
cp -r ~cs50/data/tse-output/letters-depth-6 data3



# directory not created by crawler
mkdir data

# unwriteable file
touch test_file
chmod -w test_file

#####################################
### These tests should fail ###

# 1 argument
./indexer

# 2 arguments
./indexer data1

# 3 arguments + nonreadable file 
./indexer data1 test_file

# 3 arguments + fake directory
./indexer data indexFile

######################################
### These tests should pass ####

# 1 file in the crawler directory
./indexer data1 data1/oldIndexFile

# valgrind test
valgrind --leak-check=full --show-leak-kinds=all ./indexer data1 data1/oldIndexFile

# then check it through the test
./indextest data1/oldIndexFile data1/newIndexFile

# sort files and check for differences
gawk -f indexsort.awk data1/oldIndexFile > data1/oldIndexFile.sorted
gawk -f indexsort.awk data1/newIndexFile > data1/newIndexFile.sorted
diff data1/oldIndexFile.sorted data1/newIndexFile.sorted &>/dev/null

# 3 files in the directory
./indexer data2 data2/oldIndexFile

# valgrind test
valgrind --leak-check=full --show-leak-kinds=all ./indexer data2 data2/oldIndexFile

# then check it through the test
./indextest data2/oldIndexFile data2/newIndexFile

# no diff means files are identical
gawk -f indexsort.awk data2/oldIndexFile > data2/oldIndexFile.sorted
gawk -f indexsort.awk data2/newIndexFile > data2/newIndexFile.sorted
diff data2/oldIndexFile.sorted data2/newIndexFile.sorted &>/dev/null


# 9 files in the directory 
./indexer data3 data3/oldIndexFile

# valgrind test
valgrind --leak-check=full --show-leak-kinds=all ./indexer data3 data3/oldIndexFile

# then check it through the test
./indextest data3/oldIndexFile data3/newIndexFile

# no diff means files are identical
gawk -f indexsort.awk data3/oldIndexFile > data3/oldIndexFile.sorted
gawk -f indexsort.awk data3/newIndexFile > data3/newIndexFile.sorted
diff data3/oldIndexFile.sorted data3/newIndexFile.sorted &>/dev/null

if [ $? != 0 ]
then
    echo "Index file copying failed"
fi




