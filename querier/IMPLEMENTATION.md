## CS50 Lab 5

## Antony Guzman, Feb 2020

### Implementing querier 

### Pseudocode

Process and validate 
Initialize data structure index
load the index from `indexFilename`
read search queries from stdin, one per line, until EOF.
clean and parse each query according to the syntax described below.
if the query syntax is somehow invalid, print an error message, do not perform the query, and prompt for the next query.
print the ‘clean’ query for user to see.
use the index to identify the set of documents that satisfy the query, as described below.
if the query is empty (no words), print nothing.
if no documents satisfy the query, print `No documents match`.
otherwise, rank the resulting set of documents according to its score, as described below, and print the set of documents in decreasing rank order; for each, list the score, document ID and URL. (Obtain the URL by reading the first line of the relevant document file from the `pageDirectory`.)
Exit with zero status when EOF is reached on stdin.