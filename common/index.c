/*
 * index.c
 * Antony Guzman, Feb 2020
 * Provides the functions that will be used for the TSE 
 */


#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "counters.h"
#include "memory.h"
#include "index.h"
#include "word.h"
#include "file.h"
#include "pagedir.h"
#include "hashtable.h"
#include "counters.h"

/**************** global types ****************/
typedef struct index {
  hashtable_t *hashtable; 
  int num_slots; 
} index_t;

// ************* Local Functions *************  //

/**************** index_add() ****************/
/* see index.h for description */
void index_add(index_t *index, const char *word, const int docID)
{
    // error cases
    if (index != NULL && word != NULL){

        counters_t *counter = hashtable_find(index->hashtable, word);

        // word already exist, add to its counters
        if (counter != NULL){
            counters_add(counter, docID);
        }
        //else, create new word/counters
        else{
            counter = counters_new();
            counters_add(counter,docID);
            hashtable_insert(index->hashtable, word, counter);
        }
    }
} 

/**************** index_set() ****************/
/* see index.h for description */
void index_set(index_t *index, const char *word, const int docID, int count)
{
    if (index != NULL && word != NULL){

        counters_t *counter = hashtable_find(index->hashtable,word);

        // if word already exist, reset the counter
        if (counter != NULL){
            counters_set(counter, docID, count);
        }
        //else create word inside hashtable and set its values 
        else{
            counter = counters_new();
            counters_set(counter,docID, count);
            hashtable_insert(index->hashtable, word, counter);
        }
    }   
}

/**************** help_save_hashtable() ****************/
/* see index.h for description */
void help_save_hashtable(void *arg, const char *word, void *counter)
{
    FILE *fp = arg;
    counters_t *slot_counter = counter;
    fprintf(fp, "%s ", word);
    counters_iterate(slot_counter, fp, *help_save_counters);
    fprintf(fp, "\n");  // new lines for the next word

}

/**************** help_save_counters() ****************/
/* see index.h for description */
void help_save_counters(void *arg, const int ID, const int count)
{
    FILE *fp = arg;
    fprintf(fp, "%i %i ", ID, count);

}

/**************** help_delete() ****************/
/* see index.h for description */
void help_delete(void *item)
{
    counters_t *ctr = item;
    counters_delete(ctr);
}


//************* Global Functions ********//

/**************** index_new() ****************/
/* see index.h for description */
index_t *index_new(int num_slots)
{
    index_t *index = count_malloc(sizeof(index_t));
    index->hashtable = hashtable_new(num_slots);
    index->num_slots = num_slots;
    return index;
}

/* Returns the item associated with the given word.
 * Returns NULL if it can't find the word or the index is NULL
 */
counters_t *index_get(index_t *index, const char *word) {
    counters_t *item = hashtable_find(index->hashtable, word);
    return item;
}

/**************** index_save() ****************/
/* see index.h for description */
bool index_save(char *indexFile, index_t *index)
{
    FILE *fp;
    // check error cases
    if (index == NULL || indexFile == NULL) {
        return false;
    }
    fp =fopen(indexFile, "w");

    if (fp == NULL){
        fprintf(stdout," File not writeable\n");
        return NULL;
    }
    // iterate through the hashtable and use helper function 
    else {
           hashtable_iterate(index->hashtable,fp, *help_save_hashtable);
            fclose(fp);
           return true; 
    }


}


/**************** index_load() ****************/
/* see index.h for description */
index_t *index_load(char* indexFilename, index_t *index)
{   
    FILE *fp;
    char *word;
    int ID;
    int count;
    int scan;


    // read in words and ID and count, then use helper function to set
    // it in the index struct
    if ((fp = fopen(indexFilename,"r")) != NULL){
        while ((word = freadwordp(fp)) != NULL){
            while ((scan = fscanf(fp, "%d %d",&ID,&count)) > 0){
                index_set(index, word, ID, count);
            }
            count_free(word);
        }
        fclose(fp);
    }
    return index;
}

//creates a index from crawler directory files
void index_build(const char* pageDir,index_t *index)
{
    if (pageDir != NULL && index != NULL && page_validate(pageDir) ){

        // open the crawler file to read
        FILE *fp;
        int ID= 1;
        char crawlerFile[100];
        sprintf(crawlerFile,"%s/%i", pageDir, ID);
        fp = fopen(crawlerFile,"r");
        while (fp != NULL){    
            char *result;
            int pos = 0;
            // create webpage
            webpage_t *page = page_load(pageDir, ID);
            while ((result = webpage_getNextWord(page, &pos)) != NULL){
                
                // if word is larger than 3 characters
                if(strlen(result) >=3 ){
                    index_add(index, normalize_word(result), ID);
                }
                count_free(result);
            }
            
            // go to the next crawler file
            webpage_delete(page);
            fclose(fp);
            ID++;
            sprintf(crawlerFile,"%s/%i", pageDir, ID);
            fp = fopen(crawlerFile,"r");
        }

        
    }
}

//deletes
void index_delete(index_t *index)
{
    hashtable_delete(index->hashtable, *help_delete);
    count_free(index);

}








