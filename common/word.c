/* 
 * word.c
 * Antony Guzman, Feb 2020
 * Provides a function that will be useful to crawler, indexer, and querier
 */

#include <stdio.h>
#include <ctype.h>
#include "word.h"


/*see word.h for description */
char* normalize_word(char *word)
{

  if (word == NULL){
    return NULL;
  }
  for (char *ptr = word; *ptr != '\0'; ptr++){
      *ptr= tolower(*ptr);
  }
  return word;
}



