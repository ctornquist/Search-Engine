#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "word.h"

/* words.c Caroline Tornquist 5/11/2020
* A program to make a word all lowercase
*/

/* Makes the word all lowercase and returns it.
 * Returns NULL if the word is fewer than three characters. 
 * If any of the characters aren't letters, it removes them.
 * Allocates memory for a new string that must be free'd by the user later. 
 */
char *normalizeWord(char *word){
    /*if (strlen(word)<3) { //words fewer than three letters don't count as words
        return NULL;
    }*/

    char *lower = (char *)malloc(strlen(word)*sizeof(char)+1);
    char *blank = " ";
    strcpy(lower, blank);
    char new;

    int idx = 0;
    for (int i=0; i<strlen(word); i++){
        //printf("i: %d\n", i);
        //printf("idx: %d\n", idx);
        //printf("before: %s\n", lower);


        if(isalpha(word[i])) {
            new = tolower(word[i]);
            //printf("new: %c\n", new);
            lower[idx] = new;
            idx++;
        }
        
        //printf("after: %s\n", lower);
        //printf("\n");
    }
    lower[idx+1] = '\0';  //making sure the null character is there

    if (strcmp(lower, blank) == 0){ //if all of the characters are bad, return NULL
        return NULL;
    }


    return lower;
}