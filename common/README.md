# Lab 4 - Common Directory by Caroline Tornquist    

### Compliation
Simply "make" to create the archive, and "make clean" to remove all the .o files and the archive. 
  
### Programs
  
```word.c``` contains the function ```normalizeWord``` which converts a string to only lowercase letters, adding spaces where non alphabet characters are. See ```word.h``` for full documentation. 
  
```pagedir.c``` contains functions to check if a directory is openable, to save a webpage struct to a file, to get the HTML text from a url, to scan through webpages and to load data from a file into a webpage struct. Used primarily by ```crawler.c```. 
  
```index.c``` contains functions to build an index hashtable, save one to a file, and load it back from that file. See ```index.h``` for full documentation. Used primarily by ```indexer.c``` and ```indextest.c```. 
    
