#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/word.h"
#include "../common/index.h"
#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"

/* index.c Caroline Tornquist 5/12/20
 * 
 * A program to read a directory produced by crawler, determine how many times each word appears
 * and in what files, and then print that data to the given file. 
 */

//local prototypes
void delete_counters(void *ctrs);

int main(int argc, char *argv[]){
    //have to do this before you try to acces them
    if (argc != 3){
        fprintf(stderr, "Needs two arguments.\n");
        return 1;
    }

    //PARAMETER VARS
    int len = strlen(argv[1]);
    char *filename = malloc(len+15);

    //PARAMETER CHECKING
    //if the directory doesn't end with a "/", add one
    strcpy(filename, argv[1]);                          //adding the directory
    if (strcmp(&filename[len-1], "/") != 0){            //if the last letter isn't a slash...
        strcat(filename, "/");                          //add one 
        len++;                                          //working length increases by 1
    }

    //checking if .crawler exists + is readable in that file
    strcat(filename, ".crawler");               //adding crawler
    FILE *fp1 = fopen(filename, "r");
    if (fp1 == NULL){
        fprintf(stderr, "Can't read from directory or not crawler directory\n");
        free(filename);
        return 1;
    }
    fclose(fp1);

    //checking if you can write into the given file
    FILE *fp2 = fopen(argv[2], "w");
    if (fp2 == NULL){
        fprintf(stderr, "Can't write into given file\n");
        free(filename);
        return 1;
    }
    
    //deleting the .crawler off the end of filename by adding end of string character
    filename[len+1] = '\0';

    //INDEXING VARIABLES
    hashtable_t *index = hashtable_new(350);     //hardcoding in 350 slots b/c we don't know the number of words


    //INDEXING
    index_build(filename, len, index);
    index_save(fp2, index);


    //cleaining up
    free(filename);
    fclose(fp2);
    hashtable_delete(index, &delete_counters);  

    return 0;
}

/* hashtable_delete() requires a function with a void* parameter, so this just takes care of that. 
 */
void delete_counters(void *ctrs){
    counters_t *count = ctrs;
    counters_delete(count);
}