#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "../common/word.h"
#include "../common/index.h"
#include "../libcs50/webpage.h"

/* querier.c Caroline Tornquist May 2020
 * A program to search a crawler directory and give urls that contain the words read from stdin. 
 */

//local structs
typedef struct ct_ptrs{         //to use both inside an interator
    counters_t *first;          
    counters_t *second;
} ct_ptrs_t;

typedef struct score{
    int docID;
    int score;
}score_t;


//local prototypes
void free_words(char *words[], int words_size);
bool parse_query(char *line, char *words[], int *words_size);
bool validate_query(char *words[], int words_size);
void and_seq (ct_ptrs_t *pointers);
void helper_and(void *arg, const int key, const int count);
void or_seq (counters_t *first, counters_t *second);
void helper_or(void *first, const int key, const int count);
counters_t *copy_counter(counters_t *ctrs);
void copy_iterator(void *copy, const int key, const int count);
void set_to_zero(counters_t *res_and);
void zero_helper(void *arg, const int key, const int count);
void delete_counters(void *ctrs);
void print_scores(counters_t *res);
void get_counters_num(void *arg, const int key, const int count);
void insertion_sort(void *arg, const int key, const int count);


int main(int argc, char *argv[]){
    //have to do this before you try to acces them
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
    }

    //QUERYING VARS
    hashtable_t *index = hashtable_new(350);
    char *line;

    //QUERYING
    index_load(fp2, index);

    while ((line = freadlinep(stdin)) != NULL){
        int line_len = strlen(line);           //the functional length will change when we add terminating characters
        char *words[line_len/2 +1];            //the max number of words is if the string alternates spaces and characters
        int words_size = 0;                    //to be updated by parse_query. 
        counters_t *res_and;                   //will get newed when we add first word
        counters_t *res_or = counters_new();   //need to new here
        ct_ptrs_t *pointers = malloc(sizeof(ct_ptrs_t));    //to pass multiple things into the iterator

        bool was_parsed = parse_query(line, words, &words_size);
        if (!was_parsed){
            break;
        }
        bool is_valid = validate_query(words, words_size);

        if (is_valid){
            //printing the query
            fprintf(stdout, "query: ");             
            for (int j = 0; j< words_size; j++){
                fprintf(stdout, "%s ", words[j]);
            }
            fprintf(stdout, "\n");


            res_and = copy_counter(hashtable_find(index, words[0])); //making a copy and storing
            fprintf(stdout, "first res_and: "); counters_print(res_and, stdout); fprintf(stdout, "\n");
            if (words_size == 1){ //to take care of single word queries
                res_or = res_and;
            }
            else {
                for (int i = 1; i < words_size; i++){                    //looping over each word in the query
                    if (strcmp(words[i], "or") != 0 && strcmp(words[i], "and") != 0){   //if it's not an 'or' or 'and' or the last word
                        pointers->first = res_and;
                        pointers->second = hashtable_find(index, words[i]);
                        and_seq(pointers);                              //keep updating and
                        fprintf(stdout, "%s res_and: ", words[i]); counters_print(res_and, stdout); fprintf(stdout, "\n");
                    }
                    else if (strcmp(words[i], "and") != 0){ //add everything to res_or on the last one
                        or_seq(res_or, res_and);                        //if it's an or, update the or
                        //set_to_zero(res_and);                           //set everying in and to 0
                        counters_delete(res_and);
                        res_and = counters_new();
                        fprintf(stdout, "%s res_or: ", words[i]); counters_print(res_or, stdout); fprintf(stdout, "\n");
                    }
                }
                or_seq(res_or, res_and); //afterwards, add anything in and still into or
            }

            fprintf(stdout, "answer: "); counters_print(res_or, stdout);
            printf("\n");

            print_scores(res_or);
 
            // res_or is now the final one with all the scores
            // do another iteration to get the # of items
            // one more to do an insertion sort
            // one more to print each score, docID and get the url from the first line of that doc

        }

        
        //cleaning up for new query
        free(line);
        free_words(words, words_size);
        free(pointers);
        counters_delete(res_and);
        if (words_size != 1){        //only gets newed if you have more than one word
            counters_delete(res_or);
        }
        printf("\n");
    }

    //cleaning up
    free(filename);
    hashtable_delete(index, &delete_counters);
    fclose(fp2);
}

