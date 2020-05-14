#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../common/word.h"
#include "../common/index.h"
#include "../libcs50/webpage.h"
#include "../libcs50/hashtable.h"
#include "../libcs50/counters.h"

/* indextest.c Caroline Tornquist 5/12/20
 * A program to build an index structure from a file produced by indexer, and then write it back to a new file. 
 */

//local prototypes
void delete_counters(void *ctrs);

int main(int argc, char *argv[]){

    //PARAMETER CHECKING
    if (argc != 3){
        fprintf(stderr, "Not enough arguments.\n");
        return 1;
    }
    
    FILE *fp1 = fopen(argv[1], "r");
    if (fp1 == NULL){
        fprintf(stderr, "Can't open index file\n");
        return 1;
    }

    FILE *fp2 = fopen(argv[2], "w");
    if (fp2 == NULL){
        fprintf(stderr, "Can't write to file given\n");
        fclose(fp1);
        return 1;
    }

    //TESTING
    int num_words = lines_in_file(fp1);
    hashtable_t *index = hashtable_new(num_words);

    index_load(fp1, num_words, index);
    index_save(fp2, index);

    //cleaning up
    fclose(fp1);
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