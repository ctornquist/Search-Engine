#ifndef __INDEX_H
#define __INDEX_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>
#include "../libcs50/file.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"
#include "../libcs50/webpage.h"
#include "word.h"
#include "pagedir.h"

/* This takes the name of a crawler directory path (that must end with a '/'), the length of that filename, 
 * and a hashtable pointer. 
 * Reading one file at a time from the directory, it reads each word in that file, adds it to the hashtable and puts
 * as the "item" a struct counters that has the filename where it appeared as the ID and the number of occurances as the count. 
 */
void index_build(char *filename, int len, hashtable_t *index);


/* Writes the index structure to the file given (file pointer must be open when it's passed in). 
 * Form is, on each line, "word docID count [docID count]" for every word that appears. 
 * Uses the iteration feature of the hashtable and counter modules. 
 */
void index_save(FILE *fp, hashtable_t *index);

/* Given a file created by indexer, with each line containing "word docId count [docID count]", load the data
 * into a the given hastable, where each key is a word and each item is a counters struct that contains each
 * file that word appears in and how many times it appears. 
 */
void index_load(FILE *fp, hashtable_t *index);


#endif //__INDEX_H
