#include<stdlib.h> 
#include<stdio.h> 
#include<string.h> 
#include "../common/pagedir.h" 
#include "../libcs50/webpage.h" 
#include "../libcs50/bag.h" 
#include "../libcs50/hashtable.h" 
#include "../libcs50/set.h" 
 
void delete_blank(void *blank);
 
 
int main(int argc, char* argv[]){ 
    //has to have four arguments
    //have to check the number before I try to parse the arguments
    if (argc != 4){
        fprintf(stderr, "Needs three arguments.\n");
        return 1;
    }

    //PARAMETER CHECK VARIABLES
    char *seed_url = malloc(100*sizeof(char));      //must be dynamic to normalize it
    int max_depth = atoi(argv[3]);                  //converting string to integer
    int copy = (int)(*argv[3]);                     //will be 48 if the user inputs 0
 
    //PARAMETER CHECKING
    if (max_depth == 0 && copy != 48){                      //if they entered a string 
        fprintf(stderr, "Please enter an integer for max depth.\n");
        free(seed_url);
        return 1;
    }
        
    if (max_depth < 0){                                     //negative input
        fprintf(stderr, "Depth must not be negative.\n");
        free(seed_url);
        return 1;
    }
    
    //is the URL internal
    strcpy(seed_url, argv[1]);
    bool norm = IsInternalURL(seed_url);         //also normalizes the url
    if(!norm){
        fprintf(stderr, "Seed url not internal.\n");
        free(seed_url);
        return 1;
    }

    //checking if the URL actually points somehere by seeing if we can get its HTML 
    char *copy_url = malloc(100*sizeof(char));
    strcpy(copy_url, seed_url);
    webpage_t *exist = webpage_new(copy_url, 0, NULL);
    if (!webpage_fetch(exist)){
        fprintf(stderr, "Seed url points to nonexistant server.\n");
        webpage_delete(exist);
        free(seed_url);
        return 1;
    }
    webpage_delete(exist);

    //to add a slash onto the end of the directory we have to realloc data, so must calloc it here
    char *dir = calloc(strlen(argv[2])+2, sizeof(char));
    strcpy(dir, argv[2]);

    //if the directory doesn't end with a "/", add one
    int len = strlen(argv[2]);
    if (strcmp(&dir[len-1], "/") != 0){
        dir = realloc(dir, sizeof(dir)+3*sizeof(char));
        strcat(dir, "/");
    } 

    //checking if directory is valid
    bool good = check_directory(dir);
    if (!good){
        fprintf(stderr, "Invalid directory\n");
        free(dir);
        free(seed_url);
        return 1;
    } 


    //CRAWLING VARIABLES
    bag_t *pages_to_crawl = bag_new();                 //to know what we still have to explore
    hashtable_t *urls_seen = hashtable_new(200);       //hardcoding in 200 slots
    char *blank = " ";                                 //for addding things to hashtable
    int id=1;                                          //for writing to files w page_saver
    char *id_string = calloc(3, sizeof(char));         //to use for concatinating
    char *filepath = calloc(40, sizeof(char));         //for writing to files w page_saver
    int depth = 0;                                     //what the current depth is
    webpage_t *next;                                   //the next webpage to explore

    //CRAWLING

    //making seed page, adding to pages_to_crawl and urls_seen
    webpage_t *seed = webpage_new(seed_url, depth, NULL);           //seed_url was malloc'd which we needed to do
    bag_insert(pages_to_crawl, seed);
    hashtable_insert(urls_seen, webpage_getURL(seed), blank);
 
 
    while ( (next = bag_extract(pages_to_crawl)) != NULL){      //while there are more pages to explore
        page_fetcher(next);                                     //updates the HTML of the next page

        //writing to file in dir with appropriate id number and incrementing id
        strcpy(filepath, dir);
        sprintf(id_string, "%d", id);
        strcat(filepath, id_string);
        page_saver(next, filepath);
        id++;
        
        //moving on to next level
        if (webpage_getDepth(next) < max_depth){            //if we're not at the max depth yet 
            page_scanner(next, pages_to_crawl, urls_seen);
        }
        webpage_delete(next);
    } 
 

    //cleaning up
    webpage_delete(next);
    bag_delete(pages_to_crawl, &webpage_delete);
    hashtable_delete(urls_seen, &delete_blank);
    free(dir);
    free(filepath);
    free(id_string);
    free(next);
 
    return 0; 
} 
 

void delete_blank(void *blank){
   
}