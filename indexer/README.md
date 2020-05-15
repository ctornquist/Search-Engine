# Lab 5 Indexer - Caroline Tornquist

### Usage 
Indexer is used to get the number of times a word appears in each file in a crawler directory, so that eventually we will be able to search those words. Indextest reads a file produced by indexer, recreates its file structure, and then saves it to a new file. 

command line: ./indexer crawlerDirectory fileToSaveTo
command line: ./indextest oldIndexFile newIndexFile

### Compilation
To compile indexer and indextest, simply "make". To complile either one, "make indexer" or "make indextest". To remove all the executables and test files, "make clean". "make test" will run a bash script with a number of test cases, the output of which is in testing.out. 

### Assumptions - Indexer
Indexer checks all of its arguments, and ensures the directory given is a crawler directory by checking for a ".crawler" file inside it. The directory given must have this file for the program to run correctly. The directory can end with a slash or not. In creating the hashtable to store the words, 350 slots are hardcoded in because the actual number of words is unknown. 


### Assumptions - Indextest
Indextest assumes that the first file given is indeed a file produced by indexer, with each line in the format
"word docID count [docID count]". It only checks that the first file is readable, and that the second one is writable. 