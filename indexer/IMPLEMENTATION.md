## CS50 Lab 4

## Antony Guzman, Jan 2020

### Implementing indexer 

### Pseudocode 

`indexer`:

Process and validate command-line parameters
Initialize data structure index
index_build(directory, index)
index_save(file, index);
clean up data structures

`indextest`:

Process and validate command-line parameters
Initialize data structure index
index_load(file1, index)
index_save(file2, index)
clean up data structures

### Data structures 

The indexer uses three data structures: hashtables, set, and counters. The hashtable contains sets that hold the words for the keys and counters to count the amount of times each words appears in each docID.

### Functions 
