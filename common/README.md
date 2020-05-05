# Lab 4 - Common Directory by Caroline Tornquist  
  
### Functions

```c 
/*
 * Determines if a directory exists by creating a file in that directory and 
 * trying to write to it. Returns true if the directory is valid, false otherwise. 
 * 
 * The directory must be in the form "./direc/" so make sure it's passed in that way. 
 * 
 * Limitations: the file ".crawler" will remain in the directory if the given
 * directory is valid. 
 */
bool check_directory(char *dir);

/* Uses the webpage module to get the HTML text from a given page. 
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

/* 
 * Loops through each url on the given page, checks if it is internal and normalizeable, inserts it into
 * the hashtable of urls seen, allocates a new webpage for it, and inserts it into the bag. 
 */
void page_scanner(webpage_t *page, bag_t *pages_to_crawl, hashtable_t *urls_seen);
```

### Implementation

This module relies heavily on ```webpage.c``` which was provided by the CS50 professors. ```page_fetcher``` and ```page_saver``` simply call functions from webpage,  adding formatting/editing of variables and checking for NULL pointers when appropriate. The function ```check_directory``` creates a new file inside the directory specified to determine if the directory exists and is writable. 
```page_scanner```loops through all the URLs on the given page, calling ```webpage_getNextURL``` until there are no more URLs. It then puts that URL into a webpage structure, and add the webpage to the bag of pages left to explore. 
    
