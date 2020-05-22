# Lab 6 Querier - Caroline Tornquist
 
# Pseudocode
Querier:  
1. Parse arguments, adding a slash to the end of the directory if needed, make sure there is a .crawler file that can be opened in it, and check if the file given can be read. I just appended .crawler to the end of the given filename, and then inserted the '\0' at the end to remove it after I'd checked the directory.   
2. Create a hashtable called "index" using hashtable_new
3. Call index_load to load data in to the index from the file. 
4. close file pointers. 

parse_query:  
1. loop through each character in the query. 
	* replace spaces with null characters
2. for each character
	* get the length of that word
	* normalize the word
	* insert it into the words array
	* update pointer to move to next start of word (add length of word)
3. return true if parsed correctly


validate_query:
1. if the first word in array is and or or, return false
2. if the last word in array is and or or, return false
3. 	for each word in the array
	* if that word is and or or
		* if the next word is also an and or or, return false
4. return true

res_and:
1. iterate over the first counters struct
	* if that key exists in the second
		* update the count of the first to be the minimum of the two
	* if it doesn't exist
		* set the count of the first to 0

res_or:
1. iterate over the second set
	* if that key exists in the first set
		* update its count to be the sum of the two counts
	* if it doesn't exist in the first set
	 	* add it, with count of the second set

print_scores:
1. iterate over the data set
	* add one for each iteration to get the total number of docIDs
2. initialize a docs structure with an array of struct scores of that number
3. set the docs' count to be 0
4. call insertion sort on the counter given
5. for each element in the array of struct scores
	* add the docID to the end of the filename
	* print the docID and score
	* if you can open the file, do so	
		* read the first line (the URL) and print it
	* close the file

insertion_sort:
1. if the count is not 0
	* allocate for new struct score and intialize it
	* if there's more than one item in the array
		* for each item in the array (starting at end)
			* if the count of that item is less than the current count
				* move it forward one index
				* set flag to true
			* else
				* break
		* if (flag)
			* set current item to last index searched +1
		* else
			* add current struct to end of array
	* else
		* insert into slot 0
	* increment counter

# Data Structures
The hashtable of docIDs and their counts detailed in Indexer is used here. Additionally, because counters_iterate will only take one arg, it was sometimes necessary to create structs so we could pass in multiple things. This was the purpose of struct ct_ptrs, so we could pass pointers to two counters into and_seq. To store each docID and count in an array, I created a score struct that had integer elements for each. To do the insertion sort, we needed to pass in both the array of score structs and a counter element, so I created a struct score_srt (instantiated as "docs") to store both the array and an integer counter variable. 
 
# Errors
Querier does significant error checking of the parameters. If any are not correct, it frees all of the memory allocated up to that point and exits with non zero status. 
