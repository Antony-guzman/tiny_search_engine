/*
 * pagedir.h
 * Antony Guzman, Feb 2020
 * A header file for pagedir.c, listing the functions for use in TSE
 */


#include <stdio.h>
#include "webpage.h"

/**************** Functions ****************/

/**************** pagesaver ****************/
/* Create a file (for crawler) containning info from 
 * from a webpage
 * 
 * Caller provides:
 *   valid pointer to a webpage, valid string for a directory
 * , valid integer.
 * We do:
 *   if any parameters is NULL, return false
 *   otherwise, save the URL of a page as the first line of the file
 *   the depth of the page as the second line
 *   and the rest is the HMTL of the webpage
 *   the filename will be the ID configured by crawler
 *    
 */
bool  pagesaver(webpage_t *page, char *pageDir, int ID);

/**************** page_validate ****************/
/* Validates if given directory was produced by crawler.
 *
 * Caller provides:
 *   valid pointer to string(char) to a given directory
 * We do:
 *   if pageDir is NULL, return false
 *   otherwise, check if a file by the name of "./crawler"
 *   exist within the given directory.
 *   If it does, return true
 *   else, return false
 *    
 */
bool page_validate(const char* pageDir);

/**************** page_load ****************/
/* Create a webpage from a given file (from cralwer directory)
 *
 * Caller provides:
 *  valid string for a directory and a valid int ID
 * , valid integer.
 * We do:
 *   if pageDir is NULL, return false
 *   otherwise, open the file and retrieve the URL, 
 *   depth, and HTML and put them as the parameters for a new
 *   webapge. 
 *   Clean up afterwards
 *    
 */
webpage_t* page_load(const char *pageDir, const int ID);
