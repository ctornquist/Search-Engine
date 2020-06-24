/* set.c Caroline Tornquist May 2020
 * 
 * Program to implement set.h, a collection of elements with {key, value} pairs where the keys are
 * chars and the values are types defined by the user. 
 */

#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include "set.h"
#include "memory.h"

/**************** global types ****************/
typedef struct setnode {
  char *key;              //string to represent the key
  void *item;		          // pointer to data for this item
  struct setnode *next;	      // link to next node
} setnode_t;

typedef struct set {
  struct setnode *head;	      // head of the list of items in set
} set_t;


//local prototypes. these are not visible outside of the module
static setnode_t *setnode_new(char *key, void *item);


/**************** functions ****************/

/**************** set_new ****************/
/* Create a new (empty) set.
 * 
 * We return:
 *   pointer to a new set, or NULL if error.
 * We guarantee:
 *   The set is initialized empty.
 * Caller is responsible for:
 *   later calling set_delete.
 */
set_t *set_new(void) {
  set_t *set = count_malloc(sizeof(set_t));

  if (set == NULL) {
    return NULL; // error allocating set
  } else {
    // initialize contents of set structure
    set->head = NULL;
    return set;
  }
}

/**************** set_insert ****************/
/* Insert item, identified by a key (string), into the given set.
 *
 * Caller provides:
 *   valid set pointer, valid string pointer, and pointer to item.
 * We return:
 *  false if key exists, any parameter is NULL, or error;
 *  true iff new item was inserted.
 * Caller is responsible for:
 *   later calling set_delete to free the memory used by key strings.
 * Notes:
 *   The key string is copied for use by the set; that is, the module
 *   is responsible for allocating memory for a copy of the key string, and
 *   later deallocating that memory; thus, the caller is free to re-use or 
 *   deallocate its key string after this call.  
 */
bool set_insert(set_t *set, const char *key, void *item){
    if (set == NULL || key == NULL || item == NULL) {
      fprintf(stderr, "Null pointer given\n");
      return false;
    } 

    //checking if the key already exists
    setnode_t *curr = set->head;
    while (curr != NULL) {
      if (strcmp(curr->key, key) == 0) {
        return false;
      }
      curr = curr->next;
    }
    
    //make a copy of the key
    char *copy_key = count_malloc((strlen(key)+1)*sizeof(char));
    strcpy(copy_key, key);

    setnode_t *new = setnode_new(copy_key, item); //allocate space for a new node
    
    if (new != NULL) {
      // add it to the head of the list
      new->next = set->head;
      set->head = new;       
      return true;
    }

    return false; //if any error
}

/**************** set_find ****************/
/* Return the item associated with the given key.
 *
 * Caller provides:
 *   valid set pointer, valid string pointer.
 * We return:
 *   a pointer to the desired item, if found; 
 *   NULL if set is NULL, key is NULL, or key is not found.
 * Notes:
 *   The item is *not* removed from the set.
 *   Thus, the caller should *not* free the pointer that is returned.
 */
void *set_find(set_t *set, const char *key){
    if (set == NULL) {
      fprintf(stderr, "set is null\n");
      return NULL;
    }

    if (key == NULL) {
      fprintf(stderr, "key is null\n");
      return NULL;
    }
    
    setnode_t *curr = set->head;
    while (curr != NULL) {
      if (strcmp(curr->key, key) == 0) {
        return curr->item;    //returning the item that matched
      }
      curr = curr->next;
    }

    return NULL; //if key not found
}

/**************** set_print ****************/
/* Print the whole set; provide the output file and func to print each item.
 *
 * Caller provides:
 *   valid set pointer,
 *   FILE open for writing,
 *   valid pointer to function that prints one item.
 * We print:
 *   nothing if NULL fp. Print (null) if NULL set.
 *   print a set with no items if NULL itemprint. 
 *  otherwise, 
 *   print a comma-separated list of items surrounded by {brackets}.
 * Notes:
 *   The set and its contents are not changed.
 *   The 'itemprint' function is responsible for printing (key,item).
 */
void set_print(set_t *set, FILE *fp, void (*itemprint)(FILE *fp, const char *key, void *item) ){
    if (set == NULL) {
        fprintf(stderr, "(null)\n");
    }

    if (fp != NULL){
        fputc('{', fp);
        setnode_t *curr = set->head;
        while (curr != NULL) {
            fputc('{', fp);
            // print this node
            if (itemprint != NULL) {  // print the node's item 
                (*itemprint)(fp, curr-> key, curr->item); 
            }
            curr = curr->next; //update curr
            fputs("} ", fp);
        }
        fputs("}\n", fp);
    }
}

/**************** set_iterate ****************/
/* Iterate over the set, calling a function on each item.
 * 
 * Caller provides:
 *   valid set pointer,
 *   arbitrary argument (pointer) that is passed-through to itemfunc,
 *   valid pointer to function that handles one item.
 * We do:
 *   nothing, if set==NULL or itemfunc==NULL.
 *   otherwise, call the itemfunc on each item, with (arg, key, item).
 * Notes:
 *   the order in which set items are handled is undefined.
 *   the set and its contents are not changed by this function,
 *   but the itemfunc may change the contents of the item.
 */
void set_iterate(set_t *set, void *arg, void (*itemfunc)(void *arg, const char *key, void *item)){
  if (set != NULL && itemfunc != NULL) {
    // call itemfunc with arg, on each item
    setnode_t *curr = set->head;
    while(curr != NULL) {
      (*itemfunc)(arg, curr->key, curr->item); 
      curr = curr->next;
    }
  }
}

/**************** set_delete ****************/
/* Delete set, calling a delete function on each item.
 *
 * Caller provides:
 *   valid set pointer,
 *   valid pointer to function that handles one item (may be NULL).
 * We do:
 *   if set==NULL, do nothing.
 *   otherwise, unless itemfunc==NULL, call the itemfunc on each item.
 *   free all the key strings, and the set itself.
 * Notes:
 *   We free the strings that represent key for each item, because 
 *   this module allocated that memory in set_insert.
 */
void set_delete(set_t *set, void (*itemdelete)(void *item) ){

  if (set != NULL) {
    setnode_t *curr = set->head;
    while (curr != NULL) {
      if (itemdelete != NULL) {		    // if possible...
        (*itemdelete)(curr->item);	    // delete node's item
      }
      free(curr->key);                //freeing the string that is the key
      setnode_t *next = curr->next;	    // remember what comes next
      count_free(curr);			            // free the node
      curr = next;			              // and move on to next
    }

    count_free(set);
  }
}


/**************** setnode_new ****************/
/* Allocate and initialize a setnode
 * The 'static' modifier means this function is not visible outside this file */
//Mostly taken from bag.c
static setnode_t* setnode_new(char * key, void *item)
{
  setnode_t *node = count_malloc(sizeof(setnode_t));

  if (node == NULL) {
    // error allocating memory for node; return error
    return NULL;
  } else {
    //initializing values
    node->key = key;
    node->item = item;
    node->next = NULL;
    return node;
  }
}