#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "pagedir.h"
#include "../libcs50/webpage.h"
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/file.h"

/*
 * pageDir.c    Caroline Tornquist      May 6, 2020
 * 
 * pageDir contains the definitions for functions to be used by crawler.c to maniuplate webpages. It relies 
 * heavily on webpage.c. 
 * 
 * check_directory takes a char* and determines if it's a writable directory
 * page_fetcher gets the html from a url and stores it in the html element of a struct webpage
 * page_saver writes the url, depth and html data to a file in the given directory
 * page_scanner scans the given page for urls and adds them to a bag (see bag.c)
 */


/*
 * Determines if a directory exists by creating a file in that directory and 
 * trying to write to it. Returns true if the directory is valid, false otherwise. 
 * 
 * The directory must be in the form "./direc/" so make sure it's passed in that way. 
 * 
 * Limitations: the file ".crawler" will remain in the directory if the given
 * directory is valid. 
 */
bool check_directory(char *dir){
    int len = strlen(dir);

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
    if (page == NULL || filepath == NULL || webpage_getHTML(page) == NULL || webpage_getURL(page) == NULL){
        fprintf(stderr, "Null page or filename\n");
    }
    else {  
        FILE *fp = fopen(filepath, "w");
        if (fp != NULL){
            fprintf(fp, "%s\n", webpage_getURL(page));
            fprintf(fp, "%d\n", webpage_getDepth(page));
            fprintf(fp, "%s", webpage_getHTML(page));
        }
        fclose(fp);
    }
}


/* 
 * Loops through each url on the given page, checks if it is internal and normalizeable, inserts it into
 * the hashtable of urls seen, allocates a new webpage for it, and inserts it into the bag. 
 */
void page_scanner(webpage_t *page, bag_t *pages_to_crawl, hashtable_t *urls_seen){
    if (page != NULL){ 
        int pos = 0; 
        char *next_url;
        int depth;

        while ((next_url = webpage_getNextURL(page, &pos)) != NULL){ //while there are links left on this page 
            char *blank = " ";
            bool added = false;
            
            NormalizeURL(next_url);
            if (IsInternalURL(next_url)){   //if the url is internal to the system
            added = hashtable_insert(urls_seen, next_url, blank);
                if (added){
                    depth = webpage_getDepth(page) +1;
                    webpage_t *new = webpage_new(next_url, depth, NULL);
                    bag_insert(pages_to_crawl, new);
                }
                else{
                    free(next_url); //if it wasn't added, won't get free'd when we free the bag later
                }
            }
            else{
                free(next_url);     //if it wasn't internal, won't get free'd when we free the bag later
            }
        }
    } 
} 


/* Given an open file pointer, it loads the information from that file into a webpage + returns it.
 * File is in format: 1st line is url, 2nd line is depth, rest is HTML content.
 * Rewinds the file pointer at the end so it's left at the beginning of the file.
 */
webpage_t *page_load(FILE* fp){
    char *URL;
    char *depth;
    char *HTML;
    int dep;

    URL = freadlinep(fp);               //this memory is freed when we free the webpage later
    depth = freadlinep(fp);             //have to free this memory when we're done
    HTML = freadfilep(fp);               //this memory is freed when we free the webpage later
    dep = atoi(depth);                  //converting to integer
    free(depth);

    //loading webpage + HTML text
    webpage_t *page = webpage_new(URL, dep, HTML);

    rewind(fp);
    return page;
}