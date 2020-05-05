# Lab 4 Crawler - Caroline Tornquist

###Usage 
Crawler will scrape websites for their HTML data, beginning at the seed URL and following all of the links on that page and any subsequent pages until the maxdepth is reached. It prints each webpage's URL, depth, and HTML data to a separate file in the directory specified named with a unique ID number. The user is responsible for supplying a seedURL, directory that exists (the program will not make one if it doesn't exist already) and a max depth to search. 
To compile, simply "make". Additionally, "make clean" and "make test" will clean up all of the executables and test files, and run a test case on the CS50 letters website with depth 3, respectively. To run from the command line: ./crawler seedURL pageDir maxDepth. 


###Notes 
Crawler begins by checking the parameters entered by the user. It makes sure that the max depth is an integer, and that the directory has a slash on the end (adding one if it does not). It then checks if the directory exists by passing to to pageDir's function ```check_directory``` which tries to open a file inside it. 
If all the parameters are correct, it creates a bag for webpages needed to crawl, and a hashtable that has the urls already seen (with blank character keys). Crawler then begins by scraping the seed webpage's data and writing it to the file. It iterates over pages left in the bag, passing each one to ```page_scanner`` which gets the links from each page, creates a new webpage for them, and adds them to the bag. Each page is then written to its appropriate file in turn. 
At the end of the file, each item used is freed separately. 

The only function is ```delete_blank()``` which doesn't do anything, but is necessary to pass into the hashtable function as an itemdelete function. Because the keys for hashtable are just empty chars, they don't need to be freed. 


###Assumptions 
Crawler does a significant amount of error checking based on what the user inputs. It does not assume that they input an appropriate url, a directory that exists, or an integer for the max depth. 


###Functions 


