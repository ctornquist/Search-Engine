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
} score_t;

typedef struct score_str{
    score_t **pages;
    int count;
} score_srt_t;


//local prototypes
bool parse_query(char *line, char *words[], int *words_size);
bool validate_query(char *words[], int words_size);
counters_t *copy_counter(counters_t *ctrs);
void copy_iterator(void *copy, const int key, const int count);
void and_seq (ct_ptrs_t *pointers);
void helper_and(void *arg, const int key, const int count);
void or_seq (counters_t *first, counters_t *second);
void helper_or(void *first, const int key, const int count);
int get_counters_size(int *size, counters_t *ctrs);
void size_helper(void *arg, const int key, const int count);
void print_scores(counters_t *res, char *filename, int len);
void insertion_sort(void *arg, const int key, const int count);
void free_words(char *words[], int words_size);
void free_docs(score_srt_t *docs, int num);
void delete_counters(void *ctrs);


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
    filename[len] = '\0';

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

    fprintf(stdout, "Query? ");
    while ((line = freadlinep(stdin)) != NULL){
        int line_len = strlen(line);           //the functional length will change when we add terminating characters
        char *words[line_len/2 +1];            //the max number of words is if the string alternates spaces and characters
        int words_size = 0;                    //to be updated by parse_query. 
        counters_t *res_and;                   //will get newed when we insert the first word
        counters_t *res_or = counters_new();   //need to new here
        ct_ptrs_t *pointers = malloc(sizeof(ct_ptrs_t));    //to pass multiple things into the iterator
        bool was_parsed;                        //was the query parsed correctly?
        bool is_valid;                          //was the query valid?
        bool is_empty = true;                    //is res_and empty

        was_parsed = parse_query(line, words, &words_size);
        if (was_parsed){
            is_valid = validate_query(words, words_size);

        if (is_valid){
            //printing the query
            fprintf(stdout, "Query: ");             
            for (int j = 0; j< words_size; j++){
                fprintf(stdout, "%s ", words[j]);
            }
            fprintf(stdout, "\n");


            for (int i = 0; i < words_size; i++){                    //looping over each word in the query
                if (is_empty){
                    res_and = copy_counter(hashtable_find(index, words[i]));
                    is_empty = false;
                }
                else if (strcmp(words[i], "or") != 0 && strcmp(words[i], "and") != 0){   //if it's not an 'or' or 'and' or the last word
                    pointers->first = res_and;
                    pointers->second = hashtable_find(index, words[i]);                        
                    and_seq(pointers);                              //keep updating and
                    is_empty = false;
                }
                else if (strcmp(words[i], "and") != 0){ //add everything to res_or on the last one
                    or_seq(res_or, res_and);                        //if it's an or, update the or
                    counters_delete(res_and);
                    is_empty = true;
                }
            }
            or_seq(res_or, res_and); //afterwards, add anything in and still into or

            //fprintf(stdout, "answer: "); counters_print(res_or, stdout); printf("\n");

            // res_or is now the final one with all the scores
            print_scores(res_or, filename, len);

            //clean up 
            counters_delete(res_and);

        }
        } 

        
        //cleaning up for new query
        free(line);
        free_words(words, words_size);
        free(pointers);
        counters_delete(res_or);
        fprintf(stdout, "-----------------------------------------\n");
        fprintf(stdout, "Query? ");
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


/*
 * Combines two counters sets used together in an and sequence. Updates "first" to have the minumum of each of 
 * the keys in both counters. If one key doesn't appear in both, it's count will be set to 0 in first. ct_ptrs_t 
 * is a struct * containing pointers to the two counters_t structs that will be combined. 
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
 * the keys in both counters. First is updated to contain the combination of the two. 
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


/*Gets the number of nodes with non-zero counts. Updates size to have the size. 
*/
int get_counters_size(int *size, counters_t *ctrs){
    counters_iterate(ctrs, size, &size_helper);
    return *size;
}


//helper for above
void size_helper(void *arg, const int key, const int count){
    int *size = arg;

    if (count != 0){
        (*size)++;
    }
}

/* Given the counters struct that has all the final docIDs and counts, create an array of score structs containing each
 * docID and score sorted in descending order. Print each docID, score and open the doc to read + print the url 
 * corresponding to that docID. 
 * 
 * Caller provides: the counters struct, a char* name of the directory and the length of that name. 
 */
void print_scores(counters_t *res, char *filename, int len){
    int num = 0;                                //number of docs with non-zero counts
    FILE *fp;                                   //for reading url from file
    char *val = malloc(8*sizeof(char));         //char version of docID
    int val_len = 0;                            //length of val (# of digits in docID)
    char *url;                                  //URL associated with each doc

    counters_iterate(res, &num, &size_helper);                  //getting the number of docs w/ non zero counts

    //initializing docs and inserting all of the score structs in descending order
    score_srt_t *docs = malloc(sizeof(score_srt_t));
    docs->pages = malloc(sizeof(score_t)*num);
    docs->count = 0;
    counters_iterate(res, docs, &insertion_sort);
    fprintf(stdout, "Matches %d documents (ranked)\n", docs->count);


    //printing scores and urls
    for (int i = 0; i<docs->count; i++){
        //updating the filename, have do it like this to deal with multi-digit docIDs
        sprintf(val, "%d", docs->pages[i]->docID);
        val_len = strlen(val);
        for (int k = 0; k < val_len; k++){
            filename[len+k] = val[k];            
        }
        filename[len+val_len] = '\0';
        

        //reading URL from first line of file, print docs and IDS
        fp = fopen(filename, "r");
        if (fp != NULL){
            url = freadlinep(fp);
            fprintf(stdout, "Score: %d Doc %d: ", docs->pages[i]->score, docs->pages[i]->docID);
            fprintf(stdout, "%s\n", url);
            
            //free for next time around
            free(url);
            fclose(fp);
        }

    }

    free_docs(docs, docs->count);   //free docs for next iteration
    filename[len+1] = '\0';         //remove docIDs for next iteration
    free(val);                      //clean up for next time
 
}

/* Used by counters_iterate on res_and to insert each item into an array of struct scores in descending order. 
 * Arg is a struct that contains an array of struct scores and a counter to index into the array. 
 * Uses insertion sort algorithm to get from the counters to the array. 
 */ 
void insertion_sort(void *arg, const int key, const int count ){
    if (count != 0){
        score_srt_t *docs = arg;                            //convert pointer
        score_t *new = malloc(sizeof(score_t));             //allocate for new element
        new->docID = key;                                   //update element
        new->score = count;
        bool flag = false;
        int i;
        //printf("this page first: %d\n", key);

        if (docs->count > 0){                               //if there's more than one item                   
            for (i = docs->count -1; i >= 0; i--){          //for each element in the array
                if (docs->pages[i]->score < count){         //if that element is smaller than the count..
                    //printf("moving %d to %d\n", i, i+1);
                    if (docs->pages[i+1] == NULL){          //create a new node if needed
                        //printf("allocating new one\n");
                        docs->pages[i+1] = malloc(sizeof(score_t));
                    }
                    docs->pages[i+1]->docID = docs->pages[i]->docID;        //move each element one forward
                    docs->pages[i+1]->score = docs->pages[i]->score;
                    //fprintf(stdout, "0 is: %d, %d\n", docs->pages[i]->docID, docs->pages[i]->score);
                    //fprintf(stdout, "1 is: %d, %d\n", docs->pages[i+1]->docID, docs->pages[i+1]->score);
                    flag = true;
                    //printf("changing thing around\n");
                }
                else {
                    break;
                }
            }
            if(flag){
                free(docs->pages[i+1]);                        //if items were moved, free what was in the last slot moved
                docs->pages[i+1] = new;                        //insert new item into that slot
                //printf("inserting in slot %d\n", i+1);
                //fprintf(stdout, "after 0 is: %d, %d\n", docs->pages[0]->docID, docs->pages[0]->score);
                //fprintf(stdout, "after 1 is: %d, %d\n", docs->pages[1]->docID, docs->pages[1]->score);
            } else {                                     //if nothing was moved (it belongs at the end), insert it there
                docs->pages[docs->count] = new;
            }
        } else {                                        //first time around, just insert it
            docs->pages[docs->count] = new;
        }
        docs->count = docs->count + 1;                  //increment counter
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

/* Loop over all the elements in doc's array, freeing each one, then free the struct itself
 */

void free_docs(score_srt_t *docs, int num){
    for(int i = 0; i < num; i++){
        free(docs->pages[i]);
    }
    free(docs->pages);
    free(docs);
}


/* hashtable_delete() requires a function with a void* parameter, so this just takes care of that. 
 */
void delete_counters(void *ctrs){
    counters_t *count = ctrs;
    counters_delete(count);
}