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

bool pagesaver(webpage_t *page, char *pageDir, int ID);
webpage_t* page_load(const char *pageDir, const int ID);
bool page_validate(const char* pageDir);


/**************** pagec_load() ****************/
/*see pagedir.h for description */
bool  pagesaver(webpage_t *page, char *pageDir, int ID)
{
    if (page == NULL || pageDir == NULL || ID < 0){
            return false;
    }
    FILE *fp;
    char filename[100];
    sprintf(filename,"%s/%i", pageDir, ID);

    if ((fp = fopen(filename, "w")) != NULL) {
        // each webpage is saved with a unique id
        fprintf(fp, "%s\n", webpage_getURL(page)); // first line is the url
        fprintf(fp, "%d\n", webpage_getDepth(page)); // second line is the depth
        fprintf(fp, "%s", webpage_getHTML(page)); // rest is the content
	printf("created file : %i\n",ID);         // print for test file to make sure files are created
        fclose(fp);
    }
    else {
         return false;
    }
    return true;
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

