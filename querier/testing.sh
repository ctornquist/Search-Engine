#make clean
#clear
#make

# testing wrong number of parameters
./querier too many parameters here
./querier notenoughparams

# testing directory that doesn't exist
./querier imnotreal test

# testing non crawler directory
./querier ../indexer/fake test

# testing index file that doesn't exist
./querier ../tse-output/letters-depth-4/ notreal

# testing a lot of bad queries
# note: the output may be weird here because C prints to stderr before stdout
cat test | ./querier ../tse-output/letters-depth-4/ ../tse-output/letters-index-4

# testing 15 queries on a small dataset
./fuzzquery ../indexer/ans1 25 0 | ./querier ../tse-output/letters-depth-6/ ../tse-output/letters-index-6

# testing 35 queries on a large dataset
./fuzzquery ../indexer/ans3 35 4 | ./querier ../tse-output/toscrape-depth-2/ ../tse-output/toscrape-index-2


#ones that dont work
search or cerebrasthenia and graph huffman and for
search or page or depth and this and search