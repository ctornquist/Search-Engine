# Lab 4 Crawler - Caroline Tornquist  
   
### Testing  
  
I tested crawler using a bash script ```testing.sh```. This script runs 17 different test cases, and ensures all of their outputs are correct. 
  
The first 8 test cases ensure that my paramenter checking is functional. I test having too many inputs, not enough inputs, a non internal url, an internal url that doesn't exist, a negative max depth, a non integer max depth, and giving a directory without a slash at the end.  
Each of these write a helpful error message to stderr, cleans up any allocated memory, and exits with status one. I manually ran ```myvalgrind``` on each of these cases to ensure there would be no memory leaks regardless of where the program stopped running. 

Once it was clear that my parameter checking was sound, I moved onto testing the output of the crawler. I downloaded answers given by the [given website](http://old-www.cs.dartmouth.edu/~cs50/data/tse-output/) and stored them in directories ans1 to ans9. I had to edit them slightly, as my files printed the url, depth, and html text on new lines and the given files did not. 

```testing.sh``` tests the [letters](http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html) site at various depths and seed urls, and then compares those directories to their appropriate answer directories using the diff --brief bash command. They are stored in test1-test7, and their answers are in ans1-ans7. The only difference should be that the test directories contain a ```.crawler``` file that was created to test if they were valid directories, and the ```ans``` directories do not. I manually ran ```myvalgrind``` on each of these to ensure there were no memory leaks in any test case. 

I also tested the [wikipedia page](http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/) at depths 0, 1 and 2 and spot-checked them for accuracy, but only included depths 0 and 1 in ```testing.sh``` because running at depth 2 took way too long. I also manually ran test cases with [to scrape](http://old-www.cs.dartmouth.edu/~cs50/data/tse/toscrape/index.html) and found them to be accurate, but felt ```testing.sh``` did not need any more test cases so I left them off there. 

