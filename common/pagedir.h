#ifndef __PAGEDIR_H
#define __PAGEDIR_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include"../libcs50/webpage.h"
#include "../libcs50/bag.h"
#include "../libcs50/hashtable.h"


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
bool check_directory(char *dir);

/* Uses the webpage module to get the HTML texti from a given page. 
 * Checks if the webpage is NULL, and if not it returns a char * pointer
 * to the HTML text from the url of the webpage given. 
 */
char *page_fetcher(webpage_t *page);

/* Saves the data from the given webpage to "filename" in the following format:
 * The first line is the URL
 * The second line is the depth
 * The rest of the file is the HTML text of the given webpage
 * 
 * If the filename or the webpage are null, it prints to stderr. 
 */
void page_saver(webpage_t *page, char *filepath);

/* Returns the next URL from the page, given that it is not NULL.
 * Relies on webpage.c, so see that for more details. 
 */
void page_scanner(webpage_t *page, bag_t *pages_to_crawl, hashtable_t *urls_seen);

/* Given an open file pointer, it loads the information from that file into a webpage + returns it.
 * File is in format: 1st line is url, 2nd line is depth, rest is HTML content.
 * Rewinds the file pointer at the end so it's left at the beginning of the file.
 */
webpage_t *page_load(FILE* fp);



#endif //__PAGEDIR_H
