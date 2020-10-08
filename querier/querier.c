/* ========================================================================== */
/* File: querier.c - Tiny Search Engine indexer
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
 * Output: Outputs Answers search queries submitted by the user
 * via stdin into a ranked list of documents that fulfills the queries
 * submitted by score. We rank resulting set of documents according to its
 *  score, as described below, and print the set of documents in decreasing rank 
 * order; for each, list the score, document ID and URL.
 *
 * Error Conditions: The program exits if the arguments provided do not meet the requirements,
 *  if file are not able to
 * be opened, or if memory is not allocated properly.
 */

#include <stdio.h>
#include <unistd.h>
#include "memory.h"
#include <ctype.h>
#include <sys/stat.h>
#include <string.h>
#include "index.h"
#include "pagedir.h"
#include "file.h"
#include "word.h"
#include "counters.h"

/*
 * Struct to contain docId and score from query score
 * to rank results
 * 
 */
typedef struct document { 
    int docID;
    int score;
} document_t;


/*
 * Struct to store 2 counters 
 * to intersect them
 * 
 */
typedef struct two_counters {
  counters_t *result;
  counters_t *ctrs;
} two_counters_t;



// function declarations 
void querier(index_t* index, char *pageDirectory);
void chopInput(char *input, char **words, int numWords);
void counters_intersect_helper(void *arg, const int key, int count);
void counters_union_helper(void *arg, const int key, const int count);
void count_score_helper(void *arg, const int key, int count);
void printMatches(document_t *array, int numCounters, char* pageDirectory);

int checkFormat(char* input);
int logic(char** words, int numWords);
int docCount(char *pageDirectory);
int quicksortHelper(const void *first, const void *second);

counters_t *scoreDocuments(char **words, int numWords, index_t *index);
void intersectCounters(counters_t *wordOne, counters_t *wordTwo);
void unionCounters(counters_t *wordOne, counters_t *wordTwo);

document_t *rankResults(counters_t *results, int numResults, int numFiles);


/*
 * Parse in command-line arguments and call 
 * querier function 
 * 
 * Caller provides:
 *  a valid pointer to a existing directory provided by crawler
 *  and valid indexFile created by the indexer  
 * We do:
 *  load the indexFile into an index
 *  feed arguments into querier function
 *  clean up index
 * 
 */
int main(int argc, char *agrv[]){
  //check command line arguments  
  // make sure there are the rights ones
  if (argc != 3){
    fprintf(stdout, "you must supply 2 arguments.\n");
    printf("Usage: ./querier pageDirectory indexFilename\n");
    exit(1);
  }

  char*dir_name = agrv[1];
  struct stat dir;
  if (stat(dir_name,&dir) != 0 ||  access(dir_name,W_OK) != 0){
	   fprintf(stdout," You must supply an existing and writeable directory. \n");
	   exit(1);	         
  } 

  //check the directory (make sure it was produced by crawler)
  if (!page_validate(dir_name)){
    fprintf(stdout, "Error: directory was not produced by crawler\n");
    exit(1);
  }

  FILE *indexFile;
  // check for indexFilename
	indexFile = fopen(agrv[2], "r");
	if (indexFile == NULL) {
		fprintf(stderr, "indexFilename does not exist or is unreadable\n");
		exit(1);
  }
	fclose(indexFile);

  // create index
  // int num = lines_in_file(indexFile);
  index_t *index = index_new(500);

  // load the index
  index_load(agrv[2],index);

  // go to the querier
  querier(index,dir_name);

  // clean up 
  index_delete(index);
  return 0;
 }

/*
 *  Function to received stdin from the user in the form of a query in 
 *  order to provide a list of searches that match the query given 
 * 
 *  Caller provides:
 *    valid pointer to index and pageDirectory
 *  We do:
 *    open stdin to accept valid input 
 *    check the logic and format of the input
 *    split the input into an array of words
 *    calculate the score of the array 
 *    rank the results
 *    print the matches if any
 *    clean up 
 * 
 */
