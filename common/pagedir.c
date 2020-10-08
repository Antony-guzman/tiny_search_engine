/* 
 * pagedir.c
 * Antony Guzman, Feb 2020
 * Provides functions that will be useful to crawler, indexer, and querier
 */

#include <stdio.h>
#include <stdbool.h>
#include "webpage.h"
#include "memory.h"
#include "file.h"
#include <string.h>

/**************** file-local global variables ****************/
static const char crawlerfile[] = ".crawler";

/**************** file-local function prototypes ****************/
/* none */

/**************** pagedir_init ****************/
/* see pagedir.h for documentation */
bool
pagedir_init(const char *pageDirectory)
{
  if (pageDirectory == NULL) {
    return false;
  }

  // make a filename
  int filenamelen = strlen(pageDirectory) + strlen(crawlerfile) + 2;
  char *filename = assertp(malloc(filenamelen),
                           "pagedir_init filename");
  sprintf(filename, "%s/%s", pageDirectory, crawlerfile);

  // now create the file
  FILE *fp = fopen(filename, "w");
  if (fp == NULL) {           // file creation failed
    free(filename);
    return false;
  } else {                    // file creation succeeded
    fclose(fp);
    free(filename);
    return true;
  }
}

/**************** pagedir_save ****************/
/* see pagedir.h for documentation */
void 
page_save(const webpage_t *page, const char *pageDirectory, const int documentID)
{
  if (page == NULL) {
    assertp(NULL, "pagedir_save gets NULL page");
  }
  if (pageDirectory == NULL) {
    assertp(NULL, "pagedir_save gets NULL pageDirectory");
  }
  assertp(webpage_getURL(page), "pagedir_save gets NULL url");
  assertp(webpage_getHTML(page), "pagedir_save gets NULL html");

  // create filename string from page directory and document ID
  char *filename = assertp(malloc(strlen(pageDirectory)+12), "pagedir_save");
  sprintf(filename, "%s/%d", pageDirectory, documentID);

  FILE *fp = fopen(filename, "w");
  assertp(fp, "pagedir_save cannot open file for writing");
  
  fprintf(fp, "%s\n%d\n%s\n", 
          webpage_getURL(page), 
          webpage_getDepth(page), 
          webpage_getHTML(page));

  fclose(fp);
  free(filename);
}


/**************** page_validate() ****************/
/*see pagedir.h for description */
bool page_validate(const char* pageDir)
{
    //error checking for NULL
    if (pageDir == NULL){
        return false;
    }
    // if the ./crawler file is not there, 
    // then not the correct directory
    FILE *fp;
    char filename[100];
    char *correctName = ".crawler";
    sprintf(filename, "%s/%s",pageDir,correctName);
    if ((fp = fopen(filename, "r")) == NULL) {
        return false;
    }
    fclose(fp);
    return true;
    

}

/**************** page_load() ****************/
/*see pagedir.h for description */
webpage_t* page_load(const char *pageDir, const int ID)
{
  FILE *fp;
  char filename[100];
  sprintf(filename,"%s/%i", pageDir, ID);
  
  if ((fp = fopen(filename, "r")) == NULL){
      return NULL;
  }

  // first line should be the URL	
  char *url = freadlinep(fp);
  char *copyURL = malloc(strlen(url) +1);
  strcpy(copyURL,url);

  // second line is depth
  char *second_line = freadlinep(fp);

  int depth = atoi(second_line);
  //everthing else is HMTL
  char *html = freadfilep(fp);
  char *copy = malloc(strlen(html) +1);
  strcpy(copy,html);
  webpage_t *page = webpage_new(copyURL, depth, copy);
  
  //clean up
  count_free(url);
  count_free(second_line);
  count_free(html);
  fclose(fp);

 return page;
}	

