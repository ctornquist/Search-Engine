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

/* index.c Caroline Tornquist 5/12/20
 * A program to be used by indexer.c to help building the index strucutre from webpages, printing it to a file, and
 * building it again from a previous file. 
 */

//local function prototypes
void printer_ht(void *fp, const char *key, void *ctrs);
void printer_ct(void *fp, int key, int count);


/* This takes the name of a crawler directory path (that must end with a '/'), the length of that filename, 
 * and a hashtable pointer. 
 * Reading one file at a time from the directory, it reads each word in that file, adds it to the hashtable and puts
 * as the "item" a struct counters that has the filename where it appeared as the ID and the number of occurances as 
 * the count. 
 */
void index_build(char *filename, int len, hashtable_t *index){
    int j = 1;                                  //this will be the docID
    char *val = malloc(8*sizeof(char));         //char version of docID
    int val_len = 0;                            //length of val (# of digits in docID)
    FILE *fptr;
    char *word;
    char *norm_word;
    counters_t *count;

    //adding 1 to the end of filename
    sprintf(val, "%d", j);
    filename[len] = *val;


    while ( (fptr = fopen(filename, "r")) != NULL ) {       //while there are files left to read

        webpage_t *page = page_load(fptr);                   //load with appropriate URL, depth, HTML from file
        
        int pos = 0;
        while ((word = webpage_getNextWord(page, &pos)) != NULL) {       //reading each word from the HTML
            norm_word = normalizeWord(word);                             //making all lowercase

            if (norm_word != NULL){
                if ( (count = hashtable_find(index, norm_word)) != NULL ){   //if that word already exists
                    counters_add(count, j);                             //j is the docID. will create new node if none exists.
                }
                else{                                                   //if the word isn't there yet
                    //make new counter + add first node
                    counters_t *new = counters_new();
                    counters_add(new, j);

                    //put into index structure
                    hashtable_insert(index, norm_word, new);

                }
            }
            free(word);         //malloc'd memory not put into table
            free(norm_word);
        }

        //updating the filename. have to do it like this to manage docIDs with multiple digits. 
        j++;
        sprintf(val, "%d", j);
        val_len = strlen(val);
        for (int k = 0; k < val_len; k++){
            filename[len+k] = val[k];
        }
        filename[len+val_len] = '\0';

        //cleaning up
        fclose(fptr);
        webpage_delete(page);
    }

    //cleaning up
    free(val);
}

/* Writes the index structure to the file given (file pointer must be open when it's passed in). 
 * Form is, on each line, "word docID count [docID count]" for every word that appears. 
 * Uses the iteration feature of the hashtable and counter modules. 
 */
void index_save(FILE *fp, hashtable_t *index){
    hashtable_iterate(index, fp, &printer_ht);
}

/*
 * Helper function to assist index_save with printing. Prints to the file give the word, and new line and calls 
 * counters_iterate on the counter item to print the docIDs and counts. 
 */
void printer_ht(void *fp, const char *key, void *ctrs){
    counters_t *ct = ctrs;
    FILE *fo = fp;

    fprintf(fo, "%s ", key);
    counters_iterate(ct, fp, &printer_ct);
    fprintf(fo, "\n");
}

/* Helper function for index_save. Prints the docID and count for that node. 
 */
void printer_ct(void *fp, int key, int count){
    FILE *fo = fp;
    fprintf(fo, "%d %d ", key, count);
}


/* Given a file created by indexer, with each line containing "word docId count [docID count]", load the data
 * into a the given hastable, where each key is a word and each item is a counters struct that contains each
 * file that word appears in and how many times it appears. 
 */
void index_load(FILE *fp, hashtable_t *index){
    char *word;
    int id, count;
    counters_t *counters;
    bool is_first = true;
    int num_words = lines_in_file(fp);

    for(int i=0; i<num_words; i++){                             //for each line
        word = freadwordp(fp);                                  //allocates memory that we have to free
        while ( (fscanf(fp, "%d %d", &id, &count)) == 2 ){      //will return 0 at the new line
            if (is_first == true){                              //first time around
                //make new counter + add first node
                counters_t *new = counters_new();
                counters_set(new, id, count);

                //put into index structure
                hashtable_insert(index, word, new);
                
                is_first = false;
            } else {                                            //next time, we know the word exists
                counters = hashtable_find(index, word);
                if (counters != NULL) {
                    counters_set(counters, id, count);
                }
            }
        }   
        is_first = true;
        free(word);
    }
}

