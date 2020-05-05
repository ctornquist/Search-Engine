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
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./test 0

#giving a url to a nonexistant server
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/this/isnt/real.html ./test/ 3

#testing playground depth 0
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./test1/ 0
diff -q -r ./test1/ ./ans1/

#testing playground depth 1
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./test2/ 1
diff -q -r ./test2/ ./ans2/

#testing playground depth 2
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./test3/ 2
diff -q ./test3/ ./ans3/

#testing playground depth 3
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./test4/ 3
diff -q ./test4/ ./ans4/

#testing playground depth 4
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./test5/ 4
diff -q ./test5/ ./ans5/

#testing playground depth 5
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/index.html ./test6/ 5
diff -q ./test6/ ./ans6/

#testing different seed url in playground depth 3
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/letters/C.html ./test7/ 3
diff -q ./test7/ ./ans7/

#testing wikipedia depth 0
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ ./test8/ 0
diff -q ./test8/ ./ans8/

#testing wikipedia depth 1
./crawler http://old-www.cs.dartmouth.edu/~cs50/data/tse/wikipedia/ ./test9/ 1
diff -q ./test9/ ./ans9/