void querier(index_t *index, char *pageDirectory)
{
  // read in input unitl EOF
  char*input;
  while(1){
    printf("Query: ");
    if((input = freadlinep(stdin)) == NULL) {
      printf("\n");
      count_free(input);
      break;
    }

    // check the format and get a number of words to later
    // use to chop the line of input
    int numWords = checkFormat(input);

    // error case
    if (numWords == 0) {
      fprintf(stdout,"Error: query is empty or contains invalid characters\n");
      count_free(input);
      continue;
    }
    // make an array of words based on the number of words in the input
    char *words[numWords];

    //split the input into array of words
    chopInput(input, words,numWords);
      
    // check the logic of the input
    // ensure no "ands" or "ors" are first/last/next to each other
    if(!logic(words,numWords)){
      fprintf(stdout,"Error: logic is incorrect\n");
      count_free(input);
      continue;
    }

    // print back the query given in clean form 
    printf("Evaluating Query: ");
    for (int i = 0; i < numWords; i++) {
      printf("%s ", words[i]);
    }
    printf("\n"); 

    // Create 'counters' object of all documentsID in relation to words
    // and their scores
    counters_t *queryScore = scoreDocuments(words, numWords, index);


    //find out how many scores are grated than 0
    int numCounters = 0;
    counters_iterate(queryScore, &numCounters, (*count_score_helper));

    //error case of scores being 0
    if (numCounters == 0) {
      printf("No matching documents ...\n");
       counters_delete(queryScore);
       count_free(input);
      continue;
    }

    // get the number of files in directory to later rank
    int numFiles = docCount(pageDirectory);

    // rank the results based on score 
    document_t *array = rankResults(queryScore, numCounters, numFiles);

    //print the matches to output
    printMatches(array,numCounters,pageDirectory);

    // clean up
    counters_delete(queryScore);
    count_free(array);
    count_free(input);
    }
  count_free(input);
}

/*
 *  Functionto print out matches if any 
 * 
 *  We provide:
 *    valid pointers to a list of documents, the crawler directory
 *    and integer of the number of scores above 0 (number counters)
 *  We do:
 *    go through each counter and print out the respetive file and URL
 *    clean up
 * 
 */
void printMatches(document_t *array, int numCounters, char* pageDirectory)
{
  // Print ranked results
  printf("Matches %d documents (ranked):\n", numCounters);
  for (int i = 0; i < numCounters; i++) {
    // Make filename to get URL
    int docID = array[i].docID;
    char filename[100];
    sprintf(filename, "%s/%d", pageDirectory, docID); 

    // get URL from file 
    FILE *fp = fopen(filename, "r");
    char *url = freadlinep(fp); // 


    printf("Score: %3d docID: %3d | %s\n", array[i].score,
    array[i].docID, url);

    // clean up
    fclose(fp);
    count_free(url);
  }

}

/*
 *  Function to rank results based on scores from query score
 *  calls quicksort with helper to sort them 
 *  and returns the array 
 * 
 * caller provides:
 *  a valid pointer to the results containing docID and their scores
 *  the number of scores above 0 and the number of files
 * 
 * we do:
 *  add the number of files that are in the queryScore counteres object and them
 *  unordered to a list of document structs, that contain the ID and its respective
 *  score. 
 *  then call quicksort to sort them 
 * 
 * we return;
 *  sorted by decreasing order array 
 *  
 */
document_t *rankResults(counters_t *results, int numResults, int numFiles)
{
  // initalize the array 
  document_t *array = calloc(numResults, sizeof(document_t));

  // int to add by position to the array 
  int added = 0;

  // Add results (unordered) to array
  for (int i = 1; i <= numFiles; i++) {

    // Only add if that file has score above 0
    int score = counters_get(results, i);
    if (score > 0) {

      //create document struct
      document_t result;
      result.score = score;
      result.docID = i;

      //add to array
      array[added] = result;
      added++;
    }
  }

  // Use quicksort for descending order 
  //and compare
  qsort(array, numResults, sizeof(document_t), quicksortHelper);

  return array;
}

/*
 * helper function to compaare the document structs 
 * 
 * caller provides:
 *   valid pointers to const void items 
 *   but will be casted into documents structs 
 *   (need to cast since the qsort function requires 
 *   const void item pointers for the helper function)
 * 
 * we do:
 *  cast the const void items pointers and return the
 *  difference between the socress 
 * 
 */
int quicksortHelper(const void *first, const void *second)
{
  //need to cast them
  document_t *docOne = (document_t *)first;
  document_t *docTwo = (document_t *)second;

  //the compare, so subtract the scores
  return(docTwo->score - docOne->score);
}

/*
 * Functon to check the logic of the input
 * 
 * Caller provides:
 *   a array of chopInputs that have the correct format 
 *  (alpha with spaces)
 * 
 * we do:
 *  check that 'and' and 'or' are never the first word/last/ or next to eacher
 *  other in the array 
 * 
 * We return:
 *  1 if error, 0 if no error
 * 
 */
