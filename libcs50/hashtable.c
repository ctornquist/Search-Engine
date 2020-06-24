/* hashtable.c Caroline Tornquist May 2020
 * 
 * Program to impelement hashtable.h. An array of sets (see set.c), each a collection of nodes 
 * with char keys and values defined by the user. Utilizes hashing function provided by Dartmouth faculty.
 */

#include <stdio.h>
#include <stdbool.h>
#include "set.h"
#include "jhash.h"
#include "memory.h"


/**************** global types ****************/
typedef struct hashtable {
    int num_slots;
    set_t **table;
}
hashtable_t;  // opaque to users of the module

/**************** functions ****************/

/**************** hashtable_new ****************/
/* Create a new (empty) hashtable.
 *
 * Caller provides:
 *   number of slots to be used for the hashtable (must be > 0).
 * We return:
 *   pointer to the new hashtable; return NULL if error.
 * We guarantee:
 *   hashtable is initialized empty.
 * Caller is responsible for:
 *   later calling hashtable_delete.
 */
hashtable_t *hashtable_new(const int num_slots){
    hashtable_t *ht = count_malloc(sizeof(hashtable_t));
    if (ht == NULL){
        return NULL;
    }

    ht->num_slots = num_slots;

    ht->table = (set_t **)count_calloc(num_slots, sizeof(set_t*));
    if(ht->table == NULL){
        return NULL;
    }
    for(int i = 0; i < num_slots; i++){
        ht->table[i] = NULL;
    }

    return ht;
}

/**************** hashtable_insert ****************/
/* Insert item, identified by key (string), into the given hashtable.
 *
 * Caller provides:
 *   valid pointer to hashtable, valid string for key, valid pointer for item.
 * We return:
 *   false if key exists in ht, any parameter is NULL, or error;
 *   true if new item was inserted.
 * Notes:
 *   The key string is copied for use by the hashtable; that is, the module
 *   is responsible for allocating memory for a copy of the key string, and
 *   later deallocating that memory; thus, the caller is free to re-use or 
 *   deallocate its key string after this call.  
 */
bool hashtable_insert(hashtable_t *ht, const char *key, void *item){
    if (ht == NULL || key == NULL || item == NULL){
        fprintf(stderr, "null pointer given\n");
        return false;
    }
    
    long index = JenkinsHash(key, ht->num_slots); //getting appropriate index using jhash function
    if (ht->table[index] == NULL){  //if there's no set in that slot, allocate a new one
        ht->table[index] = set_new();
    }

    bool is_inserted = set_insert(ht->table[index], key, item); //adding key/item pair to head of the list

    return is_inserted;
}

/**************** hashtable_find ****************/
/* Return the item associated with the given key.
 *
 * Caller provides:
 *   valid pointer to hashtable, valid string for key.
 * We return:
 *   pointer to the item corresponding to the given key, if found;
 *   NULL if hashtable is NULL, key is NULL, or key is not found.
 * Notes:
 *   the hashtable is unchanged by this operation.
 */
void *hashtable_find(hashtable_t *ht, const char *key){
    if (ht == NULL || key == NULL) {
        return NULL;
    }

    int index = JenkinsHash(key, ht->num_slots);
    if (ht->table[index] != NULL){
        return set_find(ht->table[index], key);       //returns the item stored at that location
    }

    return NULL;      //some error
}

/**************** hashtable_print ****************/
/* Print the whole table; provide the output file and func to print each item.
 * 
 * Caller provides:
 *   valid pointer to hashtable, 
 *   FILE open for writing,
 *   itemprint that can print a single (key, item) pair.
 * We print:
 *   nothing, if NULL fp.
 *   "(null)" if NULL ht.
 *   one line per hash slot, with no items, if NULL itemprint.
 *   otherwise, one line per hash slot, listing (key,item) pairs in that slot.
 * Note:
 *   the hashtable and its contents are not changed by this function,
 */
void hashtable_print(hashtable_t *ht, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item)){
    if (ht == NULL){
        fprintf(stderr, "pointer is null");
    }

    if(fp != NULL){
        for(int i = 0; i < ht->num_slots; i++){         //going over each set in the table
            printf("%d: ", i);
            if (ht->table[i] != NULL){                  //if there's something there to be printed
                set_print(ht->table[i], fp, itemprint);
            }
            else {
                printf("\n");
            }
        }
    }
}

/**************** hashtable_iterate ****************/
/* Iterate over all items in the table; in undefined order.
 *
 * Caller provides:
 *   valid pointer to hashtable, 
 *   arbitrary void*arg pointer,
 *   itemfunc that can handle a single (key, item) pair.
 * We do:
 *   nothing, if ht==NULL or itemfunc==NULL.
 *   otherwise, call the itemfunc once for each item, with (arg, key, item).
 * Notes:
 *   the order in which hashtable items are handled is undefined.
 *   the hashtable and its contents are not changed by this function,
 *   but the itemfunc may change the contents of the item.
 */
void hashtable_iterate(hashtable_t *ht, void *arg, void (*itemfunc)(void *arg, const char *key, void *item) ){
    if (ht != NULL && itemfunc != NULL){
        for(int i = 0; i < ht->num_slots; i++){
            if(ht->table[i] != NULL){
                set_iterate(ht->table[i], arg, itemfunc);
            }
        }
    }
}

/**************** hashtable_delete ****************/
/* Delete hashtable, calling a delete function on each item.
 *
 * Caller provides:
 *   valid hashtable pointer,
 *   valid pointer to function that handles one item (may be NULL).
 * We do:
 *   if hashtable==NULL, do nothing.
 *   otherwise, unless itemfunc==NULL, call the itemfunc on each item.
 *   free all the key strings, and the hashtable itself.
 * Notes:
 *   We free the strings that represent key for each item, because 
 *   this module allocated that memory in hashtable_insert.
 */
void hashtable_delete(hashtable_t *ht, void (*itemdelete)(void *item) ){
    if (ht != NULL){
        for(int i = 0; i < ht->num_slots; i++){
            set_delete(ht->table[i], itemdelete);      //freeing the set in each slot
        }

        free(ht->table);    //freeing table pointer
        free(ht);           //freeing hashtable itself
    }
}
