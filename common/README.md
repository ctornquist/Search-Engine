# Lab 4 - Common Directory by Caroline Tornquist  
  
### Functions

```c 
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

/* Returns the next URL from the page, given that it is not NULL.
 * Relies on webpage.c, so see that for more details. 
 */
char *page_scanner(webpage_t *page);
```

### Implementation

This module relies heavily on ```webpage.c``` which was provided by the CS50 professors. Each function apart from ```check_directory``` simply calls the similar function from webpage,  adding formatting/editing of variables and checking for NULL pointers.   
    It is supposed to be used by ``crawler.c`` to help with manipulating webpages. 
