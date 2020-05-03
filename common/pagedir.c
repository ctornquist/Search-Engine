#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "pagedir.h"
#include "../libcs50/webpage.h"

/*
 * Determines if a directory exists by creating a file in that directory and 
 * trying to write to it. Returns true if the directory is valid, false otherwise. 
 * 
 * The directory can be in the form "./dir" or "./dir/". 
 * The function will modify the former to add the final slash onto the end. 
 * 
 * Limitations: the file ".crawler" will remain in the directory if the given
 * directory is valid. 
 */
bool check_directory(char *dir){
    int len = strlen(dir);

    
    //if it doesn't end with a "/", add one
    if (strcmp(&dir[len], "/") != 0){
        dir = realloc(dir, sizeof(dir)+3*sizeof(char));
        strcat(dir, "/");   
    }

    //making the filename string to be "directory/.crawler"
    char filename[len+15];
    strcpy(filename, dir);
    strcat(filename, ".crawler");

    //if the file is writable (and the directory exists), return true 
    FILE *fp;
    fp = fopen(filename, "w");
    if (fp != NULL){
        fclose(fp);
        return true;
    }

    return false; //if you can't access the directory
}

/* Uses the webpage module to get the HTML texti from a given page. 
 * Checks if the webpage is NULL, and if not it returns a char * pointer
 * to the HTML text from the url of the webpage given. 
 */

char *page_fetcher(webpage_t *page){
    if (page == NULL){
        return NULL;
    }

    webpage_fetch(page);
    return webpage_getHTML(page);
}

/* Saves the data from the given webpage to "filename" in the following format:
 * The first line is the URL
 * The second line is the depth
 * The rest of the file is the HTML text of the given webpage
 * 
 * If the filename or the webpage are null, it prints to stderr. 
 */
void page_saver(webpage_t *page, char *filepath){
    if (page == NULL || filepath == NULL){
        fprintf(stderr, "Null page or filename");
    }
    else {  
        FILE *fp = fopen(filepath, "w");
        if (fp != NULL){
            fprintf(fp, "%s\n", webpage_getURL(page));
            fprintf(fp, "%d\n", webpage_getDepth(page));
            fprintf(fp, "%s\n", webpage_getHTML(page));
        }

    }
}


/* Returns the next URL from the page, given that it is not NULL.
 * Relies on webpage.c, so see that for more details. 
 */
char *page_scanner(webpage_t *page){
    if (page != NULL){
        int pos = 0;
        return webpage_getNextURL(page, &pos);
    }
    else{
        return NULL;
    }

}
