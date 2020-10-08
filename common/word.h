/* 
 * word.h
 * Antony Guzman, Feb 2020
 * A header file for word.c, listing the functions for use in TSE
 */


/**************** functions ****************/

/**************** normalize_word ****************/
/* Turns word given into lowercase
 *
 * Caller provides:
 *   a valid pointer to a word 
 * We do:
 *   if word == NULL, return NULL
 *   otherwise, go through each letter in the 
 *   word and convert to lower case
 * We return:
 *   a valid pointer to word.
 */
char* normalize_word(char *word);