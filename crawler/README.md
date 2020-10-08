## CS50 Lab 4


## Antony Guzman, Jan 2020


### crawler
the crawler crawls a website and retrieves webpages starting with a specified URL. It parses the initial webpage, extracts any embedded URLs and retrieves those pages, and crawls the pages found at those URLs, but limits itself to maxDepth hops from the seed URL and to URLs that are ‘internal’ to the designated CS50 server. When the crawler process is complete, the indexing of the collected documents can begin.




### Usage
./crawler [seedURL] [pageDirectory] [maxDepth]

`seedURL` must be a valid URL and internal `pageDirectory` must exist and be a writable directory `maxDepth` must be a nonnegative integer.


### Assumptions
No assumptions beyond those stated in the requirements. The current directory must be created before hand this program will not create the directory but will exit if it can't find it. Additionally, he crawler stays within the cs.dartmouth domain.


### Limitations
Program takes time to run, will run slower for increasing max depth. Only staying in the cs.dartmouth domain.


### Compilation
First make in libcs50 and common directories  to have the archive files created.
To compile, simply make. Results are saved in the directory created.

See TESTING for details of testing and an example test run.
