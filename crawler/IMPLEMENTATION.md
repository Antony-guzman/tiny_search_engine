## CS50 Lab 4

## Antony Guzman, Jan 2020

### Implementing crawler

### Pseudocode

The crawler will run as follows:

1. execute from a command line as shown in the User Interface
2. parse the command line, validate parameters, initialize other modules
3. make a webpage for the seedURL, marked with depth=0
4. add that page to the bag of webpages to crawl
5. add that URL to the hashtable of URLs seen
6. while there are more webpages to crawl,
   1. extract a webpage (URL,depth) item from the bag of webpages to be crawled,
   2. pause for at least one second,
   3. use pagefetcher to retrieve a webpage for that URL,
   4. use pagesaver to write the webpage to the pageDirectory with a unique document       ID, as described in the Requirements.
   5. if the webpage depth is < maxDepth, explore the webpage to find links:
      1. use pagescanner to parse the webpage to extract all its embedded URLs;
      2. for each extracted URL,
         1. ‘normalize’ the URL (see below)
         2. if that URL is not ‘internal’ (see below), ignore it;
         3. try to insert that URL into the hashtable of URLs seen
            1. if it was already in the table, do nothing;
            2. if it was added to the table,
               1. make a new webpage for that URL, at depth+1
               2. add the new webpage to the bag of webpages to be crawled


### Data structures

The Crawler uses bugs and hashtables (and indirectly sets). Bags were used to store webpages to explore and the hashtables were used to store the URLs of each website. Additionally, the libcs50 contains functions used by crawler to fetch and and parse the websites while the common directory also contains a pagesaver function that saves files to the chosen directories. 

### Functions

`int main(int argc, char *argv[]);`
main parses parameters and passes them to the crawler.

`void crawler(char *seed, char *pageDirectory, int maxDepth);`
crawler uses a bag to track pages to explore, and hashtable to track pages seen; when it explores a page it gives the page URL to the pagefetcher, then the result to pagesaver, then to the pagescanner.

`bool webpage_fetch(webpage_t *page);`
it  fetches the contents (HTML) for a page from a URL and returns.

`bool  pagesaver(webpage_t *page, char *pageDir, int ID)`
pagesaver outputs a page to the appropriate file.

`char *webpage_getNextURL(webpage_t *page, int *pos);`
pagescanner extracts URLs from a page and returns one at a time.



### Disclaimer 
The testing bash script testing.sh will not run wikipedia at depth page 2 since it takes a substantial amount of time
