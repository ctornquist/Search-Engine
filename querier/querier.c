#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../common/word.h"
#include "../common/index.h"
#include "../libcs50/webpage.h"

/* querier.c Caroline Tornquist May 2020
 * A program to search a crawler directory and give urls that contain the words read from stdin. 
 */

//local prototypes
void free_words(char *words[], int words_size);
void parse_query(char *line, char *words[], int *words_size);
bool validate_query(char *words[], int words_size);


int main(int argc, char *argv[]){
    /*//have to do this before you try to acces them
    if (argc != 3){
        fprintf(stderr, "usage: \"./querier pageDir indexFilename\" \n");
        return 1;
    }

    //PARAMETER CHECK VARS
    int len = strlen(argv[1]);
    char *filename = malloc(len+15);

    //PARAMETER CHECKING
    //if the directory doesn't end with a "/", add one
    strcpy(filename, argv[1]);                          //adding the directory
    if (strcmp(&filename[len-1], "/") != 0){            //if the last letter isn't a slash...
        strcat(filename, "/");                          //add one 
        len++;                                          //working length increases by 1
    }

    //checking if .crawler exists + is readable in that directory
    strcat(filename, ".crawler");               //adding crawler
    FILE *fp1 = fopen(filename, "r");
    if (fp1 == NULL){
        fprintf(stderr, "Can't read from directory or not crawler directory\n");
        free(filename);
        return 1;
    }
    fclose(fp1);

    //deleting the .crawler off the end of filename by adding end of string character
    filename[len+1] = '\0';

    //checking if you can read into the index file
    FILE *fp2 = fopen(argv[2], "r");
    if (fp2 == NULL){
        fprintf(stderr, "Can't read index file\n");
        free(filename);
        return 1;
    }*/

    //QUERYING VARS
    hashtable_t *index = hashtable_new(350);
    char *line;
    //char *blank = ' ';


    //QUERYING
    index_load(fp2, index);

    while ((line = freadlinep(stdin)) != NULL){
        int line_len = strlen(line);           //the functional length will change when we add terminating characters
        char *words[line_len/2 +1];            //the max number of words is if the string alternates spaces and characters
        int words_size = 0;                    //to be updated by parse_query. 
        counters_t *res_and;
        counters_t *res_or;

        parse_query(line, words, &words_size);
        bool is_valid = validate_query(words, words_size);

        if (is_valid){
            //printing the query
            fprintf(stdout, "query: ");             
            for (int j = 0; j< words_size; j++){
                fprintf(stdout, "%s ", words[j]);
            }
            fprintf(stdout, "\n");


            
        }

        

        
        //cleaning up
        free(line);
        free_words(words, words_size);
        counters_delete(res_and);
        counters_delte(res_or);
    }



    //cleaning up
    //free(filename);
}

void parse_query(char *line, char *words[], int *words_size){
    int line_len = strlen(line);

    //replacing all the spaces with a null character
    for (char *cp = line; *cp != '\0'; cp++){
        if (*cp == ' '){                    //if there's a space, add a terminating character               
        *cp = '\0';
        }
    }

    int word_len = 0;                      //length of current word
    char *word = line;                      //set to the first word
    for (int k =0; k < line_len; k++){      //for each character in the line
        word_len = strlen(word);
        char *new = normalizeWord(word);    //convert to lowercase
        if (new != NULL){                   //add to array
            words[*words_size] = new;
            (*words_size)++;
        }

        word += word_len +1;                //move the pointer to the next word (account for '\0')
        k += word_len;                      //move k to the next word
    }

}

bool validate_query(char *words[], int words_size){
    if (words_size < 1){
        fprintf(stderr, "No allowed words in your query.\n");
    }

    fprintf(stdout, "val and: %d\n", strcmp(words[0], "and"));
    fprintf(stdout, "val or: %d\n", strcmp(words[0], "or"));

    //can't start or end with operator
    if (strcmp(words[0], "and") == 0 || strcmp(words[0], "or") == 0 ){
        fprintf(stderr, "Queries can't start with 'and' or 'or'. \n");
        return false;
    }

    if (strcmp(words[words_size-1], "and") == 0 || strcmp(words[words_size-1], "or") == 0){
        fprintf(stderr, "Queries can't end with 'and' or 'or'. \n");
        return false;
    }

    //can't have two operators in a row
    for(int i = 0; i < words_size -1; i++){
        if ( strcmp(words[i], "and") == 0 || strcmp(words[i], "or") == 0){
            if ( strcmp(words[i+1], "and") == 0 || strcmp(words[i+1], "or") == 0 ){
                fprintf(stdout, "Can't have two operators in a row.\n");
                return false; 
            }
        }
    }

    return true; //if no errors
}


void and_seq (counters_t *first, counters_t *second){
    if (first != NULL && second != NULL){
        counters_iterate(second, second, &helper_and);
    }
}

void helper_and(void *first, const int key, const int count){
    counters_t *first = first;
    int first_count;

    if ((first_count = counters_get(first, key)) != 0){ //if they both have the key
        //if it's larger in first, set first to second's
        //if it's smaller in first, leave as is
        if ((first_count - count) > 0){
            counters_set(first, key, count);
        }
    }
}

void or_seq (counters_t *first, counters_t *second){
    if (first != NULL && second != NULL){
        counters_iterate(second, second, &helper_or;
    }
}

void helper_or(void *first, const int key, const int count){
    counters_t *first = first;
    int first_count;

    if ((first_count = counters_get(first, key)) != 0){ //if they both have the key
        counters_set(first, key, count+first_count);    //put the sum into the first counter set
    }
}

void free_words(char *words[], int words_size){
    if (words_size != 0){
        for (int i = 0; i< words_size; i++){
            free(words[i]);
        }
    }
}