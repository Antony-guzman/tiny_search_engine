## CS50  Lab 5

## Antony Guzman, Feb 2020

### Indexer Testing
the indexer creates a file that contains the contexts of an index. This index maps from words to (documentID, count) pairs, wherein each count represents the number of occurrences of the given word in the given document. However, to ensure that the index is correct, we  test it to make sure the smae results give the same test. indextest reads from the output file created and inputs into the index again and create another file with the goal of them being the same.

Another issue that arises from the nature of the modules created/used have have unspecified ordering – the order in which data appears when traversing the structure may not be the same as the order items were inserted – the file saved by the index tester may not be literally identical to the file read by that program.

We use the script `indexsort.awk` that sorts the index file into a ‘canonical’ ordering, making it possible to compare two index files for their content. 

### Additional Info
Results could also be manually evaluated. I used the output from a directory from the CS50 account on the server for testing. 