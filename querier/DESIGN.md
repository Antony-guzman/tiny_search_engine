## CS50 querier.c Design

## Antony Guzman, Feb 2020

### querier
Our Querier loads the index into memory (a data structure we developed for the Indexer) and then prompts the user for queries. Queries are comprised of words, with optional `and`/`or` operators


### basic design of querier is seen below

1. check command-line arguments
2. load the index from the IndexFile given
3. clean and parse the query
    1. check for erros of query
4. caculates the scores of the query 
5. rank the results of the document in decreasing order 
    1. if no score, update user accordingly 


