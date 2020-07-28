# Caroline Tornquist

### Usage
To start searches on a sample dataset, run 'make query' from the top level directory in terminal. Each of the three parts, crawler, indexer and querier, have their own command line interfaces specified in their directory README. 

### Compilation
To build the entire project, run `make`.
To clean up the whole project, run `make clean`.

### Overview
This tiny search engine was created to search through several Dartmouth servers. It consists of five main parts:  
```crawler```: given a seedUrl it takes data from that page, any links on that page, and any links on the following pages up to a certain depth and saves it in a directory containing one file per webpage "crawled"  
```indexer```: takes data from a directory produced by crawler and builds a large hashtable with each word from each webpage to easily access it  
```querier```: reads searches or "queries" given by the user, determines which webpages contain those words and displays them ranked based on relevancy  
```common```: contains a number of files with functions used by several of the above directories  
```libcs50```: contains several data structures utilized by the rest of the program  

The directory ```tse-output``` contains the correct output created by crawler and indexer, and is used as a comparison in bash test scrips to ensure they work properly.   
Each subdirectory crawler, indexer and querier, contains a much more detailed description of their usage, implementation and testing. 
