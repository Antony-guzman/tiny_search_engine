## CS50 Lab 5


## Antony Guzman, Jan 2020
The TSE indexer is a standalone program that reads the document files produced by the TSE crawler, builds an index, and writes that index to a file. Its companion, the index tester, loads an index file produced by the indexer and saves it to another file.


### USAGE
./indexer [pageDirectory] [indexFilename]
`pageDirectory` is a pathname of a directory produced by the Crawler and `indexFilename` is the pathname of a file into which the index should be written; the indexer creates the file (if needed) and overwrites the file (if it already exists).

./indextest [oldIndexFilename] [newIndexFilename]
`oldIndexFilename` is the name of a file produced by the indexer and `newIndexFilename` is the name of a file into which the index should be written 

### Assumptions 
The index tester may assume that

The content of the index file follows the format specified below; thus your code (to recreate an index structure by reading a file) need not have extensive error checking.
Index file format
The indexer writes the inverted index to a file, and both the index tester and the querier read the inverted index from a file; the file shall be in the following format.

one line per word, one word per line
each line provides the word and one or more (docID, count) pairs, in the format
word docID count [docID count]…
where word is a string of lower-case letters,
where docID is a positive non-zero integer,
where count is a positive non-zero integer,
where the word and integers are separated by spaces.
Within the file, the lines may be in any order.

Within a line, the docIDs may be in any order. 

No other assumptions beyond those stated in the requirements. The current directory must be created beforehand by the crawler. 

### Limitations 
Because some of the data-structure implementations have unspecified ordering - hte order in which data appears when traversing the structure may not the same as the order items were inserted. The content should all be the same but sorting is necessary to compare the files.

### Compilation 
To compile, simple `make`. Additionally each program can be compiled by simply specifying what program to run after make. Results are saved in files created.

See .TESTING.md for details 

### Additional Info
We use the directory testing from the CS50 account server to test the indexer.



