# Lab 5 Indexer - Caroline Tornquist

### Inputs and Outputs
Input: the only inputs are command-line parameters; see the README.md.

Output: We save each explored file in the crawler directory to the file given. The file has one line per word and one word per line. Each line is in the format "word docID count [docID count]", where the docID is the filename in which the word appeared, and the count is the number of times it appeared in that file. 

### Modules
We anticipate a few modules:

1. indexer, which parses arguments, initializes other modules, and calls index_build and index_save
2. index_build, which loops through the files in the directory and creates the index data structure
3. index_save, which takes the index structure and writes to the the file 

And a helper module:
1. hashtable of words, and their docIDs and counts

### Algorithmic Flow
The indexer will run as follows:

1. execute on the command line
2. parse the arguments, validate parameters, and initialize the index hashtable
3. while there are files left in the crawler directory
    * initialize a webpage struct with that file's data 
    * while there are words left in the HTML text
        * normalize the word
        * if it's already in the hashtable, update its counter with the appropriate docID and count
        * if it's not in the hashtable, create a new element and set its first counter node to the docID with a count of 1
4. iterate through each key in the hashtable
    * iterate through each node in the counter for that key, printing the docID and count to the file

    
Normalize the word means to convert it to lowercase (removing non alphabetic characters), and ignore words fewer than three letters. 


### Major Data Structures
Two modules are used here:  
1. a hashtable that has words as keys and items as counters structs, the keys of which are docIDs  
2. counters, used by above

### Testing Plan
Write a bash script to test the entire thing. These are the tests I will conduct:

1. test the program with the wrong number of arguments
2. test the program with a non-crawler directory
3. test with other incorrect command line args
4. test on various crawler directories given by cs50 staff, checking by hand to make sure the output is correct
5. using indextest, remake the index structure from a file and rewrite it, checking to make sure the new file is the same as the old one
