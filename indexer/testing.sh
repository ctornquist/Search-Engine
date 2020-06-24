make clean
make all
clear

#testing with the wrong number of arguments
./indexer here are too many arguments

#testing with too few arguments
./indexer one

#testing on a non existant directory
./indexer i_dont_exist/ idx1

#testing on a non-crawler (but existing) directory
./indexer fake idx1

# testing the directory on the directory made from crawling the letter playground to depth 5
echo Starting indexer
./indexer ../tse-output/letters-depth-5/ idx1
echo Index file made

echo Starting indextest
./indextest idx1 idx1_remake
echo Indextest file made

#sorting the files
gawk -f indexsort.awk idx1 > idx1.sorted
gawk -f indexsort.awk idx1_remake > idx1_remake.sorted
echo Files sorted

#comparing each to the correct answer file
echo Comparing files
diff --brief idx1.sorted ../tse-output/letters-index-5
diff --brief idx1_remake.sorted ../tse-output/letters-index-5
echo Done!



# testing the directory on the directory made from crawling the wiki playground to depth 1
echo Starting indexer
./indexer ../tse-output/wikipedia-depth-1/ idx2
echo Index file made

echo Starting indextest
./indextest idx2 idx2_remake
echo Indextest file made

#sorting the files
gawk -f indexsort.awk idx2 > idx2.sorted
gawk -f indexsort.awk idx2_remake > idx2_remake.sorted
echo Files sorted

#comparing each to the correct answer file
echo Comparing files
diff --brief idx2.sorted ../tse-output/wikipedia-index-1
diff --brief idx2_remake.sorted ../tse-output/wikipedia-index-1
echo Done!



# testing the directory on the directory made from crawling the to scrape playground to depth 1
echo Starting indexer
./indexer ../tse-output/toscrape-depth-1/ idx3
echo Index file made

echo Starting indextest
./indextest idx3 idx3_remake
echo Indextest file made

#sorting the files
gawk -f indexsort.awk idx3_remake > idx3.sorted
gawk -f indexsort.awk idx3_remake > idx3_remake.sorted
echo Files sorted

#comparing each to the correct answer file
echo Comparing files
diff --brief idx3.sorted ../tse-output/toscrape-index-1
diff --brief idx3_remake.sorted ../tse-output/toscrape-index-1
echo Done!



# testing the directory on the directory made from crawling the to scrape playground to depth 2
echo Starting indexer
./indexer ../tse-output/toscrape-depth-2/ idx4
echo Index file made

echo Starting indextest
./indextest idx4 idx4_remake
echo Indextest file made

#sorting the files
gawk -f indexsort.awk idx4_remake > idx4.sorted
gawk -f indexsort.awk idx4_remake > idx4_remake.sorted
echo Files sorted

#comparing each to the correct answer file
echo Comparing files
diff --brief idx4.sorted ../tse-output/toscrape-index-2
diff --brief idx4_remake.sorted ../tse-output/toscrape-index-2
echo Done!
