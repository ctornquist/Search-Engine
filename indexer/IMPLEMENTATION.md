# Lab 5 Indexer - Caroline Tornquist
 
# Pseudocode
indexer:
1. Parse arguments, adding a slash to the end of the directory if needed, make sure there is a .crawler file that can be opened in it, and checking if the file given can be written to. I just appended .crawler to the end of the given filename, and then inserted the '\0' at the end to remove it after I'd checked the directory. 
2. Create a hashtable called "index" using hashtable_new
3. Call index_build
4. Call index_save
5. Free the index, close file pointers

index_build:
1. Add a '1' to the end of the filename variable
2. while there are files to be read in the directory
    * load a webpage struct from the data in the file
    * while webpage_getNextWord doesn't return NULL
        * normalize the URL
        * get the counters associated with the word using hashtable_find
            *  if the counters isn't null, add a new node using counters_add
            * if it is null, allocate a new counters, add a new node to it, and then put it and the word into the hashtable

index_save:
1. call hashtable_iterate on index, using printer_ht as the itemfunc

printer_ht:
1. print the word
2. call counters iterate on its counters struct using printer_ct as the item funct

printer_ct:
1. print the key and count pairs for each node of the counters

page_load:
1. read the first line, store in url elt of webpage struct
2. read the second line, convert to integer, store in depth elt of webpage struct
3. read the rest of the file, store in html elt of webpage struct 

normalizeURL:
1. allocate space for a new string
2. for each character in the given string
    * if it's an alphabetic character, call to lower, add it to the end of the string
    * otherwise, add a space in that spot
3. return the new string
 
# Data Structures
The main data structure is a hashtable (which itself is an array of sets). Each element of the hashtable has a key and an item, in this case the item is a pointer to a counters struct, which also has a series of key/item pairs. In the counters struct, each key is the integer docID and the count is the number of times a specific word appears in that document. The word is used as the key for the hashtable. 

Webpage struct is also used in this to store the url, depth and HTML data, and because webpage_getNextWord() allows us to easily parse the HTML text into readable words. 
 
# Errors
Indexer does significant error checking of the parameters. If any are not correct, it frees all of the memory allocated at that point and exits with non zero status. 