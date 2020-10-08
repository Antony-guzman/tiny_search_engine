/* ========================================================================== */
/* File: indexer.c - Tiny Search Engine indexer
 *
 * Author: Antony Guzman
 * Feb 2020
 *
 * Input: 2 Arguments
 * Arg 1: pageDirectory, the pathname of a directory produced Crawler
 * Arg 2: indexFilename, the pathname of a file into which the index 
 * should be written; the indexer creates the file (if needed) 
 *  and overwrites the file (if it already exists).
 *
 * Command line options: None
 *
 * Output: This program outputs the index to the provieded directory by building an 
 * inverted-index data structure mapping from words to (documentID, count) pairs,
 *  wherein each count represents the number of occurrences of the given word in the 
 * given document. Ignore words with fewer than three characters, and “normalize” the 
 * word before indexing. Here, “normalize” means to convert all letters to lower-case.
 * create a file indexFilename and write the index to that file. 
 *
 * Error Conditions: The program exits if the arguments provided do not meet the requirements,
 *  if file are not able to
 * be opened, or if memory is not allocated properly.
 */

#include <stdio.h>
#include <stdbool.h>
#include "memory.h"
#include "index.h"
#include "pagedir.h"
#include <sys/stat.h>
#include <unistd.h>
#include <dirent.h>

int main(int argc, char * argv[]){

    //check command line arguments 
    // make sure there are the rights ones
    if (argc != 3){
        fprintf(stdout, "you must supply 2 arguments.\n");
        printf("Usage: ./indexer pageDirectory indexFilename\n");
        exit(1);
    }
    

    //check if directoy exist 
    char*dir_name = argv[1];
    struct stat dir;
    if (stat(dir_name,&dir) != 0 ||  access(dir_name,W_OK) != 0){
	   fprintf(stdout," You must supply an existing and writebale directory. \n");
	   exit(1);	   
      
    }

    //check the directory (make sure it was produced by crawler)
    if (!page_validate(dir_name)){
        fprintf(stdout, "Error: directory was not produced by crawler\n");
        exit(1);
    }

    // create index
    index_t *index = index_new(300);

    // make the index from the directory 
    index_build(dir_name,index);

    // // put the index into a file 
    index_save(argv[2],index);

    // clean up
    index_delete(index);
    

    return 0;
}
