#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "word.h"

/* words.c Caroline Tornquist 5/11/2020
* A program to make a word all lowercase
*/

/* Makes the word all lowercase and returns it.
 * Returns NULL if the word is fewer than three characters. 
 * If any of the characters aren't letters, it replaces them with a space.
 * Allocates memory for a new string that must be free'd by the user later. 
 */
char *normalizeWord(char *word){
    if (strlen(word)<3) {
        return NULL;
    }

    char *lower = (char *)malloc(strlen(word)*sizeof(char)+1);
    strcpy(lower, word);
    char new;
    char *blank = " ";

    for (int i=0; i<strlen(word); i++){
        if(isalpha(lower[i])) {
            new = tolower(lower[i]);
            lower[i] = new;
        }
        else { //if the character is not in the alphabet, replace it with a space
            lower[i] = *blank;
        }
    }

    return lower;
}