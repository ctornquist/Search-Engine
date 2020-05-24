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
./querier ../indexer/crawler1/ notreal

# testing a lot of bad queries
# note: the output may be weird here b/c C prints to stderr before stdout
cat test | ./querier ../indexer/crawler1/ ../indexer/ans1

# testing 15 queries on a small dataset
./fuzzquery ../indexer/ans1 25 2 | ./querier ../indexer/crawler1/ ../indexer/ans1

# testing 35 queries on a large dataset
./fuzzquery ../indexer/ans3 35 4 | ./querier ../indexer/crawler3/ ../indexer/ans3
