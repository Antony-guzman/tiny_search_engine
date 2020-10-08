## CS50 Lab 6

## Antony Guzman, Feb 2020

### Querier
The TSE Querier is a standalone program that reads the index file produced by the TSE Indexer, and page files produced by the TSE Crawler, and answers search queries submitted via stdin.

### USAGE
 `./querier` `pageDirectory`  `indexFilename`
`page Directory` is the pathname of a directory produced by the Crawler and `indexFilename` is the pathname of a file produced by the indexer. 


### Assumptions
The querier may assume that the input directory and files follow the designated formats.

pageDirectory has files named 1, 2, 3, …, without gaps.
The content of files in pageDirectory follow the format as defined in the specs.
The content of the file named by indexFilename follows the Index file format provided and followed by the indexer.

### Limitations
The user can't search for the word 'and' or 'or' because they are recognzed as special words. The user can't search for words smaller than 3 because no words smaller than 3 letters are accepted. 


### Compilation 
To compile, simple `make`. See TESTING.md for details of testing.

See .TESTING.md for details 