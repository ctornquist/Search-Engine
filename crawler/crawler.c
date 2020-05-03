#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include "../common/pagedir.h"
#include "../libcs50/webpage.h"


int main(int argc, char* argv[]){

    //pagedir has to realloc to add a slash onto the end, so must malloc data here
    char *direc = malloc(strlen(argv[1])*sizeof(char)+1);
    strcpy(direc, argv[1]);
    bool good = check_directory(direc);
    if (good){
        printf("success\n");
    }
    else{
        printf("failure\n");
    }
    free(direc);

    char *url = malloc(100*sizeof(char));
    strcpy(url, "http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html");
    char *html = malloc(100*sizeof(char));
    html = NULL;
    
    webpage_t *page = webpage_new(url, 0, html);
    
    page_fetcher(page);
    
    char *filename = "./test/0";
    page_saver(page, filename);

    
    free(url);
    free(html);

    return 0;
}
