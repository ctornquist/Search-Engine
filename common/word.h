#ifndef __WORD_H
#define __WORD_H

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<stdbool.h>

/* word.h   Caroline Tornquist May 2020
 * 
 * Function definition for normalize word. 
 */

/* Makes the word all lowercase and returns it.
 * Returns NULL if the word is fewer than three characters. 
 * If any of the characters aren't letters, it removes them. 
 * Allocates memory for a new string that must be free'd by the user later. 
 */
char *normalizeWord(char *word, bool three_letter_allowed);

#endif //__WORD_H