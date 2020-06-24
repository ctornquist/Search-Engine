/* counters.c Caroline Tornquist May 2020
 * 
 * Program to implement functionality specified in counters.h. Creates a data type that holds an integer
 * key and its count. 
 */

#include <stdio.h>
#include <stdbool.h>
#include "counters.h"
#include "memory.h"

typedef struct countersNode {
    int key; //integer key
    int count; //number of times the key appears
    struct countersNode *next;
} countersNode_t;

typedef struct counters {
  struct countersNode *head;	      //head of the list of items in bag
} counters_t;

//local function
static countersNode_t* countersNode_new(int key);

/* Create a new (empty) counter structure.
 *
 * We return:
 *   pointer to a new counterset; NULL if error. 
 * We guarantee:
 *   counterset is intialized empty.
 * Caller is responsible for:
 *   later calling counters_delete();
 */
counters_t *counters_new(void) {

    counters_t *counter = count_malloc(sizeof(counters_t));

    if (counter == NULL) {
        return NULL; //can't intialize counter
    }
    else {
        counter->head = NULL; //intializing counter structure
        return counter;
    }
}

/**************** counters_add ****************/
/* Increment the counter indicated by key.
 * 
 * Caller provides:
 *   valid pointer to counterset, and key(must be >= 0)
 * We return:
 *   the new value of the counter related to the indicated key.
 *   0 on error (if ctrs is NULL or key is negative)
 * We guarantee:
 *   counter values will always be >= 1.
 * We do:
 *  ignore if ctrs is NULL or key is negative.
 *  if the key does not yet exist, create a counter for it and initialize to 1.
 *  if the key does exist, increment its counter by 1.
 */
int counters_add(counters_t *ctrs, const int key) {
    if (key < 0) {
        fprintf(stderr, "Enter a key >=0\n");
        return 0;
    }

    if (ctrs == NULL){
        fprintf(stderr, "Pointer to counter is null\n");
        return 0;
    }

    int found = 0;
    for (countersNode_t *curr = ctrs->head; curr != NULL; curr = curr->next) {
        if (curr->key == key) {
            int ct = curr->count;
            curr->count = ct+1; //adding one to the count
            found = 1;
            return ct+1;
        }
    }

    if (found == 0) { //if the key did not exist
        countersNode_t *new = countersNode_new(key);
        if (new != NULL) {
            // add it to the head of the list
            new->next = ctrs->head;
            ctrs->head = new;
        }   
    }

    return 1;
}

/**************** counters_get ****************/
/* Return current value of counter associated with the given key.
 *
 * Caller provides:
 *   valid pointer to counterset, and key(must be >= 0)
 * We return:
 *   current value of counter associte with the given key, if present,
 *   0 if ctrs is NULL or if key is not found.   
 * Note:
 *   counterset is unchanged as a result of this call.
 */
int counters_get(counters_t *ctrs, const int key) {
    if (ctrs == NULL) {
        fprintf(stderr, "Null counter pointer\n");
        return 1;
    }

    if (key>=0){
        countersNode_t *curr = ctrs->head;
        while (curr != NULL) { //looping through the keys, returning count if match occurs
            if (curr-> key == key) {
                return curr->count;
            }
            curr = curr->next;
        }
    }

    return 0; //if key not found
}

/**************** counters_set ****************/
/* Set the current value of counter associated with the given key.
 *
 * Caller provides:
 *   valid pointer to counterset, 
 *   key(must be >= 0), 
 *   counter value(must be >= 0).
 * We return:
 *   false if ctrs is NULL, if key < 0 or count < 0, or if out of memory.
 *   otherwise returns true.
 * We do:
 *   If the key does not yet exist, create a counter for it and initialize to
 *   the given value. 
 *   If the key does exist, update its counter value to the given value.
 */
bool counters_set(counters_t *ctrs, const int key, const int count) {
    if (ctrs == NULL) {
        fprintf(stderr, "Null counter pointer\n");
        return false;
    }

    if (key < 0) {
        fprintf(stderr, "Key must be >=0 n");
        return false;
    }

    if (count < 0) {
        fprintf(stderr, "Count must be >=0 n");
        return false;
    }

    int found = 0;
    countersNode_t *curr = ctrs->head;
    while (curr != NULL) { //looping through the keys
        if (curr-> key == key) {
            curr->count = count;  //updating count if match occurs
            found = 1;
            return true;
        }
        curr = curr->next;
    }

    if (found == 0){ //if no match occured
        countersNode_t *new = countersNode_new(key);
        if (new != NULL) {
            //intialize values
            new->key = key;
            new->count = count;

            // add it to the head of the list
            new->next = ctrs->head;
            ctrs->head = new;
        } 
    }

    return true;
}

/**************** counters_print ****************/
/* Print all counters; provide the output file.
 *
 * Caller provides:
 *   valid pointer to counterset, 
 *   FILE open for writing.
 * We print:
 *   Nothing if NULL fp. 
 *   "(null)" if NULL ctrs.
 *   otherwise, comma=separated list of key=counter pairs, all in {brackets}.
 */
void counters_print(counters_t *ctrs, FILE *fp) {
    if (ctrs == NULL) {
        fprintf(stderr, "(null)\n");
    }
    else if (fp != NULL) {
        countersNode_t *curr = ctrs->head;
        while (curr != NULL) {
            fprintf(fp, "{%d, %d}\n", curr->key, curr->count);
            curr = curr->next;
        }
    }
}

/**************** counters_iterate ****************/
/* Iterate over all counters in the set.
 *
 * Caller provides:
 *   valid pointer to counterset, 
 *   arbitrary void*arg,
 *   valid pointer to itemfunc that can handle one item.
 * We do:
 *   nothing, if ctrs==NULL or itemfunc==NULL.
 *   otherwise, call itemfunc once for each item, with (arg, key, count).
 * Note:
 *   the order in which items are handled is undefined.
 *   the counterset is unchanged by this operation.
 */
void counters_iterate(counters_t *ctrs, void *arg, void (*itemfunc)(void *arg, const int key, const int count)){
    if (ctrs != NULL && itemfunc != NULL) {
        // call itemfunc with arg, on each item
        for (countersNode_t *curr = ctrs->head; curr != NULL; curr = curr->next) {
            (*itemfunc)(arg, curr->key, curr->count); 
        }
    }
}

/**************** counters_delete ****************/
/* Delete the whole counterset.
 *
 * Caller provides:
 *   a valid pointer to counterset.
 * We do:
 *   we ignore NULL ctrs.
 *   we free all memory we allocate for this counterset.
 */
void counters_delete(counters_t *ctrs) {
    if (ctrs != NULL) {

        countersNode_t *curr = ctrs->head;
        while (curr != NULL) {
            countersNode_t *next = curr->next;	    // remember what comes next
            count_free(curr);			            // free the node
            curr = next;			                // and move on to next
        }

        count_free(ctrs);                           //free the head
    }
}


/**************** countersNode_new ****************/
/* Allocate and initialize a countersNode */
// the 'static' modifier means this function is not visible 
// outside this file
static countersNode_t* countersNode_new(int key){
  countersNode_t *node = count_malloc(sizeof(countersNode_t));

  if (node == NULL) {
    // error allocating memory for node; return error
    return NULL;
  } else {
    node->key = key;
    node->count = 1;
    node->next = NULL;
    return node;
  }
}