# Lab 4 Crawler - Caroline Tornquist  

### Implementation

To implement the ```crawler.c``` module, I made use of several other provided modules, namely ```webpage.c```, ```bag.c``` and ```hashtable.c``` (which itself relies on ```set.c```). Each page that the crawler looked at was stored as a ```webpage_t *```, which has url, depth, and html elements. This made it easy to organize the data from each page, and access it efficiently. 

I made use of the bag module because of its ability to store any item, and it's LIFO implementation that allowed me to do a BFS-type search on the webpages. The bag module handles all of the inserting and deleteing (free'ing) of the nodes, which simplifies this system. I also made use of the hashtable module because of it's ability to store unique keys. If I insert a url into the hashtable and it returns false, then I know I have already seen that url and can dismiss it. The bag is then useful for my list of pages I have yet to crawl, and the hashtable is useful for my list of pages I have seen already

The main thrust of this program (after the error checking) is contained in a while loop that loops until there are no pages left in the bag ```pages_to_crawl```. It calls ```page_saver``` to save the data to the appropriate file, and then passes that page to ```page_scanner```. The functionality of the ```pageDir``` functions can be read in their [README.md](../common/README.md). It does all of the data collection and adding to the hashtable and bag. At the end of the program, all of the data is freed. 