int logic(char** words, int numWords)
{
  //first word can't be and/or
  if (strcmp(words[0],"and") == 0 || strcmp(words[0],"or") == 0 ){
    return 0;
  } 

  // last word cant be and/or
  if (strcmp(words[numWords - 1], "and") == 0 || strcmp(words[numWords - 1], "or") == 0 ) {
    return 0;
  }

  // 'and' cant be besides another 'and' or 'or'
  for (int i = 1; i < numWords - 1; i++) {
    if (strcmp(words[i], "and") == 0) {
      // and to the left
      if (strcmp(words[i - 1], "and") == 0) {
        return 0;
      }
      // or to the left
      if (strcmp(words[i - 1], "or") == 0) {
        return 0;
      }
        // and to the right
      if (strcmp(words[i + 1], "and") == 0) {
        return 0;
      }
        // or to the right
      if (strcmp(words[i + 1], "or") == 0) {
        return 0;
      }
    }
  }  
  // 'or' cant be besides another 'and' or 'or'
  for (int i = 1; i < numWords - 1; i++) {
    if (strcmp(words[i], "or") == 0) {

      // and to the left
      if (strcmp(words[i - 1], "and") == 0) {
        return 0;
      }

      // or to the left
      if (strcmp(words[i - 1], "or") == 0) {
        return 0;
      }

      // and to the right
      if (strcmp(words[i + 1], "and") == 0) {
        return 0;
      }
      // or to the right
      if (strcmp(words[i + 1], "or") == 0) {
        return 0;
      }
    }
  }  
  return 1;
}
/*
 * Function to split the input pointer of char
 * by chopping:
 *  by sliding pointers along the array for the word
 * 
 * caller provide:
 *  char pointer to the input, a array of pointers to words, 
 *  and number of words of in the array 
 *  
 * we do:
 *  set a start pointer to the beginning of a word
 *  set the rest pointer at the end of word and insert
 *  a null character
 *  nomarliza the word and insert into the array
 *  then reset start to rest pointer for the next word

 */
void chopInput(char *input, char **words, int numWords)
{
  // pointers to chop
  char *start =input;
  char *rest = input;

  for (int i=0; i< numWords;i ++){
    //go to the beginning fo the first word
    while(!isalpha(*start)){
      start++;
    }
    //move rest to end of the word
    rest = start;
    while(isalpha(*rest) != 0){
      rest++;
    }

    //inset null character
    *rest = '\0';

    //turn to lower case
    words[i] = normalize_word(start);

    // prerp for next word by setting start to where rest left off
    start = rest;
  }
}


/*
 * checks format and returns num of words
 * 
 * caller provides:
 *  valid pointer to the input (char)
 * we do:
 *  check whether any of the char given are not alpha or spaces
 *  space should not matter but if the input is only string, then error
 *  
 *  iterate again to find out how many words are in the input to later
 *  split. 
 * 
 * we return:
 *  then number of words in the input
 *  0 if invalid format (empty or contains invalid characters)
 */
int checkFormat(char* input)
{
  // counters for nonalpha and spaces
  int nonalpha = 0;
  int spaces =0;

  //iterate through the pointer of char
  for (char *ptr = input; *ptr != '\0'; ptr++){

    // count the spaces if any
    if (isspace(*ptr)){
      spaces++;
    }
    //counts nonalpha and nonspace characters
    if(!isalpha(*ptr) && !isspace(*ptr)){
      nonalpha++;
    }
  }

  // if there is 1 or more non alpha chars or
  // there's only spaces, return 0
  if (nonalpha >0 || spaces == strlen(input)){
    return 0;
  }

  //figure out how many words 
  char *iterator2 = input;
  int numWords = 0;

  while (*iterator2 != '\0') {
    // Find beginning of a word
    while (!isalpha(*iterator2)) {
      iterator2++;
    }
    // add word to length, catch empty queries
    if (isalpha(*iterator2)) {
      numWords++;
    }

    // Move to end of word, stop if we reach null
    while (isalpha(*iterator2) != 0 && *iterator2 != '\0') {
      iterator2++;
    }
  }
  return numWords;
}

/*
 * Function to count the number of files within 
 * a crawler directory
 *  
 * We provide:
 *  a valid pointer to a crawler directory 
 * 
 * We do:
 *  iterate through the files in the directory, checking if they exist
 *  and incrementing counter 
 * 
 * Notes:
 *  -1 at the end because we need to start with 1 since file 0 does not exist
 * 
 */
int docCount(char *pageDirectory)
{
  // goes into the directory and gets the number of documents
	FILE *fp;

  // get the name of the crawler files
	char crawlerFile[100];
	int docID = 1;
	sprintf(crawlerFile, "%s/%d", pageDirectory, docID);

  // open and check the if they exist, increaming the count
	fp = fopen(crawlerFile, "r");
	while (fp != NULL) {
		fclose(fp);
		docID++;
		sprintf(crawlerFile, "%s/%d", pageDirectory, docID);
		fp = fopen(crawlerFile, "r");
	}
	return docID - 1;

}

