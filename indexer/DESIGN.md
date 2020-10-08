## CS50 indexer.c Design

## Antony Guzman, Feb 2020

### indexer
The job of the `indexer` is to read the documents in the `pageDirectory` output by the `crawler`, build an inverted index mapping from words to documents, and write that index to a file. It’s called an inverted index because it maps from words to documents, the opposite of a document (itself a sequence of words).

The inverted index is implemented using the `hasthtable`, `set`, and `counters` data types. The index maps from words to (docID,count) pairs representing the number of occurrences of that word in that document. we use a `hashtable` wherein each item is a `counters` where we use the docID as a key.


### Basic design of indexer is seen below:

Process and validate command-line parameters
Initialize data structure index
index_build(directory, index)
index_save(file, index);
clean up data structures