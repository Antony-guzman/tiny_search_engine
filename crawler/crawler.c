  
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
#include <stdbool.h>
#include <string.h>
#include <unistd.h>
#include <dirent.h>
#include "bag.h"
#include "hashtable.h"
#include "webpage.h"
#include <sys/stat.h>
#include "pagedir.h"
#include "memory.h"


#define MEMTEST true
void crawler(char *seed, char *pageDirectory, int maxDepth);

//  parses parameters and passes them to the crawler.
int main(int argc, char* argv[])
{

    // check command line arguments
    // Make sure that there are the right number of arguments
    if (argc != 4) {
        fprintf(stdout, "You must supply 3 arguments.\n");
        printf("The first argument is the seed URL. \n The second argument is the target directory. \n"
            "The third argument is the max depth which to crawl.\n");
        exit(1);
    }

   // check the directory 
   char* dir_name = argv[2];
   struct stat dir;
   if (stat(dir_name,&dir) != 0 ||  access(dir_name,W_OK) != 0){
	   fprintf(stdout," You must supply an existing and writebale directory. \n");
	   exit(1);	   
      
   }

    // check the Max-Depth parameters
    int Max_Depth = atoi(argv[3]);
    if (Max_Depth < 0){
	    fprintf(stdout, "The depth of the crawler must be a non negative integer. \n");
	    exit(1);
    }

   //create an file to ensure that it is a crawler produced direcoty
   FILE *fp;
   char filename[10];
   sprintf(filename, "%s/.crawler",dir_name);
   fp = fopen(filename, "w");
   fclose(fp);

   // check whether seedURL submitted is the 'correct' one for this program (is it internal)
   char *seedURL = argv[1];
   if (!IsInternalURL(seedURL)){
	fprintf(stdout, "The seedURL must be internal \n");
	exit(1);
   }
   // pass the parameters to the crawler
   crawler(seedURL, dir_name, Max_Depth);

}

//uses a bag to track pages to explore, and hashtable to track pages seen;
// when it explores a page it gives the page URL to the pagefetcher, then the 
// result to pagesaver, then to the pagescanner
void crawler(char *seed, char *pageDirectory, int maxDepth){

   //check the seed and make baseurl (check if crawling websites are internal)
   hashtable_t *table = hashtable_new(100);
   bag_t *bag = bag_new();


   //insert seed to start crawling
   char *copy = malloc(strlen(seed) +1);
   strcpy(copy,seed);
   webpage_t *seedpage = webpage_new(copy, 0, NULL);
   bag_insert(bag, seedpage);
   hashtable_insert(table, seed, " ");

   webpage_t *current_webpage;
   int depthID = 1;
   while ((current_webpage = bag_extract(bag))!= NULL){
	   if(webpage_fetch(current_webpage)){
             if ((pagesaver(current_webpage,pageDirectory, depthID) == false)){
			    fprintf(stdout,"error: page not saved\n");
			     }	   
	   }
	   else{
	    fprintf(stdout,"error: page not fetched\n");
	   }
	   if (webpage_getDepth(current_webpage) < maxDepth){
		int pos =0;
		char *result;

		while((result = webpage_getNextURL(current_webpage, &pos)) != NULL){
		     if(IsInternalURL(result)== false){
			    count_free(result);

		     }
		     else{
			     if (hashtable_insert(table, result, &depthID) == true){
			     char *copy = malloc(strlen(result) +1);
			     strcpy(copy,result);
			     webpage_t *new = webpage_new(copy,depthID ,NULL);
			     bag_insert(bag, new);
			     }
			     count_free(result);
		     }
		     }	     
		     
		 
		}
	   
	   webpage_delete(current_webpage);
	    
	   depthID++;
   		
   }
  hashtable_delete(table,NULL);
  bag_delete(bag,webpage_delete);

}
