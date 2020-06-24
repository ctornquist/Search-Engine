# Crawler - Caroline Tornquist

### To Run
To run from the command line: ./crawler seedURL pageDir maxDepth.

### Compilation
To compile, simply "make". 

Note: the archives "common.a" and "libcs50.a" are needed to compile this directory. If they don't exist, run "make" in both the common and libcs50 directories to create them. 
 
"Make clean" will clean up all of the executables and test files. "Make test" will run ```testing.sh```, a bash script with a number of test cases. 

### Usage 
Crawler will scrape websites for their HTML data, beginning at the seed URL and following all of the links on that page and any subsequent pages until the maxdepth is reached. It prints each webpage's URL, depth, and HTML data to a separate file in the directory specified named with a unique ID number. The user is responsible for supplying a seedURL, directory that exists (the program will not make one if it doesn't exist already) and a max depth to search.  


### Notes 
Crawler begins by checking the parameters entered by the user. It makes sure that the max depth is an integer, and that the directory has a slash on the end (adding one if it does not). It then checks if the directory exists by passing to to pageDir's function ```check_directory``` which tries to open a file inside it. 
If all the parameters are correct, it creates a bag for webpages needed to crawl, and a hashtable that has the urls already seen. Crawler then begins by scraping the seed webpage's data and writing it to a file named 1. It then passes the seed to ``page_scanner`` which gets the links from each page, creates a new webpage for them, and adds them to the bag. Each page is then written to its appropriate file in turn. The program runs until there are no webpages left in the bag.
At the end of the file, the bag and hashtable are free'd (which frees everything inside them) and everything else malloc'd is also freed individually.


### Assumptions 
Crawler does a significant amount of error checking based on what the user inputs. It does not assume that they input an appropriate url, a directory that exists, or an integer for the max depth. 
No assumptions are made other than those made clear in the file. 


### Functions 
The only function is ```delete_blank()``` which doesn't do anything, but is necessary to pass into the hashtable function as an "itemdelete" function to iterate over. Because the keys for hashtable are just empty chars, they don't need to be free'd. 
