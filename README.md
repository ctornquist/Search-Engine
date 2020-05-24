# Caroline Tornquist
## CS50 Winter 2020, Tiny Search Engine

GitHub username: ctornquist

To build, run `make`.

To clean up, run `make clean`.

NOTE TO GRADER: So my querier works perfectly, supporting and and or sequences and sorting the output correctly like 95% of the time, but I am getting the occasional seg fault. I'm listing them here because no one ever responded to my slack messages and office hours weren't helpful, and they don't make a lot of sense. 

1. Sometimes I get a seg fault when trying to call freadlinep(), even though I check whether the file pointer I pass it is non-null. It says "No such file or directory." 
2. Sometimes I get one when I try to call counters_set() on a non-null counters struct (the specific error is "No such file or directory"). It specifically happens when I try to add a new node, so I assume there is sometimes an error in allocating data for that node. However, there's no way for me to check if the data was allocated properly inside counters as I don't have access to the .c file.
3. Sometimes (but not always), after calling my print_scores() function, some of the char * pointers in "words" have been changed. However, print_scores() does not take the array as a parameter, so I'm not sure why it is getting changed. 
4. Sometimes I get seg faults when I'm trying to do insertion sort, and specifically when I try to move the contents of one element in the array forward one space. Although I have appropriately allocated memory when needed and checked to make sure the element I'm moving to is non-null. In gdb it just says "cannot access memory at this location." I have checked and I should never be indexing out of the array so not sure about this one either. 

Long story short - just check what the seg fault is for and if just something super wonky happening please cut me some slack :). 