/*
 * A function to break up the query into distinct words, remove non alphabet characters, and insert
 * the words into an array of char*'s. 
 * Returns false if there are bad characters in the string (non alphabet).
 */
bool parse_query(char *line, char *words[], int *words_size){
    int line_len = strlen(line);

    //replacing all the spaces with a null character
    for (char *cp = line; *cp != '\0'; cp++){
        if (!isalpha(*cp) && !isspace(*cp)){
            fprintf(stderr, "Only letters allowed. \n");
            return false; 
        }
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

    return true;
}

/*
 * Checks if the query starts or ends with an operator, or if it has two operators in a row. 
 * Returns whether the query is valid or not. 
 */

bool validate_query(char *words[], int words_size){
    if (words_size < 1){
        fprintf(stderr, "No allowed words in your query.\n");
        return false; 
    }

    //fprintf(stdout, "val and: %d\n", strcmp(words[0], "and"));
    //fprintf(stdout, "val or: %d\n", strcmp(words[0], "or"));

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


/*
 * Creates a copy of the passed in counters. User is responsible for freeing it later. 
 */
counters_t *copy_counter(counters_t *ctrs){
    counters_t *copy = counters_new();
    counters_iterate(ctrs, copy, &copy_iterator);
    return copy;
}

//helper function for above. 
void copy_iterator(void *copy, const int key, const int count){
    counters_t *copy_c = copy;
    counters_set(copy_c, key, count);
}

/* Sets every count in the counters to 0. 
 */
void set_to_zero(counters_t *res_and){
    counters_iterate(res_and, res_and, &zero_helper);
}

//helper for above
void zero_helper(void *arg, const int key, const int count){
    counters_t *res_and = arg;
    counters_set(res_and, key, 0);
}


/*
 * Combines two counters sets used together in an and sequence. Updates "first" to have the minumum of each of 
 * the keys in both counters. If one key doesn't appear in both, it's count will be set to 0. 
 */
void and_seq (ct_ptrs_t *pointers){
    counters_iterate(pointers->first, pointers, &helper_and);
}

/* Helper function to use with counters_iterate to get an and sequence. 
 */
void helper_and(void *arg, const int key, const int count){
    ct_ptrs_t *pointers = arg;
    int second_count;

    if ((second_count = counters_get(pointers->second, key)) != 0){ //if they both have the key
        //if it's larger in first, set first to second's
        //if it's smaller in first, leave as is
        if ((count - second_count) > 0){
            counters_set(pointers->first, key, second_count);
        }
    }
    else {                                             //if 2nd doesn't have the key, set to 0 in the first
        counters_set(pointers->first, key, 0);
    }
    
}

/*
 * Combines two counters sets used together in an or sequence. Updates "first" to have the sum of each of 
 * the keys in both counters. 
 */
void or_seq (counters_t *first, counters_t *second){
    if (first != NULL && second != NULL){
        counters_iterate(second, first, &helper_or);
    }
}

/* Helper function to use with counters_iterate to get an or sequence. 
 */
void helper_or(void *first, const int key, const int count){
    counters_t *first_c = first;
    int first_count; 
 
    if ((first_count = counters_get(first_c, key)) != 0){ //if they both have the key
        counters_set(first_c, key, count+first_count);    //put the sum into the first counter set
    }
    else { //if first doesn't have the key, insert it with second's count
        counters_set(first_c, key, count);
    }
}

void print_scores(counters_t *res){
    int num = 0;
    printf("print scores\n");
    counters_iterate(res, &num, &get_counters_num);
    printf("num is: %d\n", num);

    counters_iterate(res, pages, &insertion_sort);

}

//gets the number of nodes with non-zero counts
void get_counters_num(void *arg, const int key, const int count){
    int *size = arg;

    if (count != 0){
        (*size)++;
    }
}

void insertion_sort(void *arg, const int key, const int count ){
    if (count != 0){
        score_t *pages = arg;
        score_t item = {key, count};

        

    } 
} 
 

/*
 * Loops over all the char*'s in words and free's each of them. 
 */
void free_words(char *words[], int words_size){
    if (words_size != 0){
        for (int i = 0; i< words_size; i++){
            free(words[i]);
        }
    }
}

/* hashtable_delete() requires a function with a void* parameter, so this just takes care of that. 
 */
void delete_counters(void *ctrs){
    counters_t *count = ctrs;
    counters_delete(count);
}