/*
 * Functiont to combine results, w
 * 
 * Caller provides:
 *  valid pointer to an array of words, an index, and the number of words
 *  within that array 
 * 
 * We do:
 *  run through the array of words, and depending on conjuctions, do either
 *  intersection or union
 * 
 * if wordA and wordB:
 *   the set of documents that match both words is the intersection of the two 
 *   sets, and the score for each document (per the specs) is the minimum of the 
 *   count for each document.
 * 
 * if wordA or wordB: 
 *   the set of documents that match either word is the union of the two sets, 
 *   and the score for each document (per the definition above) is the sum of the 
 *   count for each document. 
 *    
 * We return;
 *  Create 'counters' object of all documentsID in relation to words
 *  and their scores
 *  
 * 
 */
counters_t *scoreDocuments(char **words, int numWords, index_t* index)
{

  //create a counters object that hols everything seen so far
  //and keeps being added to 
  counters_t *total = counters_new();

  //get the first word 
  counters_t *first = index_get(index, words[0]);

  //add it total 
  unionCounters(total,first);

  // if there is more words that follow the correct 
  // logic then go through the array
  int i =1;
  while (i < numWords){

    //skip over 'and' and move on to the next word
    if (strcmp(words[i], "and")== 0){
      i++;
      continue;
    }
    else if(strcmp(words[i], "or") == 0){
      //go to the next word
      i++;

      //get the merge/union of the word following or and modify total
      counters_t * currentWord = index_get(index,words[i]);
      unionCounters(total,currentWord);

      //move on
      i++;
    }
    else{
      // get the intersection of a normal word thats not 'and' or 'or'
      // basically ignoring 'and', modify total 
      counters_t * currentWord = index_get(index,words[i]);
      intersectCounters(total,currentWord);

      //move on
      i++;
    }
  }
  return total;
}

/* 
 * iterate through a second counters, updating first
 * counters accordingly with goal of having an intersection
 *  of two counters
 * 
 * We provide:
 *    valid pointers to two counters
 * We do:
 *    call counters_iterate with helper function
 */
void intersectCounters(counters_t *wordOne, counters_t *wordTwo)
{
  // create a struct that has both counters
  // and pass it in as the arg of counters_terate

  //malloc
  two_counters_t *counters = malloc(sizeof(two_counters_t));

  // set both counters into the struct
  counters->result = wordOne;
  counters->ctrs = wordTwo;

  //itereate with helper function 
  counters_iterate(wordOne, counters, counters_intersect_helper);

  // clean up
  count_free(counters);
}


/* 
 * intersect the second counters into the first counters
 *  the second counters is unchanged 
 * 
 * We provide:
 *    valid pointers to arg, key, and count from counters_t
 * We do:
 *    iterate through the second counters
 *    if it exist in the first set or doesn't  
 *    update first set accordingly
 */
void counters_intersect_helper(void *arg, const int key, int count)
{
  //set the arg as the counters holding both counters
  two_counters_t *counters = arg;

  int num = 0;

  //find the min and set it
  if (count > counters_get(counters->ctrs,key)){
    num = counters_get(counters->ctrs,key);
  }
  else{
    num = count;
  }
  counters_set(counters->result,key,num);
}

/* 
 * iterate through a second counters, updating first
 * counters accordingly with goal of having a union of two 
 * counters
 * 
 * We provide:
 *    valid pointers to two counters
 * We do:
 *    call counters_iterate with helper function
 */
void unionCounters(counters_t *wordOne, counters_t *wordTwo)
{
  counters_iterate(wordTwo,wordOne, counters_union_helper);
}

/* 
 * merge the second counters into the first counters
 *  the second counters is unchanged 
 * 
 * We provide:
 *    valid pointers to arg, key, and count from counters_t
 * We do:
 *    iterate through the second counters
 *    if it exist in the first set or doesn't  
 *    update first set accordingly
 */
void counters_union_helper(void *arg, const int key, const int count)
{
  // get first counters set
  counters_t *wordOne = arg;

  int countB = count;


  //find the same key in 1st word
  int countA = counters_get(wordOne, key);

  //if doesnt exist, add it to the first counters
  if (countA == 0){
    counters_set(wordOne,key,countB);
  }

  //if it does exist, add them up
  else{
    countA += countB;
  }

}

/*
 *  counter helper that carries a counter to count the number
 *  of scores above 0 
 * 
 *  We provide:
 *    valid pointers to arg, key, and count from counters_t
 *  We do:
 *    if the count for the key is above 0, increment the total
 * 
 */
void count_score_helper(void *arg, const int key, int count) 
{
  if (count > 0) {
    int *total = arg;
    (*total)++;
  }
}

