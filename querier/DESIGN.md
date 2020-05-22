# Lab 6 Querier - Caroline Tornquist

### Inputs and Outputs
Input: The only inputs are command-line parameters; see the README.md.

Output: Output is printed to stdin, and is the urls that best match the query ranked in descending order. 

### Modules
We anticipate a few modules:

1. querier, which parses arguments, initializes other modules, and calls index_build and index_save
2. parse_query and validate_query, which cut the query into indvidual words, convert them to lowercase, and ensure there
are not ands and ors in the wrong place. 
3. and_seq, which takes two counters and combines them following the rules of an and sequence
4. or_seq, which takes two counters and combines them following the rules of an or sequence
5. print_query, which sorts and displays the urls found

And a helper module:
1. words: an array of char *s, each index being a word in the query
2. res_and: a counters struct of the running and sequence
3. res_or: a counters struct of the running or sequence
4. docs: an array of score struct, where each score struct contains a docID and score element

### Algorithmic Flow
The querier will run as follows:

1. execute on the command line
2. parse the arguments, validate parameters, and initialize the array of words
3. intialize hashtable of indices, call index_load to load it from the file given
4. while the user continues to enter queries
    * parse the query, chopping it into individual words, and validate it, ensuring there are no stray "and"s or "or"s
    * for each word in the query
        * 
        * if res_and is empty, add that word's IDs and scores to res_and
        * if you don't read an or, add that word to res_and
        * if you read an or, add res_and to res_or, delete res_and
    * add res_and to res_or
5. determine the number of docIDs in res_or with non-zero counts
6. initialize an array of struct scores of that size
7. for each element in res_or
    * loop over all the prior elements in the array
        * if any have smaller counts that the current one, move them one index to the right
    * if any elements were moved, insert current one into array slot that was last moved
    * else, insert current one at the end of the array
8. for each element in the array
    * print its docID and count
    * open the file associated with that docID, read the URL (first line) and print
9. free structs appropriately

### Major Data Structures
Two modules are used here:  
1. a hashtable that has words as keys and items as counters structs, the keys of which are docIDs  
2. counters, used for res_and, res_or

### Testing Plan
Write a bash script to test the entire thing. These are the tests I will conduct:

1. test the program with the wrong number of arguments
2. test the program with a non-crawler directory
3. test with other incorrect command line args
4. test on various crawler directories given by cs50 staff, checking by hand to make sure the output is correct
5. use fuzzquery.c to generate lots of inputs, pipe to my program and check by hand to ensure the output is correct
