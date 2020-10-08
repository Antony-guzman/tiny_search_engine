  
/* ========================================================================== */
/* File: indextest.c 
 * A test program for indexer 
 * 
 * Author: Antony Guzman
 * Feb 2020
 *
 * Input: 2 Arguments
 * Arg 1: oldIndexFilename, where oldIndexFilename 
 * is the name of a file produced by the indexer
 * Arg 2: newIndexFilename, where newIndexFilename is the name of a file
 *  into which the index should be written.
 *
 * Command line options: None
 *
 * Output: This program load the index from the oldIndexFilename into an
 * inverted-index data structure and creates a file newIndexFilename and 
 * write the index to that file. 
 *
 * Error Conditions: The program exits if the arguments provided do not meet the requirements, if file are not able to
 * be opened, or if memory is not allocated properly.
 * 
 * 
 */
#include "index.h"
#include "pagedir.h"

int main(int argc, char* argv[])
{

    // check command line arguements
    //make sure that there are the right numbers of arguemtns
    if (argc != 3){
        fprintf(stdout, "you must supply 2 arguments.\n");
        printf("Usage: ./indexer oldIndexFilename newIndexFilename\n");
        exit(1);
    }



    // create index
    index_t *index = index_new(300);

    // load oldIndexFilename into an index
    index_load(argv[1],index);

    //output the index into a newIndexFilename
    index_save(argv[2],index);

    //clean up
    index_delete(index);

    return 0;
}
