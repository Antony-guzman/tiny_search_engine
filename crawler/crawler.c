  
/* ========================================================================== */
/* File: crawler.c - Tiny Search Engine web crawler
 *
 * Author: Antony Guzman
 * Feb 2020
 *
 * Input: 3 Arguments
 * Arg 1: The url that you want to crawl 
 * Arg 2: The directory that you want to put the output file in. It must have already been created and must be writeable.
 * Arg 3: The depth that you wish to crawl to. This depth must non-negative
 *
 * Command line options: None
 *
 * Output: This program outputs file to the provided directory. These files, labeled 1,2,3,etc, contain the url of
 * the page crawled, the depth at which it was crawled, and the html curled from that url.
 *
 * Error Conditions: The program exits if the arguments provided do not meet the requirements, if file are not able to
 * be opened, or if memory is not allocated properly.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "webpage.h"
#include "pagedir.h"
#include "bag.h"
#include "hashtable.h"
#include "memory.h"
/**************** file-local global variables ****************/
static const int maxMaxDepth = 10;

/**************** local function prototypes ****************/
/* not visible outside this file */
static void parse_args(const int argc, char *argv[], 
                       char **seedURL, char **pageDirectory, int *maxDepth);
static void crawler(char *seed, char *pageDirectory, int maxDepth);
static void page_scan(webpage_t *page, bag_t *to_crawl, hashtable_t *seen);

// log one word (1-9 chars) about a given url
inline static void logr(const char *word, const int depth, const char *url)
{
#ifdef APPTEST
  printf("%2d %*s%9s: %s\n", depth, depth, "", word, url);
#else
  ;
#endif
}
/**************** parse_args ****************/
/* Parse the command-line arguments, filling in the parameters;
 * if any error, print to stderr and exit.
 */
static void
parse_args(const int argc, char *argv[], 
           char **seedURL, char **pageDirectory, int *maxDepth)
{
  /**** usage ****/
  char *program = argv[0];
  if (argc != 4) {
    fprintf(stderr, "usage: %s: seedURL pageDirectory maxDepth\n", program);
    exit (1);
  }

  /**** seedURL ****/
  *seedURL = argv[1];
  if (!NormalizeURL(*seedURL)) {
    fprintf(stderr, "usage: %s: un-normalizable seedURL '%s'\n", 
            program, *seedURL);
    exit (2);
  }
  if (!IsInternalURL(*seedURL)) {
    fprintf(stderr, "usage: %s: non-internal seedURL '%s'\n", 
            program, *seedURL);
    exit (3);
  }
  
  /**** pageDirectory ****/ 
  *pageDirectory = argv[2];
  if (!pagedir_init(*pageDirectory)) {
    fprintf(stderr, "usage: %s: invalid or unwritable directory '%s'\n", 
            program, *pageDirectory);
    exit (4);
  }

  /**** maxDepth ****/
  char *maxDepthString = argv[3];
  char excess; // any characters seen after an integer
  if (sscanf(maxDepthString, "%d%c", maxDepth, &excess) != 1) {
    fprintf(stderr, "usage: %s: invalid maxDepth '%s'\n", 
            program, maxDepthString);
    exit (5);
  }

  if (*maxDepth < 0 || *maxDepth > maxMaxDepth) {
    fprintf(stderr, "usage: %s: maxDepth '%d' must be in range [0:%d]\n",
            program, *maxDepth, maxMaxDepth);
    exit (6);
  } 
}


/**************** main ****************/
//  parses parameters and passes them to the crawler.
int main(int argc, char* argv[])
{
   // crawler parameters from the command line
   char *seedURL = NULL;
   char *dir_name = NULL;
   int maxDepth = 0;

   parse_args(argc, argv, &seedURL, &dir_name, &maxDepth);
   
   // pass the parameters to the crawler
   crawler(seedURL, dir_name, maxDepth);

   //exit success
   return 0;

}

//uses a bag to track pages to explore, and hashtable to track pages seen;
// when it explores a page it gives the page URL to the pagefetcher, then the 
// result to page_sav, then to the pagescanner
void crawler(char *seedURL, char *pageDirectory, int maxDepth)
{

   // allocate data structures
   bag_t *pages_to_crawl = bag_new();
   assertp(pages_to_crawl, "pages_to_crawl");
   const int TableSize = 200;
   hashtable_t *pages_seen = hashtable_new(TableSize);
   assertp(pages_seen, "pages_seen");

   // malloc a copy of the seedURL to store in the webpage_t below
   char *seedcopy = malloc(strlen(seedURL)+1);
   strcpy(seedcopy, seedURL);

   // initialize a WebPage representing the seed URL at depth 0, and add to bag
   bag_insert(pages_to_crawl, webpage_new(seedcopy, 0, NULL));
   // insert seedURL to hashtable; the 'item' value is unused. 
   hashtable_insert(pages_seen, seedURL, "seed"); 

   // initialize our document ID series
   int documentID = 0;
   // start crawling!
   webpage_t *page;
   while ( (page = bag_extract(pages_to_crawl)) != NULL) {
      // fetch the page, filling in page's html
      if (webpage_fetch(page)) {
         // save the fetched page to a file
         page_save(page, pageDirectory, ++documentID);
      
      //    // if we should explore another level, ...
         if (webpage_getDepth(page) < maxDepth) {
            // scan the page to extract URLs and put them in the bag
            page_scan(page, pages_to_crawl, pages_seen);
         }
      } 
      // // finished with this web page
      webpage_delete(page);
   }
  // clean up
  hashtable_delete(pages_seen, NULL);
  bag_delete(pages_to_crawl, webpage_delete);
  #ifdef MEMTEST
  // report on our own memory use
  count_report(stdout, "crawler");
#endif

}

/**************** page_scan ****************/
/* Scan the given page to extract any links (URLs); for any not 
 * already seen before, add them to the bag of pages yet to crawl.
 */
static void
page_scan(webpage_t *page, bag_t *pages_to_crawl, hashtable_t *pages_seen)
{
  assertp(page, "page_scan page==NULL");
  assertp(pages_to_crawl, "page_scan pages_to_crawl==NULL");
  assertp(pages_seen, "page_scan pages_seen==NULL");


  // extract URLs from the page, and consider each in turn
  char *url = NULL;        // will be filled with pointer to a new url
  int pos = 0;                // start at beginning of page

  while ((url = webpage_getNextURL(page, &pos)) != NULL) {
    // check whether it is internal to crawl domain
    if (IsInternalURL(url)) { // side effect: URL normalized
      if (hashtable_insert(pages_seen, url, "seen")) {
        // never seen it before: add it bag to to be crawled
        webpage_t *new = webpage_new(url, webpage_getDepth(page)+1, NULL);
        assertp(new, "webpage_new in page_scan");
        bag_insert(pages_to_crawl, new);
	// do not free(url) because it is saved in the webpage_t
      } 
      else {
        // ignore it, we've seen it before
	      free(url);
      }
    } else {
      free(url);
    }
  }
}



