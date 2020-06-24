make clean
make
clear

#not enough inputs
./crawler bad arguments

#too many inputs
./crawler there are too many arguments here

#giving a negative integer for max depth
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./test/ -10

#giving a non-integer for max depth
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./test/ hello

#giving a directory that doesn't exist
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./not_there/ 3

#giving a non-internal url
./crawler buzzfeed.com ./test 3

#giving directory without slash
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./tests/test 0

#giving a url to a nonexistant server
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/this/isnt/real.html ./tests/test/ 3

echo testing different crawler directories...
#testing playground depth 0
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./tests/test1/ 0
diff -q -r ./tests/test1/ ../tse-output/letters-depth-0/

#testing playground depth 1
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./tests/test2/ 1
diff -q -r ./tests/test2/ ../tse-output/letters-depth-1/

#testing playground depth 2
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./tests/test3/ 2
diff -q ./tests/test3/ ../tse-output/letters-depth-2/

#testing playground depth 3
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./tests/test4/ 3
diff -q ./tests/test4/ ../tse-output/letters-depth-3/

#testing playground depth 4
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./tests/test5/ 4
diff -q ./tests/test5/ ../tse-output/letters-depth-4/

#testing playground depth 5
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./tests/test6/ 5
diff -q ./tests/test6/ ../tse-output/letters-depth-5/
  
#testing wikipedia depth 0  
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ ./tests/test8/ 0
diff -q ./tests/test8/ ../tse-output/wikipedia-depth-0/
 
#testing wikipedia depth 1 
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ ./tests/test9/ 1
diff -q ./tests/test9/ ../tse-output/wikipedia-depth-1/

echo ...all tests passed!