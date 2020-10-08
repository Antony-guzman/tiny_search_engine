/*
 * index.h
 * Antony Guzman, Feb 2020
 * A header file for index.h listing the functions need to create/load from the index
 * 
 * An "index" is a hashtable of counter sets. Our index maps from words to (docID, count) pairs,
 * representing the number of occurrences of that word in that document. We use a
 * hashtable wherein each item is a counters_t where we use the docID
 * as a key. 
 * 
 */
#include <stdio.h>
#include <stdbool.h>
#include "hashtable.h"
#include "counters.h"

/**************** global types ****************/
typedef struct index index_t;   // opaque to users of the module

/************* Local Functions ***************/

/**************** index_add ****************/
/* Insert items into the given index.
 *
 * Caller provides:
 *   valid pointer to index, valid string for word, valid integer.
 * We do:
 *   if word already exist within the hashtable, update its counters
 *   otherwise, create a counters_t struct for the word and update 
 *   accordingly 
 *    
 */
void index_add(index_t *index, const char *word, const int docID);


counters_t *index_get(index_t *index, const char *word);

/**************** index_set ****************/
/* Set the items into the given index
 *
 * Caller provides:
 *   valid pointer to index, valid string for word, valid integer.
 * We do:
 *   if word already exist within the hashtable, reset the count key and 
 *   int value to those given 
 *   otherwise, create a new counter and update accordingly
 *   
 */
void index_set(index_t *set, const char *word, const int docID, int count);

/**************** help_save_hashtables ****************/
/* Helper function to  pass to hashtable_iterate that 
 * prints the index file
 * 
 * arg is a valid file pointer with word being the key for the
 * hashtable and counter being a valid pointer to a counter_t.
 * 
 */
void help_save_hashtable(void *arg, const char *word, void *counter);

/**************** help_save_counters ****************/
/* Helper function to  pass to counters_iterate that 
 * prints the index file
 * 
 * arg is a valid file pointer with ID being the key and count being .
 *  the counter.
 * 
 */
void help_save_counters(void *arg, const int ID, const int count);

/**************** help_delete ****************/
/* Helper function to  pass to hashtable_delete that deletes
 *  the word and counters structures
 * 
 */
void help_delete(void *item);


/************* Global Functions ***************/

/************* index_new **********************/
/* Create a new (empty) index.
 *
 * Caller provides:
 *   number of slots to be used for the hashtable (must be > 0).
 * We guarantee:
 *   The index is initialized empty.
 * Caller is responsible for:
 *   later calling index_delete.
 */
index_t *index_new(int num_slots);

/************* index_save **********************/
/* We ouput the index into an indexFile
 *
 * Caller provides:
 *   A valid pointer to a existing and writeable file and valid
 *   pointer to an index.
 * We do:
 *   If indexFile pointer is not valid, or index == NULL, do 
 *   nothing.
 *   otherwise,call helper function to iterate through the 
 *   hashtable within the index to output the data within 
 * 
 */
bool index_save(char *indexFile, index_t *index);

/************* index_ load **********************/
/* Create/set an index from a given indexFile
 *
 * Caller provides:
 *   A valid pointer to a existing and readable file and valid
 *   pointer to an index.
 * We do:
 *   If indexFile pointer is not valid, or index == NULL, do 
 *   nothing.
 *   otherwise, retrieve words from the file while retrieving 
 *   the docID and count line by line and inputting the data into
 *   an index by calling a helper function to "set" it into the 
 *   hashtable 
 * 
 */
index_t *index_load(char* indexFilename, index_t *index);

/************* index_build **********************/
/* Builds the index from a file produced by the crawler
 * directory.
 * 
 * Caller provides:
 *   valid pointer to a directory produce by crawler and valid
 *   pointer to an index.
 * We do:
 *   if index ==NULL or the pageDir is not a crawler produced 
 *   directory, do nothing
 *   otherwise, read through all the files in the directory 
 *   put each word longer than 3 words into the hashtable with 
 *   counters where the filename/ID of the document is the key
 *   and the count is how many times in each file. 
 * Notes:
 *    We normalize each word (conver word to lowercase).
 */
void index_build(const char* pageDir,index_t *index);

/************* index_delete **********************/
/* Delete index, calling helper function.
 *
 * Caller provides:
 *   valid index pointer,
 *   valid pointer to function that handles hashtable_delete
 * We do:
 *   if hashtable==NULL, do nothing.
 *   otherwise, unless itemfunc==NULL, call the helper function 
 *   on the hashtable
 * Notes:
 *   We free the hashtable within the index that frees up the counters
 *   within the table.
 */
void index_delete(index_t *index);
