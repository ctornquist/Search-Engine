# Lab 6 Caroline Tornquist

### Usage 
Querier is used to get parse queries, search the given indexfile for those words and print (in ranked order) the urls of pages that contain those words. It supports "and seqences" where the words must both show up in the same document, and "or sequences" where either word may appear in any document. Crawler directory should have been produced by crawler, and indexFile should be its acccompanying file created by indexer. 
I implemented the full requirements spec, which supports and and or operations and sorts the pages in decreasing order.

command line: ./querier crawlerDirectory indexFile

### Assumptions
Querier checks its arguments, ensuring that the directory was produced by crawler and the indexFile is readable. However, it assumes that the directory has files 1, 2, 3... without gaps, and that the file given was actually created by indexer. 

### Compilation
To compile querier, simply "make". To remove all the executables and test files, "make clean". "make test" will run a bash script with a number of test cases, the output of which is in testing.out.  

Note: the archives "common.a" and "libcs50.a" are needed to compile this directory. If they don't exist, run "make" in both the common and libcs50 directories to create them. 

### Major Functions
 ```c
 /*
 * A function to break up the query into distinct words, remove non alphabet characters, and insert
 * the words into an array of char*'s. 
 * Returns false if there are bad characters in the string (non alphabet).
 */
bool parse_query(char *line, char *words[], int *words_size);

/*
 * Checks if the query starts or ends with an operator, or if it has two operators in a row. 
 * Returns whether the query is valid or not. 
 */

bool validate_query(char *words[], int words_size);

/*
 * Creates a copy of the passed in counters. User is responsible for freeing it later. 
 */
counters_t *copy_counter(counters_t *ctrs);

/*
 * Combines two counters sets used together in an and sequence. Updates "first" to have the minumum of each of 
 * the keys in both counters. If one key doesn't appear in both, it's count will be set to 0 in first. ct_ptrs_t is a struct * containing pointers to the two counters_t structs that will be combined. 
 */
void and_seq (ct_ptrs_t *pointers);

/*
 * Combines two counters sets used together in an or sequence. Updates "first" to have the sum of each of 
 * the keys in both counters. First is updated to contain the combination of the two. 
 */
void or_seq (counters_t *first, counters_t *second);

/*Gets the number of nodes with non-zero counts. Updates size to be the size. 
*/
int get_counters_size(int *size, counters_t *ctrs);

/* Given the counters struct that has all the final docIDs and counts, create an array of score structs containing each
 * docID and score sorted in descending order. Print each docID, score and open the doc to read + print the url 
 * corresponding to that docID. 
 * 
 * Caller provides: the counters struct, a char* name of the directory and the length of that name. 
 */
void print_scores(counters_t *res, char *filename, int len);

/* Used by counters_iterate on res_and to insert each item into an array of struct scores in descending order. 
 * Arg is a struct that contains an array of struct scores and a counter to index into the array. 
 * Uses insertion sort algorithm to get from the counters to the array. 
 */ 
void insertion_sort(void *arg, const int key, const int count );

//additional helper functions to assist with iterting
//additional functions to free the various structs
```